from vpython import vector, arrow, scene
import serial
import numpy as np

# === SERIAL CONFIG ===
PORT = '/dev/tty.usbmodem1101'
BAUD = 38400
ser = serial.Serial(PORT, BAUD, timeout=1)

# === VPYTHON SCENE SETUP ===
scene.title = "Live IMU Orientation"
scene.background = vector(0.2, 0.2, 0.2)
scene.forward = vector(-1, -1, -1)
scene.width = 800
scene.height = 600

imu_arrow = arrow(pos=vector(0, 0, 0), axis=vector(0, 0, 1), color=vector(1, 0, 0), shaftwidth=0.1)

# === ROTATION UTILITY ===
def euler_to_vector(roll_deg, pitch_deg, yaw_deg):
    roll = np.radians(roll_deg)
    pitch = np.radians(pitch_deg)
    yaw = np.radians(yaw_deg)

    Rz = np.array([
        [np.cos(yaw), -np.sin(yaw), 0],
        [np.sin(yaw),  np.cos(yaw), 0],
        [0, 0, 1]
    ])

    Ry = np.array([
        [np.cos(pitch), 0, np.sin(pitch)],
        [0, 1, 0],
        [-np.sin(pitch), 0, np.cos(pitch)]
    ])

    Rx = np.array([
        [1, 0, 0],
        [0, np.cos(roll), -np.sin(roll)],
        [0, np.sin(roll),  np.cos(roll)]
    ])

    R = Rz @ Ry @ Rx
    return R @ np.array([0, 0, 1])

# === LOOP ===
while True:
    line = ser.readline().decode(errors='ignore').strip()
    if line.startswith("Roll:"):
        try:
            roll_str = line.split("Roll:")[1].split("Pitch:")[0].strip()
            pitch_str = line.split("Pitch:")[1].split("Yaw:")[0].strip()
            yaw_str = line.split("Yaw:")[1].split("|")[0].strip()

            roll = float(roll_str)
            pitch = float(pitch_str)
            yaw = float(yaw_str)

            n = euler_to_vector(roll, pitch, yaw)
            imu_arrow.axis = vector(n[0], n[1], n[2])

        except Exception as e:
            print("Parse error:", e)
