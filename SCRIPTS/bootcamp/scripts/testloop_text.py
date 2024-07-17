import subprocess

# Assuming the file 'z_positions.txt' exists and has the correct format
with open('z_positions.txt', 'r') as file:
    for line in file:
        z_pos = line.strip().split()  # Split each line into a list of values
        command = ["python", "movetest.py"] + z_pos
        subprocess.run(command)