#!/usr/bin/env python


from Bridge import *

import ConfigParser


config = ConfigParser.ConfigParser()
config.read('/home/dylan/.config/bridge/bridge.cfg')


listen = int(config.get("Ports", 'listen') )
pPort = int(config.get("Ports", 'primaryMonome') )
sPort =  int(config.get("Ports", 'secondaryMonome') )


lClients = [ (prfx,int(port)) if int(port)!=0 else ('',0) 
		for prfx,port in config.items('LeftClients')]

rClients = [ (prfx,int(port)) if int(port)!=0 else ('',0) 
		for prfx,port in config.items('RightClients')]


if __name__ == "__main__":
	bridge = Bridge(listen, primaryMonome=pPort, secondaryMonome=sPort,
			leftClients=lClients, rightClients=rClients )

	bridge.run()

	bridge.free()

