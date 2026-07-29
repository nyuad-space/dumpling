import argparse
import csv
import matplotlib.pyplot as plt


def read_csv(path):
    t = []
    ax = []
    ay = []
    az = []
    gx = []
    gy = []
    gz = []
    temp = []

    with open(path, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            t.append(float(row["timestamp_ms"]) / 1000.0)
            ax.append(float(row["ax_mps2"]))
            ay.append(float(row["ay_mps2"]))
            az.append(float(row["az_mps2"]))
            gx.append(float(row["gx_rads"]))
            gy.append(float(row["gy_rads"]))
            gz.append(float(row["gz_rads"]))
            temp.append(float(row["temp_c"]))

    return t, ax, ay, az, gx, gy, gz, temp


def main():
    parser = argparse.ArgumentParser(description="Plot saved IMU CSV")
    parser.add_argument("--file", required=True, help="CSV file path")
    args = parser.parse_args()

    t, ax, ay, az, gx, gy, gz, temp = read_csv(args.file)

    fig, (p1, p2, p3) = plt.subplots(3, 1, sharex=True, figsize=(10, 7))

    p1.plot(t, ax, label="ax")
    p1.plot(t, ay, label="ay")
    p1.plot(t, az, label="az")
    p1.set_ylabel("Accel (m/s^2)")
    p1.grid(True)
    p1.legend(loc="upper left")

    p2.plot(t, gx, label="gx")
    p2.plot(t, gy, label="gy")
    p2.plot(t, gz, label="gz")
    p2.set_ylabel("Gyro (rad/s)")
    p2.grid(True)
    p2.legend(loc="upper left")

    p3.plot(t, temp, label="temp_c", color="tab:red")
    p3.set_ylabel("Temp (C)")
    p3.set_xlabel("Time (s)")
    p3.grid(True)
    p3.legend(loc="upper left")

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()
