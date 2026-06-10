# Notes
1. Using classes 
- when the module owns a hardware device (FlashLogger, LSM6DSOnode, etc.)
- carries related objects' states together
- carries initialize and runtime methods together

2. Reading data from flash
New pipeline: sensor -> board -> serial/USB stream -> Py app/dashboard -> live plots + CSV save