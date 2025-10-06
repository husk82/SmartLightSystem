Smart Light Automation System

A smart lighting project using STM32F446RE, TSL2591 light sensor, ESP8266, and OLED display, paired with a Flask web server for real-time monitoring.

Features

Reads ambient light and adjusts LED brightness via PWM

Displays lux and WiFi status on OLED

Sends lux data to a Flask server running on Raspberry Pi 5

Simple web interface with current data and project description

Hardware

STM32F446RE, TSL2591 sensor, OLED, ESP8266, LEDs

Raspberry Pi 5 for Flask server

Software

STM32 firmware (C)

Flask web server (Python)

Setup

Flash STM32 firmware

Run Flask server on Raspberry Pi

Access data via browser at http://<raspberry_pi_ip>:5000

Future Improvements

Store lux data in PostgreSQL

Add interactive graphs and analytics

Expand multi-sensor support
