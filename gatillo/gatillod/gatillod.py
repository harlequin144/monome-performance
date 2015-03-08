import liblo
from time import sleep 

import serial
from serial.tools import list_ports

from switchbox_config import *


def remove_all(lst, item):
	if lst == ():
		return ()
	elif lst[0] == item:
		return remove_all(lst[1:], item)
	elif lst[0] != item:
		return (lst[0],) + remove_all(lst[1:], item)



def combo_search( combo_build, press_stack, combos):
	if press_stack == ():
		return combo_build

	elif combo_build == ():
		possible_combos = [c for c in combos if c[-1] == press_stack[-1]]

		if possible_combos != []:
			if len(press_stack) >= min(map(lambda x: len(x), possible_combos)):
				return combo_search( (press_stack[-1],),
						press_stack[:len(press_stack)-1], 
						[c[:len(c)-1] for c in possible_combos])
		return ()


	else:
		f = lambda x: x[-1] == press_stack[-1] 
		possible_combos = filter(f, filter( lambda x: len(x) > 0, combos))

		if possible_combos != []:
			return combo_search( (press_stack[-1],) + combo_build,
					press_stack[:len(press_stack)-1], 
					[c[:len(c)-1] for c in possible_combos])

		else:
			return combo_search( combo_build,
					press_stack[:len(press_stack)-1], combos)


def response_loop(ser):
	press_stack = ()

	while( True ):
		try:
			read = ser.read(2)
			if len(read) == 2:
				pin = ord(read[0])
				state = ord(read[1])
				#print 'pin ' + str(pin) + " to " + str(state)
				if state == 1:
					press_stack = press_stack + (pin, )

					key = combo_search((), press_stack, press_combos.keys())
					print key
					#if key != ():
						#press_combos[ key ]()


				elif state == 0:
					press_stack = remove_all( press_stack, pin )

				#	if pin in release.keys():
				#		release[pin]()

		except serial.SerialException:
			print "Serial Exception! The port appears to have been closed"
			break


def look_for_device( desc ):
	print "Waiting and looking for the device"

	while( True ):
		if teensy_id in [i for _, _, i in list_ports.comports()]:
			try:
				ser = serial.Serial( desc )
				ser.timeout = 10
				print "Found the device and connected. Starting the responce loop."
				return ser

			except serial.SerialException:
				print 'Serial Exception! Attempted to open the device, but failed.'

		sleep(1)



 
while(True):
	serial_dev = look_for_device("/dev/ttyButtonBox")
	response_loop( serial_dev )
