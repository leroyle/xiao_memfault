
# NOTE: Major bug found, FreeRTOS systick timer is broken. This will still demo fault data capturing, but tasks do not get 
switched as one expects.

## NOTE: This is a PlatformIO project work in progress.
### NOTE: Throughout the course of developing this project we on several occasions have blown out bootloader code which has required a reload of the boot loader. This due to our experimenting, not the fault of the Memfaul SDK. If using this project be prepared for the same. This may require reloading via the device SWD/J-Link interface.

#### A simple example implementation of Memfault.com (remote monitoring for Embedded devices)
Memfault is a framework that allows remote monitoring of your embedded devices.

Our main interest is in its ability to upload core dump data for analysis on the cloud platform. While typically the data is uploaded via a WiFi or BLE connection I'm thinking/hoping we can upload at least limited function trace back info via LoRaWAN. This remains to be seen.

One of several Memfault videos:  https://www.youtube.com/watch?v=ge-ebleCi8o

We have been messing with it on the Seeed Studio Xiao BLE as well as the RAK Wisblock dev board for a couple of weeks. At this point just using a very simple "hello world" application using PlatformIO.

#### Integrating Memfault into this project
The folks at Memfault have provides guides for integrating their SDK into your microcontoller environment (https://docs.memfault.com/docs/mcu/introduction). While these docs did not specifically mention our target nRF52 using nRF SDK with the provided example and the docs we were able to get things working.

Our approach was to start with their SDK provided example found at
```
 ~/Memfault_SDK_INSTALL_DIR/examples/nrf5/apps/memfault_demo_app
```
Once that was working we then merged that with our PlatformIO Adafruit framework used by the target dev boards.

Memfault on nrf52 uses the Nordic NRF SDK and lately is now supporting the newer Nordic nRFConnect SDK. Both of the nRF52 boards we are using are using the Adafruit nRF52 implementation for runtime support.
 
We do have some Memfault/Nordic API collisions so the build is a customized one. The Memfault code is built against the Nordic SDK include files, while the Adafruit runtime uses its own version of Nordic support files.

There are copy scripts that will copy in the appropriate files thus this example requires minimal hand editing of the Memfault or Adafruit library files, though there are a few.

As mentioned our work is still in the works and only within a very simple Hello World app.

### Disclaimer: the folks at Memfault have not condoned this approach. this approach may prove to fall apart within a real life application. It would be best to reconcile things such that all pieces of your app use the same version of common API calls...  
If anyone is interested in poking at this do let me know. We need to clean up the repo a bit, but what we have is at:
https://github.com/leroyle/xiao_memfault


#### To use this repo, 
##### NOTE This assumes you already have VSCode and it's PlatformIO extension installed as well as a functional Hello World app to ensure the base products are installed correctly. The following product versions were used for this exercise:

Dependent software/versions used

Ubuntu Linux version 20.04

VSCode - version 1.74.2

PlatformIO Core - version 6.1.6

PlatformIO framework version
* framework-arduinoadafruitnrf52 - version: 1.10300.0 (1.3.0)

PlatformIO Platform
* nordicnrf52 - version: 9.4.0

GNU gcc arm compiler
* toolchain-gccarmnoneeabi @ 1.70201.0 (7.2.1)

Memfault SDK
* version BUILD ID: 1184, GIT COMMIT: b60ef08d3

Memfault CLI
* used to upload core data to the Memfault cloud (Note requires Python 3.x

Seeed Studio Xiao BLE Bootloader Version:
   * UF2 Bootloader 0.6.1 lib/nrfx (v2.0.0) lib/tinyusb (0.10.1-293-gaf8e5a90) lib/uf2 (remotes/origin/configupdate-9-gadbb8c7)
   * Model: Seeed XIAO nRF52840
   * Board-ID: Seeed_XIAO_nRF52840_Sense
   * SoftDevice: S140 version 7.3.0
   * Date: Nov 12 2021

RAK Wireless Wisblock Bootloader Version:
   * UF2 Bootloader 0.4.2 lib/nrfx (v2.0.0) lib/tinyusb (0.10.1-293-gaf8e5a90) lib/uf2 (remotes/origin/configupdate-9-gadbb8c7)
   * Model: WisBlock RAK4631 Board
   * Board-ID: WisBlock-RAK4631-Board
   * Date: Dec  1 2021
   * SoftDevice: S140 6.1.1

There may be other dependencies that we have satisfied long ago and are not directly aware of.

https://docs.memfault.com/docs/ci/install-memfault-cli/

#### TO use this sample
* ensure the above dependent software items have been installed and a simple "Hello World" builds and executes
* clone the repository to a local directory
* inspect the following two copy scripts, modify the source and destination directories set in these scripts as appropriate for your installation
- copy scripts located within the project at
```
assets/MemFault/cpNrfSrcFiles
assets/MemFault/cpMemfltSrcFiles
```

- after the directories have been modified execute the two scripts. Note and fix any reported error messages.
- to build the app, there are 6 defined targets

These PlatformIO build targets will create an app that supports a command line interface. Once the terminal/JLinkRTTClient is running, type in "help" for a list of commands.
Note: The Memfault cli targets do not support input backspace. If you mistype a command, just hit enter and try again.

#### Targets for RAK Wisblock
* wisblock_term
   * A Memfault implementation that uses a virtual serial terminal (putty etal) as input/output
* wisblock_rtt  
   *  A Memfault implementation that uses the micro's RTT interface and depends on having a Segger Debug module, Segger JLinkRTTClient software and ad connected  debugger GDB or Segger Ozone
* wisblock_cli
   *  A custom command line interface that does "NOT" use Memfault functionality at all. Here just for grins.

#### Targets for Seeed Studio Xiao BLE
* xiao_term
   * A Memfault implementation that uses a virtual serial terminal (putty etal) as input/output
* xiao_rtt
   * A Memfault implementation that uses the micro's RTT interface and depends on having a Segger Debug module, Segger JLinkRTTClient software and ad connected  debugger GDB or Segger Ozone
* xiao_cli
   * A custom command line interface that does "NOT" use Memfault functionality at all. Here just for grins.

#### to build the wisblock_term target
```
pio run -e wisblock_term 2>&1 | tee build_log
```
#### To upload the build target
```
pio run -e wisblock_term -t upload 2>&1 | tee build_log
```


There are three versions of the target defined within the platformio.ini file

* _term - a Memfault supplied command line interface that supports the Memfault functionality.  This uses a virtual serial terminal as the input/output source
 
* _rtt - a Memfault supplied command line interface that supports the Memfault functionality.  This the Segger Real Time Transfer (RTT) protocol which requires 
  * Segger J-Link debug device connected via the boards SWD interface
  * Segger J-Link terminal client software 
  * A debugger connected to the target platform, either GDB or Ozone. For this implementation we use Segger Ozone

* _cli - just a simple command line interface that does "not" use Memfault functionality.  This uses a virtual serial terminal as the input/output source




##### The RAK Wireless Wisblock defined targets
```
wisblock_rtt  
wisblock_term  
wisblock_cli  
```

##### The Seeed Studio Xiao BLE defined targets
```
xiao_rtt  
xiao_term  
xiao_cli  
```
 defined targets
95
xiao_rtt  
96
xiao_term  
97
xi
#### Help output from _term targets
```
t> help
clear_core: Clear an existing coredump
drain_chunks: Flushes queued Memfault data. To upload data see https://mflt.io/posting-chunks-with-gdb
export: Export base64-encoded chunks. To upload data see https://mflt.io/chunk-data-export
get_core: Get coredump info
get_device_info: Get device info
test_assert: Trigger memfault assert
test_busfault: Trigger a busfault
test_hardfault: Trigger a hardfault
test_memmanage: Trigger a memory management fault
test_usagefault: Trigger a usage fault
test_log: Writes test logs to log buffer
test_log_capture: Trigger capture of current log buffer contents
test_reboot: Force system reset and track it with a trace event
test_trace: Capture an example trace event
help: Lists all commands
mflt>
```
#### Help output from _rtt targets
```
rtt_cli:~$ help
Available Memfault Commands:
crash: trigger a crash
test_log: writes test logs to log buffer
trigger_logs: trigger capture of current log buffer contents
trace: capture trace event
core_storage_test: verify coredump storage implementation with test patterns
clear_core: clear the core
get_core: gets the core
get_device_info: display device information
reboot: reboots system and tracks it with a trace event
export: Can be used to dump chunks to console or post via GDB
hang: Force a hang to test software watchdog functionality
rtt_cli:~$ 
```

#### Help output from _cli targets
```
* help
        Print list of commands
 * get-led
        Get led status
 * get-adc
        Read adc value
 * hello
        Print hello message
 * stackCheck
        Dump Stack Statistics
 * heapCheck
        Dump Heap Statistics
 * appVersion
        Get App Version String
> 
```


####Testing the implementation
Execution:

* At the "mflt>" prompt enter "test_hardfault".  
* The serial terminal will freeze for a few seconds and terminate.
* At this point restart the serial terminal.
  * At the next "mflt>" enter "get_core".
  * This should show a "Has coredump with size: XXXX  ?"
* Now enter the cli "export" command.
* This will result in a bunch of base64 strings being dumped to the serial terminal. 
* Copy the dumped data (or use the virtual terminal's log file) to a file of any name, for instance "log".

Within the projects assets directory is a python script that can be used to upload the data to the Memfault cloud. One need not parse the terminal log file, you can upload the entire file, the cloud processing will handle the decoding.
The file:
```
assets/MemFault/postLogtoCloud
```
will need to be edited to add your own Memfault supplied "project-key", see a note in the file. This should be obtained when you create an account/device on the Memfault cloud.

once you have dumped the data via "export" another "get_core" the cli should report "No coredump present!".

Memfault has much more functionality that uploading core dump data, this demo only scratches the surface of it's capability. Refer to the main Memfault site at www.memfault.com or Memfault videos on YouTube for more details.



