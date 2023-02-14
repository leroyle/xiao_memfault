

RE: Memfault.com remote monitoring for Embedded devices
The good folks at Memfault.com have opened up thier SDK and cloud platform 
for eval purposes.
Memfault is a framework that allows remote monitoring of your embedded devices.

My main interest is in its ability to upload core dump data for analysis on 
the cloud platform. While typically the data is uploaded via a wifi or BLE connection I'm thinking/hoping we can upload at least limited function trace back info via LoRaWAN. This remains to be seen. 

One of several videos:  https://www.youtube.com/watch?v=ge-ebleCi8o

I've been messing with it on the Seeed Studio Xiao BLE as well as the RAK
 Wisblock dev board for a couple of weeks. At this point just using a very 
simple "hello world" application using PlatformIO.

Memfault on nrf52 uses the Nordic NRF SDK and lately is now supporting the 
newer Nordic nRFConnect SDK. Both of the nRF52 boards I'm using are using the
 Adafruit nRF52 implementation for runtime support.
 
We do have some Memfault/Nordic API collisions so the build is a customized one.
The Memfault code is built against the Nordic SDK include files, while the 
Adafruit runtime uses its own version of Nordic support files.

There are copy scripts that will copy in the appropiate files thus this 
example requires minimal hand editing of the Memfault or Adafruit library
 files, though there are a few.

As memtioned my work is still in the works and only within a very simple Hello World app. If anyone is interested in poking at this do let me know. I need to clean up the repo a bit, but what I have is at: 
https://github.com/leroyle/xiao_memfault

