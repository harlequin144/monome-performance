from gatillod import combo_search
import itertools 

combos = [
	(2,),
	(5,),
	(6,),
	(7,),
	(8,),
	(9,),

	(2, 6),
	(6, 2),
	(9, 10),

	(14,),
	(4,10,7),
	(4,9,7),
	(4,7),
	(1,7)
]

def powerset(lst):
	return reduce(
			lambda result, x: 
			result + [subset + (x,) for subset in result], lst, [()])


for press in combos:
	assert combo_search( (), press, combos) == [press]




#for stack in powerset((1,2,3,4,6,7,8,9,10)):
#for stack in powerset((2,4,6,7,8,9,10,14)):
for stack in powerset((2,4,6,7,8,9,10)):
	for press_stack in itertools.permutations(stack):
		#print "attempting: " + str(press_stack)

		pset = powerset(press_stack)
		ret = combo_search( (), press_stack, combos)
		#print ret

		if len(press_stack) > 0 and press_stack[-1] == 7:

			if (4,10,7) in pset and (4,9,7) in pset:
				assert ret == [(4,10,7), (4,9,7)]

			elif (4,10,7) in pset and (4,9,7) not in pset:
				assert ret == [(4,10,7)]
				
			elif (4,10,7) not in pset and (4,9,7) in pset:
				assert ret == [(4,9,7)]

			else:


				if (4,7) in pset and (1,7) in pset:
					assert ret == [(4,7), (1,7)] or [(1,7), (4,7)]

				elif (4,7) not in pset and (1,7) in pset:
					assert ret ==  [(1,7)]

				elif (4,7) in pset and (1,7) not in pset:
					assert ret == [(4,7)]

				#elif (4,7) not in pset and (1,7) not in pset:
				else:
					#print press_stack
					#print ret
					assert ret == [(7,)]


		elif len(press_stack) > 0 and press_stack[-1] == 10:
			if (9,10) in pset:
				assert ret == [(9,10)]

		elif len(press_stack) > 0 and press_stack[-1] == 6:
			if (2,6) in pset:
				assert ret == [(2,6)]

		elif len(press_stack) > 0 and press_stack[-1] == 2:
			if (6,2) in pset:
				assert ret == [(6,2)]

		elif len(press_stack) > 0:
			if press_stack[-1] == 2:
				assert ret == [(2,)]
			elif press_stack[-1] == 5:
				assert ret == [(5,)]
			elif press_stack[-1] == 6:
				assert ret == [(6,)]
			elif press_stack[-1] == 7:
				assert ret == [(7,)]
			elif press_stack[-1] == 8:
				assert ret == [(8,)]
			elif press_stack[-1] == 9:
				assert ret == [(9,)]
			elif press_stack[-1] == 15:
				assert ret == [(14,)]


for stack in powerset((0,1,2,3,4,5,6,7,8,9)):
	if 9 in stack:
		#print "attempting: " + str(stack)
		ret = combo_search( (), stack + (10,), combos)
		#print ret
		assert ret == [(9,10)]
	else:
		#print "attempting: " + str(stack)
		ret = combo_search( (), stack + (10,), combos)
		#print ret
		assert ret == []

for stack in powerset((0,1,2,3,4,5,6,7,8)):
	assert combo_search( (), stack + (10,), combos) == []


