#!/usr/local/bin/python3
import serial
import sys

#Change to fit your system
s = serial.Serial("/dev/tty.usbmodem12341")
while True:
    try:
        sys.stdout.buffer.write(s.read(16))#16 bytes (128 bits) is the size of RNG "packet"
        sys.stdout.flush()
    except KeyboardInterrupt:
        s.close()
        sys.exit(0)
