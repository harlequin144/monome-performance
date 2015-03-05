SerialPort.listDevices

~box = SerialPort("/dev/ttyACM0",crtscts: true);
~box = SerialPort("/dev/ttyButtonBox", crtscts: true);


~box.close;
~box.next;
~box.read



//read 10bit serial data sent from Arduino's Serial.println
(
r = Routine({
    var byte, str, res;
	var pin, state;
	var buttons = Array.fill(16, {false});


	inf.do{|i|
        pin = ~box.read;
		state = ~box.read;
		("Pin: " ++ pin ++ " State: " ++ state).postln;

		if( (pin >= 0) && (pin <= 15) ){
			case
			{ state == 1 }
			{ buttons[pin] = true }
			{ state == 0 }
			{ buttons[pin] = false }
		};
		buttons.postln;
	};
}).play;
)

r.stop;
