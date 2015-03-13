TriggerSynth {
  var <quantize = 0;
  var <interval = 0;

  var buffers;
  var latest_pull;

  *new {|buf, quantize = 0, interval = 0|
	^super.new.init(buf, quantize, interval);
  }
      
  init {|bufs, quantize = 0, interval = 0|
    quantize = quantize;
    interval = interval;

      // if buffers...
  	bufs.postln;
    buffers = bufs;
    "triggersynth buffer".postln;
    buffers.postln;
  }

  pull {
    latest_pull = Synth(\sampletrigger, [\bufnum, choose(buffers)]);
  }

  halt { latest_pull.free }
}


Trigger { //sample trigger

  classvar instance_num = 0;

  var buffers;
  var <triggers;
  
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

    buffers = Dictionary();
    triggers = Dictionary();
    trigger_queue = [];
    
    
    OSCdef(osc_symbol ++ "pull", 
        {|msg| 
            "pull osc".postln;
            this.pull(msg[1])
        }, osc_path +/+ "pull"); 

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

   //if( quantize > 0 ){
  	 //if( awaiting_trigger && trans_on )
       //{}
   //}
  }


  pull {|trig_key|
	"pull".postln;
    trig_key.postln;
    triggers.postln;
	// check that the buffers and triggers are valid.
	// this is an invariant.. I think
    
    if( (triggers[trig_key].quantize > 0) && trans_on )
    { trigger_queue.add(trig_key) }
      { triggers[trig_key.asSymbol()].pull() }
  }


  halt {|trig_key|
	triggers[trig_key].halt()
	// check that the buffers and triggers are valid.
	// this is an invariant.. I think
  }

  addBuffer {|name, path|
	buffers[name] = Buffer.read(Server.local, path);
  }

  addTrigger {|name, bufs, quantize, interval|
    if( triggers[name] == nil ){

    }
    { triggers[name].add( TriggerSynth([buffers[bufs]], quantize, interval)) }
  }


}

