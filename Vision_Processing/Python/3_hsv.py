
import cv2
import numpy as np

def binarize(im):
    '''Turn into white any portion of the image that is not zero'''
    new = np.zeros_like(im, dtype=np.uint8)
    new[im > 1] = 255
    return new

if __name__ == '__main__':

    img = cv2.imread('demo.png')
    
    # convert to HSV colorspace
    hsv = cv2.cvtColor(img, cv2.cv.CV_BGR2HSV)

    h, s, v = cv2.split(hsv)
    
    cv2.imshow("h", binarize(h))
    cv2.imshow("s", binarize(s))
    cv2.imshow("v", binarize(v))
    
    cv2.waitKey(0)

