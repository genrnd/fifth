fifth examples
==============

There are some examples (mnemonic used):

  * for5to1.txt -- simple cycle which prints from 5 to 1
  * factorial.txt -- calculation of factorial (5!)
  * demo.txt -- demo with leds burning, moving rectangle, character printing
  * leds.txt -- leds are burned cyclically
  * pseudopong.txt -- rectangle runs from left to right and vice versa
  * text.txt -- drawing simple text onto screen

How to run examples?
--------------------

Run examples without debug info:

  ../scripts/text2bin.py for5to1.txt | ../fifth 2>/dev/null
  ../scripts/text2bin.py factorial.txt | ../fifth 2>/dev/null

Run examples with displaying of stack and memory contents:

  ../scripts/text2bin.py for5to1.txt | ../fifth 
  ../scripts/text2bin.py factorial.txt | ../fifth 

