This project allows you to turn a Teensy microcontroller (or any other microcontroller) into a high quality (if slow) (psuedo)random number generator. The random numbers are sent to the PC over serial.

This project uses Von Neumann debiasing to convert ADC readings into high-quality debiased entropy. See Entropy.h/cpp for details.

It then takes those debiased readings and mixes them together using SHA-256. See Randomizer.h/cpp for details.

As a (very good) side effect of using Von Neumann debiasing, the device will block (not print any data to serial) if the ADCs don't change. This is a good thing. To get random data, make sure the ADCs are able to pick up environmental noise. To test it out, you can put your finger up against the ADC pins. If you're going to put this in a server box or something, you can attach the ADCs to long floating wires to pick up environmental EM noise. Be creative.

Reading from ADCs is slow, as is SHA256 (on an 8-bit microcontroller). You could get a much better speed if you A)used something besides ADCs for entropy and/or B)used an ARM instead. This code should be pretty trivial to port. The only platform-dependent code is in analog.h/c and Entropy.cpp (the ADC stuff) and usb_serial.h/c and main.cpp (the serial communication stuff). You could also tune some things to give more speed at the cost of randomness quality. Running the device at 8MHz, I get 60-100 bytes per second of random data. 16MHz is probably twice as fast. Not incredibly, but pretty good for key generation/server stuff, especially on a budget.

Since this code doesn't rely on interrupts (and shouldn't break if interrupted), it should be able to be run alongside pretty much any other code. Considerations must be made for the fact that this code is designed to hang forever in the event of a lack of entropy (as a security feature).