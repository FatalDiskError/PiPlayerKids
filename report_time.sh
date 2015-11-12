#!/bin/sh
shift
exec /usr/bin/time -f '\tElapsed real: %e\tCPU: %P\tExit status: %x\n' sh -c "$@"
# argv="%C"