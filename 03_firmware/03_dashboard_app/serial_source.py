# Used for LIVE SERIAL MODE (or SerialOutMode::Debug)
# Reads live data from board over USB serial.
# Source: USB serial / Output: app structure "Sample"

# Pipeline: [open serial port -> read lines -> ignore junk -> format csv into ImuSample -> save file on pc]

import csv
import serial
from sample_model import ImuSample


class SerialSampleSource:
    def __init__(self, port, baudrate=115200, csv_output_path=None):
        self.port = port
        self.baudrate = baudrate
        self.csv_output_path = csv_output_path
        self.serial_port = None
        self.csv_file = None
        self.csv_writer = None
        self.header_seen = False

    def open(self): # connect to serial port
        self.serial_port = serial.Serial(self.port, self.baudrate, timeout=1)

        if self.csv_output_path:
            self.csv_file = open(self.csv_output_path, "w", newline="")
            self.csv_writer = csv.writer(self.csv_file)
            self.csv_writer.writerow(ImuSample.csv_header())
            self.csv_file.flush()

    def close(self):
        if self.csv_file:
            self.csv_file.close()
            self.csv_file = None

        if self.serial_port:
            self.serial_port.close()
            self.serial_port = None

    # wait for next valid serial line -> parse -> return one sample (depends on SAMPLE_INTERVAL_MS)
    def read_next_sample(self): 
        if not self.serial_port:
            return None

        while True:
            raw = self.serial_port.readline() # read one line from serial
            if not raw:
                return None

            try:
                line = raw.decode("utf-8", errors="ignore").strip() # decode bytes into text
            except Exception:
                continue

            if not line:
                continue

            if line.startswith("#"):
                continue

            if line.startswith("timestamp_ms"):
                self.header_seen = True
                continue

            parts = [p.strip() for p in line.split(",")] # parts = raw text from serial
            if len(parts) != 8:
                continue

            try:
                sample = ImuSample.from_csv_row(parts) # sample = structured data
            except Exception:
                continue

            if self.csv_writer: # save to pc if needed
                self.csv_writer.writerow(sample.to_csv_row()) # save normalized version of valid sample
                self.csv_file.flush()

            return sample