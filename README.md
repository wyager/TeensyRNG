This project allows you to turn a Teensy microcontroller (or any other microcontroller) into a high quality (if slow) random number generator. The random numbers are sent to the PC over serial.

This project uses Von Neumann debiasing to convert ADC readings into high-quality debiased entropy. See Entropy.h/cpp for details.

It then takes those debiased readings and mixes them together using SHA-256. See Randomizer.h/cpp for details.

As a (very good) side effect of using Von Neumann debiasing, the device will block (not print any data to serial) if the ADCs don't change. This is a good thing. To get random data, make sure the ADCs are able to pick up environmental noise. To test it out, you can put your finger up against the ADC pins. If you're going to put this in a server box or something, you can attach the ADCs to long floating wires to pick up environmental EM noise. Be creative.
