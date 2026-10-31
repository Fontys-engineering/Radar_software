#!/usr/bin/env bash
###############################################################################
# radar_cli.sh - send commands to the AWR2944 CLI UART (TI demo + plasma cmds)
#
#   scripts/radar_cli.sh -f scripts/cfg/plasma.cfg     send a whole cfg file
#   scripts/radar_cli.sh plasmaStats                   send one command
#   scripts/radar_cli.sh plasmaCfg -1 0.3 8.0 12 3 0 7
#
# Options:
#   -p <port>     CLI port            (default /dev/ttyACM0, env RADAR_PORT)
#   -b <baud>     baud rate           (default 115200)
#   -t <seconds>  reply timeout/line  (default 5)
#   -k            keep going after an Error reply (default: stop)
#   -d <seconds>  pause after each reply      (default 0.05)
#
# Every line is sent with '\n' and the script waits for the CLI's reply:
# "Done", "Error <n>" or "... is not recognized", then pauses before the next
# line: the board prints its prompt after "Done" and only then reads again, and
# its SCI UART has a 1 byte RX buffer, so a char sent too early is lost
# (e.g. "flushCfg" arrives as "lushCfg"). Lines starting with '%' and
# blank lines are skipped locally, same as the TI Visualizer does.
###############################################################################
set -u

PORT=${RADAR_PORT:-/dev/ttyACM0}
BAUD=115200
TIMEOUT=5
DELAY=0.05
KEEP_GOING=0
CFG_FILE=""

usage() { sed -n '3,21p' "$0" | sed 's/^# \{0,1\}//'; exit 1; }

while getopts "p:b:t:f:d:kh" opt; do
    case $opt in
        p) PORT=$OPTARG ;;
        b) BAUD=$OPTARG ;;
        t) TIMEOUT=$OPTARG ;;
        f) CFG_FILE=$OPTARG ;;
        k) KEEP_GOING=1 ;;
        d) DELAY=$OPTARG ;;
        *) usage ;;
    esac
done
shift $((OPTIND - 1))

if [[ -z $CFG_FILE && $# -eq 0 ]]; then
    usage
fi
if [[ -n $CFG_FILE && ! -r $CFG_FILE ]]; then
    echo "error: cannot read $CFG_FILE" >&2
    exit 1
fi
if [[ ! -e $PORT ]]; then
    echo "error: $PORT not found (board connected? try -p /dev/ttyACMx)" >&2
    exit 1
fi

stty -F "$PORT" "$BAUD" raw -echo -echoe -echok -echoctl -echoke || exit 1
exec 3<>"$PORT"

# drop whatever is still queued (banner, old prompt)
while read -r -t 0.2 -u 3 _; do :; done

# send one command, print the board output, return 0 on Done
send_cmd() {
    local cmd=$1 line
    printf '%s\n' "$cmd" >&3
    while IFS= read -r -t "$TIMEOUT" -u 3 line; do
        line=${line%$'\r'}
        line=${line#mmwDemo:/>}
        [[ -z $line || $line == "$cmd" ]] && continue   # skip board echo
        echo "  $line"
        case $line in
            Done*) sleep "$DELAY"; return 0 ;;
            Error*|*"not recognized"*) sleep "$DELAY"; return 1 ;;
        esac
    done
    echo "  (timeout, no reply in ${TIMEOUT}s)"
    return 1
}

# run one command and apply the stop/keep-going policy
run() {
    echo "> $1"
    if ! send_cmd "$1" && [[ $KEEP_GOING -eq 0 ]]; then
        echo "stopped at: $1" >&2
        exit 1
    fi
}

if [[ -n $CFG_FILE ]]; then
    # load the file first: interleaving 'read' on the file and on the tty
    # mixes up bash's read buffer (prompt bytes end up in cfg lines)
    mapfile -t cfg_lines < "$CFG_FILE"
    for line in "${cfg_lines[@]}"; do
        line=${line%$'\r'}
        line=${line%%%*}                     # strip trailing % comments
        read -ra words <<< "$line"            # trim and squeeze spaces
        [[ ${#words[@]} -eq 0 ]] && continue
        run "${words[*]}"
    done
else
    run "$*"
fi
