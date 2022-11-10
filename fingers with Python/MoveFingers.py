import mediapipe as mp
import cv2
import math, numpy as np
import serial

mp_drawing = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands
cap = cv2.VideoCapture(0)
_,img = cap.read()
h,w,c = img.shape
#arduino = serial.Serial('COM14',9600)

with mp_hands.Hands(model_complexity = 1,min_detection_confidence=0.5, min_tracking_confidence=0.5, max_num_hands = 1) as hands: 
    while cap.isOpened():
        ret, frame = cap.read()
        image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        image = cv2.flip(image, 1)
        image.flags.writeable = False
        results = hands.process(image)
        image.flags.writeable = True 
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
        if results.multi_hand_landmarks: #4 8 12 16 20 0
            for hand in results.multi_hand_landmarks:
                mp_drawing.draw_landmarks(image, hand, mp_hands.HAND_CONNECTIONS, 
                                        mp_drawing.DrawingSpec(color=(0, 0, 0), thickness=2, circle_radius=4),
                                        mp_drawing.DrawingSpec(color=(0, 0, 0), thickness=2, circle_radius=2))
            
            middle_po = np.array([results.multi_hand_landmarks[0].landmark[0].x*w,results.multi_hand_landmarks[0].landmark[0].y*h],dtype = np.int64)
            
            fingers_position = [
                np.array([results.multi_hand_landmarks[0].landmark[4].x*w,results.multi_hand_landmarks[0].landmark[4].y*h],dtype = np.int64),
            np.array([results.multi_hand_landmarks[0].landmark[8].x*w,results.multi_hand_landmarks[0].landmark[8].y*h],dtype = np.int64),
            np.array([results.multi_hand_landmarks[0].landmark[12].x*w,results.multi_hand_landmarks[0].landmark[12].y*h],dtype = np.int64),
            np.array([results.multi_hand_landmarks[0].landmark[16].x*w,results.multi_hand_landmarks[0].landmark[16].y*h],dtype = np.int64),
            np.array([results.multi_hand_landmarks[0].landmark[20].x*w,results.multi_hand_landmarks[0].landmark[20].y*h],dtype = np.int64)
            ]
            
            fingers_dis = list(map(lambda xy : int(((middle_po[0]-xy[0])**2 +(middle_po[1]-xy[1])**2)**(1/2)),fingers_position))
            arduino_input_dis = f"{fingers_dis[0]} {fingers_dis[1]} {fingers_dis[2]} {fingers_dis[3]} {fingers_dis[4]}\n"
            #arduino.write(b'OFF')
            print(arduino_input_dis.encode())
        cv2.imshow('Fingers', image)
        if cv2.waitKey(1) == 27:
            break
cap.release()
cv2.destroyAllWindows()