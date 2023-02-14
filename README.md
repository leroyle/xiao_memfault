
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

- download 
