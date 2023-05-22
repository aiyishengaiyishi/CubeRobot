import serial
import time

arduino = serial.Serial(port='/dev/ttyUSB0',baudrate=9600)
time.sleep(3)

f = open("../Kociemba/output.txt", "r")
operations = f.readline()
arduino.write(operations.encode())
f.close()
