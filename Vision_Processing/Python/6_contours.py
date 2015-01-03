
import cv2

    
if __name__ == '__main__':
    
    # the '0' parameter means load a grayscale image
    img = cv2.imread('morphed.png', 0)
   
    # nothing easier
    contours, hierarchy = cv2.findContours(img.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_TC89_KCOS)
    print contours
    
    # draw the found contours on the image
    # -> but you can't show colors on a grayscale image, so convert it to color
    color_img = cv2.cvtColor(img, cv2.cv.CV_GRAY2BGR)
    
    # then draw it
    p = cv2.approxPolyDP(contours, 45, False)
    
    cv2.drawContours(color_img, p, -1, (0,0,255), thickness=2)
    
    # show it
    cv2.imshow('contours', color_img)
    cv2.imwrite('contours.png', color_img)
    
    cv2.waitKey(0)
    