import pygame
#from pygame.locals import *
import time

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
  pygame.draw.rect(screen,pygame.Color("red"),(10,10,770,430),2) #Draw the red outer box
  print("pygame")
  pygame.display.flip()
  print("end")

  return

if __name__ == "__main__":

	Numeral = "" #Numeral is the number display
	Message = "" #Message is a fullscreen message
	f=open("logfile",'w')
	#initialise pygame
	#pygame.mixer.pre_init(44100, -16, 1, 1024*3) #PreInit Music, plays faster
	pygame.init() #Initialise pygame
	screen = pygame.display.set_mode((800,480),pygame.FULLSCREEN) #Full screen 640x480
	background = pygame.Surface(screen.get_size()) #Create the background object
	background = background.convert() #Convert it to a background
	UpdateDisplay(msg="hola")
	time.sleep(1)
	UpdateDisplay()
	for i in reversed(range(1,6)):
		Numeral = str(i)
		UpdateDisplay(num=Numeral)
		print("numeral written")
		print(Numeral)
		time.sleep(1)