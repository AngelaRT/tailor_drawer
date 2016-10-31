import cv2
import cv
import numpy as np
import serial #cargamos la libreria serial
import threading
import time

#Iniciamos la camara
captura=cv2.VideoCapture(0)
#Iniciamos la comunicacion serial
#ser = serial.Serial('/dev/ttyACM0', 9600)

def showVideo(cap):
   key=0
   print "hola"
   while(key!=27):
      print "hola2"
      flag, frame = cap.read()
      print "frame"
      cv2.putText(frame,"3", (100,80), cv2.FONT_HERSHEY_SIMPLEX, 2, 255)
      print "lalla"
      time.sleep(1)
      cv2.imshow('PON UNA POSE GUAPA!', frame)
      time.sleep(1)

      key = cv2.waitKey(5) & 0xFF
thread = threading.Thread(target=showVideo, args=(captura,))

key=0
numSnapshot=0
first=True
while(key!=27):
   key = cv2.waitKey(5) & 0xFF
   if first:
      first=False
      thread.start()
   #if ser.read()=='s':
    #  print "recibo"
     # flag, frame = captura.read()
      #if flag:
       #  cv2.imshow('ESTA ES TU FOTO!', frame)
        # cv2.imwrite("%d.png"%numSnapshot, frame)
         #numSnapshot+=1
      #else:
       #  print "Try again"

cv2.destroyAllWindows()
#thread.exit()
