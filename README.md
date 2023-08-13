# mbed_os_api_example_code
### Prerequisite
1. Mbed OS Simulator. For downloading and installation follow this link:
   https://os.mbed.com/blog/entry/introducing-mbed-simulator/
### Example 1
A simple machine control system using a microcontroller. The machine has start and stop buttons. When the start button is pressed, a motor is powered on. The motor will not start if the safety guard is open. The motor must be immediately turned off if the guard is opened, the stop button is pressed, or the operating temperature exceeds a certain value. The motor can be restarted if the stop button is released and/or the temperature falls to an acceptable value, but only if the guard is closed.

The following inputs and outputs are needed, with proposed logic values:

Push Button and Switches
1. Push button 1: User start button; closure = logic 1
2. Push button 2: Stop; closure = logic 1
3. Switch 1: Machine guard sensor; closed condition = logic 1
4. Temperature Sensor: Temperature sensor; excess temperature = logic 1

LEDs: 
1. LED 1: Motor is ready to run, i.e. guard is closed, temperature is OK. This LED should flash when machine is in "not ready" state.
2. LED 2: Motor is running.
3. LED 3: Fault condition – guard opened while running, shows for 0.5 s.
4. LED 4: Fault condition – excess temperature, shows for 0.5 s.

Connection Diagram: https://github.com/snehasish1991/mbed_os_api_example_code/blob/main/Connection%20Diagrams/Machine_control_excercise_1.PNG
