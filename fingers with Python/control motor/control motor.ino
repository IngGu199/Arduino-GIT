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

String str="180,180,180,180,180\n";
int CoP = 0,Check = 0;
int MDegree[5] = {0};

void loop() {
  if(Serial.available()){ // 시리얼 포트로 데이터를 받기
    str = Serial.readString(); // 받은 데이터를 저장
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

  MDegree[0] = De1;
  MDegree[1] = De2;
  MDegree[2] = De3;
  MDegree[3] = De4;
  MDegree[4] = De5;
  servo1.write(MDegree[0]);
  servo2.write(MDegree[1]);
  servo3.write(MDegree[2]);
  servo4.write(MDegree[3]);
  servo5.write(MDegree[4]);
}