import cv2

recx = [400, 300, 200]
recy = [105, 200, 295]
jpgs = ["jpgs/U.jpg",
        "jpgs/D.jpg",
        "jpgs/L.jpg",
        "jpgs/R.jpg",
        "jpgs/F.jpg",
        "jpgs/B.jpg"]

class ColorHSV:
    def __init__(self, color, iLowH, iHighH, iLowS, iHighS, iLowV, iHighV):
        self.color = color
        self.iLowH = iLowH
        self.iHighH = iHighH
        self.iLowS = iLowS
        self.iHighS = iHighS
        self.iLowV = iLowV
        self.iHighV = iHighV

def printFrame(frame):
    for i in range(3):
        for j in range(3):
            x = recx[i]
            y = recy[j]
            cv2.rectangle(frame,(x,y),(x+20,y+20),(170,234,242),1,lineType=cv2.LINE_AA)

def videoNotify():
    fource = cv2.VideoWriter_fourcc(*"mp4v")
    video = cv2.VideoCapture(0)
    resulte = cv2.VideoWriter('video.mp4',fource,20.0,(640,480))
    cnt = 0
    while video.isOpened():
        ret,frame = video.read()
        if ret is True:
            resulte.write(frame)
            frame = cv2.flip(frame, 1)
            printFrame(frame)
            cv2.imshow('video', frame)
            if cv2.waitKey(1) == ord('q'):
                cv2.imwrite(jpgs[cnt], frame)
                cnt += 1
                if (cnt == 6):
                    break
        else:
            break
    video.release()
    resulte.release()
    cv2.destroyAllWindows()

def colorNotify():
    YELLOW_HSV = ColorHSV("yellow", 10, 59, 70, 255, 0, 255)
    WHITE_HSV = ColorHSV("white", 0, 180, 0, 69, 0, 255)
    BLUE_HSV = ColorHSV("blue", 90, 149, 70, 255, 0, 255)
    GREEN_HSV = ColorHSV("green", 60, 89, 70, 255, 0, 255)
    RED_HSV = ColorHSV("red", 150, 199, 70, 255, 0, 255)
    ORANGE_HSV = ColorHSV("orange", 0, 9, 70, 255, 0, 255)
    COLORS_HSV = [YELLOW_HSV, WHITE_HSV, BLUE_HSV, GREEN_HSV, RED_HSV, ORANGE_HSV]

    colors = ['y', 'w', 'b', 'g', 'r', 'o', 'Null']
    colors_list = []
    delta = 2

    for cnt in range(6):
        img = cv2.imread(jpgs[cnt])
        color_list = []
        for i in range(3):
            for j in range(3):
                y = recy[i]
                x = recx[j]
                color_img = img[y + delta : y + 20 - delta, x + delta : x + 20 - delta]
                color_img = cv2.resize(color_img, None, fx=3.0, fy=3.0)

                hsv_b = cv2.cvtColor(color_img, cv2.COLOR_BGR2HSV)
                mean = cv2.mean(hsv_b)
                H = int(mean[0])
                S = int(mean[1])
                V = int(mean[2])

                color = colors[6]
                for k in range(6):
                    temp = COLORS_HSV[k]
                    if H >= temp.iLowH and H <= temp.iHighH and S >= temp.iLowS and S <= temp.iHighS and V >= temp.iLowV and V <= temp.iHighV:
                        color = colors[k]
                        break
                color_list.append(color)
                # with open('../Kociemba/input.txt', 'a+') as f:
                #     print(H, S, V, color, file=f)

        colors_list.append(color_list)
        # with open('../Kociemba/input.txt', 'a+') as f:
        #     print("", file=f)

    with open('../Kociemba/input.txt', 'a+') as f:
        for i in range(6):
            for j in range(9):
                print(colors_list[i][j], end="", file=f)
            print("", file=f)

if __name__ == '__main__':
    videoNotify()
    colorNotify()
