import cv2
import numpy as np
import serial #cargamos la libreria serial
import threading

import RPi.GPIO as GPIO

#Iniciamos la camara
captura=cv2.VideoCapture(1)
#Iniciamos la comunicacion serial
ser = serial.Serial('/dev/ttyACM0', 9600)

def showVideo(cap):
   key=0
   while(key!=27):
      flag, frame = cap.read()
      cv2.imshow('PON UNA POSE GUAPA!', frame)
      key = cv2.waitKey(5) & 0xFF
thread = threading.Thread(target=showVideo, args=(captura,))

GPIO.setmode(GPIO.BOARD)GPIO.P
GPIO.setup(11,GPIO.IN, pull_up_down=PUD_DOWN)
GPIO.setup(7,GPIO.OUT)
GPIO.ouput(7,0)
prevInput=0
try:
   while True:
      if first:
         first=False
         thread.start()
      currentInput = GPIO.input(11)
      if not prevInput and currentInput:
         print "recibo"
         prevInput=currentInput
         flag, frame = captura.read()
         if flag:
            cv2.imshow('ESTA ES TU FOTO!', frame)
            cv2.imwrite("%d.png"%numSnapshot, frame)
            numSnapshot+=1
         else:
            print "Try again"
except KeyboardInterrupt:
   GPIO.cleanup()
# key=0
# numSnapshot=0
# first=True
# while(key!=27):
#    key = cv2.waitKey(5) & 0xFF
#    if first:
#       first=False
#       thread.start()
#    if ser.read()=='s':
#       print "recibo"
#       flag, frame = captura.read()
#       if flag:
#          cv2.imshow('ESTA ES TU FOTO!', frame)
#          cv2.imwrite("%d.png"%numSnapshot, frame)
#          numSnapshot+=1
#       else:
#          print "Try again"

cv2.destroyAllWindows()
#thread.exit()