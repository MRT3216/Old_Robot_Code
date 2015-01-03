
import cv2

def threshold_range(im, lo, hi):
    '''Returns a binary image if the values are between a certain value'''
    
    unused, t1 = cv2.threshold(im, lo, 255, type=cv2.THRESH_BINARY)
    unused, t2 = cv2.threshold(im, hi, 255, type=cv2.THRESH_BINARY_INV)
    return cv2.bitwise_and(t1, t2)
    
if __name__ == '__main__':
    
    img = cv2.imread('demo.png')
    
    # convert to HSV colorspace
    hsv = cv2.cvtColor(img, cv2.cv.CV_BGR2HSV)

    h, s, v = cv2.split(hsv)
    
    # these parameters will find 'green' on the image
    h = threshold_range(h, 30, 75)
    s = threshold_range(s, 188, 255)
    v = threshold_range(v, 16, 255)

    # combine them all and show that
    combined = cv2.bitwise_and(h, cv2.bitwise_and(s, v))
    cv2.imshow('combined', combined)
    
    # store the image for other demo projects
    cv2.imwrite('combined.png', combined)

    
    cv2.waitKey(0)
    