s.boot
s.shutdown
(
var drum_set_path = "/home/dylan/music/samples/VintageDrumSamples24bit/Roland TR-808";

var snare_buf = Buffer.read(s, drum_set_path +/+ "TR-808Snare01.wav");
var kick_buf = Buffer.read(s, drum_set_path +/+ "TR-808Kick03.wav");
var hato_buf = Buffer.read(s, drum_set_path +/+ "TR-808Hat_O01.wav");
var hatc_buf = Buffer.read(s, drum_set_path +/+ "TR-808Hat_C01.wav");
var ride_buf = Buffer.read(s, drum_set_path +/+ "TR-808Ride01.wav");

//make the samplers for those buffers.
~snare = STrigger( snare_buf, "/sc/sampler/snare" );
~kick = STrigger( kick_buf, "/sc/sampler/kick" );
~hato = STrigger( hato_buf, "/sc/sampler/hato" );
~hatc = STrigger( hatc_buf, "/sc/sampler/hatc" );
~ride = STrigger( ride_buf, "/sc/sampler/ride" );


// pitched sampler
// random sampler like jenifer hale
// build/loop sampler
// interval sampler for hi hats
// quantization
)


~hato.gollli
~ride.go

"arbeit".scramble.asSymbol;

("poo" ++ 3 ++ "asd")