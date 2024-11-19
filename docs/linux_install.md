# Linux Installation

@tableofcontents

Generic Linux devices are supported via SPIDEV, PiGPIO, MRAA, RPi native via BCM2835, or using LittleWire.

@note The SPIDEV option should work with most Linux systems supporting spi userspace device.


### Automatic Installation (New)

Using CMake: (See the [instructions using CMake](using_cmake.md) for more information and options)

1. Download the install.sh file from [https://github.com/nRF24/.github/blob/main/installer/install.sh](https://github.com/nRF24/.github/blob/main/installer/install.sh)
   ```shell
   wget https://raw.githubusercontent.com/nRF24/.github/main/installer/install.sh
   ```
2. Make it executable
   ```shell
   chmod +x install.sh
   ```
3. Run it and choose your options
   ```shell
   ./install.sh
   ```

   @warning
   `SPIDEV` is now always selected as the default driver because
   all other Linux drivers have been removed (as of v2.0).
   See [RF24 issue #971](https://github.com/nRF24/RF24/issues/971) for rationale.

   It will also ask to install a python package named [pyRF24](https://github.com/nRF24/pyRF24).
   This is not the same as the traditionally provided python wrappers because the pyRF24 package can be
   used independent of the C++ installed libraries. For more information on this newer python
   package, please check out [the pyRF24 documentation](https://nrf24.github.io/pyRF24/).
4. Try an example from one of the libraries
   ```shell
   cd ~/rf24libs/RF24/examples/linux
   ```

   Edit the gettingstarted example, to set your pin configuration
   ```shell
   nano gettingstarted.cpp
   ```

   Build the examples. Remember to set the `RF24_DRIVER` option according to the one that was
   selected during the scripted install.
   ```shell
   mkdir build && cd build
   cmake .. -D RF24_DRIVER=SPIDEV
   make
   ```

   Run the example
   ```shell
   sudo ./gettingstarted
   ```

See the [instructions using CMake](using_cmake.md) for more information and options
