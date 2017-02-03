import pycom
import time
pycom.heartbeat(False)

for cycles in range(50): # stop after 10 cycles
    pycom.rgbled(0x7f0000) # red
    time.sleep(1)
    pycom.rgbled(0x0040ff) # blau
    time.sleep(1)
    
    
