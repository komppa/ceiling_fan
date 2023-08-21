'''
    This script takes wifi credentials from wifi_credentials.ini file and
    passes it to the compiler as a definition.
'''


Import("env")
import configparser

wifi_credentials = open('wifi_credentials.ini', 'r').read().strip()

config = configparser.ConfigParser()
config.read('wifi_credentials.ini')

ssid = config['WIFI']['SSID']
password = config['WIFI']['PASSWORD']

server_address = config['MQTT']['ADDRESS']

if ssid == '':
    raise Exception('WiFi SSID is not set in wifi_credentials.ini')

if password == '':
    print('WARNING: WiFi password is not set in wifi_credentials.ini')

env.Append(CPPDEFINES=[
    ("WIFI_SSID", "\\\"{}\\\"".format(ssid))
])

env.Append(CPPDEFINES=[
    ("WIFI_PASSWORD", "\\\"{}\\\"".format(password))
])

env.Append(CPPDEFINES=[
    ("MQTT_SERVER_ADDRESS", "\\\"{}\\\"".format(server_address))
])