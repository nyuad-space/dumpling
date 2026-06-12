# Defines structure of a python object to hold one IMU sample

from dataclasses import dataclass


@dataclass
class ImuSample:
    timestamp_ms: int
    ax_mps2: float
    ay_mps2: float
    az_mps2: float
    gx_rads: float
    gy_rads: float
    gz_rads: float
    temp_c: float

    @staticmethod
    def csv_header():
        return [
            "timestamp_ms",
            "ax_mps2",
            "ay_mps2",
            "az_mps2",
            "gx_rads",
            "gy_rads",
            "gz_rads",
            "temp_c",
        ]

    @staticmethod
    def from_csv_row(row): # for displaying sensor data
        return ImuSample(
            timestamp_ms=int(row[0]),
            ax_mps2=float(row[1]),
            ay_mps2=float(row[2]),
            az_mps2=float(row[3]),
            gx_rads=float(row[4]),
            gy_rads=float(row[5]),
            gz_rads=float(row[6]),
            temp_c=float(row[7]),
        )

    def to_csv_row(self):  # for saving live data to file
        return [
            self.timestamp_ms,
            self.ax_mps2,
            self.ay_mps2,
            self.az_mps2,
            self.gx_rads,
            self.gy_rads,
            self.gz_rads,
            self.temp_c,
        ]