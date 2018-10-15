# Solar Switch

Code compiles in Arduino IDE and layout files open in Eagle.

This was designed as a cheap/easy/hackable method of controlling how solar and AC mains power is routed. By default, this system monitors a solar battery bank voltage,
when it detects that the batteries are low, it switches the load to AC mains to allow the batteries to recharge - at which time it switches back to solar power.

This was mostly designed for hobbyists who have a only a few solar panels and want to power their projects/computers from solar, but do not have a big enough system
to run their load 24/7.

It was designed using the ATMEGA328P in the Arduino IDE and a compatible Arduino shield header. This allows the user to add any number of accesories to the system.
Some of the ideas that I had in mind were: Sd card logging and Wifi/GSM notifications.

The most current iteration is seen here:
![Prototype 3](Photos/proto_3.jpg?raw=true "Prototype 3")

Our first Arduino prototype board is seen here:
![Prototype 2](Photos/proto_2.jpg?raw=true "Prototype 2")