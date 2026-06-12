# Used for PLAYBACK MODE (or SerialOutMode::Csv)
# Reads saved data from pc.
# Source: / Output: app structure "Sample"

# Pipeline: [open saved file -> read lines -> ignore junk -> format csv into ImuSample -> save file on pc]

import csv
from sample_model import ImuSample


class CsvPlaybackSource:
    def __init__(self, csv_input_path):
        self.csv_input_path = csv_input_path
        self.samples = []
        self.index = 0

    def open(self): # 
        with open(self.csv_input_path, "r", newline="") as f:
            reader = csv.reader(f)
            header_skipped = False
            for row in reader:
                if not row:
                    continue

                if not header_skipped:
                    header_skipped = True
                    if row[0].strip() == "timestamp_ms":
                        continue

                self.samples.append(ImuSample.from_csv_row(row)) # read in csv row as sample data

    def close(self):
        pass

    # read next row
    def read_next_sample(self):
        if self.index >= len(self.samples):
            return None

        sample = self.samples[self.index]
        self.index += 1
        return sample

    def reset(self):
        self.index = 0