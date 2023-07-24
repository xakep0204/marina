import serial
import numpy as np


with serial.Serial('/dev/tty.usbmodem1301',38400,timeout=1) as ser:
    print("Serial port open")
    state_vector = np.zeros(6)
    while True:
        try:
            line = ser.readline()
            line = line.decode("utf-8")
            if "AX" in line:
                state_vector[0] = float(line.split(" ")[1])
            elif "AY" in line:
                state_vector[1] = float(line.split(" ")[1])
            elif "AZ" in line:
                state_vector[2] = float(line.split(" ")[1])
            elif "GX" in line:
                state_vector[3] = float(line.split(" ")[1])
            elif "GY" in line:
                state_vector[4] = float(line.split(" ")[1])
            elif "GZ" in line:
                state_vector[5] = float(line.split(" ")[1])
            print(state_vector)
        except KeyboardInterrupt:
            break

