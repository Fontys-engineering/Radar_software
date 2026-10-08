# 07 - Data Handoff Contract ()

This is the interface between the plasma measurement code and the Ethernet streamer.

## Ownership

| Item                                                              | Owner                                           |
| ----------------------------------------------------------------- | ----------------------------------------------- |
| `plasma_stream.{h,c}` (ring + API), `Plasma_Record` layout        | Producer (Mete)                                 |
| Ethernet task, lwIP/CPSW, wire framing, UDP vs TCP, host receiver | Consumer (Vlad)                                 |
| Init order, task priorities, memory placement                     | Shared (this doc + [08](08_init_and_config.md)) |

## Mechanism

- **Single-producer / single-consumer ring** of `PLASMA_RING_SLOTS` pre-allocated, fixed-size slots in MSS L2 cache
- Both tasks run on the same R5F core. The indices are `volatile uint32_t` with a compiler barrier, so no lock is needed.
- **The producer never blocks.** If the ring is full, `acquireWrite()` returns NULL, and the producer increments `dropCount` and skips the frame (**drop-newest**). Drop-newest keeps the ring strictly SPSC: the producer never touches the read index.
- **The consumer blocks** on a counting semaphore (`SemaphoreP_constructCounting`, max = slots) that the producer posts on each commit.
- The consumer may hold a slot (e.g. while lwIP sends from it) until `release()`. Slots are released **in order**.

## API (`Radar_Program_mss/plasma/plasma_stream.h`)

```c
#define PLASMA_RING_SLOTS        16U
#define PLASMA_RECORD_MAX_BYTES  4096U          /* slot size, incl. header */

typedef struct {
    uint32_t committed;     /* records published            */
    uint32_t dropped;       /* producer found ring full     */
    uint32_t maxFill;       /* high-water mark (slots)      */
} PlasmaStream_Stats;

int32_t              PlasmaStream_init(void);                         /* before any producer/consumer use */
Plasma_Record*       PlasmaStream_acquireWrite(void);                 /* producer; non-blocking; NULL = full */
void                 PlasmaStream_commit(Plasma_Record *rec, uint32_t totalBytes);
const Plasma_Record* PlasmaStream_acquireRead(uint32_t timeoutTicks); /* consumer; NULL on timeout */
void                 PlasmaStream_release(const Plasma_Record *rec);  /* consumer; in-order */
void                 PlasmaStream_getStats(PlasmaStream_Stats *out);
```

**Rules:**

- Call from task context only. Never call from an ISR. ISR is bad news and will crash things when time is right.
- Exactly one producer task and one consumer task.
- A slot pointer is valid between `acquireRead` and `release`. After `release` it is not.
- If the consumer passes slot memory to CPSW DMA (zero-copy, e.g. `netbuf_ref` → pbuf), it must `CacheP_wb` the range first and must not `release` until the TX completes. **Recommendation:** use `NETCONN_COPY` / `pbuf_take` at first.

## Record layout (`Radar_common/plasma/plasma_types.h`)

All fields are **little-endian**. The record start is 8-byte aligned. Sections are 4-byte aligned and zero-padded. Floats are IEEE-754 binary32.

### Header — 48 bytes

| Off | Type | Field          | Notes                                                                                                     |
| --- | ---- | -------------- | --------------------------------------------------------------------------------------------------------- |
| 0   | u32  | `magic`        | `0x4D534C50` (bytes on the wire: `'P','L','S','M'`)                                                       |
| 4   | u16  | `version`      | `PLASMA_RECORD_VERSION`, starts at 1                                                                      |
| 6   | u16  | `headerBytes`  | 48                                                                                                        |
| 8   | u32  | `totalBytes`   | header + all sections; ≤ `PLASMA_RECORD_MAX_BYTES`                                                        |
| 12  | u32  | `seq`          | +1 per committed record. Gaps on the host mean network loss; `dropCount` covers on-device loss.           |
| 16  | u32  | `frameNumber`  | BSS frame counter (from DPC stats)                                                                        |
| 20  | u32  | `dropCount`    | cumulative producer drops                                                                                 |
| 24  | u64  | `timestampUs`  | MSS time at record build                                                                                  |
| 32  | u8   | `subFrameIdx`  | 0..numSubFrames-1                                                                                         |
| 33  | u8   | `numSubFrames` | 1 (full-band) or K (D2 mode)                                                                              |
| 34  | u16  | `numSections`  |                                                                                                           |
| 36  | u32  | `fCenterKHz`   | chirp centre frequency of this (sub)frame                                                                 |
| 40  | u32  | `dspCycles`    | PlasmaDsp_process cycle count                                                                             |
| 44  | u32  | `flags`        | bit0 `VALID` (peak above SNR threshold), bit1 `CAL_APPLIED`, bit2 `PHASE_UNWRAP_RESET`, bit3 `SATURATION` |

### Sections

Each section starts with `{u16 type; u16 rsvd; u32 bytes}`, 8 B, where `bytes` includes this header.

| Type | Name            | Body                                                                                                                                          | Size (example)                                          |
| ---- | --------------- | --------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------- |
| 1    | `BOUNDARY`      | `f32 rangeM; f32 velCoarseMps; f32 velFineMps; f32 snrDb; f32 phaseRad` (unwrapped across frames); `f32 displacementM; u16 peakBin; u16 rsvd` | 8 + 28 = 36 B                                           |
| 2    | `RANGE_PROFILE` | `u16 firstBin; u16 numBins; f32 binSizeM; u16 powDbX100[numBins]` (+pad)                                                                      | 8 + 8 + 2·256 = 528 B (256 bins = 512 real ADC samples) |
| 3    | `IQ_TAPS`       | `u16 firstBin; u16 numBins; u16 numChirps; u16 rsvd; {i16 re; i16 im}[numBins][numChirps]` (Rx-combined)                                      | 8 + 8 + 4·3·64 = 784 B                                  |
| 4    | `DIAG`          | `u32 interFrameMarginUs; i16 tempC; u16 rsvd`                                                                                                 | 16 B                                                    |

- Which sections are present is chosen by `plasmaCfg outputMask` ([08](08_init_and_config.md)). `BOUNDARY` is always present.

- **The host must skip unknown section types** using `bytes`. That gives forward compatibility.

- Typical record (full-band, all sections): 48 + 36 + 528 + 784 + 16 = **1412 B**. That fits one UDP datagram (≤ 1472 B payload at 1500 MTU). D2 subframes (128 bins) are smaller. Keep `numBins × nPhaseBins` within that envelope where possible.

- Expected rates:

  | Mode                       | Records/s | Payload      |
  | -------------------------- | --------- | ------------ |
  | Full-band at 100 Hz        | 100       | ≈ 1.1 Mbit/s |
  | D2 (4 subframes) at 100 Hz | 400       | ≈ 3.5 Mbit/s |

## Timing contract

|                  | Requirement                                                                                                                                                   |
| ---------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Producer latency | `commit` within ≤ 0.2 ms of the DPM result. The producer never waits on the consumer.                                                                         |
| Consumer         | Must sustain the average record rate. Bursts up to `PLASMA_RING_SLOTS` are absorbed (16 slots ≈ 160 ms at 100 Hz).                                            |
| Priorities       | Consumer task and lwIP `tcpip` thread **< 8** (UART/producer) and therefore below DPM (9) and ctrl (10). Suggested: ENET consumer 6, tcpip 6. CLI stays at 7. |
| Overflow         | Reported in `dropCount` (record header) and `PlasmaStream_getStats`. It is never an assert.                                                                   |
