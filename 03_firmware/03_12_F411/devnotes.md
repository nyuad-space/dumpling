# Notes
1. Using classes 
- when the module owns a hardware device (FlashLogger, LSM6DSOnode, etc.)
- carries related objects' states together
- carries initialize and runtime methods together

2. New pipeline: Visualize data
- (Debug) Live serial mode: F411 sensor -> USB serial CSV stream -> Py app -> live plots + CSV save
- (Post-flight) Playback mode:                 -> saved CSV file -> Py app -> replay plot

# Errata
- Add telemetry (LoRa breakout) for real-time data visualization 
  (current: save to onboard flash- export over usb serial- app saves .csv to pc- app loads for playback)
- 

# TODO
- one firmware for both logging and export (controlled by F405?)