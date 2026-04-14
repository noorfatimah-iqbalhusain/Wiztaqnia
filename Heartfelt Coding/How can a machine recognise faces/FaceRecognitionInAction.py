#FaceRecognitionInAction
#Made by www.wiztaqnia.com
#Modified date 25/03/2023
import cv2
import numpy as np
import face_recognition
import os

path= 'People' #'People' directory contains the images of people to be recognised in the webcam feed
pics=[]
NameOfPerson=[]
myList=os.listdir(path)

for i in myList:
    curImg=cv2.imread(f'{path}/{i}')                #cv2.imread(f'{current image present in the path}/{name of the image})
    pics.append(curImg)                             #import current image to pics[]
    NameOfPerson.append(os.path.splitext(i)[0])     #import text before .jpeg to NamesOfPeople

def findEncodings(pics):                            #find encoding for each image in pics[]
    encodeList = []
    for img in pics:
        img=cv2.cvtColor(img,cv2.COLOR_BGR2RGB)     #convert the image from RGB to BGR format (default Color space for OpenCV is BGR)
        encode=face_recognition.face_encodings(img)[0]
        encodeList.append(encode)
    return encodeList

encodeKnown=findEncodings(pics)

cap=cv2.VideoCapture(0)
while True:
    success, img = cap.read()
    imgSmall=cv2.resize(img,(0,0),None,0.25,0.25)               #reduce the size of the image from webcam feed to speed up the process
    imgSmall=cv2.cvtColor(imgSmall,cv2.COLOR_BGR2RGB)

    faceCurFrame=face_recognition.face_locations(imgSmall)
    encodesCurFrame=face_recognition.face_encodings(imgSmall,faceCurFrame)

    for encodeFace,facLoc in zip(encodesCurFrame,faceCurFrame): #find the face in webcam feed of closest match to the pics in 'People' directory
        match=face_recognition.compare_faces(encodeKnown,encodeFace)
        faceDis=face_recognition.face_distance(encodeKnown,encodeFace)
        matchIndex=np.argmin(faceDis)
        if match[matchIndex]:
            name=NameOfPerson[matchIndex].upper()
            y1,x2,y2,x1= facLoc
            y1,x2,y2,x1 = y1*4,x2*4,y2*4,x1*4                                          #convert the image from webcam feed to its original size
            cv2.rectangle(img,(x1,y1),(x2,y2),(255,255,0),2)                           #draw a cyan rectangle around the face
            cv2.rectangle(img, (x1, y2-35), (x2, y2), (255, 255, 0))                   #draw a cyan rectangle around the name of the person
            cv2.putText(img,name,(x1+6,y2-6),cv2.FONT_HERSHEY_SIMPLEX,1,(255,255,0),2) #display the name of the person

    cv2.imshow('Output',img)
    cv2.waitKey(1)
