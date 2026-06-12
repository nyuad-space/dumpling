# Creates GUI+plots

import argparse
import tkinter as tk
from tkinter import filedialog, messagebox

from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure

from serial_source import SerialSampleSource
from csv_source import CsvPlaybackSource


# Dashboard logic
class TelemetryApp:
    def __init__(self, root, source, playback_mode=False, playback_interval_ms=50):
        self.root = root
        self.source = source
        self.playback_mode = playback_mode
        self.playback_interval_ms = playback_interval_ms

        self.max_points = 300

        self.timestamps = []
        self.ax = []
        self.ay = []
        self.az = []
        self.gx = []
        self.gy = []
        self.gz = []
        self.temp = []

        self.running = True

        self.root.title("dumpling telemetry")

        self.status_var = tk.StringVar(value="idle")
        self.current_var = tk.StringVar(value="No sample yet")

        top = tk.Frame(root)
        top.pack(side=tk.TOP, fill=tk.X)

        tk.Label(top, textvariable=self.status_var).pack(side=tk.LEFT, padx=8, pady=8)
        tk.Label(top, textvariable=self.current_var).pack(side=tk.LEFT, padx=8, pady=8)

        if self.playback_mode:
            tk.Button(top, text="Pause/Resume", command=self.toggle_running).pack(side=tk.RIGHT, padx=8)
            tk.Button(top, text="Restart", command=self.restart_playback).pack(side=tk.RIGHT, padx=8)

        self.figure = Figure(figsize=(10, 7), dpi=100)
        self.ax_accel = self.figure.add_subplot(311)
        self.ax_gyro = self.figure.add_subplot(312)
        self.ax_temp = self.figure.add_subplot(313)

        self.canvas = FigureCanvasTkAgg(self.figure, master=root)
        self.canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

        self.redraw_plot()

    def toggle_running(self):
        self.running = not self.running

    def restart_playback(self):
        if hasattr(self.source, "reset"):
            self.source.reset()
            self.timestamps.clear()
            self.ax.clear()
            self.ay.clear()
            self.az.clear()
            self.gx.clear()
            self.gy.clear()
            self.gz.clear()
            self.temp.clear()

    def append_sample(self, sample): # update with new sample
        self.timestamps.append(sample.timestamp_ms / 1000.0)
        self.ax.append(sample.ax_mps2)
        self.ay.append(sample.ay_mps2)
        self.az.append(sample.az_mps2)
        self.gx.append(sample.gx_rads)
        self.gy.append(sample.gy_rads)
        self.gz.append(sample.gz_rads)
        self.temp.append(sample.temp_c)

        if len(self.timestamps) > self.max_points:
            self.timestamps = self.timestamps[-self.max_points:]
            self.ax = self.ax[-self.max_points:]
            self.ay = self.ay[-self.max_points:]
            self.az = self.az[-self.max_points:]
            self.gx = self.gx[-self.max_points:]
            self.gy = self.gy[-self.max_points:]
            self.gz = self.gz[-self.max_points:]
            self.temp = self.temp[-self.max_points:]

        self.current_var.set(
            f"t={sample.timestamp_ms} ms | "
            f"a=({sample.ax_mps2:.3f}, {sample.ay_mps2:.3f}, {sample.az_mps2:.3f}) | "
            f"g=({sample.gx_rads:.3f}, {sample.gy_rads:.3f}, {sample.gz_rads:.3f}) | "
            f"T={sample.temp_c:.3f} C"
        )

    def redraw_plot(self):
        self.ax_accel.clear()
        self.ax_gyro.clear()
        self.ax_temp.clear()

        self.ax_accel.plot(self.timestamps, self.ax, label="ax")
        self.ax_accel.plot(self.timestamps, self.ay, label="ay")
        self.ax_accel.plot(self.timestamps, self.az, label="az")
        self.ax_accel.set_ylabel("Accel (m/s^2)")
        self.ax_accel.legend(loc="upper left")
        self.ax_accel.grid(True)

        self.ax_gyro.plot(self.timestamps, self.gx, label="gx")
        self.ax_gyro.plot(self.timestamps, self.gy, label="gy")
        self.ax_gyro.plot(self.timestamps, self.gz, label="gz")
        self.ax_gyro.set_ylabel("Gyro (rad/s)")
        self.ax_gyro.legend(loc="upper left")
        self.ax_gyro.grid(True)

        self.ax_temp.plot(self.timestamps, self.temp, label="temp_c", color="tab:red")
        self.ax_temp.set_ylabel("Temp (C)")
        self.ax_temp.set_xlabel("Time (s)")
        self.ax_temp.legend(loc="upper left")
        self.ax_temp.grid(True)

        self.figure.tight_layout()
        self.canvas.draw()

    def poll(self): # heartbeat check for update
        if self.running:
            sample = self.source.read_next_sample()
            if sample is not None:
                self.append_sample(sample)
                self.redraw_plot()

                if self.playback_mode:
                    self.status_var.set("Playback: running")
                else:
                    self.status_var.set("Live serial: connected")
            else:
                if self.playback_mode:
                    self.status_var.set("Playback: complete")
                else:
                    self.status_var.set("Live serial: waiting for data")

        self.root.after(self.playback_interval_ms, self.poll)


def main():
    parser = argparse.ArgumentParser(description="dumpling telemetry app")
    subparsers = parser.add_subparsers(dest="mode", required=True)

    live_parser = subparsers.add_parser("live", help="Live serial mode")
    live_parser.add_argument("--port", required=True, help="Serial port, e.g. COM5 or /dev/tty.usbmodem123")
    live_parser.add_argument("--baud", type=int, default=115200)
    live_parser.add_argument("--save", default="live_capture.csv", help="CSV output path")

    playback_parser = subparsers.add_parser("playback", help="Playback CSV mode")
    playback_parser.add_argument("--file", help="CSV input path")

    args = parser.parse_args()

    root = tk.Tk()

    if args.mode == "live":
        source = SerialSampleSource(
            port=args.port,
            baudrate=args.baud,
            csv_output_path=args.save,
        )
        source.open()
        app = TelemetryApp(root, source, playback_mode=False, playback_interval_ms=50)

    else:
        file_path = args.file
        if not file_path:
            file_path = filedialog.askopenfilename(
                title="Select CSV log file",
                filetypes=[("CSV files", "*.csv"), ("All files", "*.*")],
            )
            if not file_path:
                messagebox.showerror("Error", "No CSV file selected.")
                return

        source = CsvPlaybackSource(file_path)
        source.open()
        app = TelemetryApp(root, source, playback_mode=True, playback_interval_ms=30)

    def on_close():
        source.close()
        root.destroy()

    root.protocol("WM_DELETE_WINDOW", on_close)
    root.after(100, app.poll)
    root.mainloop()


if __name__ == "__main__":
    main()