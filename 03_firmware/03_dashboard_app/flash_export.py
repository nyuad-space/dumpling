# Export downloader 
# Captures data contents from serial -> Save as csv on pc.

import serial


class FlashExportClient:
    def __init__(self, port, baudrate=115200):
        self.port = port
        self.baudrate = baudrate
        self.serial_port = None

    def open(self):
        self.serial_port = serial.Serial(self.port, self.baudrate, timeout=2)

    def close(self):
        if self.serial_port:
            self.serial_port.close()
            self.serial_port = None

    # detect export markers and save contents to PC
    def export_to_csv(self, output_path):
        if not self.serial_port:
            raise RuntimeError("Serial port not open")

        in_export = False

        with open(output_path, "w", newline="") as f:
            while True:
                raw = self.serial_port.readline()
                if not raw:
                    if in_export:
                        raise RuntimeError("Timed out before EXPORT_END")
                    continue

                line = raw.decode("utf-8", errors="ignore").strip()

                if not line:
                    continue

                if line == "EXPORT_BEGIN":
                    in_export = True
                    continue

                if line == "EXPORT_END":
                    break

                if line.startswith("EXPORT_ERROR"):
                    raise RuntimeError(line)

                if in_export:
                    f.write(line + "\n")

        return output_path