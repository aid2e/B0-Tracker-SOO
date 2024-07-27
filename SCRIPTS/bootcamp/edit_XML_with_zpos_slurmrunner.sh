#!/bin/bash

#############################################
# 
# Intent is for this bash script to edit the potions of the four
# B0 Tracker disks in associated XML files and create unique names for the XML
# detector geometry files and associated scripts based on the four z 
# positions of the B0 Tracker disks under test from the optimization code.
#
# This is to prevent overwriting of the files when running batch jobs. 
# 
# This script is called from movetest.py
#
#############################################

export EIC_PROJECT_DIR="/sciclone/data10/szelitch/AID2E"
export DETECTOR_PATH="/sciclone/data10/szelitch/AID2E/epic_install/share/epic"

# Assign command line arguments to variables
NEW_PHRASE_1="$1"
NEW_PHRASE_2="$2"
NEW_PHRASE_3="$3"
NEW_PHRASE_4="$4"
NEW_PHRASE_5="${1}_${2}_${3}_${4}"
OUTPUT_DIR="${5}"

# Define the file path
FILE_PATH_5="$EIC_PROJECT_DIR/scripts/B0_tracker.xml_ReplaceMe"
FILE_PATH_6="$EIC_PROJECT_DIR/scripts/far_forward.xml_ReplaceMe"
FILE_PATH_7="$EIC_PROJECT_DIR/scripts/epic_craterlake_18x275.xml_ReplaceMe"
FILE_PATH_8="$EIC_PROJECT_DIR/scripts/npsim_eicrecon_analysis_slurmrunner_ReplaceMe.sh"

# Copy renamed files (only once)
cp -n "$FILE_PATH_5" "$DETECTOR_PATH/compact/far_forward/B0_tracker.xml_${NEW_PHRASE_5}"
cp -n "$FILE_PATH_6" "$DETECTOR_PATH/compact/far_forward.xml_${NEW_PHRASE_5}"
cp -n "$FILE_PATH_7" "$DETECTOR_PATH/epic_craterlake_18x275.xml_${NEW_PHRASE_5}"
cp -n "$FILE_PATH_8" "$EIC_PROJECT_DIR/scripts/np_ei_an/npsim_eicrecon_analysis_slurmrunner_${NEW_PHRASE_5}.sh"

# Define more file paths
FILE_PATH_9="$DETECTOR_PATH/compact/far_forward/B0_tracker.xml_${NEW_PHRASE_5}"
FILE_PATH_10="$DETECTOR_PATH/compact/far_forward.xml_${NEW_PHRASE_5}"
FILE_PATH_11="$DETECTOR_PATH/epic_craterlake_18x275.xml_${NEW_PHRASE_5}"
FILE_PATH_12="$EIC_PROJECT_DIR/scripts/np_ei_an/npsim_eicrecon_analysis_slurmrunner_${NEW_PHRASE_5}.sh"

# Use sed to search and replace the phrases
sed -i "s/posLayer1/$NEW_PHRASE_1/g" "$FILE_PATH_9"
sed -i "s/posLayer2/$NEW_PHRASE_2/g" "$FILE_PATH_9"
sed -i "s/posLayer3/$NEW_PHRASE_3/g" "$FILE_PATH_9"
sed -i "s/posLayer4/$NEW_PHRASE_4/g" "$FILE_PATH_9"

# Use sed to search and replace the phrases in the other files
sed -i "s/ReplaceMe/$NEW_PHRASE_5/g" "$FILE_PATH_10"
sed -i "s/ReplaceMe/$NEW_PHRASE_5/g" "$FILE_PATH_11"
sed -i "s/ReplaceMe/$NEW_PHRASE_5/g" "$FILE_PATH_12"
#used -i "s/UniqueDir/$OUTPUT_DIR/g" "$FILE_PATH_12"


echo "Replacement complete."