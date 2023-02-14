
# NOTE: This is a PlatformIO project work in progress

#### A simple example implementation of Memfault.com (remote monitoring for Embedded devices)
Memfault is a framework that allows remote monitoring of your embedded devices.

My main interest is in its ability to upload core dump data for analysis on the cloud platform. While typically the data is uploaded via a wifi or BLE connection I'm thinking/hoping we can upload at least limited function trace back info via LoRaWAN. This remains to be seen.

One of several Memfault videos:  https://www.youtube.com/watch?v=ge-ebleCi8o

I've been messing with it on the Seeed Studio Xiao BLE as well as the RAK Wisblock dev board for a couple of weeks. At this point just using a very simple "hello world" application using PlatformIO.

Memfault on nrf52 uses the Nordic NRF SDK and lately is now supporting the newer Nordic nRFConnect SDK. Both of the nRF52 boards I'm using are using the Adafruit nRF52 implementation for runtime support.
 
We do have some Memfault/Nordic API collisions so the build is a customized one. The Memfault code is built against the Nordic SDK include files, while the Adafruit runtime uses its own version of Nordic support files.

There are copy scripts that will copy in the appropiate files thus this example requires minimal hand editing of the Memfault or Adafruit library files, though there are a few.

As memtioned my work is still in the works and only within a very simple Hello World app. If anyone is interested in poking at this do let me know. I need to clean up the repo a bit, but what I have is at:
https://github.com/leroyle/xiao_memfault


#### To use this repo, 
##### NOTE This assumes you already have VSCode and it's PlatformIO extension installed as well as a functional Hello World app to ensure the base products are installed correctly. The following product versions were used for this exercise:
Dependent software versions used

VSCode - version 1.74.2

PlatformIO Core - version 6.1.6

PlatformIO framework version
- framework-arduinoadafruitnrf52 - version: 1.10300.0 (1.3.0)

PlatformIO Platform
- nordicnrf52 - version: 9.4.0

gcc compiler - toolchain-gccarmnoneeabi @ 1.70201.0 (7.2.1)

#### TO use this sample
- ensure the above dependent software items have been installed and a simple "Hello World" builds and executes
- clone the repository to a local directory
- inspect the copy scripts at
```
assets/MemFault/cpNrfSrcFiles
assets/MemFault/cpMemfltSrcFiles
```
-- inspect the two above copy scripts and modify the source and destination directories as appropriat for your installation

- after the directories have been modified execute the two scripts. Note and fix any reported error messages.
- to build the app, there are 6 defined targets

These PlatformIO build targets will create an app that supports a command line interface. Once the terminal/JLinkRTTClient is running, type in "help" for a list of commands.
Note: The Memfault cli targets do not support input backspace. If you mistype a command, just hit enter and try again.

#### Targets for RAK Wisblock
-- wisblock_term - a Memfault implementation that uses a virtual serial terminal (putty etal) as input/output
-- wisblock_rtt  - a Memfault implementation that uses the micro's RTT interface and depends on having a Segger Debug module, Segger JLinkRTTClient software and ad connected  debugger GDB or Segger Ozone
-- wisblock_cli - a custom command line interface that does "NOT" use Memfault functionality at all. Here just for grins.

#### Targets for Seeed Studio Xiao BLE
-- xiao_term - a Memfault implementation that uses a virtual serial terminal (putty etal) as input/output
-- xiao_rtt  - a Memfault implementation that uses the micro's RTT interface and depends on having a Segger Debug module, Segger JLinkRTTClient software and ad connected  debugger GDB or Segger Ozone
-- xiao_cli - a custom command line interface that does "NOT" use Memfault functionality at all. Here just for grins.

#### to build the wisblock_term target
```
pio run -e wisblock_term 2>&1 | tee build_log
```
#### To upload the build target
```
pio run -e wisblock_term -t upload 2>&1 | tee build_log
```


There are three versions of the target defined within the platformio.ini file

* _term - a Memfaul supplied command line interface that supports the Memfault functionality.  This uses a virtual serial terminal as the input/output source
 
* _rtt - a Memfaul supplied command line interface that supports the Memfault functionality.  This the Segger Real Time Transfer (RTT) protocol which requires 
  * Segger J-Link debug device connected via the boards SWD interface
  * Segger J-Link terminal client software 
  * A debugger connected to the target platform, either GDB or Ozone. For this implementation I use Segger Ozone

* _cli - just a simple command line interface that does "not" use Memfault functionality.  This uses a virtual serial terminal as the input/output source




The defined targets

wisblock_rtt  
wisblock_term  
wisblock_cli  

xiao_rtt  
xiao_term  
xiao_cli  

#### Help output from _term targets
```
t> help^M
clear_core: Clear an existing coredump^M
drain_chunks: Flushes queued Memfault data. To upload data see https://mflt.io/posting-chunks-with-gdb^M
export: Export base64-encoded chunks. To upload data see https://mflt.io/chunk-data-export^M
get_core: Get coredump info^M
get_device_info: Get device info^M
test_assert: Trigger memfault assert^M
test_busfault: Trigger a busfault^M
test_hardfault: Trigger a hardfault^M
test_memmanage: Trigger a memory management fault^M
test_usagefault: Trigger a usage fault^M
test_log: Writes test logs to log buffer^M
test_log_capture: Trigger capture of current log buffer contents^M
test_reboot: Force system reset and track it with a trace event^M
test_trace: Capture an example trace event^M
help: Lists all commands^M
mflt>
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




