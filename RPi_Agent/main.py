
import asyncio
import struct
from bleak import BleakScanner, BleakClient
from influxdb_client_3 import Point, InfluxDBClient3

# Define UUIDs for characteristics
CHARACTERISTICS = {
    "pump": ["19B11A01-F8F2-537E-4F6C-D104768A1215", "byte"],
    "water_flow": ["19B11A02-F8F2-537E-4F6C-D104768A1215", "float"],

    "soil_pelargonium": ["19B11B01-F8F2-537E-4F6C-D104768A1215", "int"],
    "soil_kiwi_Boskoop": ["19B11B02-F8F2-537E-4F6C-D104768A1215", "int"],
    "soil_kiwi_Issai": ["19B11B03-F8F2-537E-4F6C-D104768A1215", "int"],
    "soil_impatiens": ["19B11B04-F8F2-537E-4F6C-D104768A1215", "int"],
    "soil_strawberry": ["19B11B05-F8F2-537E-4F6C-D104768A1215", "int"],
    "soil_rosemary": ["19B11B06-F8F2-537E-4F6C-D104768A1215", "int"],

    "temperature": ["19B11C01-F8F2-537E-4F6C-D104768A1215", "float"],
    "temperature2": ["19B11C02-F8F2-537E-4F6C-D104768A1215", "float"],
    "humidity": ["19B11C03-F8F2-537E-4F6C-D104768A1215", "float"],
    "pressure": ["19B11C04-F8F2-537E-4F6C-D104768A1215", "float"],

    "battery_shuntVoltage": ["19B11D01-F8F2-537E-4F6C-D104768A1215", "float"],
    "battery_busVoltage": ["19B11D02-F8F2-537E-4F6C-D104768A1215", "float"],
    "battery_current_mA": ["19B11D03-F8F2-537E-4F6C-D104768A1215", "float"],
    "battery_power_mW": ["19B11D04-F8F2-537E-4F6C-D104768A1215", "float"],

    "load_shuntVoltage": ["19B11E01-F8F2-537E-4F6C-D104768A1215", "float"],
    "load_busVoltage": ["19B11E02-F8F2-537E-4F6C-D104768A1215", "float"],
    "load_current_mA": ["19B11E03-F8F2-537E-4F6C-D104768A1215", "float"],
    "load_power_mW": ["19B11E04-F8F2-537E-4F6C-D104768A1215", "float"],

    "solar_shuntVoltage": ["19B11F01-F8F2-537E-4F6C-D104768A1215", "float"],
    "solar_busVoltage": ["19B11F02-F8F2-537E-4F6C-D104768A1215", "float"],
    "solar_current_mA": ["19B11F03-F8F2-537E-4F6C-D104768A1215", "float"],
    "solar_power_mW": ["19B11F04-F8F2-537E-4F6C-D104768A1215", "float"],
}

# BLE target
TARGET_NAME = "AI.EcoSense"

# InfluxDB credentials
INFLUXDB_URL = "http://localhost:8181"
INFLUXDB_TOKEN = "apiv3_YrDkl5I1T5EZgiIhuSY-ErLkCXqqfsVY-87AzBGSEDoD7ePqEyHYwV_C8rLnTK5EdxAGtTBrClPtlVH5EEx2cw"
INFLUXDB_ORG = "home"
INFLUXDB_DB = "home"

client = InfluxDBClient3(host=INFLUXDB_URL,
                         database=INFLUXDB_DB,
                         token=INFLUXDB_TOKEN,)

async def find_device(name=TARGET_NAME, timeout=30):
    print("Scanning for BLE devices...")
    devices = await BleakScanner.discover(timeout=timeout)
    for device in devices:
        if name in device.name:
            print(f"Found {name} at address {device.address}")
            return device.address
    print("Device not found.")
    return None

def parse_float(data: bytearray) -> float:
    return struct.unpack('<f', data)[0]

def parse_int(data: bytearray) -> int:
    return struct.unpack('<I', data)[0]

def handle_notification(key, *, is_float=False, is_int=False):
    def handler(sender, data):
        if is_float:
            value = parse_float(data)
        elif is_int:
            value = parse_int(data)
        else:
            value = data[0]

        # Store in InfluxDB
        point = (
            Point("ai_garden")
            .tag("room", "balcony")
            .field(key, value)
        )
        client.write(point)
#        print(f"✅ Data written to InfluxDB. {key}")
    return handler

async def subscribe_to_notifications(address):
    try:
        async with BleakClient(address) as client:
            if not client.is_connected:
                 print("❌ Failed to connect.")
                 return

            print("✅ Connected to BLE device. Listening for updates...")

            # Subscribe to characteristics
            for key in CHARACTERISTICS:
                await client.start_notify(CHARACTERISTICS[key][0], handle_notification(
                    key,
                    is_float=True if "float" in CHARACTERISTICS[key][1] else False,
                    is_int=True if "int" in CHARACTERISTICS[key][1] else False)
                )

            while client.is_connected:
                # Get current system time
            	now = datetime.now()
                # Format time into bytes (e.g., Year, Month, Day, Hour, Minute, Second, DayOfWeek)
            	# This is a simplified example; for actual BLE Time Service, you'd follow its spec
            	time_bytes = bytes([
                    now.year - 2000, # Assuming Arduino handles year as offset from 2000
                    now.month,
                    now.day,
                    now.hour,
                    now.minute,
                    now.second,
                    now.weekday() # Monday is 0, Sunday is 6
            	])

                print(f"Writing time {now} ({time_bytes}) to Arduino...")
                await client.write_gatt_char("19B11701-F8F2-537E-4F6C-D104768A1215", time_bytes)
                print("Time written.")

                await asyncio.sleep(10)  # keep running and receiving notifications until connection ends

    # except BleakError as e:
    #     print(f"❌ BLE Error: {e}")
    except Exception as e:
        print(f"💥 Unexpected Error: {e}")

async def main():
    address = await find_device()
    if not address:
        return

    while True:
        await subscribe_to_notifications(address) 
        print("🔁 Attempting reconnection in 5 seconds...")
        await asyncio.sleep(5)

if __name__ == "__main__":
    asyncio.run(main())

