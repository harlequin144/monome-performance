

import liblo
import serial
import re
from serial.tools import list_ports
import buttonbox


#teensy_desc = 'USB VID:PID=16c0:0483 SNR=478540'
teensy_desc = 'USB VID:PID=16c0:0483 SNR=725240'
#paths = [ [['']*12], [['']*12] ]
paths = [['']*12, ['']*12]


def parse_config():
	f = open('/home/dylan/.config/switchserialosc.cfg', 'r')

	regex = re.compile(r'\d+\s+(1|0)\s+\d+\s+((/\w+)+)\s+(\d+\s+)*')

	line = f.readline()
	while(line != ''):

		if(re.match(regex, line)):
			#print 'found a matching line'
			line_list = line.split()
			pin = int(line_list[0])
			pos = int(line_list[1])

			#print pin
			#print pos
			paths[pos][pin] = (' ').join(line_list[2:])

		line = f.readline()


def look_for_device(desc):
	ser = serial.Serial()
	
	while(ser.name == None):
		#ports = list_ports.comports()

		#for port in ports:
			#port_name, _, port_desc = port[0], port[1], port[2]

			#if port_desc == desc:
				#print 'we found the device, lets see if we can open it'

		try:
			ser = serial.Serial( "/dev/ttyButtonBox" )
			print 'succesfully opened'
			break

		except:
			print 'failed to open the device'

		# need to sleep here

	return ser


def response_loop(ser):
	run = True

	while(run):

		#respond to serial

#
# This needs to be a try thingy
#
		#pin = ord(ser.read(1))
		#state = ord(ser.read(1))

		read = ser.read(2)
		if len(read) == 2:
			pin = ord(read[0])
			state = ord(read[1])
			print 'pin ' + str(pin) + " to " + str(state)
			if state == 1:
				if pin in buttonbox.press.keys():
					buttonbox.press[pin]()
			elif state == 0:
				if pin in buttonbox.release.keys():
					buttonbox.release[pin]()

		# Check to see if we're still connected
		run = False
		for port in list_ports.comports():
			_ , _, port_desc = port[0], port[1], port[2]

			if port_desc == teensy_desc:
				run = True


#ser = look_for_device(teensy_desc)
# main loop

#parse_config()
#print(paths)

#while(True):

print list_ports.comports()
print "looking"
serial_dev = look_for_device(teensy_desc)
print "finished looking for device"

serial_dev.timeout = 10
response_loop(serial_dev)


