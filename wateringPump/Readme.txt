so, I have connected the 3v relay module and the pump. in the relay module, COM is connected to the positive wire of the pump (red). NO is connected to the vcc of breadboard. negative wire of the pump (Black) is connected to the GND of breadboard. VCC and GND of the relay module are also connected to the vcc and gnd of the breadboard. Relay mmodule's  "IN" is connected to D27 of the ESP32. Similarly ESP32's 3V3 and GND are also connected with the proper combination of the breadboard.

Code to run the pump with delay defined is given. We can change or modify the system later. For now, let's go with the basics!

UPDATE 1

Now let's take it to the cloud. We already opened a project in the blynk with just a switch button with the virtual pin v1.The arduino code with the according collaboration IoT is given.
