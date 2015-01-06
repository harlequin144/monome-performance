# This programs will serve as a command line utility to change the key of qseq.
# This should also be import able by other python programs.

import liblo
import sys
#A0 to A7, 27.5 to 3520.00
freqs = [27.5*(2**(1/float(12)))**n for n in range(85)]

diaMajor = [0,2,4,5,7,9,11]
diaMinor = [0,2,3,5,7,8,11]
pentaMajor = [0,2,4,7,9]
pentaMinor = [0,3,5,7,10]

note = {'a':0, 'b':2, 'c':3, 'd':5, 'e':7, 'f':8, 'g':10, 
				'A':0, 'B':2, 'C':3, 'D':5, 'E':7, 'F':8, 'G':10}
				



def read_args(arg):
	arg = arg[:4]	#Strip the end off.

	root = 'A'
	octave = 0
	sharp = False
	scale = diaMajor
	
	for char in arg:
		if char in "pPdD":
			if char == "p":
				scale = pentaMajor
			elif char == 'P':
				scale = pentaMinor
			elif char == 'd':
				scale = diaMinor
			elif char == 'D':
				scale = diaMajor

		elif char in "abcdefgABCDEFG":
			root = char

		elif char in "0123456789":
			octave = int(char)

		elif char == '#':
			sharp = True

	return root, octave, sharp, scale



def generate_freqs(root='a', octave=0, sharp=False, scale=diaMinor):
	root_index = (12*octave + note[root])

	if sharp:
		root_index = root_index+1
	 
	indecies = [(a,b) for a in range(root_index,len(freqs),12) for b in scale 
			if a+b < len(freqs)]

	return_freqs = [freqs[root+offset] for root, offset in indecies]
	return return_freqs[:15]

	
def send_to_qseq(notes, port):
	try:
		target = liblo.Address(7720)
		for i, freq in zip(range(15,0,-1),notes[::-1]):
			#print freq, i
			liblo.send(target, '/qseq/change_note', i, freq)
	except liblo.AddressError, err:
		print str(err)
		sys.exit()



if __name__ == "__main__":
	args = read_args(sys.argv[1])

	notes = generate_freqs(*args)
	send_to_qseq(notes, 12)
