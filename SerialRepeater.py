#!/usr/bin/python
import serial
import sys

#Change to fit your system
s = serial.Serial("/dev/tty.usbmodem12341")
while True:
    try:
        sys.stdout.write(s.read(32))#32 bytes (256 bits) is the size of RNG "packet"
        sys.stdout.flush()
    except KeyboardInterrupt:
        s.close()
        sys.exit(0)
