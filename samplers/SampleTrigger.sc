
Trigger { //sample trigger

/*
 * Behavior:
 * This is a very simple sampler designed to just trigger a sample. It can
 * recieve one command: go. When it recieves it, it plays it's sample all the
 * way through and then stops. If commands to go are fast enough, then samples
 * may overlap. Quantization when the transport is on will be available too.
 *
 */

    classvar instance_num = 0;
	
    var sample_buffer;

    //var triggers
	//RTrigger
    //ITrigger
    
    var synth;
    var path = "";
    var trans_on = false;
    var awaiting_trigger = false;

 *new {|buffer, osc_responder_path, transportPort = 8001, quantize = 0|
	 instance_num = instance_num + 1;

	 ^super.new.init(buffer, osc_responder_path, transportPort, quantize);
 }

 init {|buffer, osc_responder_path, transportPort = 8001, quantize = 0|
	sample_buffer = buffer;

	synth = Synth(\sampletrigger, [\bufnum, sample_buffer]);
	path = osc_responder_path;

	// transport listening

	if( path != "" ) { 
		OSCdef( 
			("trigger" ++ instance_num ++ "oscresp").asSymbol, 
			{ 
				if( trans_on )
				{ awaiting_trigger = true }
				{ synth.set(\t_trig, 1) }
			}, 
			path 
		) 
	};

	if( quantize != 0 ){
		OSCdef( 
			("trigger" ++ instance_num ++ "trans_stop").asSymbol, 
			{ 
				trans_on = false;
				awaiting_trigger = false;
			}, 
			path +/+ "transport/stop"
		);

		OSCdef( 
			("trigger" ++ instance_num ++ "tickresp").asSymbol, 
			{|msg| 
				trans_on = true;
				this.tickResponder(msg[1]);

			}, 
			path +/+ "transport/tick"
		);

		//transport.sendMsg("/transport/add_tick_client", 57120, "/sc/trigger");
	}
 }

 go {
	 synth.set(\t_trig, 1);
 }

 tick_responder {|tick|
	 //if( quantize > 0 ){
		 //if( awaiting_trigger && trans_on )
         //{}
//
	 //}
 }

}

