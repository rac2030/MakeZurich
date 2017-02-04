import serial
import time
import datetime

promini = '/dev/cu.SLAB_USBtoUART'
ser = serial.Serial(promini, 9600)
 
repeatTime = 1000 # milliseconds

def writeData(value):
    # Get the current data
    today = datetime.date.today() 
    
    # Open log file 2012-6-23.log and append
    with open(str(today)+'.log', 'ab') as f: 
        
        f.write(value) 
        # Write our integer value to our log
        
        f.write('\n') 
        # Add a newline so we can retrieve the data easily later, could use spaces too.

timer = time.time() # Timer to see when we started
while True:
    time.sleep(0.25) # sleep for 250 milliseconds
    if time.time() - timer > repeatTime:
    # If the current time is greater than the repeat time, send our 'get' command again 
        serial.write("t\n")
        timer = time.time() # start the timer over
        
    if ser.inWaiting() > 2: # if we have \r\n in there, we will have two characters, we want more!
        value = serial.read() # Read the data from the 
        value = value.strip('\r\n') # Arduino will return DATA\r\n when println is used 
        
        try:
            value = int(value) #This is likely where you will get 'duff' data and catch it.
            writeData(value) # Write the data to a log file
            
        except:
            serial.write("t\n") # Try it again!