
import cv2
    
vc = cv2.VideoCapture()

if not vc.open(0):
    print "Could not connect to webcam"
    exit(1)

while cv2.waitKey(30) <= 0:
    success, img = vc.read()
    if not success:
        break
    
    cv2.imshow("Webcam", img)    
    
    