# AI.EcoSense RPi Agent

The AI.EcoSense RPi Agent collects sensor data from an Arduino Nano 33 BLE Sense rev2 and stores it in InfluxDB, visualized via Grafana.

## 🌿 Overview

The RPi Agent is a crucial part of the AI.EcoSense system, responsible for:

- Collecting environmental data (temperature, humidity, barometric pressure) and soil moisture data.

- Transmitting this data wirelessly via Bluetooth Low Energy (BLE).

- Storing the collected time-series data in InfluxDB.

- Enabling data visualization through Grafana dashboards.

## 🚀 Deployment Steps

The following steps will guide you through setting up the AI.EcoSense RPi Agent. Ensure you are logged in as the pi user on your Raspbian system for all operations.

1. Clone the Repository

```bash
git clone https://github.com/markub3327/AI.EcoSense.git
cd AI.EcoSense
```

2. Set up Docker Environment

   The project uses Docker to manage InfluxDB and Grafana services.

An external hard drive can represent these paths.
```bash
mkdir -p /mnt/data0/influxdb/data
mkdir -p /mnt/data0/influxdb/config
mkdir -p /mnt/data0/grafana
```

4. Run Docker Containers

Navigate to the directory containing docker-compose.yml and start the services:
```bash
cd RPi_Agent/influxdb-docker-server # Make sure you are in the root of the cloned repository
docker-compose up -d
```
This command will start InfluxDB 3 Enterprise on port 8181 and Grafana on port 3000.

5. Install Python Dependencies

The main.py script requires specific Python packages. Install them locally using pip:
```bash
sudo apt install -y python3-pip
pip3 install bleak influxdb-client-3
```

6. Configure and Run BLE Gateway Service

The main.py script acts as the BLE gateway, connecting to the Arduino and sending data to InfluxDB.

```bash
sudo nano /etc/systemd/system/BLE_IoT_hub.service
```

Add the following content to the file:
```ini
[Unit]
Description=AI.EcoSense BLE Gateway Service
After=network.target

[Service]
User=pi
Group=pi
WorkingDirectory=/home/pi/AI.EcoSense
ExecStart=/usr/bin/python3 /home/pi/AI.EcoSense/main.py
Restart=always
RestartSec=5

[Install]
WantedBy=multi-user.target
```

Reload the systemd daemon, enable, and start the service:
```bash
sudo systemctl daemon-reload
sudo systemctl enable BLE_IoT_hub.service
sudo systemctl start BLE_IoT_hub.service
```

You can check the status of the service using:
```bash
sudo systemctl status ecosense-ble-gateway.service
journalctl -u ecosense-ble-gateway.service -f
```

## ⚠️ Troubleshooting

 - **Docker Containers don't start**: Check docker-compose logs for errors. Ensure port 8181 and 3000 are not in use by other processes.

 - **BLE Device isn't found**: Ensure your Arduino Nano 33 BLE Sense is powered on and advertising. Verify TARGET_NAME in main.py matches your device's advertised name.

 - **InfluxDB connection issues**: Double-check INFLUXDB_URL, INFLUXDB_TOKEN, INFLUXDB_ORG, and INFLUXDB_DB in your **.env** file and **main.py**. Ensure InfluxDB container is running and accessible.

 - **main.py errors**: Check the **journalctl** output for Python traceback errors. Ensure all dependencies are installed.

