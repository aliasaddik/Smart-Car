 
#include <Arduino_FreeRTOS.h>
#include <Wire.h>
#include <TEA5767.h>
#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>
#include <TouchScreen.h>
MCUFRIEND_kbv tft;
#include <stdint.h>
 
 
TEA5767 Radio; //Pinout SLC and SDA - Arduino uno pins A5 and A4
//(int CS=A3, int RS=A2, int WR=A1, int RD=A0, int RST=A

//Variables:
double old_frequency;
double frequency;
int search_mode = 0;
int search_direction;
unsigned long last_pressed;
unsigned char buf[5];
int stereo;
int signal_level;
double current_freq;
unsigned long current_millis = millis();
int inByte;
int flag=0;

int flag1 = 0 ;
int flag2 = 0 ;
int flag3 = 0 ;

#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
 
#define YP A2  
#define XM A3 
#define YM 8   
#define XP 9  
#define LCD_RESET A4
#define off 31


#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


TouchScreen ts = TouchScreen(XP, YP, XM, YM, 50);
 

// Black Line Follower 
int buzzer=32;
int IR1=34;      //Right sensor
int IR2=35;    //left Sensor
// motor one
int enA = 44;    //Right motor
int MotorAip1=41;
int MotorAip2=40;
// motor two
int enB = 45;    //Left motor
int MotorBip1=38;
int MotorBip2=39;
// motor three //backLeft
int MotorCip1 =53;
int MotorCip2=52;
int MotorDip1 =36;
int MotorDip2=37;

int  xValue, yValue,xPosition,yPosition;
int mid  = 51, topl=50,topm=49,topr=48, botl=47,botm=43,botr=42;
//butt=2;
int buttonState = 0;
int IR1Value=0 ;
int IR2Value=0 ; 

//Radio 
int playing = 0;
int on = 0 ; 
int previous = 0 ; 

void linekeepsetup(){
  // put your setup code here, to run once:
  pinMode(buzzer,OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(IR1,INPUT);
 
  pinMode(IR2,INPUT);
  pinMode(MotorAip1,OUTPUT);
  pinMode(MotorAip2,OUTPUT);
  pinMode(MotorBip1,OUTPUT);
  pinMode(MotorBip2,OUTPUT);
  pinMode(MotorCip1,OUTPUT);
  pinMode(MotorCip2,OUTPUT);
  pinMode(MotorDip1,OUTPUT);
  pinMode(MotorDip2,OUTPUT);

  pinMode(off,OUTPUT);
}
 
 
void radiosetup(){
 tft.reset();
 uint16_t identifier = tft.readID();
 tft.begin(identifier);  
 Radio.init();
 Radio.set_frequency(100.6); //On power on go to station 100.4
 playing = 1; 
 on =1 ;
 previous = 0 ;
 Serial.begin(9600);
}

void setup() 
{
  //testText();
  // put your setup code here, to run once:
  Serial.begin(9600);
  linekeepsetup();
 
  radiosetup();
  digitalWrite(off,0);
  xTaskCreate(carLoop, "lane keeep assist", 128, NULL, 2, NULL);
  xTaskCreate(radioloop, "Radio", 128, NULL, 1, NULL);


}
void loop() 
{}
void carLoop(void *pvParameters) {
  while (1) {

    IR1Value = digitalRead(IR1);
    IR2Value = digitalRead(IR2);
    //Serial.print("IR1Value");
    //Serial.println(IR1Value);
    //Serial.print("IR2VALUE");
    //Serial.println(IR2Value);
   if(IR1Value==0 && IR2Value ==0) //IR will glow on white line
  {      Serial.println("I will do a BIG delay");

    //Stop both Motors
    digitalWrite(MotorAip1,LOW);
    digitalWrite(MotorAip2,LOW);
    digitalWrite(MotorBip1,LOW);
    digitalWrite(MotorBip2,LOW);
    digitalWrite(MotorCip1,LOW);
    digitalWrite(MotorCip2,LOW);
    digitalWrite(MotorDip2,LOW);
    digitalWrite(MotorDip1,LOW);


   analogWrite (enA, 0);
   analogWrite (enB, 0);
   digitalWrite(buzzer,LOW);
vTaskPrioritySet(NULL,0);
//delay(1000);
  }

   if(IR1Value==1 && IR2Value==1)  //IR not on white line
  {
    //Move both the Motors
    digitalWrite(MotorAip1,HIGH);
    digitalWrite(MotorAip2,LOW);
    digitalWrite(MotorBip1,LOW);
    digitalWrite(MotorBip2,HIGH);
    digitalWrite(MotorCip1,LOW);
    digitalWrite(MotorCip2,HIGH);
    digitalWrite(MotorDip1,HIGH);
    digitalWrite(MotorDip2,LOW);
    analogWrite (enA, 80);
    analogWrite (enB, 80);
    digitalWrite(buzzer,LOW);
 
  }

   if(IR1Value==0 && IR2Value==1) 
  {
    //Tilt robot towards left by stopping the left wheel and moving the right one
    digitalWrite(MotorAip1,HIGH);
    digitalWrite(MotorAip2,LOW);
    digitalWrite(MotorBip1,LOW);
    digitalWrite(MotorBip2,HIGH);
    digitalWrite(MotorCip1,HIGH);
    digitalWrite(MotorCip2,LOW);
        digitalWrite(MotorDip1,HIGH);
    digitalWrite(MotorDip2,LOW);
    analogWrite (enA, 150);
    analogWrite (enB, 80);
   // Serial.println("Hello");
     digitalWrite(buzzer,HIGH);
   // Serial.println(buzzer);
//   delay(200);
  }

   if(IR1Value==1 && IR2Value ==0)
  {
    //Tilt robot towards right by stopping the right wheel and moving the left one
         // If I want to turn right then the speed of the right wheel should be less than that of the left wheel, here, let a be the right wheel
    digitalWrite(MotorAip1,HIGH);
    digitalWrite(MotorAip2,LOW);
    digitalWrite(MotorBip1,LOW );
    digitalWrite(MotorBip2,HIGH);
    digitalWrite(MotorCip1,HIGH);
    digitalWrite(MotorCip2,LOW);
        digitalWrite(MotorDip1,HIGH);
    digitalWrite(MotorDip2,LOW);
    analogWrite (enA, 80);
   analogWrite (enB, 150);
    digitalWrite(buzzer,HIGH);
//   delay(200);
  }

  Serial.println("END OF CAR TASK");
//delay(500);
  Serial.println("/car task delay finished");
}}

unsigned long testText1Playing() {

  tft.fillScreen(BLACK);
  unsigned long start = micros();

  tft.setCursor(30,30);
  tft.setTextColor(WHITE);  
  tft.setTextSize(3);
  tft.println("100.6 is playing");
  
  tft.setCursor(80, 135);
  tft.setTextColor(WHITE);  
  tft.setTextSize(3);
  tft.println("98.3");
    
   
  tft.setCursor(80, 240);
  tft.setTextColor(WHITE);  
  tft.setTextSize(3);
  tft.println("105.9");
  return micros() - start;
}

unsigned long testText2Playing() {

  tft.fillScreen(BLACK);
  unsigned long start = micros();

  tft.setCursor(80,30);
  tft.setTextColor(WHITE);  
  tft.setTextSize(3);
  tft.println("100.6");
  
  tft.setCursor(30, 135);
  tft.setTextColor(WHITE);  
  tft.setTextSize(3);
  tft.println("98.3 is playing");
    
   
  tft.setCursor(80, 240);
  tft.setTextColor(WHITE);  
  tft.setTextSize(3);
  tft.println("105.9");
  return micros() - start;
}

unsigned long testText3Playing() {

  tft.fillScreen(BLACK);
  unsigned long start = micros();

  tft.setCursor(80,30);
  tft.setTextColor(WHITE);  
  tft.setTextSize(3);
  tft.println("100.6");
  
  tft.setCursor(80, 135);
  tft.setTextColor(WHITE);  
  tft.setTextSize(3);
  tft.println("98.3");
    
   
  tft.setCursor(30, 240);
  tft.setTextColor(WHITE);  
  tft.setTextSize(3);
  tft.println("105.9 is playing");
  return micros() - start;
}

unsigned long testText() {

  tft.fillScreen(BLACK);
  unsigned long start = micros();

  tft.setCursor(80,30);
  tft.setTextColor(WHITE);  
  tft.setTextSize(3);
  tft.println("100.6");
  
  tft.setCursor(80, 135);
  tft.setTextColor(WHITE);  
  tft.setTextSize(3);
  tft.println("98.3");
    
   
  tft.setCursor(80, 240);
  tft.setTextColor(WHITE);  
  tft.setTextSize(3);
  tft.println("105.9");
  Serial.println("ana 5lst testtext");
  return micros() - start;
}


void radioloop(void * pvParameters) {
  while (1) {


//      if(playing != previous && on == 1)
//      {
//        if(playing == 1)
//           testText1Playing();
//        else
//           if(playing == 2)
//             testText2Playing(); 
//           else
//             if(playing == 3)
//               testText3Playing();
//             else
//               testText();      
//      }

         testText();

    
  TSPoint p = ts.getPoint();
 if (p.z > ts.pressureThreshhold) {
   if(p.y>= 352 && p.y<=680){
    previous = playing;
      if(p.x>=777 && p.x<=848){
  digitalWrite(off,1);
        Serial.println("1");
        playing =1 ;
        Radio.set_frequency(100.4);
      }
      else
      if(p.x>= 526 && p.x <= 576){
        
  digitalWrite(off,1);
        Serial.println("2");
        playing =2 ;
        Radio.set_frequency(98.2);
      }
      else
      if(p.x>=272 && p.x<=313 ){
        
        digitalWrite(off,1);
        Serial.println("3");
        playing =3 ;
        Radio.set_frequency(105.3);
      }
    }
    
 }
}}
