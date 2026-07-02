;/*
; *  Copyright (C) 2024 Texas Instruments Incorporated
; *
; *  Redistribution and use in source and binary forms, with or without
; *  modification, are permitted provided that the following conditions
; *  are met:
; *
; *    Redistributions of source code must retain the above copyright
; *    notice, this list of conditions and the following disclaimer.
; *
; *    Redistributions in binary form must reproduce the above copyright
; *    notice, this list of conditions and the following disclaimer in the
; *    documentation and/or other materials provided with the
; *    distribution.
; *
; *    Neither the name of Texas Instruments Incorporated nor the names of
; *    its contributors may be used to endorse or promote products derived
; *    from this software without specific prior written permission.
; *
; *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
; *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
; *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
; *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
; *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
; *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
; *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
; *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
; *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
; *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
; *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
; */
    .text

    .global Power_Down
    .def Power_Down
    .ref contextSave
    .ref Context_triggerSleep
Power_Down:
    MVKL contextSave,a0
    MVKH contextSave,a0
    ADD a0, 4, a0
    STW b3,  *a0++
    STW b10, *a0++
    STW b11, *a0++
    STW b12, *a0++
    STW b13, *a0++
    STW b14, *a0++
    STW b15, *a0++
    STW a10, *a0++
    STW a11, *a0++
    STW a12, *a0++
    STW a13, *a0++
    STW a14, *a0++
    STW a15, *a0++
    MVKL Context_triggerSleep, a1
    MVKH Context_triggerSleep, a1
    B a1
    NOP 5
    NOP 5
    NOP 5
    NOP 5
    NOP 5

    .global Power_Up
    .def Power_Up
Power_Up:
    MVKL contextSave,a0
    MVKH contextSave,a0
    ADD a0, 4, a0
    LDW *a0++, b3
    LDW *a0++, b10
    LDW *a0++, b11
    LDW *a0++, b12
    LDW *a0++, b13
    LDW *a0++, b14
    LDW *a0++, b15
    LDW *a0++, a10
    LDW *a0++, a11
    LDW *a0++, a12
    LDW *a0++, a13
    LDW *a0++, a14
    LDW *a0++, a15
    B b3
    NOP 5
    NOP 5
    NOP 5
    NOP 5
    NOP 5