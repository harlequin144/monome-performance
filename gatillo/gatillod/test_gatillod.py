from gatillod import combo_search
import itertools 

fun = lambda: True

press_combos = {
	(2,): fun,
	(5,): fun,
	(6,): fun,
	(7,): fun,
	(8,): fun,
	(9,): fun,

	(2, 6): fun,
	(6, 2): fun,
	(9, 10): fun,

	(14,):  fun,
}

release = {
}

combos = press_combos.keys()

def powerset(iterable):
	"powerset([1,2,3]) --> () (1,) (2,) (3,) (1,2) (1,3) (2,3) (1,2,3)"
	s = list(iterable)
	#return chain.from_iterable(combinations(s, r) for r in range(len(s)+1))
	return [itertools.combinations(s, r) for r in range(len(s)+1)]

def powerset(lst):
	return reduce(
			lambda result, x: 
			result + [subset + (x,) for subset in result], lst, [()])


#print powerset((0,1,2,3,4,5,6,7,8,9))

assert combo_search( (), (10,), combos) == ()


#for stack in powerset((0,1,2,3,4,5,6,7,8)):
for stack in powerset((0,7,8)):
	print stack + (10,)
	combo_search( (), stack + (10,), combos) 
	assert combo_search( (), stack + (10,), combos) == ()
#assert combo_search( (), , combos) == 
#assert combo_search( (), , combos) == 
#assert combo_search( (), , combos) == 
#assert combo_search( (), , combos) == 
#assert combo_search( (), , combos) == 
#assert combo_search( (), , combos) == 
#assert combo_search( (), , combos) == 
#assert combo_search( (), , combos) == 
#assert combo_search( (), , combos) == 
#assert combo_search( (), , combos) == 
#assert combo_search( (), , combos) == 
#assert combo_search( (), , combos) == 
#assert combo_search( (), , combos) == 

