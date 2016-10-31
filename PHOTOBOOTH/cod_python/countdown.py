import cv2
import cv
import sys
from datetime import datetime
import time
import RPi.GPIO as GPIO

# Initialize variables
camSource = 0
running = True
saveCount = 0
nSecond = 0
totalSec = 5
strSec = '54321'
keyPressTime = 0.0
startTime = datetime.now()
timeElapsed = 0.0
startCounter = True
endCounter = False
finishStr = 'Smile!'
smileTime=False

GPIO.setmode(GPIO.BOARD)GPIO.P
GPIO.setup(11,GPIO.IN, pull_up_down=PUD_DOWN)
GPIO.setup(7,GPIO.OUT)
GPIO.ouput(7,0)
# Start the camera
camObj = cv2.VideoCapture(camSource)
if not camObj.isOpened():
    sys.exit('Camera did not provide frame.')

frameWidth = int(camObj.get(cv2.cv.CV_CAP_PROP_FRAME_WIDTH))
frameHeight = int(camObj.get(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT))

# Start video stream
while running:
    readOK, frame = camObj.read()
    if smileTime:
        cv2.imshow('video', frame)
        smileTime=False
        time.sleep(2)
    # Display counter on screen before saving a frame
    if startCounter:
        if nSecond < totalSec: 
            # draw the Nth second on each frame 
            # till one second passes  
            cv2.putText(img = frame, 
                        text = strSec[nSecond],
                        org = (int(frameWidth/2 - 30),int(frameHeight/2)), 
                        fontFace = cv2.FONT_HERSHEY_DUPLEX, 
                        fontScale = 6, 
                        color = (255,255,255),
                        thickness = 5, 
                        lineType = cv2.CV_AA)

            timeElapsed = (datetime.now() - startTime).total_seconds()
            print 'timeElapsed: {}'.format(timeElapsed)

            if timeElapsed >= 1:
                nSecond += 1
#                print 'nthSec:{}'.format(nSecond)
                timeElapsed = 0
                startTime = datetime.now()             

        else:
            if nSecond < totalSec+1:
                cv2.putText(img = frame, 
                    text = finishStr,
                    org = (int(frameWidth/5-30),int(frameHeight/2)), 
                    fontFace = cv2.FONT_HERSHEY_DUPLEX, 
                    fontScale = 6, 
                    color = (255,255,255),
                    thickness = 5, 
                    lineType = cv2.CV_AA)
                timeElapsed = (datetime.now() - startTime).total_seconds()
                print 'timeElapsed: {}'.format(timeElapsed)

                if timeElapsed >= 1:
                    nSecond += 1
    #                print 'nthSec:{}'.format(nSecond)
                    timeElapsed = 0
                    startTime = datetime.now() 
            else: 
                cv2.imwrite('img' + str(saveCount) + '.jpg', frame)  
                            
    #            print 'saveTime: {}'.format(datetime.now() - keyPressTime)

                saveCount += 1
                startCounter = False
                nSecond = 1 
                smileTime=True

    # Get user input
    keyPressed = GPIO.input(11)
    if keyPressed == ord('s'):
        print "start counter"
        startCounter = True
        startTime = datetime.now()
        keyPressTime = datetime.now()

    elif cv2.waitKey(3) == ord('q'):
        print "out"
        # Quit the while loop
        running = False
        cv2.destroyAllWindows()

    # Show video stream in a window    
    height, width = frame.shape[:2]
    scaleHeight = 750#int(round(1.3*width))
    scaleWidth = 800#int(round(1.3*height))
    res = cv2.resize(frame,(scaleWidth, scaleHeight), interpolation = cv2.INTER_CUBIC)
    cv2.imshow('video', res)

    cv.ResizeWindow('video', scaleWidth, scaleHeight+20)


camObj.release()