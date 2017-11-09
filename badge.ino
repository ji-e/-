// We'll use SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>
#include <SD.h>

// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(2, 3); // RX, TX

const int ir=A5; //적외선
const int buttonPin = 5; //XBee 통신 버튼
const int ledPin =  13; 
const int CS_PIN= 4; 
const int POW_PIN=8; //전원을 공급

int buttonState = 0; 
int state = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.println("Initializing Card");
  pinMode(CS_PIN, OUTPUT); //CS 핀을 출력으로 설정
  pinMode(POW_PIN, OUTPUT); //8번 핀을 전원으로 사용하므로 출력으로 설정
  digitalWrite(POW_PIN, HIGH); //아두이노 공식 이더넷 실드를 사용하면 주석 처리

if(!SD.begin(CS_PIN))
    {
        Serial.println("Card Failure");
        return;
    }
    Serial.println("Card Ready");
}

void loop()
{ 
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH){
      state=1-state; 
  }

if(state==1)
{
   XBee.begin(9600);
   digitalWrite(ledPin, HIGH); //XBee통신이 되고 있다는 것을 알려줌
   int distance=analogRead(ir); //IR 센서에서 장애물 거리값을 읽음
   int range = (6787 / (distance - 3)) - 4; //IR 센서 값을 CM로 변환
   delay(100); //장애물을 감지하는 시간 간격 지정

    long timeStamp=millis();
    File dataFile=SD.open("log.csv", FILE_WRITE);

    if(dataFile)
    {
        dataFile.print(timeStamp);
        dataFile.print(",");
        dataFile.println(range);
        dataFile.close(); //close() 함수로 연결을 끊어야만 실제로 데이터가 입력

//디버깅을 위해 화면에 동일한 문자열 입력
        Serial.print(timeStamp);
        Serial.print(",");
        Serial.println(range);
    }
    else
    {
        Serial.println("Couldn't open log file");
    }

// 사람이 20~80CM의 범위로 다가오면
if (range>=20&&range<=80)
  {
    int a=1;
    XBee.write(a);
  }
delay(100);
}

else if(state==0)
  {
     digitalWrite(ledPin, LOW); //XBee통신을 하고 있지 않다는 것을 알려줌
     int b=0;
     XBee.write(b);
     XBee.end();
                                 
  }
delay(100);
}
