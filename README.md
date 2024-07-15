# B0-Tracker-SOO
Single Objective Optimization of B0 Tracker at ePIC EIC


## The B0 Tracker branch

* Check out the talk by Alex Jentsch at one of the Far Forward meetings on [Dec 5 2023](https://indico.bnl.gov/event/21467/contributions/84287/attachments/51407/87899/B0_field_map_tracking_performance_first_pass_Jentsch_12_5_23.pdf)

The B0 Tracker branch is in `b0-field-map-testing` at [`epic`](https://github.com/eic/epic) repository. DO 

```bash
git checkout b0-field-map-testing
```

Now build this to be used as your `epic` as we did the tutorials

## The steering file

The steering file is for throwing protons between 80GeV to 100GeV at the B0 Trackers with crossing angle included as well as the theta range between 0.006rad to 0.012rad. However, the overall coverage is supposed to be 0.03rad to 0.025rad.


## The npsim command 

The geometry we will use for this is `epic_craterlake_18x275.xml`

```bash 
npsim --steeringFile ddsim_steer_B0_testing.py --N [number_of_events] --compactFile $DETECTOR_PATH/epic_craterlake_18x275.xml --outputFile output.edm4hep.root
```