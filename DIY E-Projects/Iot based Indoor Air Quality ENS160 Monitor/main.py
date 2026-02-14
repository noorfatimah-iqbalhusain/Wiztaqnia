'''
 IoT_based_Indoor_Air_Quality_ENS160_Monitor
 Made by www.wiztaqnia.com
 Modified date: 22/11/2024
 Typical pin layout used:
 ------------------------------------------------
 Signal           Gravity ENS160     Raspberry        
                  Air Quality           Pi         
                    Sensor            Pico W                  
 ------------------------------------------------
 3.3V                   +             3V3
 GND(Ground)            -             GND
 C(I2C clock line)      C             GP27_A1
 D(I2C data line)       D             GP26_A0      
 '''
from machine import Pin, I2C, ADC
from PiicoDev_ENS160 import PiicoDev_ENS160
from PiicoDev_Unified import sleep_ms
from umqtt.simple import MQTTClient
import network
import utime

# WiFi credentials
WIFI_SSID = "your SSID"
WIFI_PASSWORD = "your password"

# Adafruit IO account credentialss 
AIO_SERVER = "io.adafruit.com"
AIO_PORT = 1883
AIO_USER = "your user name"
AIO_KEY = "your key"
AIO_CLIENT_ID = " Iot_based_Indoor_Air_Quality_ENS160_Monitor" # Name the client ID with anything of your choice
AIO_TOPIC_TEMP = "{}/feeds/temp".format(AIO_USER)    # Set topic for "temp" feed
AIO_TOPIC_AQI = "{}/feeds/aqi".format(AIO_USER)      # Set topic for "aqi" feed
AIO_TOPIC_TVOC = "{}/feeds/tvoc".format(AIO_USER)    # Set topic for "tvoc" feed
AIO_TOPIC_ECO2 = "{}/feeds/eco2".format(AIO_USER)    # Set topic for "eco2" feed

def connect_to_wifi(ssid, password):
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(ssid, password)
    while not wlan.isconnected():
        pass
    print('Connected to WiFi')

# Initialise I2C Communication
i2c = I2C(1, scl=Pin(27), sda=Pin(26), freq=400000)

# Initialise the Gravity ENS160 Air Quality Sensor
monitor = PiicoDev_ENS160()

# Initialize Raspberry Pi Pico W's inbuilt temperature sensor
inbuilt_temp = ADC(4)
conversion_factor = 3.3 / (65535)

# Connect to WiFi Network
connect_to_wifi(WIFI_SSID, WIFI_PASSWORD)

# Connect to Adafruit IO 
client = MQTTClient(AIO_CLIENT_ID, AIO_SERVER, AIO_PORT, AIO_USER, AIO_KEY)
client.connect()

while True:
    # Read from the Gravity ENS160 Air Quality Sensor
    aqi = monitor.aqi
    tvoc = monitor.tvoc
    eco2 = monitor.eco2

    # Read the temperature
    reading = inbuilt_temp.read_u16() * conversion_factor 
    temperature = 27 - (reading - 0.706)/0.001721

    # Publish air quality metrics to Adafruit IO
    client.publish(AIO_TOPIC_TEMP, "{}".format(temperature))      # publish the temperature to the "temp" feed
    client.publish(AIO_TOPIC_AQI, "{}".format(aqi.rating))        # publish the AQI rating to the "aqi" feed
    client.publish(AIO_TOPIC_TVOC, "{} ppb".format(tvoc))         # publish the TVOC value to the "tvoc" feed
    client.publish(AIO_TOPIC_ECO2, "{} ppm".format(eco2.value))   # publish the ECO2 value to the "eco2" feed

    sleep_ms(10000) #Add a time delay enough to update data in real time and avoid Data Throttle Warning.
    
    '''Note:- Adafruit.IO free accounts have a limit of 30 data points per minute.
    If you’re publishing data every 3 seconds, that’s 20 data points per minute for each feed.
    If you have more than one feed, you’ll quickly exceed this limit.
    To avoid this, you can increase the delay between data publications'''
