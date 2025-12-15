#!/bin/bash

# Set your SLURM account/group here
SLURM_ACCT='hallc'   # <-- Replace with your actual account/group
WORKFLOW="neupane_workflow"

SRCDIR="$HOME"   # Directory with job scripts/executables
DESTDIR="/farm_out/$USER/test" # Output directory for this test
OUTPUT_GLOB='match:gntp.*.ghep.root' # Output files pattern

JOB_SCRIPT="${SRCDIR}/run_gevgen.sh"
NUM_JOBS=80
EVENTS_PER_JOB=1250000

ECHO="echo" # Used to echo swif2 commands for testing
if [ "${1:-unset}" == "submit" ]; then
  ECHO="" # disables echo, actually runs swif2 commands
fi

for JOBID in $(seq 1 $NUM_JOBS); do
  OUTFILE="gntp.${JOBID}.ghep.root"
  $ECHO \
  echo $(OUTFILE)
  pwd
  swif2 add-job \
    -workflow "$WORKFLOW" \
    -account "$SLURM_ACCT" \
    -partition 'production' \
    -cores '2' \
    -ram '3G' \
    -disk-scratch '3G' \
    -time 5h \
    -shell '/bin/bash' \
    -output "match:${OUTFILE}" "$DESTDIR/" \
    "$JOB_SCRIPT" "$EVENTS_PER_JOB" "$JOBID"
done

if [ -n "$ECHO" ]; then exit 0; fi

echo "-- Active SWIF workflows --------"
echo swif2 list

echo
echo "-- SWIF jobs for workflow $WORKFLOW --------"
echo swif2 status -workflow "$WORKFLOW" -jobs

echo
echo "NOTE: If the workflow looks good, execute it with:"
echo " swif2 run -workflow '$WORKFLOW'"
