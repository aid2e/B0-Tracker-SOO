#!/bin/bash

#SBATCH --job-name=SMZtest              # Job name
#SBATCH --output=SMZtest_%A_%a.out  # Output file
#SBATCH --error=SMZtest_%A_%a.err   # Error file
#SBATCH --time=03:00:00                 # Time limit hrs:min:sec
#SBATCH --nodes=1                       # Number of nodes
#SBATCH --ntasks=1                      # Number of tasks
#SBATCH --ntasks-per-node=1             # Number of tasks per node
#SBATCH --cpus-per-task=1               # Number of CPUs per task
#SBATCH --mem-per-cpu=4G                # Main memory in MByte per CPU


#
# To actually run the job, you need to use:
# sbatch --array=0-1 slurm_test.sh
# where --array=0-X specifies that you want to run the job X+1 times
#

# Create a unique output directory using the job ID
#OUTPUT_DIR="/sciclone/data10/szelitch/AID2E/job_${SLURM_JOB_ID}_${SLURM_ARRAY_TASK_ID}"
#mkdir -p $OUTPUT_DIR

# Change to the directory where you submitted the job
#cd $OUTPUT_DIR

# Export environment variables
export EIC_PROJECT_DIR="/sciclone/data10/szelitch/AID2E"
export DETECTOR_PATH="/sciclone/data10/szelitch/AID2E/epic_install/share/epic"
export SCRIPT_PATH="/sciclone/data10/szelitch/AID2E/scripts"

# Example command to run a Python script
#python $SCRIPT_PATH/movetest_slurmrunner.py 589.5 615.5 643.5 670.5
python $SCRIPT_PATH/movetest_slurmrunner.py 607.5 633.5 661.5 688.5