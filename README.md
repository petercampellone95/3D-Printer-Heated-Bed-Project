# 3D-Printer-Heated-Bed-Project

The purpose of this project was to retrofit my personal 3D printer with a custom designed heated bed solution. Originally, the 3D printer featured an acrylic plastic printing bed although this caused the finished prints to stick to the bed and required them to be pried off with considerable force. Upgrading to a heated bed allows for easy removal of finished prints and opens up the option to print using ABS plastic. Instead of purchasing a prebuilt kit I preferred to build and design my own solution from the ground up. The base of the heated bed is a 1/4" thick  9" x 9" sheet of 6061 aluminum. Aluminum is preferrable to acrylic for printing due to its conductive properties allowing it to retain heat and distribute it evenly amongst the entire surface. The heating element is a 200W silicone heating pad which is stuck to the back of the aluninum plate using an adhesive. Running at 12V and consuming 200W of power requires a power supply capable of supplying at least 17A of current continously therefore a 25A supply was chosen to leave some extra headroom for powering other devices if needed. By using a solid state relay the power to the silicone heater can be shut on and off using the digital output from the ATMEGA328p and then by implementing a simple hysteresis algorithim the SSR can be switched on and off to maintain a relatively constant temperature. The NTC3950 thermistor in the silicone heater reports the bed temperature to the ATMEGA328p and is used in the hysteresis algorithim. Further, two capactive touch sensors where added to allow the user to change the bed temperature without having to reporgram the ATMEGA328p. Initially, the temperature reporting by the NTC3950 was not entirely accurate and after checking the temperature with various IR thermometers and other sensors the temperature calculating function in the ATMEGA328p's firmware was modified to more accurately report the bed temperature. From here, the next logical upgrade would be to implement a PID controller/PID control loop algorithim in lieu of the hysteresis algorithim although for now the hystersis algorithim has performed well. The bed is mounted using 4 bolts that run through springs that sit in between the 3D printer Z-axis gantry and two metal slats that run parallel underneath the bed. Tightening or lowering these bolts will raise or lower the bed in each respective corner allowing the user to level the bed so that the distance between the extruder nozzle is the same across the entire bed.


Parts List:
  - 9" x 9" x 0.250" 6061 Aluminium Plate
  - 12V 200W Silicone Heater with NTC 3950 Thermistor
  - 12V 25A Switching Power Supply
  - SSR-25DD DC-DC Solid State Relay 25A DC 5-60V
  - ATMEGA328p 8-Bit AVR Microcontroller
  - 20x4 Character LCD with I2C serial backpack
  - (2) TTP223 Capacitive Touch Switch
  - Assorted bolts, springs, and other hardware


Google Photos Link: https://photos.app.goo.gl/8MBN9eY2s5ktbLVa8
