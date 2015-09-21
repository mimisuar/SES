CPU Info:

$1000 (4096 bytes of ram)

Memory Map:

$000 - $3FF	: Ram
$400 - $40F	: Data Stack
$410			: Keyboard (W, A, S, D, O, K, Enter/Return and Shift)
$411 - $413	: Mouse (XYLRD)
$414 - $419	: GPU Stuff
$420 - $43F	: APU Stuff
$500 - $FFF	: Prg Data 

[A]ccumulator - Used for math operations (byte)
[Z]Register - Used for storing other numbers (byte)

(PC)Program Counter - What operation the program is executing
(DataSP)Data Stack Pointer - Where in the data stack we are. Used by push, pop operations.
(CallSP)Call Stack Pointer - where in the call stack we are. Used by call, ret operations.

Keyboard values:
The keyboard values are stored as a single bit in memory address $410.
W 		= $80
A 		= $40
S 		= $20
D 		= $10
O 		= $08
K 		= $04
Return 	= $02
Shift	= $01

Mouse Registers:
MouseX 		= $411
MouseY 		= $412
MouseStatus = $413

Mouse Values:
Left 	= $01
Right 	= $02
Middle  = $04

$414 = GPU Ctrl
$415 = GPU Address (Upper)
$416 = GPU Address (Lower)
$417 = GPU Port

GPU:
$000 - $2FF: Sprites
$300 - $5FF: Backgrounds
$600 - $63F: Palettes
$640 - $73F: Object Attriubte Memory
$740 - $FFF: Background Table