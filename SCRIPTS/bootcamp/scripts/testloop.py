import subprocess

# Define a list of z-position values. Each sublist represents a set of arguments for one call.
z_positions = [
    [589.5, 615.5, 643.5, 670.5],
    [590.0, 616.0, 644.0, 671.0],
    [590.5, 616.5, 644.5, 671.5],
    [591.0, 617.0, 645.0, 672.0]
]

# Loop through each set of z-positions and call movetest.py with them as arguments
for z_pos in z_positions:
    command = ["python", "movetest.py"] + [str(pos) for pos in z_pos]
    subprocess.run(command)