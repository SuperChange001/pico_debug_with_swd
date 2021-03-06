# Pico probe test
This repo provides 
1. an example code that just blinks LED
2. instruction of 
   - build/install openocd device on Ubuntu 2022.04
   - how to use openocd connect you debugger and pico in Clion
3. troubleshooting

## Preparation

### Hardware
1. You would need to have two MCU module which has the RP2040 MCU on it, on as the `debugger` and the other one as the `target platform`.

2. And the `debugger` should be flashed with a firmware which makes it behaves as an openocd device. A prebuilt firmware can be found in this repo, in the utils folder and is named `pico_probe.uf2`. Of course, you can build one from its [source code](https://github.com/raspberrypi/picoprobe.git).
Pin mapping of my prebuilt firmware is different as the official configuration from raspberrypi, my mapping is show as below:

|GPIO of RP2040|Function|
|:---:|:---:|
|GPIO2|UART_TX|
|GPIO3|UART_RX|
|GPIO4|SWCLK|
|GPIO5|SWDIO|
|GPIO6|PROB_RESET|

If you don't like my/official configuration you can change it in [this file](https://github.com/raspberrypi/picoprobe/blob/master/src/picoprobe_config.h), then build it by yourself.

3. Now you can connect `Debugger` to the `target`, connection should be like below:

|Debugger|Target|comment|
|:---:|:---:|:---:|
|GPIO4|SWCLK||
|GPIO5|SWDIO||
|GND|GND||
|Power(3.3V or 5V)|Power(3.3V or 5V)|optional|

### Openocd software
The openocd software for RP2040 is a special one, you must build it from its [source code](https://github.com/raspberrypi/openocd). To build it, the official commands suggested by raspberrypi are:
```
sudo apt install automake autoconf build-essential texinfo libtool libftdi-dev libusb-1.0-0-dev
git clone https://github.com/raspberrypi/openocd.git --branch rp2040 --depth=1 --no-single-branch
cd openocd
./bootstrap
./configure --enable-picoprobe
make -j4
sudo make install # if you have other version of opencd in your system, you should not install it
```
**But**, it is possible some errors will show up:
  - during `./bootstrap`, if you saw any error message, you can execute it again, the second time you execute it, the error message will disappear.
  - this one (`./configure --enable-picoprobe`) could also fail, then you need install `pkg-config` with
```
sudo apt install pkg-config
```
4. To test if you build it correctly:
```
$ sudo src/openocd -f interface/picoprobe.cfg -f target/rp2040.cfg -s tcl
Open On-Chip Debugger 0.11.0-g610f137 (2022-05-06-14:33)
Licensed under GNU GPL v2
For bug reports, read
        http://openocd.org/doc/doxygen/bugs.html
Info : only one transport option; autoselect 'swd'
adapter speed: 5000 kHz

Info : Hardware thread awareness created
Info : Hardware thread awareness created
Info : RP2040 Flash Bank Command
Info : Listening on port 6666 for tcl connections
Info : Listening on port 4444 for telnet connections
Info : clock speed 5000 kHz
Info : SWD DPIDR 0x0bc12477
Info : SWD DLPIDR 0x00000001
Info : SWD DPIDR 0x0bc12477
Info : SWD DLPIDR 0x10000001
Info : rp2040.core0: hardware has 4 breakpoints, 2 watchpoints
Info : rp2040.core1: hardware has 4 breakpoints, 2 watchpoints
Info : starting gdb server for rp2040.core0 on 3333
Info : Listening on port 3333 for gdb connections
```
5. You may wonder if we can execute it without root, yes it is possible. To do so, you need to configure udev rules,
    - execute `lsusb` in your terminal, the device id is the info we need
    ```
    $ sudo lsusb
    Bus 001 Device 021: ID 2e8a:0004 Raspberry Pi Picoprobe
    ```
   - replace the id you got to the file I provided in utils(**2e8a**, **0004**), name as `99-openocd.rules`
   - then copy it to `/etc/udev/rules.d`, and reload rules
   ```
   sudo cp 99-openocd.rules /etc/udev/rules.d/
   sudo udevadm control --reload
   ```
   - plug out the debugger and replug it in again, now you can execute it without su

## Work openocd with Clion
1. Go to File>Setting>Embedded Development, change the openOCD Location to its executable file on your PC, should be under the `PATH_OPENOCD_SOURCE_CODE/src/openocd`.
   - There is a button called test, you can click it to check if you configured it correctly.
2. Edit the run/debug configuration
   ![run_debug_configurations](https://github.com/SuperChange001/pico_debug_with_swd/blob/master/documents/edit_config.png)
   ![create_new_configuration](https://github.com/SuperChange001/pico_debug_with_swd/blob/master/documents/add_new_configure.png)
   ![add_configure_of_2040](https://github.com/SuperChange001/pico_debug_with_swd/blob/master/documents/add_config_of_rp2040.png)
3. Now you can flash RP2040 by click a button.
   
## Acknowledgement
1. This work was inspired by [this tutorial](https://www.digikey.de/en/maker/projects/raspberry-pi-pico-and-rp2040-cc-part-2-debugging-with-vs-code/470abc7efb07432b82c95f6f67f184c0?utm_adgroup=General&utm_source=google&utm_medium=cpc&utm_campaign=Dynamic%20Search_EN_RLSA_Site%20Visitor&utm_term=&productid=&gclid=CjwKCAjwjtOTBhAvEiwASG4bCO5ZrciK4GKWSvYaCMsHhGXzWKo_XRwZu5Jmpcpx9cw1yDE4YP94khoCtvoQAvD_BwE) from [ShawnHymel](https://www.digikey.de/en/maker/profiles/72825bdd887a427eaf8d960b6505adac).
2. Of course, most of the instructions and commands can also be found in the [documentation of RP2040](https://datasheets.raspberrypi.com/rp2040/hardware-design-with-rp2040.pdf).
3. The most important part of enabling the CLion to access the customed `openocd` debugger is to add the `udev` rule. I followed [one answer from StackOverflow](https://stackoverflow.com/questions/41066653/openocd-error-libusb-open-failed-with-libusb-error-not-supported).