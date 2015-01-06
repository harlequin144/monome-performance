Choppa {

	//classvar numMicronomes;
	const path = "/sc/choppa";
	const lPath = "/sc/choppa/grid/led";

	// State
	var show_cnt = 0;

	// Osc
	var bridge;

	var sampdir = "/home/dylan/music/samples/hits/export";
	var buffers;



	*new {|bridgePortNum = 8000|
		^super.new.init(bridgePortNum)
	}

	init {|bridgePortNum = 8000|
		bridge = NetAddr.new("localhost", bridgePortNum);

		buffers = [
			Buffer.read(Server.local, sampdir +/+ "C.wav"),
			Buffer.read(Server.local, sampdir +/+ "C#.wav"),
			Buffer.read(Server.local, sampdir +/+ "D.wav"),
			Buffer.read(Server.local, sampdir +/+ "D#.wav"),
			Buffer.read(Server.local, sampdir +/+ "E.wav"),
			Buffer.read(Server.local, sampdir +/+ "F.wav"),
			Buffer.read(Server.local, sampdir +/+ "F#.wav"),
			Buffer.read(Server.local, sampdir +/+ "G.wav"),
			Buffer.read(Server.local, sampdir +/+ "G#.wav"),
			Buffer.read(Server.local, sampdir +/+ "A.wav"),
			Buffer.read(Server.local, sampdir +/+ "A#.wav"),
			Buffer.read(Server.local, sampdir +/+ "B.wav")
		];

		// Osc Responder Registration
		OSCdef(\choppa_press,
			{|msg, time|
				if(msg[3] == 1)
				{ this.pressResponder(msg[1], msg[2]) }
			},
			path +/+ 'grid/key');


		OSCdef(\choppa_hide,
			{
				show_cnt = 0; 
				//sending a request to bridge to hide
				bridge.sendMsg(path+/+"hide"); 
			},
			path+/+'hide');

		OSCdef(\choppa_show,
			{
				show_cnt = show_cnt + 1;
				//"show increased".postln;
				//show_cnt.postln;
				this.show;
			},
			path+/+'show');

		this.show();
	}

	/*
	 * Press Functions
	 */

	pressResponder {|xPos, yPos|
		if( show_cnt == 0)
		{ show_cnt = 1 };

		case
		{ (xPos < 2) && (yPos < 2) }
		{
			show_cnt = 0; 
			//sending a request to bridge to hide
			bridge.sendMsg(path+/+"hide"); 
		}
		{ (yPos == 3) || (yPos == 4) }
		{
			if( (xPos > 3) && (xPos < 16) ){
				{
					Out.ar(0,
						PlayBuf.ar(1, 
							buffers[xPos-4], 
							BufRateScale.kr(buffers[xPos-4]),
							doneAction:2
						)
					);
				}.play();
			}

		}
	}


	show { // Light up the monome
		if( show_cnt > 0){
			// Left side
			bridge.sendMsg(lPath +/+ "map", 0,0,
				3,3,0, 80, 80, 0,0,0);
			// Right Side
			bridge.sendMsg(lPath +/+ "map",8,0,
				0,0,0, 171,171, 0,0,0);
		};
	}


}
