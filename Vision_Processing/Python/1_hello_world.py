
import cv2
import numpy as np

# create a numpy array to hold the data
# -> w=200, h=100, channels=3
img = np.zeros(shape=(100,200,3), dtype=np.uint8)

# write red text to image (note BGR, not RGB!)
cv2.putText(img, "Hello World!", (50,50), 
            cv2.FONT_HERSHEY_PLAIN, 1, (0,0,255))

cv2.imshow('Image', img)
cv2.waitKey(0)


