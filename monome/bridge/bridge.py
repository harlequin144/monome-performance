#!/usr/bin/env python


import sys
sys.path.insert(0, '/home/dylan/.config/bridge')


import bridge_config
from Bridge import *


#
# Is there led security? if you have a given client selected and another one
# sends an led message, will it be blocked???

if __name__ == "__main__":
	bridge = Bridge(
			server_port = bridge_config.server_port, 
			top_clients = bridge_config.top_clients, 
			mid_clients = bridge_config.mid_clients, 
			bot_clients = bridge_config.bot_clients
		)

	bridge.run()

