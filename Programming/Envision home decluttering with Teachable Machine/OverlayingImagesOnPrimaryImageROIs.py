#OverlayingImagesOnPrimaryImageROIs
#Made by www.wiztaqnia.com
#Modified date 14/01/2024
import cv2 as cv
from matplotlib import pyplot
from PIL import Image
background=cv.imread('Resources/Background.png')
imgRGB=cv.cvtColor(background,cv.COLOR_BGR2RGB) #convert the image from RGB to BGR (default color space for openCV is BGR)
pyplot.figure('Background')
pyplot.imshow(imgRGB)            #show the 'Background' window
imgPrimary = Image.open('Resources/Background.png')
clutter=Image.open('Resources/Clutter/1.png')
arrow=Image.open('Resources/Arrow.png')
bin=Image.open('Resources/Bins/1.png')
imgPrimary.paste(clutter, (775,130), mask=clutter)
imgPrimary.paste(arrow,(810,260), mask=arrow)
imgPrimary.paste(bin, (745,360), mask=bin)
imgPrimary.show()                #show the Resultant Overlay Image
