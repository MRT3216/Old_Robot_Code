
import cv2

if __name__ == '__main__':
    
    img = cv2.imread('combined.png')
    
    # fill in the holes
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (2,2), anchor=(1,1))
    morphed_img = cv2.morphologyEx(img, cv2.MORPH_CLOSE, kernel, iterations=9)
    
    # show it
    cv2.imshow('morphed', morphed_img)
    
    # store that for other demo code
    cv2.imwrite('morphed.png', morphed_img)
    
    cv2.waitKey(0)
    