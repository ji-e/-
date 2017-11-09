#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

#define PIN 11 //NeoPixel 핀 번호
const int BUTTON_INT= 0; //interrupt를 위한 버튼
uint16_t i=15;
const int buttonPin = 10; //NeoPixel 전원 버튼
const int btn=12; //XBee통신 전원 버튼
const int led=13;
const int SPEAKER=9; 
int btnstate=0; 
int bstate=0;
int buttonState = 0; 
int state = 0;
int a;

//네오픽셀을 사용하기 위해 객체 하나를 생성한다. 
//첫번째 인자값은 네오픽셀의 LED의 개수
//두번째 인자값은 네오픽셀이 연결된 아두이노의 핀번호/
//세번째 인자값은 네오픽셀의 타입에 따라 바뀌는 flag
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

SoftwareSerial XBee(4, 5); // RX, TX

void setup()
{
  pinMode(buttonPin, INPUT);
  pinMode(led, OUTPUT);
  pinMode(btn, INPUT);
  strip.begin(); //네오픽셀을 초기화하기 위해 모든LED를 off시킨다
  strip.show(); 
  attachInterrupt(BUTTON_INT, speaker, RISING); //interrupt
  Serial.begin(9600);

}


//interrupt 함수
void speaker()
{
//state가 1이면 경고음을 약 5초 동안 출력
  if(state==1)
  { 
    tone(SPEAKER, 400, 5000); 
  }
}


void loop()
{
  btnstate = digitalRead(btn);
     
   if (btnstate == HIGH)
   {
      bstate=1-bstate; 
    }

//XBee 통신 시작
   if (bstate==1)
    {
      XBee.begin(9600);
      digitalWrite(led, HIGH);
    
    
  if (XBee.available())
  { 
    a=XBee.read();
    if(a==1)
     {
      Serial.println(a);
      tone(SPEAKER, 400); 
      colorWipe(strip.Color(255, 0, 0), 50); //빨간색 출력
      colorWipe(strip.Color(0, 0, 0), 50); //조명 꺼짐
      theaterChase(strip.Color(255, 0, 0), 50); //빨간색 출력  
      colorWipe(strip.Color(255, 0, 0), 50); //빨간색 출력
     } 
  }
  noTone(SPEAKER);
  delay(100);
}

//XBee 통신 종료
   else if (bstate==0)
   {
     Serial.println(a);
     XBee.end();
     
     buttonState = digitalRead(buttonPin);
     digitalWrite(led,LOW);

    if (buttonState == HIGH)
    {
      state=1-state; 
    }

//state가 1이면 NeoPixel에서 흰색 조명 출력
    if(state==1)
      { 
        for( i=0; i<strip.numPixels(); i++){
           strip.setPixelColor(i, 255,255,255);
           strip.show();
       }
  delay(100); 
}

//state가 0이면 NeoPixel 조명 꺼짐
else if(state==0)
   { 
     for( i=0; i<strip.numPixels(); i++){
       strip.setPixelColor(i, 0,0,0);
       strip.show();
   }
delay(100);
}
   } 
 delay(100);
}

//NeoPixel에 달린 LED를 각각 주어진 값 색으로 채워나가는 함수
void colorWipe(uint32_t c, uint8_t wait)
{
  for(uint16_t i=0; i<strip.numPixels(); i++){
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

//입력한 색으로 LED를 깜빡거리며 표현하는 함수
void theaterChase(uint32_t c, uint8_t wait)
{
  for (int j=0; j<10; j++){ 
    for (int q=0; q < 3; q++){
      for (int i=0; i < strip.numPixels(); i=i+3) 
      {
        strip.setPixelColor(i+q, c);    
      }
      strip.show();
      delay(wait);
      for (int i=0; i < strip.numPixels(); i=i+3){
        strip.setPixelColor(i+q, 0);
      }
    }
  }
}
