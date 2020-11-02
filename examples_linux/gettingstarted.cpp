/*
 * See documentation at https://tmrh20.github.io/RF24
 * See License information at root directory of this library
 * Author: Brendan Doherty (2bndy5)
 */

/**
 * A simple example of sending data from 1 nRF24L01 transceiver to another.
 *
 * A challenge to learn new skills:
 * This example uses the RF24 library's default settings which includes having
 * dynamic payload length enabled. Try adjusting this example to use
 * statically sized payloads.
 *
 * This example was written to be used on 2 or more devices acting as "nodes".
 * Use `ctrl+c` to quit at any time.
 */
#include <ctime>       // time()
#include <iostream>
#include <string>
#include <time.h>      // CLOCK_MONOTONIC_RAW, timespec, clock_gettime()
#include <printf.h>
#include <RF24/RF24.h> // delay()

using namespace std;

/****************** Linux ***********************/
// Radio CE Pin, CSN Pin, SPI Speed
// CE Pin uses GPIO number with BCM and SPIDEV drivers, other platforms use their own pin numbering
// CS Pin addresses the SPI bus number at /dev/spidev<a>.<b>
// ie: RF24 radio(<ce_pin>, <a>*10+<b>); spidev1.0 is 10, spidev1.1 is 11 etc..

// Generic:
RF24 radio(22, 0);

/****************** Linux (BBB,x86,etc) ***********************/
// See http://tmrh20.github.io/RF24/pages.html for more information on usage
// See http://iotdk.intel.com/docs/master/mraa/ for more information on MRAA
// See https://www.kernel.org/doc/Documentation/spi/spidev for more information on SPIDEV


// Let these addresses be used for the pair
uint8_t address[6] = "1Node";
// It is very helpful to think of an address as a path instead of as
// an identifying device destination

// For this example, we'll be using a payload containing
// a single float number that will be incremented
// on every successful transmission
float payload = 0.0;

void setRole(); // prototype to set the node's role
void master();  // prototype of the TX node's behavior; called by setRole()
void slave();   // prototype of the RX node's behavior; called by setRole()

// custom defined timer for evaluating transmission time in microseconds
struct timespec startTimer, endTimer;
uint32_t getMicros(); // prototype to get ellapsed time in microseconds

int main() {
    // perform hardware check
    if (!radio.begin()) {
        cout << "nRF24L01 is not responding!!" << endl;
        return 0; // quit now
    }

    // print example's introductory prompt
    cout << "RF24/examples_linux/GettingStarted\n";

    // Set the PA Level low to try preventing power supply related problems
    // because these examples are likely run with nodes in close proximity to
    // each other.
    radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.

    // Fot this example, we use the same address to send data back and forth
    // set the addresses for both RX and TX nodes
    radio.openWritingPipe(address);    // always uses pipe 0
    radio.openReadingPipe(0, address); // using pipe 0

    // for debugging, uncomment the follow 2 lines
    // printf_begin();
    // radio.printDetails();

    // ready to execute program now
    setRole(); // calls master() or slave() based on user input
    return 0;
}

/**
 * set this node's role from stdin stream.
 * this only considers the first char as input.
 */
void setRole() {
    string input = "";
    while (!input.length()) {
        cout << "*** PRESS 'T' to begin transmitting to the other node\n";
        cout << "*** PRESS 'R' to begin receiving from the other node\n";
        cout << "*** PRESS 'Q' to exit" << endl;
        getline(cin, input);
        if (input.length() >= 1) {
            if (input[0] == 'T' || input[0] == 't')
                master();
            else if (input[0] == 'R' || input[0] == 'r')
                slave();
            else if (input[0] == 'Q' || input[0] == 'q')
                break;
            else
                cout << input[0] << " is an invalid input. Please try again." << endl;
        }
        input = ""; // stay in the while loop
    } // while
} // setRole()


/**
 * make this node act as the transmitter
 */
void master() {
    radio.stopListening();                                          // powerUp() into TX mode

    // address for this example doesn't change
    // radio.openWritingPipe(0, address);

    unsigned int failure = 0;                                            // keep track of failures
    while (failure < 6) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &startTimer);            // start the timer
        bool report = radio.write(&payload, sizeof(float));         // transmit & save the report
        uint32_t timerEllapsed = getMicros();                          // end the timer

        if (report) {
            // payload was delivered
            cout << "Transmission successful! Time to transmit = ";
            cout << timerEllapsed;                                  // print the timer result
            cout << " us. Sent: " << payload << endl;               // print payload sent
            payload += 0.01;                                        // increment float payload

        } else {
            // payload was not delivered
            cout << "Transmission failed or timed out" << endl;
            failure++;
        }

        // to make this example readable in the terminal
        delay(1000);  // slow transmissions down by 1 second
    }
    cout << failure << " failures detected, going back to setRole()" << endl;
}

/**
 * make this node act as the receiver
 */
void slave() {

    // address for this example doesn't change
    // radio.openReadingPipe(0, address);

    radio.startListening();                                  // powerUp() into RX mode

    time_t startTimer = time(nullptr);                       // start a timer
    while (time(nullptr) - startTimer < 6) {                 // use 6 second timeout
        uint8_t pipe;
        if (radio.available(&pipe)) {                        // is there a payload? get the pipe number that recieved it
            uint8_t bytes = radio.getDynamicPayloadSize();   // get the size of the payload
            radio.read(&payload, bytes);                     // fetch payload from FIFO
            cout << "Received " << (unsigned int)bytes;      // print the size of the payload
            cout << " bytes on pipe " << (unsigned int)pipe; // print the pipe number
            cout << ": " << payload << endl;                 // print the payload's value
            startTimer = time(nullptr);                      // reset timer
        }
    }
    cout << "Timeout reached. Nothing received in 6 seconds" << endl;
    radio.stopListening();
}


/**
 * Calculate the ellapsed time in microseconds
 */
uint32_t getMicros() {
    // this function assumes that the timer was started using
    // `clock_gettime(CLOCK_MONOTONIC_RAW, &startTimer);`

    clock_gettime(CLOCK_MONOTONIC_RAW, &endTimer);
    uint32_t seconds = endTimer.tv_sec - startTimer.tv_sec;
    uint32_t useconds = (endTimer.tv_nsec - startTimer.tv_nsec) / 1000;

    return ((seconds) * 1000 + useconds) + 0.5;
}
