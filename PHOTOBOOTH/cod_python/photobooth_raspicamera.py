from picamera import PiCamera
from time import sleep
from gpiozero import Button
import pygame
from pygame.locals import *
from threading import Thread
from time import sleep
import time
import sys

def UpdateDisplay(num="",msg=""):
   #init global variables from main thread
  global SmallMessage
  global TotalImageCount
  global screen
  global background
  global pygame
  global f

  background.fill(pygame.Color("black"))
  elem=""
  font = pygame.font.Font(None, 0)
  if(len(num)>0): # Else if the number exists display it
    font = pygame.font.Font(None, 800)
    elem=num
  elif(len(msg)>0): #If the big message exits write it
    font = pygame.font.Font(None, 180)
    elem=msg
  print("font")
  text = font.render(elem, 1, (255,0,0))
  print("text")
  textpos = text.get_rect()
  textpos.centerx = background.get_rect().centerx
  textpos.centery = background.get_rect().centery
  print("textpos")
  background.blit(text, textpos)
  print("background")
  screen.blit(background, (0,0))
  print("screen")
  #pygame.draw.rect(screen,pygame.Color("red"),(10,10,770,430),2) #Draw the red outer box
  print("pygame")
  pygame.display.flip()
  print("end")

  return

#def main(threadName, *args):
	# global closeme
	# global timepulse
	# global Numeral
	# global Message
	# global f
if __name__ == "__main__":

	button = Button(22)
	Numeral = "" #Numeral is the number display
	Message = "" #Message is a fullscreen message
	f=open("logfile",'w')
	#initialise pygame
	#pygame.mixer.pre_init(44100, -16, 1, 1024*3) #PreInit Music, plays faster
	pygame.init() #Initialise pygame
	screen = pygame.display.set_mode((1280,1024),pygame.FULLSCREEN) #Full screen 640x480
	background = pygame.Surface(screen.get_size()) #Create the background object
	background = background.convert() #Convert it to a background

	print("ini")
	Message = "Loading..."
	UpdateDisplay(msg=Message)
	print("waiting 5")
	time.sleep(5) #5 Second delay to allow USB to mount
	print("awaken")
	camera = PiCamera()
	print("initizalized")
	#Transparency allows pigame to shine through
	camera.preview_alpha = 200
	camera.vflip = False
	camera.hflip = True
	camera.rotation = 90
	camera.brightness = 45
	camera.exposure_compensation = 6
	camera.contrast = 8
	camera.resolution = (2592,1944)#(1280,1024)
	print("start camera")
	camera.start_preview()
	Message = ""
	UpdateDisplay()
	print("clean screen")
	t1=time.time()
	frame = 1
	while True: 
		if time.time()-t1 > 30:
			camera.stop_preview()
			break
		try:
			if time.time()-t1 > 5:
				t1=time.time()
				#button.wait_for_press()
				Message=""
				for i in reversed(range(1,6)):
					Numeral = str(i)
					UpdateDisplay(num=Numeral)
					timepulse = 0.1*i
					print("numeral written")
					print(Numeral)
					time.sleep(1)
				Numeral=""
				Message="PAA TAA TAA"
				UpdateDisplay(msg=Message)
				time.sleep(2)
				camera.capture('/home/pi/tests/imagen%03d.jpg' % frame)
				frame += 1
				Message = ""
				UpdateDisplay()
				timepulse=999

			print(time.time())

			print(t1)			    
		except KeyboardInterrupt:
			camera.stop_preview()
			f.close()
			break
	f.close()
#Thread(target=main, args=('Main',1)).start()
time.sleep(5)
