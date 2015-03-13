
Server.local.makeGui
// Three kinds of samplers:
// - triggers
// - loopers
// - pitched
// ????? build/loop sampler

//var snare_buf = Buffer.read(s, drum_set_path +/+ "TR-808Snare01.wav");
//var kick_buf = Buffer.read(s, drum_set_path +/+ "TR-808Kick03.wav");
//var hato_buf = Buffer.read(s, drum_set_path +/+ "TR-808Hat_O01.wav");
//var hatc_buf = Buffer.read(s, drum_set_path +/+ "TR-808Hat_C01.wav");
//var ride_buf = Buffer.read(s, drum_set_path +/+ "TR-808Ride01.wav");






p = {Out.ar(0,Pan2.ar(PlayBuf.ar(1, ~t, BufRateScale.kr(~t), doneAction: 2)))}.play
p.free

s.boot
~t = Buffer.read(Server.local, "/home/dylan/music/samples/g-nome-samples/shield-killer.wav");
~p = TriggerSynth(List[~t])
~p.pull

(
var drum_set_path = "/home/dylan/music/samples/VintageDrumSamples24bit/Roland TR-808";

~trigger = Trigger("/sc/trigger");

~trigger.addBuffer(\snare, drum_set_path +/+ "TR-808Snare01.wav");
//~trigger.addBuffer(\kick, drum_set_path +/+ "TR-808Kick03.wav");
//~trigger.addBuffer(\hat0, drum_set_path +/+ "TR-808Hat_O01.wav");
//~trigger.addBuffer(\hatc, drum_set_path +/+ "TR-808Hat_C01.wav");

//~trigger.triggers[\snare]



~trigger.addTrigger(\snare, \snare);
//~trigger.addTrigger(\name, quantize, interval)
//~trigger.addTrigger([\name], quantize, interval)


//"/sc/trigger/trig", "name"
//"/sc/trigger/kill", "name"
)


~trigger.pull(\snare)

