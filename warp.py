import cv2
import numpy as np

circles = np.zeros((4, 2), np.int)
counter = 0
w, h = 720, 720
image_path = 'C:/Users/ASUS/Desktop/3D_Look/images/'


def mousePoints(event, x, y, flags, params):
    global counter
    if event == cv2.EVENT_LBUTTONDOWN:
        circles[counter] = x * 1.5 + 540, y * 1.5 + 540
        counter += 1
        print(counter)


for i in range(595, 600):
    img = cv2.imread(image_path + str(i) + '.png')
    while True:
        if counter == 4:
            counter = 0
            pts1 = np.float32([circles[0], circles[1], circles[2], circles[3]])
            pts2 = np.float32([[0, 0], [w, 0], [w, h], [0, h]])
            matrix = cv2.getPerspectiveTransform(pts1, pts2)
            out = cv2.warpPerspective(img, matrix, (w, h))
            cv2.imwrite(image_path + 'warped_2/' + str(i) + '.jpg', out)
            cv2.imshow("Output Image", out)
        if counter > 0:
            cv2.destroyWindow("Output Image")
        """for x in range(4):
            cv2.circle(img, (circles[x][0], circles[x][1]), 3, (0, 255, 0), cv2.FILLED)"""

        cv2.imshow("Original"+str(i), cv2.resize(img[540:1620, 540:1620], (720, 720)))
        cv2.setMouseCallback("Original"+str(i), mousePoints)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            circles = np.zeros((4, 2), np.int)
            cv2.destroyAllWindows()
            break
