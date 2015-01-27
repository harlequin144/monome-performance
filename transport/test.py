#!/usr/bin/env python

from liblo import *
import sys
import time


time_list = []


class MyServer(ServerThread):
	    
	def __init__(self):
		ServerThread.__init__(self, 1234)

										
	#@make_method('/foo', 'ifs')
	#def foo_callback(self, path, args):
		#i, f, s = args
		#print "received message '%s' with arguments: %d, %f, %s" % (path, i, f, s)
	
	@make_method('/transport/tick', 'i')
	def fallback(self, path, args, types, src, data):
		if( args[0] == 0 ):
			time_list.append(time.time())
			print "yep"

	@make_method(None, None)
	def generic(self, path, args, types, src, data):
		print "yep"
		print path

try:
	server = MyServer()
except ServerError, err:
	print str(err)
	sys.exit()

server.start()
raw_input("press enter to quit...\n")



pairs = zip(time_list[:len(time_list)-1], time_list[1:])
print time_list
print pairs

deltas = [b - a for (a,b) in pairs]
print deltas
avg = sum(deltas)/len(deltas)
print avg
print "converted to bpm"
print 60.0 / avg

