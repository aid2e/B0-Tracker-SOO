#!/bin/bash

export EIC_PROJECT_DIR="/sciclone/data10/szelitch/AID2E"

cd $EIC_PROJECT_DIR

# it seems like I need to be in the eic-shell already or the script stops
# running after this line.
#./eic-shell

source $EIC_PROJECT_DIR/epic_install/setup.sh

# Run detector simulation (npsim) for -N=<number> events
# (!) Using *.edm4hep.root in the output is mandatory
# Looks like you can enter a bunch of command line options
# --gun.particle="pi-" or --gun.particle="pi+"
# --gun.momentumMin 10*MeV --gun.momentumMax 30*GeV 
# --gun.distribution uniform
npsim --steeringFile $EIC_PROJECT_DIR/scripts/ddsim_steer_B0_testing.py -N=200 --outputFile=$EIC_PROJECT_DIR/B0output/ReplaceMe_output.edm4hep.root --compactFile $DETECTOR_PATH/epic_craterlake_18x275.xml_ReplaceMe
 

# Run EICrecon reconstruction
# (!) Use the same geometry/detector as for simulation
#     e.g. source /opt/detector/setup.sh
# default output name is podio_output.root
# Tried to set a different output file name but it didn't work
eicrecon -Pdd4hep:xml_files=${DETECTOR_PATH}/epic_craterlake_18x275.xml_ReplaceMe -Pplugins=tracking -Ppodio:output_collections=ReconstructedChargedParticles,ReconstructedParticles,ReconstructedChargedWithoutPIDParticleAssociations,GeneratedParticles,MCParticles,ReconstructedChargedParticleAssociations -Ppodio:output_file=$EIC_PROJECT_DIR/B0output/ReplaceMe_podio_output.root $EIC_PROJECT_DIR/B0output/ReplaceMe_output.edm4hep.root


# Run the analysis 
# Was trying to use Karthik's AnalyzeMomemtum.C
# root -l -b -q "$EIC_PROJECT_DIR/AnalyzeMomentum.C(\"podio_output.root\", \"analysis_output.root\")"
# but couldn't get the histograms filled, so am using the tutorial example
root -l -b -q "$EIC_PROJECT_DIR/scripts/efficiencyAnalysis_new.C(\"$EIC_PROJECT_DIR/B0output/ReplaceMe_podio_output.root\", \"$EIC_PROJECT_DIR/B0output/ReplaceMe_\")"

