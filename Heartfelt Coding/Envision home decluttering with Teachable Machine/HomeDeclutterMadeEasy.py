#HomeDeclutterMadeEasy
#Made by www.wiztaqnia.com
#Modified date 14/01/2024
import cvzone
from cvzone.ClassificationModule import Classifier
import cv2 as cv
import os
import random
import vlc
import time
import numpy as np

def store_labels():           #store the clutter item's labels
labels = {}
with open("Resources/Model/labels.txt", "r") as label:
text = label.read()
lines = text.split("\n")
for line in lines[0:-1]:
hold = line.split(" ", 1)
labels[hold[0]] = hold[1]
return labels
def import_images(directory): #import images from 'Resources' directory
imgList = []
pathList = os.listdir(directory)
for path in pathList:
imgList.append(cv.imread(os.path.join(directory, path),cv.IMREAD_UNCHANGED))            #cv.imread((os.path.join(current folder,current image file path),retain image transparency)
return imgList
cam = cv.VideoCapture(0)
classifier = Classifier('Resources/Model/keras_model.h5', 'Resources/Model/labels.txt') #load the trained model along with the respective clutter item's labels
labels = store_labels()
arrow=cv.imread('Resources/Arrow.png',cv.IMREAD_UNCHANGED)
notification=cv.imread('Resources/Notification.png',cv.IMREAD_UNCHANGED)
clutterList=[]
clutterList=import_images("Resources/Clutter") #import all the clutter items images
binsList=[]
binsList=import_images("Resources/Bins")       #import all the bin images
captions = ['Exude Good Vibes',                #list of captions
'Decluttering ai\'nt no joke',
'Journey is the reward',
'Fantastic !',
'You\'re a Green Hero!',
'Superb !',
'Clean home = clean mind']
matchIndex={0:None,1:0,2:0,3:0,4:1,5:1,6:1,7:2,8:2,9:2} #bin 0=E-Waste;bin 1=ReUse;bin 2=Donate
bin=0
while True:
_, img = cam.read()
imgResize=cv.resize(img,(485,301)) #cv.resize(img,(w,h)) #resize the webcam feed to overlay on the background image
background = cv.imread('Resources/Background.png')
predection = classifier.getPrediction(img)
clutter = np.argmax(predection[0])
classID=predection[1]
if classID!=0:
background=cvzone.overlayPNG(background,clutterList[classID-1],(775,130))                                             #show the clutter item based on the prediction
background = cv.rectangle(background, (775,130), (775+128,130+128),(255,0,255), 1)                                    #draw a pink box around the clutter item image
background=cv.putText(background, labels[str(clutter)], (773, 275), cv.FONT_HERSHEY_COMPLEX_SMALL, 1, (255,0,255), 1) #display the predicted clutter item's label
background=cvzone.overlayPNG(background,arrow,(810,260))
bin=matchIndex[classID]
if bin==matchIndex[classID]: 
ambience = vlc.MediaPlayer('Resources/positive_notification.mp3')
ambience.play()                                                                                        #play audio for pop-up notification
random.shuffle(captions)
text = random.choice(captions)
background = cvzone.overlayPNG(background, notification, (125, 50))
background = cv.putText(background,text, (192,125), cv.FONT_HERSHEY_COMPLEX_SMALL, 1, (0, 255, 255),2) #display random text from the list of captions
time.sleep(0.5)                                                                                        #delay of 0.5 seconds
background=cvzone.overlayPNG(background,binsList[bin],(745,360))
background [209:209+301,122:122+485]=imgResize                                                         #background[x:x+w,y:y+h]=imgResized
cv.imshow('Webcam',background)
cv.waitKey(1)
