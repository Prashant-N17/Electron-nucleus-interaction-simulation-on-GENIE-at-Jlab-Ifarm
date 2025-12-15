#!/bin/bash

# Usage: genie-job.sh <EVENTS> <JOBID>

if [ $# != 1 ]; then
  echo "Usage: $0 <EVENTS> <JOBID>"
  exit 1
fi

JOBID=$1

SRCDIR="$HOME"
DESTDIR="/farm_out/neupane/test"

set -e

echo
echo "-- Environment ---------------------------------------------"
env

echo
echo "-- Launching Apptainer Container and Setting Up GENIE Environment --"

CONTAINER="/cvmfs/singularity.opensciencegrid.org/fermilab/fnal-dev-sl7:latest"
APPTAINER="/cvmfs/oasis.opensciencegrid.org/mis/apptainer/current/bin/apptainer"

$APPTAINER shell --shell=/bin/bash -B /cvmfs,/work/hallc/e1206107/neupane/genie-works --ipc --pid "$CONTAINER" <<EOF
cd $HOME
source /cvmfs/fermilab.opensciencegrid.org/products/genie/bootstrap_genie_ups.sh
source /cvmfs/fermilab.opensciencegrid.org/products/common/etc/setups
source setup_GENIE.sh https://github.com/e4nu/Generator-NuHepMC.git hepmc3-Q2_04 false false $GENIE/config/ true
#gevgen -n ${EVENTS} -p 11 -t 1000060120 -e 4.012 --tune GEM21_11a_00_000 --event-generator-list EMDIS -->
cd /work/hallc/e1206107/neupane/genie-works/Generator
gntpc -f gst -i gntp.$1.ghep.root -o gntp.$1.gst.root
echo
pwd
EOF

echo

