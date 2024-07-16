#!/bin/tcsh

#SBATCH --job-name=JOB_NAME             # Job name
#SBATCH --output=LOG_DIR/JOB_NAME.out   # Output file
#SBATCH --error=LOG_DIR/JOB_NAME.err    # Error file
#SBATCH --time=03:00:00                 # Time limit hrs:min:sec
#SBATCH --nodes=1                       # Number of nodes
#SBATCH --ntasks=1                      # Number of tasks
#SBATCH --ntasks-per-node=1             # Number of tasks per node
#SBATCH --cpus-per-task=1               # Number of CPUs per task
#SBATCH --mem-per-cpu=4G                # Main memory in MByte per CPU


echo "RUNNING SIMULATIONS IN ${HOSTNAME}"

module load singularity

if [ $? -ne 0 ]; then
    echo "module load failed with ${?}"
fi

echo "Checking if singularity is available"

which singularity

if [ $? -ne 0 ]; then
    echo "Singularity not found. Exiting..."
    exit 1
fi

echo "Singularity found at $(which singularity) and version is $(singularity --version)"

# Change to the directory where you submitted the job
cd OUTPUT_DIR

# Your script or command here
echo "Starting job"
chmod 777 SCRIPTFILE
EIC_SHELL -- SCRIPTFILE
