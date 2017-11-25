# Basement-Control

Capture the bits and pieces needed to make the basement control project work.

Blocks:
Arduino - used to measure temperature and light level. Also is a stand alone with a push button and small display. Communicates information by USB port.
Raspberry Pi - used to gather information from the Arduino and communicate with the X10 controller to turn the lights on and off
NAS - used to authorize access and provide a communication path between the RPi and the Internet.
