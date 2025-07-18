# AI.EcoSense - Arduino Component 🌿

## Overview

The Arduino component of AI.EcoSense serves as the edge sensor node for the smart home and garden automation system. Built around the Arduino Nano 33 BLE Sense rev2, this component handles real-time environmental monitoring, soil moisture sensing, and wireless data transmission to the Raspberry Pi hub via Bluetooth Low Energy.

## Hardware Platform

### Primary Controller
- **Arduino Nano 33 BLE Sense rev2**: Main microcontroller with integrated sensors and BLE connectivity

### Integrated Sensors
- **HS3003**: High-precision temperature and humidity sensor
- **LPS22HB**: Barometric pressure sensor
- **Additional GPIO**: For connecting external sensors


## Features

### 🌡️ Environmental Monitoring
- **Temperature & Humidity**: Precise readings using integrated HS3003 sensor
- **Barometric Pressure**: Atmospheric pressure monitoring via LPS22HB
- **Soil Moisture**: Multi-point soil moisture measurement

### 📡 Wireless Communication
- **Bluetooth Low Energy**: Energy-efficient data transmission using NINA-B306
- **Real-time Data Streaming**: Continuous sensor data broadcasting
- **Low Power Operation**: Optimized for battery-powered deployment

### ⚡ Power Management
- **Battery Monitoring**: Built-in power consumption tracking
- **Solar Charging Support**: Compatible with solar power systems

### 🔧 Smart Automation
- **Automated Watering Logic**: Local decision-making for irrigation control
- **Threshold-based Alerts**: Configurable sensor thresholds
- **Fail-safe Operations**: Local backup systems for critical functions

