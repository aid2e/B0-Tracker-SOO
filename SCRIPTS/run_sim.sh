#!/bin/bash

RUNNING_DIR=$PWD
echo "Starting to perform simulation in ${RUNNING_DIR}" 

EPIC_INSTALL=epic_install
EICRECON_INSTALL=eicrecon_install
CODE_DIR=code_dir
OUT_DIR=out_dir
N_EVENTS=n_events

source $EPIC_INSTALL/setup.sh
source $EICRECON_INSTALL/bin/eicrecon-this.sh

export DETECTOR="detector_name"
export DETECTOR_CONFIG="detector_name"

compactFile=${DETECTOR_PATH}/${DETECTOR_CONFIG}.xml
#echo "Running overlap check"

#checkOverlaps -c $compactFile > $PWD/logs/overlap_check.log 2>$PWD/logs/overlap_check.err

echo "copying steeringFile to $PWD"
cp ${CODE_DIR}/ddsim_steer_B0_testing.py $PWD/
echo "Running npsim simulation"
npsim --steering ddsim_steer_B0_testing.py --numberOfEvents $N_EVENTS --compactFile $compactFile --outputFile ${DETECTOR}.edm4hep.root > $PWD/logs/sim_log.out 2>$PWD/logs/sim_log.err

echo "Running reconstruction"
eicrecon -Pdd4hep:xml_files=$compactFile -Ppodio_output_include_collections=ReconstructedParticles,GeneratedParticles,ReconstructedChargedParticles,B0TrackerRecHits ${DETECTOR}.edm4hep.root > $PWD/logs/recon_out.log 2>$PWD/logs/recon_err.log

echo "analyzing output"
echo ".... copying SimpleAnalysis.C to $PWD"
cp ${CODE_DIR}/SimpleAnalysis.C $PWD/
root -q -b ''${CODE_DIR}'/SimpleAnalysis.C("podio_output.root")' > $PWD/logs/ana_out.log 2>$PWD/logs/ana_err.log


