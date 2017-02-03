import network
import time


wlan = network.WLAN(mode=network.WLAN.STA)
for n in wlan.scan():
    print(n.ssid)

ssid = input("ssid: ")
password = input("passwort: ")
wlan.connect(ssid, auth=(network.WLAN.WPA2, password))
while not wlan.isconnected():
    time.sleep_ms(50)
print(wlan.ifconfig())
