import argparse
import csv
import time
import serial


CSV_HEADER = [
    "timestamp_ms",
    "ax_mps2",
    "ay_mps2",
    "az_mps2",
    "gx_rads",
    "gy_rads",
    "gz_rads",
    "temp_c",
]


def looks_like_header(parts):
    return len(parts) == 8 and parts[0] == "timestamp_ms"


def looks_like_sample(parts):
    if len(parts) != 8:
        return False
    try:
        int(parts[0])
        for x in parts[1:]:
            float(x)
        return True
    except ValueError:
        return False


def main():
    parser = argparse.ArgumentParser(description="Capture live CSV from serial to file")
    parser.add_argument("--port", required=True, help="Serial port, e.g. /dev/cu.usbmodem2101")
    parser.add_argument("--baud", type=int, default=115200)
    parser.add_argument("--out", required=True, help="Output CSV file")
    args = parser.parse_args()

    ser = serial.Serial(args.port, args.baud, timeout=1)

    sample_count = 0
    last_report = time.time()

    with open(args.out, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(CSV_HEADER)

        try:
            while True:
                raw = ser.readline()
                if not raw:
                    continue

                line = raw.decode("utf-8", errors="ignore").strip()
                if not line:
                    continue

                parts = [p.strip() for p in line.split(",")]

                if looks_like_header(parts):
                    continue

                if not looks_like_sample(parts):
                    continue

                writer.writerow(parts)
                sample_count += 1

                now = time.time()
                if now - last_report >= 1.0:
                    print(f"captured {sample_count} samples")
                    last_report = now

        except KeyboardInterrupt:
            print(f"\nDone. Saved {sample_count} samples to {args.out}")
        finally:
            ser.close()


if __name__ == "__main__":
    main()