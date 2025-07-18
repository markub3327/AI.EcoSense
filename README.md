# AI.EcoSense 🌿 Smart Home & Garden Automation System

[![Latest Release](https://img.shields.io/github/release/markub3327/AI.EcoSense)](https://github.com/markub3327/AI.EcoSense/releases)
[![Open Issues](https://img.shields.io/github/issues/markub3327/AI.EcoSense)](https://github.com/markub3327/AI.EcoSense/issues)
[![Weekly Commits](https://img.shields.io/github/commit-activity/w/markub3327/AI.EcoSense)](https://github.com/markub3327/AI.EcoSense)
[![Repository Size](https://img.shields.io/github/repo-size/markub3327/AI.EcoSense)](https://github.com/markub3327/AI.EcoSense)

## 🌱 Overview

AI.EcoSense is an open-source project designed to simplify garden care using Arduino Nano 33 BLE Sense and Raspberry Pi. The system automates plant watering, monitors environmental conditions, and provides real-time data visualization through a Dockerized environment.

## 🔧 System Architecture

| Component         | Specification                           |
|-------------------|-----------------------------------------|
| Operating System  | Raspbian 12 (bookworm)                  |
| Main Controllers  | • Raspberry Pi 4                        |
|                   | • Arduino Nano 33 BLE Sense rev2        |
| Communication     | NINA-B306 Bluetooth Module              |
| Sensors           | • HS3003 (Temperature & Humidity)       |
|                   | • LPS22HB (Barometric Pressure)         |
|                   | • Soil Moisture Sensors (8595193516237) |

## 📊 Features

### Environmental Monitoring
- Real-time temperature and humidity monitoring (HS3003)
- Barometric pressure tracking (LPS22HB)
- Soil moisture measurement
- Water level monitoring

### Smart Automation
- Automated watering system based on soil moisture
- BLE wireless communication
- Docker-based deployment
- Data visualization through Grafana dashboards

## 🐳 Software Stack
- InfluxDB for time-series data storage
- Grafana for data visualization
- Custom BLE gateway service
- Containerized deployment with Docker

## 🚀 Getting Started
1. Set up the hardware components
2. Deploy Docker services on Raspberry Pi
3. Configure the BLE gateway
4. Import Grafana dashboards

## 🤝 Contributing
Contributions are welcome! Please feel free to submit issues and pull requests.

## 📄 License
This project is licensed under the MIT License - see the LICENSE file for details.