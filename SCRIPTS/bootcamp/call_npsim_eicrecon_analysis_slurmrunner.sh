#!/bin/bash

export EIC_PROJECT_DIR="/sciclone/data10/szelitch/AID2E"

/sciclone/data10/szelitch/AID2E/eic/eic-shell -- $EIC_PROJECT_DIR/scripts/np_ei_an/npsim_eicrecon_analysis_slurmrunner_"${1}_${2}_${3}_${4}".sh