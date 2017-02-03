from network import LoRa
import binascii


# Initialize LoRa in LORAWAN mode.
lora = LoRa(mode=LoRa.LORAWAN)
devEUI = binascii.hexlify(lora.mac())

# show DevEUI
print("LoPy DevEUI; ",  devEUI)
