#main
#Made by www.wiztaqnia.com
#Modified date 17/05/2024
from machine import Pin,Timer
led = machine.Pin('LED', machine.Pin.OUT)
blink=True
clock=Timer()

def basic(timer):
    global led, blink
    blink=not blink
    led.value(blink)
    
clock.init(freq=1,mode=Timer.PERIODIC,callback=basic)
