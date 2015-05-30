TriggerSynth {
  var <quantize = 0;
  var <interval = 0;

  var buffers;
  var latest_pull;

  *new {|buf, quantize = 0, interval = 0|
	^super.new.init(buf, quantize, interval);
  }
      
  init {|bufs, quantize = 0, interval = 0|
    buffers = bufs;
  	//bufs.postln;
    quantize = quantize;
    interval = interval;
  }

  pull { 
	  latest_pull = Synth(\sampletrigger, [\bufnum, choose(buffers)]) 
  }

  halt { latest_pull.free }
}


/*
 * Idea, when a synth is pulled, let the pull function return a distance in
 * ticks that we would like to have it pulled again.
 *
 * Types
 * - retrigger - while it is on, it will be triggered at intervals.
 * - sequence - when pulled, it is advanced and retirggered so as to make a
 *		  sequence of triggers.
 * - rand - every trigger triggers a random one in the list. 
 * - 
 *
 * TriggerSynth(buffer, 
 *
 */


Trigger { //sample trigger

  classvar instance_num = 0;

  var triggers; // a dictionary of TriggerSynths, which have lists of 
  
  var osc_path = "";

  var transport;
  var trans_on = false;
  var trigger_queue;


  *new {|osc_responder_path, transportPort = 8001|
    instance_num = instance_num + 1;
    ^super.new.init(osc_responder_path, transportPort);
  }

  init {|osc_responder_path, transportPort = 8001|
    var osc_symbol = ("trigger" ++ instance_num).asSymbol;
    osc_path = osc_responder_path;

    transport = NetAddr.new("localhost", transportPort);
    trans_on = false;

    triggers = Dictionary();
    trigger_queue = [];
    
    
    OSCdef(osc_symbol ++ "pull", 
      {|msg| this.pull(msg[1]) }, osc_path +/+ "pull"); 

    OSCdef(osc_symbol ++ "halt", 
      {|msg| this.halt(msg[1])}, osc_path +/+ "halt");
    
    OSCdef(osc_symbol ++ "transstop", 
      this.transport_stop(), osc_responder_path +/+ "transport/stop");
   
    OSCdef(osc_symbol ++ "transtick", 
      {|msg| this.tickResponder(msg[1]); }, 
      osc_responder_path +/+ "transport/tick");
    

    transport.sendMsg("/transport/add_tick_client", 57120, "/sc/trigger");
  }

  transport_stop { 
    trans_on = false;
    trigger_queue.clear();
  }

  tick_responder {|tick|
    if( trans_on == false )
    { trans_on = true };

    if( trigger_queue.size > 0 ) { 
	  trigger_queue.do(
		{|trig_key|
		  if( (tick % triggers[trig_key].quantize) == 0 )
		  { triggers[trig_key].pull() }
		}
	  )
	};
  }


  pull {|trig_key|
    if( (triggers[trig_key].quantize > 0) && trans_on )
    { trigger_queue.add(trig_key) }
    { triggers[trig_key.asSymbol()].pull() }
  }


  halt {|trig_key|
	triggers[trig_key].halt()
	// check that the triggers are valid.
	// this is an invariant.. I think
  }

  addTrigger {|name, buffs, quantize, interval|
	triggers[name] = TriggerSynth(buffs, quantize, interval);
  }


}
