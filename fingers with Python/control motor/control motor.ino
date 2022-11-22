#include <Servo.h> //서보모터 라이브러리(설치안해두됨)

Servo servo1,servo2,servo3,servo4,servo5;


void setup() {
  Serial.begin(9600);
  servo1.attach(5);
  servo2.attach(7);
  servo3.attach(9);
  servo4.attach(11);
  servo5.attach(13);

  servo1.write(180);
  servo2.write(180);
  servo3.write(180);
  servo4.write(180);
  servo5.write(180);
}

String str="";
int CoP = 0,Check = 0;
double MDegree[5] = {180,180,180,180,180};
int finger_max[5], finger_min[5],finger_G[5];
double finger_distance_Max[5], finger_distance_Min[5];

void loop() {
  if(Serial.available()){ // 시리얼 포트로 데이터를 받기

    str = Serial.readString(); // 받은 데이터를 저장

    if(str[0] == 'B' || str[0] == 'S' || str[0] == 'M')
    {
      CoP = 1;
    }
    else
    {
      CoP = 0;
    }


    int Co1 = str.indexOf(","); // 첫번째 콤마위치
    int Co2 = str.indexOf(",",Co1+1); // 두번째 콤마 위치
    int Co3 = str.indexOf(",",Co2+1);
    int Co4 = str.indexOf(",",Co3+1);
    int strlength = str.length(); // 문자열 길이
    int De1 = str.substring(CoP, Co1).toInt(); // 첫번째 토큰
    int De2 = str.substring(Co1+1, Co2).toInt(); // 두번째 토큰
    int De3 = str.substring(Co2+1,Co3).toInt(); // 세번째 토큰
    int De4 = str.substring(Co3+1,Co4).toInt();
    int De5 = str.substring(Co4+1,strlength-1).toInt();


    if(str[0] == 'B')
    {
      finger_max[0] = De1;
      finger_max[1] = De2;
      finger_max[2] = De3;
      finger_max[3] = De4;
      finger_max[4] = De5;
    }
    else if(str[0]=='S')
    {
      finger_min[0] = De1;
      finger_min[1] = De2;
      finger_min[2] = De3;
      finger_min[3] = De4;
      finger_min[4] = De5;
      
      finger_distance_Max[0] = finger_max[0]/finger_G[0];
      finger_distance_Max[1] = finger_max[1]/finger_G[1];
      finger_distance_Max[2] = finger_max[2]/finger_G[2];
      finger_distance_Max[3] = finger_max[3]/finger_G[3];
      finger_distance_Max[4] = finger_max[4]/finger_G[4];

      finger_distance_Min[0] = finger_min[0]/finger_G[0];
      finger_distance_Min[1] = finger_min[1]/finger_G[1];
      finger_distance_Min[2] = finger_min[2]/finger_G[2];
      finger_distance_Min[3] = finger_min[3]/finger_G[3];
      finger_distance_Min[4] = finger_min[4]/finger_G[4];
    }
    
    if(str[0]=='M')
    {
      finger_G[0] = De1;
      finger_G[1] = De2;
      finger_G[2] = De3;
      finger_G[3] = De4;
      finger_G[4] = De5;
    }
    else
    {
      MDegree[0] = map(De1, finger_G[0]*finger_distance_Min[0], finger_G[0]*finger_distance_Max[0], 125, 180);
      MDegree[1] = map(De2, finger_G[1]*finger_distance_Min[1], finger_G[1]*finger_distance_Max[1], 0, 180);
      MDegree[2] = map(De3, finger_G[2]*finger_distance_Min[2], finger_G[2]*finger_distance_Max[2], 0, 180);
      MDegree[3] = map(De4, finger_G[3]*finger_distance_Min[3], finger_G[3]*finger_distance_Max[3], 0, 180);
      MDegree[4] = map(De5, finger_G[4]*finger_distance_Min[4], finger_G[4]*finger_distance_Max[4], 0, 180);

      servo1.write(MDegree[0]);
      servo2.write(MDegree[1]);
      servo3.write(MDegree[2]);
      servo4.write(MDegree[3]);
      servo5.write(MDegree[4]);
    }    
   }

}