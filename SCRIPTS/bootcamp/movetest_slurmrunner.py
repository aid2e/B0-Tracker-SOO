import subprocess
import sys
import os

##########################################################################################
# 
# This python script runs two bash scripts in sequence.
#
# The first script edits the potions of the four
# B0 Tracker disks in associated XML files and creates unique names for the XML
# detector geometry files and associated scripts based on the four z 
# positions of the B0 Tracker disks under test from the optimization code.
# Those points are the four arguments passed to this python script from the command line.
#
# The second script runs the npsim, eicrecon, and analysis programs, and returns the
# output of the analysis program as two floating point numbers. 
#
# Usage to run the default detector configuration:
# python movetest.py 589.5 615.5 643.5 670.5
#
##########################################################################################

# Set the EIC_PROJECT_DIR environment variable
os.environ['EIC_PROJECT_DIR'] = "/sciclone/data10/szelitch/AID2E"

# Define the path to your bash scripts using the environment variable
script1_path = os.path.join(os.environ['EIC_PROJECT_DIR'], 'scripts', 'edit_XML_with_zpos_slurmrunner.sh')
script2_path = os.path.join(os.environ['EIC_PROJECT_DIR'], 'scripts', 'call_npsim_eicrecon_analysis_slurmrunner.sh')

# If getting arguments from the command line, use sys.argv
arg1 = sys.argv[1]
arg2 = sys.argv[2]
arg3 = sys.argv[3]
arg4 = sys.argv[4]
#arg5 = sys.argv[5]

# Run the first bash script
subprocess.run(['bash', script1_path, arg1, arg2, arg3, arg4])


# Run the second bash script
subprocess.run(['bash', script2_path,arg1, arg2, arg3, arg4])

## Now the efficiencyAnalysis.C macro outputs to output.txt
## Read the last line from outfile.txt
#outputfile = f"{arg1}_{arg2}_{arg3}_{arg4}_output.txt"

##with open(os.path.join(os.environ['EIC_PROJECT_DIR'], arg5 + '/' + outputfile), 'r') as file:
#with open(os.path.join(arg5 + '/' + outputfile), 'r') as file:
#    lines = file.readlines()
#    last_line = lines[-1].strip()

## Process the last line
#f1_str, f2_str = last_line.split()  # Correctly split the last line into two parts
#f2 = float(f2_str)  # Convert the second part to float
#print(f"f1: {f1}, f2: {f2}")  # Print the floats