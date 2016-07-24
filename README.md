# Circular Clock with an ESP8266 and a WS2812B led ring

This is code to make an "analog" clock from an ESP8266 module and a WS2812b led
ring.

## Wiring

* Power needs to go to the led ring (I use 3.3v and it works)
* Data line is connected from GPIO3 (D4 on the NodeMCU unit)
* GPIO16 (D0 on NodeMCU) is connected to reset for deep sleep

## Operation

The unit connects to a fixed WiFi network and gets time from an NTP server
(with SNTP protocol). It then sets the leds as for an analog clock with a red led for the hour and a blue led for the minutes. It then goes to sleep until the time
comes to change the leds position.

## Build

This uses [platform.io](http://platform.io/), you need to install that first.

* Copy src/config.h.example to src/config.h
* Edit the src/config.h to the values you need: access point name, password and ntp server (usually your router)
  You can use the existing default for the ntp server but it will be slower since it is a random server in a large pool rather  than your local router on your network.
* Actual build: `make build`
* Upload sketch to esp8266: `make upload`
