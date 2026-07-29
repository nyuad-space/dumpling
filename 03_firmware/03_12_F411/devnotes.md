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

# TODO (immediate)
- Finalize F411:
  - batched/timed flush
  - integrity check for export: size/checksum validation
  - how to not seek EOF every sample when appending



  # TODO F405

  Recommended practical split:

F411 finalization sprint (short):
add drop/error counters
change flush strategy to batched or timed flush
add one integrity check for export
F405 start immediately in parallel:
RTOS task partition
launch-stage state machine
logger task with queue-based ingestion compatible with current schema

turn this into a concrete F405 task breakdown next (task list with module boundaries and suggested RTOS task rates/priorities).


F405 main functions:
- Communicates with MPU6050 (main IMU) to determine upright position- 
- Triggers flight monitoring upon launch detection
- Manages three flash files:
- **Circular Buffer**: Logs during flight-ready state with overwrite on overflow
- **Main Buffer**: Logs during flight, no wrap-around
- **Status File**: Logs sensor configuration, buffer states, memory info, etc.