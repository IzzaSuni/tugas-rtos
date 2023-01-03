#define BLYNK_TEMPLATE_ID "TMPLs94mmoYi"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd1(0x27,16,2);
#include <EEPROM.h>
#define trigPin 14
#define echoPin 12
#define buttonAdd 15
#define buttonMenu 16 //Active LOW
#define buttonPop 0
#define Relay1 9 //active LOW
#define Relay2 13 //active LOW
#define Buzzer 3
#define sizeE 512
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
byte addr=0;      //address set tinggi
byte addr2=200;    //addr flag kondisi 0 untuk tersedia 1 mengisi
byte persen;
byte flag;
byte flag2=7;
byte flag1=0;
char auth[]="5q1tiZ8KGMZ635rWWjgJPeUJhazfRAMK";
char ssid[] = "KONTRAKAN UYE";
char pass[] = "KUSANG123";
float tinggi,cm,temp,kosong,full;
long duration; 
int distance,baca;
int SISTEM=1,buttonRelay1Online,buttonRelay2Online;  //Virtual Pin
bool isConnecteds,tryReconnect=false;
#include <Arduino_FreeRTOS.h>
WidgetLCD lcd(V1);
const int sensor = A1;
int tempc;
float tempf;
float vout;
float adc;

void Main(void *param);
void showData(void *param);
void setTinggi(void *param);
void geetDataFromSensor(void *param);
TaskHandle_t Main;
TaskHandle_t showData;
TaskHandle_t setTinggi;
TaskHandle_t getDataFromSensor;

void setup() {
  Serial.begin(9600);
  int tinggiVal;
  EEPROM.begin(sizeE);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);
  pinMode(Buzzer,OUTPUT);
  pinMode(SISTEM, INPUT);
  pinMode(buttonMenu,INPUT);
  pinMode(buttonAdd,INPUT);
  pinMode(buttonPop, INPUT); 
  digitalWrite(Relay,LOW);
  pinMode(sensor, INPUT);
  lcd1.init();
  lcd1.backlight();
  lcd1.print("SISTEM TANGKI AIR");
  lcd1.clear();
  lcd1.print(" SETTING TINGGI");
  lcd1.setCursor(0,1);
  lcd1.print("TINGGI:");
  tinggi=EEPROM.get(addr,tinggiVal);
  delay(500);
  lcd1.print(tinggi);
  lcd1.print("cm"); 
  delay(1000);
  lcd1.clear();
  full=0.9*tinggi;
  kosong=tinggi-full;
  Blynk.begin(auth, ssid, pass);
  lcd1.clear();
  lcd1.print("SET KONEKSI.");
  lcd1.setCursor(0,1);
  lcd1.print("KE INTERNET.");
  WiFi.begin(ssid, pass); 
  Blynk.config(auth);  // in place of Blynk.begin(auth, ssid, pass);
  Blynk.connect(1111);
  isConnecteds=Blynk.connect();
  lcd1.clear();
  if(isConnecteds==true){
  lcd1.print("SUKSES KONEKSI.");   
   lcd1.setCursor(0,1);
  lcd1.print("MODE ONLINE");
  }
  else if(isConnecteds==false){
  lcd1.print("GAGAL KONEKSI.");   
  lcd1.setCursor(0,1);
  lcd1.print("MODE OFFLINE");
  }
  delay(1000);
  EEPROM.end();
  pinMode(sensor, INPUT);
  xTaskCreate(Main,"Task1",100,NULL,1,&amp;Task_Handle1); 
  xTaskCreate(getDataFromSensor,"Task2",100,NULL,3,&amp;Task_Handle3);
  xTaskCreate(showData,"Task3",100,NULL,1,&amp;Task_Handle1); 
  xTaskCreate(setTinggi,"Task4",100,NULL,2,&amp;Task_Handle2); 
}

void loop() {
}

 


 void showData(void *param){
  (void) param;
  for(;;)
  {
  if(flag1!=5){
  lcd1.setCursor(0,0);
  lcd1.print(persen);
  lcd1.print("%");
  lcd.print(0,0,persen);
  if(persen>=100){
  lcd.print(3,0,"% ");
  }
  if(persen<100)
  {
  lcd1.setCursor(3,0);
  lcd1.print("  ");
  lcd.print(2,0,"%  ");
  }
  if(persen<10)
  {
  lcd1.setCursor(2,0);
  lcd1.print("  ");
  lcd.print(1,0,"%  ");
  }
  }
 
 }
} 

void getDataFromSensor(void *param){
    (void) param;
   for(;;) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // menerima suara ultrasonic
  cm = duration * 0.034 / 2; 
  temp=cm-kosong;
  baca=full-temp;
  persen=(baca/full)*100;
  adc = analogRead(sensor);
  vout = adc / 1023 * 5;
  tempc = vout * 100;
  Blynk.virtualWrite(V7,cm); 
  Blynk.virtualWrite(V8,tempc); 
  }
}

void setTinggi(void *param){
  (void) param;
 for(;;) {
  EEPROM.begin(sizeE);
 int tinggiVal;
 lcd1.clear();
 lcd.clear();
 while(digitalRead(buttonMenu)==LOW){ 
 SoundMenu();
 lcd1.setCursor(0,0);
 lcd1.print("Ofline SetTinggi");
 lcd.print(0,0,"Ofline SetTinggi");
 digitalWrite(Relay,LOW); 
 if(digitalRead(buttonAdd)==HIGH){
  tinggi+=1.0;
  delay(1);
 }
 if(digitalRead(buttonPop)==LOW){
  tinggi-=1.0;
  if(tinggi<30){
    tinggi=30;
    }
  delay(1);
 }
 if(tinggi<100){
  lcd1.setCursor(14,1);
  lcd1.print(" ");
 }
 lcd1.setCursor(0,1);
 lcd1.print("TINGGI:");
 lcd.print(0,1,"TINGGI:");
 lcd1.print(tinggi);
 lcd1.print("cm");
 lcd.print(7,1,tinggi);
 }
 full=0.9*tinggi;
 kosong=tinggi-full;
 lcd1.clear();
 lcd.clear();
 EEPROM.put(addr,tinggiVal=tinggi);
 lcd1.setCursor(0,0);
 lcd1.print("SUDAH DI SET");
 lcd.print(0,0,"SUDAH DI SET");
 lcd1.setCursor(0,1);
 lcd1.print("TINGGI:");
 lcd.print(0,1,"TINGGI:");
 lcd1.print(tinggi);
 lcd1.print("cm"); 
 lcd.print(7,1,tinggi);
 delay(1000);
  lcd1.clear();
 lcd.clear();
 EEPROM.end();
 }
}

void loop() {
}

BLYNK_WRITE(V5){
  int Value=param.asInt();
  SISTEM=Value;
}
BLYNK_WRITE(V6){
  int Value=param.asInt();
  buttonRelay1Online=Value;
}

BLYNK_WRITE(20){
  int Value=param.asInt();
  buttonRelay2Online=Value;
}

void Main(void *param) {    
 isConnecteds=Blynk.connected();
 if(isConnecteds==true){
  Blynk.run(); 
 if(SISTEM==1){
  while(SISTEM==1){
  lcd1.backlight();
  Blynk.run(); 
  EEPROM.begin(sizeE);
  GetData();
  ShowData();
  Condition();
 if(digitalRead(buttonMenu)==LOW){
   SetTinggi();
 }
 if(buttonRelay1Online==1){
    Blynk.run();
    digitalWrite(Relay1,HIGH)
 }
 if(buttonRelay1Online==0){
    Blynk.run();
    digitalWrite(Relay1,LOW)
 }
 if(buttonRelay2Online==1){
    Blynk.run();
    digitalWrite(Relay2,HIGH)
 }
 if(buttonRelay2Online==0){
    Blynk.run();
    digitalWrite(Relay2,LOW)
 }
  EEPROM.end();
  Blynk.virtualWrite(V10,persen);
  Blynk.virtualWrite(V11,cm);
 }
 }
 else if(SISTEM==0){
 while(SISTEM==0){
 lcd1.noBacklight();
 lcd1.setCursor(0,0);
 lcd1.print("Sistem Shut Down");
 lcd1.setCursor(0,1);
 lcd1.print("POWER SAVING...");
 lcd.print(0,0,"Sistem Shut Down");
 lcd.print(0,1,"POWER SAVING...");  
 digitalWrite(Buzzer,LOW);
 digitalWrite(Relay,LOW);
 }
 lcd.clear();
 lcd1.clear();
 }
 Blynk.virtualWrite(V10,persen);
 Blynk.virtualWrite(V11,persen);

 }
 else if(isConnecteds==false){
  lcd1.backlight(); 
  EEPROM.begin(sizeE);
  GetData();
  ShowData();
  Condition();
 if(digitalRead(buttonMenu)==LOW){
   SetTinggi();
 }
  EEPROM.end();
 }
}

 

 
 
