import liblo
from time import sleep 

import serial
from serial.tools import list_ports


def remove_all(lst, item):
	if lst == ():
		return ()
	elif lst[0] == item:
		return remove_all(lst[1:], item)
	elif lst[0] != item:
		return (lst[0],) + remove_all(lst[1:], item)

def pop(lst):
	return lst[:len(lst)-1]


def maximal_elements(l):
	if len(l) == 0:
		return []
	else:
		max_len = max(map(len, l))
		return filter(lambda x: len(x) == max_len, l)

	

def combo_search( combo_build, press_stack, combos):
	if False:
		print "combo_build: " + str( combo_build)
		print "press_stack: " + str( press_stack)
		print "combos: " +  str(combos)
		print ""

	if press_stack == ():
		#print "KILL CASE"
		if () in combos:
			#print "there"
			return [combo_build]
		else:
			#print "here"
			return []

	elif combo_build == ():
		#print "EMPTY COMBO_BUILD"
		possible_combos = [c for c in combos if c[-1] == press_stack[-1]]

		if len(possible_combos) != 0:
			pop_combos = map(pop, possible_combos)

			
			return combo_search((press_stack[-1],), pop(press_stack), pop_combos)

		return []


	else:
		#print "ELSE"
		right_size = lambda x: len(x) in range(1,len(press_stack) + 1)
		last_element = lambda x: x[-1] == press_stack[-1] 
		possible_combos = filter(right_size, combos)
		possible_combos = filter(last_element, possible_combos)
		pop_combos = map(pop, possible_combos)

		l = []
		l = l + combo_search(combo_build, pop(press_stack), combos)

		if len(possible_combos) == 0:

			l = l + combo_search(combo_build, pop(press_stack), combos)

		else:
			combo_build = (press_stack[-1],) + combo_build

			l = l + combo_search(combo_build, pop(press_stack), pop_combos)

			if () in combos:
				l = l + [combo_build[1:]]

		return list(set(maximal_elements( l )))







def response_loop(ser, press_combos, release):
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
					press_stack = remove_all(press_stack, pin)

				#	if pin in release.keys():
				#		release[pin]()

		except serial.SerialException:
			print "Serial Exception! The port appears to have been closed"
			break


def look_for_device( desc, teensy_id):
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
