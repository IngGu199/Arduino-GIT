void LeftMotor(int Speed = 200, int Front = 1)
{ /*
  if(Speed != 0)
  {
    Speed-=20;
  } */
  analogWrite(3,Speed);
  if(Front)
  {
    digitalWrite(2,HIGH);
    digitalWrite(4,LOW);
  }
  else
  {
    digitalWrite(2,LOW);
    digitalWrite(4,HIGH);
  }
}
int LLEFT, LEFT, FRONT, RIGHT, RRIGHT, CLIP,sum;
void GetLedAnswer()
{
  LLEFT = digitalRead(8);
  LEFT = digitalRead(9);
  FRONT = digitalRead(10);
  RIGHT = digitalRead(11);
  RRIGHT = digitalRead(12);
  CLIP = digitalRead(13);
  sum = FRONT+LEFT+RIGHT+RRIGHT+LLEFT;
}
void RightMotor(int Speed = 200, int Front = 1)
{
  analogWrite(6,Speed);
  if(Front)
  {
    digitalWrite(5,HIGH);
    digitalWrite(7,LOW);
  }
  else
  {
    digitalWrite(5,LOW);
    digitalWrite(7,HIGH);
  }
}

void setup() {
  for(int k = 2; k <=7; k++)
  {
    pinMode(k,OUTPUT);
  }
  for(int k = 8; k <=13; k++)
  {
    pinMode(k,INPUT);
  }
}

void loop() {
  // 검은색 = 0, 하얀색 = 1
  GetLedAnswer();
  if(sum == 0)
  {
    Stop:;
    LeftMotor(0,0);
    RightMotor(0,0);
  }
  else
  {
    if(!RIGHT || (!FRONT && !RIGHT))
    {
      LeftMotor(250,1);
      RightMotor(250,0);
      if(!RRIGHT)
      {
        LeftMotor(255,1);
        RightMotor(255,1);
        delay(150);
        LeftMotor(250,1);
        RightMotor(250,0);
        while(1)
        {
          LEFT = digitalRead(9);
          if(!LEFT)
          {
            break;
          }
        }
      LeftMotor(250,0);
      RightMotor(250,1);    
      }
    }
    if(!LEFT || (!FRONT && !LEFT))
    {
      LeftMotor(250,0);
      RightMotor(250,1);
      if(!LLEFT)
      {
        GetLedAnswer();
        if(sum == 0)
        {
          goto Stop;
        }
        LeftMotor(255,1);
        RightMotor(255,1);
        delay(150);
        LeftMotor(250,0);
        RightMotor(250,1);
        while(1)
        {
          RIGHT = digitalRead(11);
          if(!RIGHT)
          {
            break;
          }
        }
      LeftMotor(250,1);
      RightMotor(250,0);    
      }
    }
    if((!FRONT && LEFT && RIGHT) || (!FRONT && !LEFT && !RIGHT))
    {
      LeftMotor(250,1);
      RightMotor(250,1);
    }
  }
  if(CLIP)
  {
    LeftMotor(250,0);
    RightMotor(250,0);
    delay(1000);
  }
}