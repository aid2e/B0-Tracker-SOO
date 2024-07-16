

import argparse, os
from submit_sim import run_sim

if __name__ == "__main__":
    """
    summary: This script takes in a configuration template files, 
    Make modification to the epic xml files and makes a submit script and mentions how to run the submit script as well 
    """
    parser = argparse.ArgumentParser(description="Write a config file for a simulation")
    parser.add_argument("-xml", "--xml_file", help="Name of the simulation", default = "epic_craterlake_18x275")
    parser.add_argument("-epic", "--epic_install", 
                        help="""Path to the epic install,
                        Check out /volatile/halld/home/ksuresh/ePIC-EIC-2024/epic_install/share/epic
                        """, 
                        required=True,
                        )
    parser.add_argument("-c", '--config', help="Path to the configuration file", required=True)
    parser.add_argument("-r", '--run', help="Run the simulations", action="store_true")
    parser.add_argument("-z", '--z_center', help="Z center of the detector in dm", type = float, default = 63)
    args = parser.parse_args() 
    
    # Define the simulation parameters
    sim_name = args.xml_file
    path_to_epic_install = args.epic_install
    config = args.config
    
    interlayer_distance = [25, 32, 23, 27, 29, 31, 35]
    z_center = args.z_center
    
    commands = []
    
    # modify the epic_xml first
    
    for l in interlayer_distance:
        c_name = f"{z_center}cm_ilayer_{l}cm"
        name = f"{sim_name}_{c_name}"
        os.system(f"ls -lhtr {path_to_epic_install}/{sim_name}.xml")
        os.system(f"cp {path_to_epic_install}/{sim_name}.xml {path_to_epic_install}/{name}.xml")
        os.system(f"ls -lhtr {path_to_epic_install}/{name}.xml")
        
        ## replace far_forward.xml from the file to far_forward_63cm_ilayer_25cm.xml
        
        with open(f"{path_to_epic_install}/{name}.xml", "r") as f:
            contents = f.read()
            contents = contents.replace("far_forward.xml", f"far_forward_{c_name}.xml")
        with open(f"{path_to_epic_install}/{name}.xml", "w") as f:
            f.write(contents)
        
        ## replace B0_tracker.xml from file compact/far_forward/B0_tracker.xml to compact/far_forward/B0_tracker_63cm_ilayer_25cm.xml
        new_farfoward = f"{path_to_epic_install}/compact/far_forward_{c_name}.xml"
        os.system("ls -lhtr " + path_to_epic_install + "/compact/far_forward.xml")
        os.system("cp " + path_to_epic_install + "/compact/far_forward.xml " + new_farfoward)
        os.system("ls -lhtr " + new_farfoward)
        with open(new_farfoward, "r") as f:
            contents = f.read()
            contents = contents.replace("B0_tracker.xml", f"B0_tracker_{c_name}.xml")
        with open(new_farfoward, "w") as f:
            f.write(contents)
        
        ## replace B0_tracker.xml from file compact/far_forward/B0_tracker.xml to compact/far_forward/B0_tracker_63cm_ilayer_25cm.xml
        new_B0_tracker = f"{path_to_epic_install}/compact/far_forward/B0_tracker_{c_name}.xml"
        os.system("ls -lhtr " + path_to_epic_install + "/compact/far_forward/B0_tracker.xml")
        os.system("cp " + path_to_epic_install + "/compact/far_forward/B0_tracker.xml " + new_B0_tracker)
        os.system("ls -lhtr " + new_B0_tracker)
        with open(new_B0_tracker, "r") as f:
            contents = f.read()
            contents = contents.replace("INTERLAYER_DISTANCE", f"{l}")
            contents = contents.replace("Z_CENTER", f"{z_center/10.}")
            xpos = z_center*(-0.16000052/63.0)
            contents = contents.replace("X_MOVE", f"{xpos}")
        with open(new_B0_tracker, "w") as f:
            f.write(contents)
        
        ## write configuration 
        with open(config, "r") as f:
            contents = f.read()
            contents = contents.replace("CONFIG_NAME", name)
            contents = contents.replace("OUTPUT_NAME", name)
        config_name = "config_" + name + ".json"
        with open(config_name, "w") as f:
            f.write(contents)
        
        ## Submit simulations 
        if args.run:
            run_sim(config_name)
        
        else:
            commands.append(f"python3.6 submit_sim.py -c {config_name}")
    print(f"Simulation {name} is ready to be submitted")
    for c in commands:
        print (c)
            
            
   