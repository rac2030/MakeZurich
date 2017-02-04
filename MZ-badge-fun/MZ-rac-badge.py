from microbit import *
import radio
import random
import music

# update this user number  or enable badge for staff
# user = 'U1234'
user = 'STAFF'

# Create the "flash" animation frames. Can you work out how it's done?
flash = [Image().invert()*(i/9) for i in range(7, -1, -1)]
must_send = random.randint(60000, 180000)   # forced flash at random delay
cnt_must_send = 0
# cnt_msg_send = 0


def display_msg(x, msg):
#    cnt_msg_send = 0
    while True:
        buzzer()
        display.scroll(x, delay=100, wait=True, loop=False)
#        if cnt_msg_send != 5:
#            radio.send_bytes(msg)
#            cnt_msg_send = cnt_msg_send + 1
        if button_b.was_pressed():
            msg = ''
            msg_u = ''
            break


def buzzer():
    for freq in range(880, 1760, 16):
        music.pitch(freq, 4)
    for freq in range(1760, 880, -16):
        music.pitch(freq, 4)


# start here
radio.on()
radio.config(channel=43, queue=10, length=128, power=7, data_rate=radio.RATE_2MBIT)


while True:
    cnt_must_send = cnt_must_send + 1
    if must_send == cnt_must_send:
        radio.send_bytes('flash')
        cnt_must_send = 0
        must_send = random.randint(60000, 180000)   # forced flash at random delay
    # Button A sends a "flash" message.
    if button_a.was_pressed():         #
        radio.send_bytes('BGDG Zürich was here')
        display.show(Image.DUCK)
        cnt_must_send = 0
        display.set_pixel(2, 1, 5)
        sleep(100)
        display.set_pixel(2, 1, 0)
    if button_b.was_pressed():         #
        radio.send_bytes('BMake a break and have a kitkat')
        display.show(Image.GHOST)
        cnt_must_send = 0
        display.set_pixel(2, 1, 5)
        sleep(100)
        display.set_pixel(2, 1, 0)
    if accelerometer.was_gesture('shake'):
        radio.send_bytes('BShake your body and dance around')
        display.show(Image.PACMAN)
        cnt_must_send = 0
        display.set_pixel(2, 1, 5)
        sleep(100)
        display.set_pixel(2, 1, 0)
    msg = radio.receive_bytes()
    if not msg:
        display.set_pixel(2, 2, 3)
    else:
        display.set_pixel(2, 3, 5)
        sleep(100)
        display.set_pixel(2, 3, 0)
        msg_u = str(msg, 'utf8')
        msg_u_len = 0
        msg_u_len = len(msg_u)
        if msg_u == 'flash':
            sleep(random.randint(50, 550))
            display.show(flash, delay=100, wait=False)
            cnt_must_send = 0
        # Randomly re-broadcast the flash message after a
        # slight delay.
            if random.randint(0, 9) == 0:
                sleep(500)
                radio.send('BMakeZurich has been pawned')         # a-ha
        elif msg_u[0] == 'B':                                            # broadcast
            msg_u = msg_u[1:msg_u_len]
            display_msg(msg_u, msg)
        elif msg_u[0:5] == user:                                         # User or Staff addressed
            msg_u = msg_u[5:msg_u_len]
            display_msg(msg_u, msg)
            # display.scroll(msg_u, delay=100, wait=False, loop=False)   # for debugging