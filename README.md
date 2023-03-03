# Human Detection with an ESP32-cam
## _AI Security Cam ESP32-cam(s)_



Initial plan was to connect a motion detector to an ESP32cam as a trigger to capture and send an image to a local MQTT broiker however given the amount of false-alarm a second ESP32-cam was repurposed to serve as a human detector which will send a signal to a WiFi connected ESP32-cam to start image capture.

## Features
- ESP32cam ML
- - Recognize humans in an ESP32cam camera frame, turn on FLASH LED indicating recognition, send LOW signal to GPIO
- - Turn on FLASH LED indicating recognition
- - Send LOW signal to GPIO 12

- ESP32 PB
- - Connect to WiFi and MQTT broker on boot
- - Listen to GPIO 12 for LOW signal (from push-button, detector, sensor etc)
- - Turn ON FLASH LED, capture IMAGE, turn light OFF
- - Send image to MQTT broker

## Installation

Install the MQTT PubSubClient and WiFi libraries for ESP32cam_PB

```sh
#include <PubSubClient.h>
#include <WiFi.h>
```

Install tesnsorflow lite libraries and esp32 camera handling libraries in the Eloquent packages for the ESP32cam_ML

```sh
#include "EloquentTinyML.h"
#include "eloquent.h"
```
