import mediapipe as mp
import cv2
import math, numpy as np
import serial
import time

mp_drawing = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands
cap = cv2.VideoCapture(0)
_,img = cap.read()
h,w,c = img.shape
arduino = serial.Serial('COM15',9600,timeout = 0.5,writeTimeout=0)
con = 0

cal_degrees = lambda x,NMI,NMA,MMI,MMA : (x-NMI) * (MMA - MMI) / (NMA - NMI) + MMI

min_degrees = [0,0,0,0,0]
max_degrees = [180,180,180,180,180]

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
            
            middle_po = np.array([results.multi_hand_landmarks[0].landmark[0].x*w,results.multi_hand_landmarks[0].landmark[0].y*h])
            
            fingers_position = [
                [results.multi_hand_landmarks[0].landmark[4].x*w,results.multi_hand_landmarks[0].landmark[4].y*h],
            [results.multi_hand_landmarks[0].landmark[8].x*w,results.multi_hand_landmarks[0].landmark[8].y*h],
           [results.multi_hand_landmarks[0].landmark[12].x*w,results.multi_hand_landmarks[0].landmark[12].y*h],
            [results.multi_hand_landmarks[0].landmark[16].x*w,results.multi_hand_landmarks[0].landmark[16].y*h],
            [results.multi_hand_landmarks[0].landmark[20].x*w,results.multi_hand_landmarks[0].landmark[20].y*h]
            ]
            
            fingers_Propo = [
                [results.multi_hand_landmarks[0].landmark[1].x*w,results.multi_hand_landmarks[0].landmark[1].y*h],
            [results.multi_hand_landmarks[0].landmark[5].x*w,results.multi_hand_landmarks[0].landmark[5].y*h],
            [results.multi_hand_landmarks[0].landmark[9].x*w,results.multi_hand_landmarks[0].landmark[9].y*h],
            [results.multi_hand_landmarks[0].landmark[13].x*w,results.multi_hand_landmarks[0].landmark[13].y*h],
            [results.multi_hand_landmarks[0].landmark[17].x*w,results.multi_hand_landmarks[0].landmark[17].y*h]
            ]
            
            fingers_dis = np.array(list(map(lambda xy : int(((middle_po[0]-xy[0])**2 +(middle_po[1]-xy[1])**2)**(1/2)),fingers_position)))
            fingers_Propo = np.array(list(map(lambda xy : int(((middle_po[0]-xy[0])**2 +(middle_po[1]-xy[1])**2)**(1/2)),fingers_Propo)))
            
            Pkey = cv2.waitKey(1)
            if Pkey == ord('s'):
                
                min_distance = fingers_dis.copy()
                NBD = max_distance/pro_distance
                NSD = min_distance/pro_distance
                
                print("Small Succ")
                lTime = time.time()
                con = 2
            elif Pkey == ord('b'):
                
                max_distance = fingers_dis.copy()
                pro_distance = fingers_Propo.copy()
                
                print("Big Succ")
            elif Pkey == ord('c'):
                if con == 2:
                    con = 0 
                else:
                    con = 2
            if con == 2:    
                if time.time() - lTime > 2:
                    print("GoGo")
                    anything_out_dis = list(map(cal_degrees, fingers_dis, NSD*fingers_Propo,NBD*fingers_Propo, min_degrees,max_degrees))
                    if anything_out_dis[0] < 135:
                        anything_out_dis[0] = 135
                    elif anything_out_dis[0] > 180:
                        anything_out_dis[0] = 180
                    for k in range(1,5):
                        if anything_out_dis[k] > 180:
                            anything_out_dis[k] = 180
                        elif anything_out_dis[k] < 0:
                            anything_out_dis[k] = 0
                    arduino_dis = f"{int(anything_out_dis[0])},{int(anything_out_dis[1])},{int(anything_out_dis[2])},{int(anything_out_dis[3])},{int(anything_out_dis[4])}\n"
                    print(arduino_dis)
                    arduino.write(arduino_dis.encode())
                    
                    lTime = time.time()                    
        cv2.imshow('Fingers', image)
        if cv2.waitKey(1) == 27:
            break
cap.release()
cv2.destroyAllWindows()