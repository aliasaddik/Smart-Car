
#define joyX A5
#define joyY A4
#define lightS A0
#define LEDPIN 11         //LED brightness (PWM) writing

#include <Arduino_FreeRTOS.h>
#include <Semphr.h>

#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include <stdint.h>
#include <Arduino_FreeRTOS.h>
SemaphoreHandle_t Sem = NULL;

int  xValue, yValue,xPosition,yPosition;
int mid  = 2, topl=3,topm=4,topr=5, botl=9,botm=7,botr=8;
//butt=2;
int buttonState = 0;
void lightSetup(){
 pinMode(lightS,INPUT);  
  pinMode(LEDPIN, OUTPUT); 
}

void gearSetup(){
pinMode(mid,OUTPUT);
  pinMode(topl,OUTPUT);
  pinMode(topm,OUTPUT);
  pinMode(topr,OUTPUT);
  pinMode(botl,OUTPUT);
  pinMode(botm,OUTPUT);
  pinMode(botr,OUTPUT);
 
}
 void lightLoop(){
       xSemaphoreTake( Sem , portMAX_DELAY);

 ///lightSensorr
float reading = analogRead(lightS); //Read light level
float square_ratio = reading / 1023.0;      //Get percent of maximum value (1023)
square_ratio = pow(1-square_ratio, 2.0);      //Square to make response more obvious
analogWrite(LEDPIN, 255.0 * square_ratio);  //Adjust LED brightness relatively

  
  //print the values with to plot or view
  Serial.print(xValue);
  Serial.print("\t");
  Serial.println(yValue);
  Serial.print(reading);
  Serial.print("\t");
  Serial.println(square_ratio);
     xSemaphoreGive(Sem);

  delay(500);
 }
  void gearLoop(){
   while(1){
     xSemaphoreTake( Sem , portMAX_DELAY);
     xValue = analogRead(joyX);
  yValue = analogRead(joyY);
 
 
   if(xValue<=506 &&xValue>=496 && yValue<=518&& yValue>=508)
 {
  digitalWrite(mid,HIGH);
  digitalWrite(botm,HIGH);
  digitalWrite(topm,HIGH);
  digitalWrite(botr,HIGH);
  digitalWrite(topr,HIGH);
  digitalWrite(topl,HIGH);
  digitalWrite(botl,HIGH);
  
 }
 //d
 else if(xValue<=506 &&xValue>=496 && yValue<=5&& yValue>=0)
 {
   Serial.println("ana hena");
  digitalWrite(mid,LOW);
  digitalWrite(botm,LOW);
  digitalWrite(botl,LOW);
  digitalWrite(botr,LOW);
  digitalWrite(topr,LOW);
  
  digitalWrite(topl,HIGH);
  digitalWrite(topm,HIGH);
  
 }
  //p
  else if(xValue<=1028 &&xValue>=1018 && yValue<=518&& yValue>=508)
 {
  digitalWrite(topl,LOW);
  digitalWrite(botl,LOW);
  digitalWrite(topm,LOW);
  digitalWrite(topr,LOW);
  digitalWrite(mid,LOW);
  
  digitalWrite(botm,HIGH);
  digitalWrite(botr,HIGH);
 }
 //n
  else if(xValue<=5 &&xValue>=0 && yValue<=518&& yValue>=508)
 {
  digitalWrite(botr,LOW);
  digitalWrite(botl,LOW);
  digitalWrite(mid,LOW);
  digitalWrite(botm,HIGH);
   digitalWrite(topr,HIGH);
  digitalWrite(topl,HIGH);
  digitalWrite(topm,HIGH);
 }
 //r
 else if(xValue<=506 &&xValue>=496 && yValue<=1028&& yValue>=1018)
 {
  digitalWrite(botl,LOW);
  digitalWrite(mid,LOW);

  digitalWrite(botm,HIGH);
  digitalWrite(botr,HIGH);
  digitalWrite(topr,HIGH);
  digitalWrite(topl,HIGH);
  digitalWrite(topm,HIGH);
 }
     
 else{
  digitalWrite(mid,HIGH);
  digitalWrite(botm,HIGH);
  digitalWrite(topm,HIGH);
  digitalWrite(botr,HIGH);
  digitalWrite(topr,HIGH);
  digitalWrite(topl,HIGH);
  digitalWrite(botl,HIGH);
 }
   xSemaphoreGive(Sem);
     delay(500);

  }
 }
void setup() {
  lightSetup();
  gearSetup();
  xTaskCreate(lightLoop,"light", 128, NULL, 1, NULL);
  xTaskCreate(gearLoop,"Gear", 128, NULL, 1, NULL);
  Sem = xSemaphoreCreateMutex ();
  Serial.begin(9600);
}
 

void loop() {
  // put your main code here, to run repeatedly:

}
