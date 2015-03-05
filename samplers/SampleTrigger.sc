
STrigger { //sample trigger

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
 var synth;
 var path = "";

 *new {| buffer, osc_responder_path |
	 instance_num = instance_num + 1;

	 ^super.new.init(buffer, osc_responder_path );
 }

 init {| buffer, osc_responder_path |
	sample_buffer = buffer;

	synth = Synth(\dtrigger, [\bufnum, sample_buffer]);
	path = osc_responder_path;

	if( path != "" )
	{ OSCdef( ("trigger" ++ instance_num ++ "oscresp").asSymbol, 
	{ synth.set(\t_trig, 1) }, path ) };
 }

 go {
	 synth.set(\t_trig, 1);
 }
}
