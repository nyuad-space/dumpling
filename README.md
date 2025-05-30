## Errata v1 (2025)

### Hardware

#### Layout: Unconnected
- **Main Boot0** not connected to **3.3V**
- **Main I2C** pull-ups to **3.3V** missing
- **Main RX/TX** lines unconnected
- **5.1kΩ resistors** missing from BOM

#### Pin Configuration
- **Boot1 pin** is floating  
  ➤ To boot the main MCU into DFU mode, **hold down the 3rd reset button**, which pulls Boot1 low.
