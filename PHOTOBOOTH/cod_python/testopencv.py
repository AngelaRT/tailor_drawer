import numpy as np
import cv2

import cv2
import numpy as np

#cv2.putText(frame,"3", (100,80), cv2.FONT_HERSHEY_SIMPLEX, 2, 255)
if __name__ == "__main__":
	captura=cv2.VideoCapture(0)
	flag=0
	numSnapshot=0
	try:
	   while True:
	   	flag, frame = captura.read()
	   	if flag:
	   		print("img")
	   		cv2.imshow('foto{}'.format(numSnapshot), frame)
	   		numSnapshot+=1
	   	else:
	   		print("Try again")
	except KeyboardInterrupt:
		cv2.destroyAllWindows()
