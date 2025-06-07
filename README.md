## Errata v1 (2025)

### Hardware

- Pull down Boot1 instead of floating. For booting F405 with DFU, hold down the 3rd reset button (may still work arbitrarily).
- SWD TagConnect: Layout is too close to XT30 or M.2 sockets and connector is difficult to source.
- Check and recheck DRC.
- Look out for "x"ed POS in the BOM file. May have been accidentally toggled off.
- Reset button is too small & hard to reach
- Vertical USB is less stress-tolerant (pulls on solder)
- Arbitrary STM32F411 DFU bootloading error
- Check forward current rating for Schottky diodes. Generally 1A would work
- Consider using more current sensors i.e. measure current draw from each board, from VIN total etc.
