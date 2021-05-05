import RPi.GPIO as GPIO
import time
import numpy as np
import cv2
import threading

frame = None

def steam():
    cap = cv2.VideoCapture("/dev/video0")
    cap.set(5,30)
    while True:
        global frame 
        ret, frame = cap.read()

but_pin = 18  # Board pin 18


def main():
    # Pin Setup:
    GPIO.setmode(GPIO.BOARD)  # BOARD pin-numbering scheme
    GPIO.setup(but_pin, GPIO.IN)  # button pin set as input
    
    x = threading.Thread(target = steam)
    x.start()
    global frame 
    print("Starting demo now! Press CTRL+C to exit")
    try:
        while True:
                print("Waiting for button event")            
                GPIO.wait_for_edge(but_pin, GPIO.FALLING)
                # event received when button pressed
                print("Button Pressed!")
                cv2.imwrite("alpha.jpg", frame)
                time.sleep(1)
    finally:
        GPIO.cleanup()  # cleanup all GPIOs

if __name__ == '__main__':
    main()
