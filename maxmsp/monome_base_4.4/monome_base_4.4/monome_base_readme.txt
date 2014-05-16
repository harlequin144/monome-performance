monome base 4.2
a small collection of useful max/msp patches

http://monome.org


these patches require running monomeserial concurrently. get it at http://monome.org/data

max/msp runtime is sufficient. you do not need to purchase max/msp. run the absolute latest version to ensure these will work correctly.

files with leading underscores are sub-patches required by other files. don't run them directly (nothing great will happen).


quick descriptions:



monome_test - basic functionality testing

prefix_rotate - tool for mapping keys/midi to prefix context switching, for running multiple patches at once



vu_flash - full flash over a specific threshold


vu_scroll - scrolling audio levels with specifiable speeds and decay


vu_spec - super lo-fi spectral display

(256 versions of the above patches by vlad spears)



slide - virtual midi cc sliders

monome_midi - map keypads to midi notes, leds to incoming midi notes, adc values to cc's

monome_midi_press - more complex one-way midi note mapping


monome_midi_slide - bank of 8 virtual cc sliders (old)



monome40h_fake - (not well supported) virtual clickable 40h



howto/osc_howto.mxb - 40h osc basics walkthrough


howto/40hserial_howto.mxb - messy 40h serial protocol walkthrough



routers/256_router - (obsolete) pre-monomeserial for the 256
routers/40h_router - (obsolete) early version of monomeserial for 40h


additionally, if you're using the runtime and need to change your dsp settings or wish to record to disk, use the patches in the /max_helpers folder.




help with prefixes:

initially the 40h only communicated on the /40h prefix using osc. this was limiting in several ways, and now you can dynamically change the prefix either in monomeserial or with osc messages which are implanted in the max/msp patches.

how this is useful? by each patch or application having it's own prefix, you can "tell" monomeserial to have the 40h pay attention to one specific app on the fly. so if you have two running at once (say _vu_scroll and _midi_slide) at the same time, you can also run _prefix_rotate and map keyboard strokes to toggle between these.

note that these patches don't clear or refresh their displays on prefix changes (we need to add this) but we're expecting to take advantage of this feature extensively in our upcoming designs.

the short story: if the patch isn't working, click the [/sys/prefix /whatever] button, and it'll probably start working. if it doesn't, maybe you forgot to run monomeserial.


cheers.