#!/usr/bin/python

import sys
import time
import difflib
import pigpio

RX=19

try:
        pi = pigpio.pi()
        pi.set_mode(RX, pigpio.INPUT)
        pi.bb_serial_read_open(RX, 9600, 8)

        print "DATA - SOFTWARE SERIAL:"
        while 1:
                (count, data) = pi.bb_serial_read(RX)
                if count:
                        print data
			print(time.strftime("%d.%m.%Y %H:%M:%S"))
			print
                time.sleep(1)

except:
        pi.bb_serial_read_close(RX)
        pi.stop()
