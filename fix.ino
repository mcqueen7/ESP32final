#include <BluetoothSerial.h>
#include "pitches.h"
#include <Servo.h>
#define LED 13
#define RS 33      //右
#define LS 25      //左
#define IN1 12
#define IN2 14
#define IN3 26
#define IN4 27
#define triggerPin 32 //用來trigger
#define echoPin  35 //用來接收echo
#define servo1pin 16
#define servo2pin 17
#define musicchannel 3
#define hornchannel 4
int s1, s2;
long duration, cm, inches;
int dangerstate=0; //0=normal,1=less than 7,2=less than 3
BluetoothSerial BT;
Servo servo1;
Servo servo2;
int  servo1pos=90;
int  servo2pos=90;
bool musicstate=0; //0 pause,1 play
int musici; //for music play;
int hornstate=0; //0 stop,1 ring
int horni; //for horn play;
int musicntime=0;
int musicptime=0;
int hornntime=0;
int hornptime=0;
int musicpausetime=0; //formusicplaying
int hornpausetime=0; //forhorncplaying
int selfgostate=1; //0 control, 1 selfgo

int danger1melody[]{    //total 8
  NOTE_C6,0,NOTE_C6,0,NOTE_C6,0,NOTE_C6,
};
int danger1duration[]{
  4,4,4,4,4,4,4,4,
};
int danger2melody[]{  //total 8
  NOTE_C7,0,NOTE_C7,0,NOTE_C7,0,NOTE_C7,
};
int danger2duration[]{  
  16,16,16,16,16,16,16,16,
};
int hornmelody[]{ //total 8
  NOTE_F6,NOTE_AS6,NOTE_F6,NOTE_AS6,NOTE_F6,NOTE_AS6,NOTE_F6,NOTE_AS6,
};
int hornduration[]{
  4,4,4,4,4,4,4,4,
};
int musicmelody[] = {   //total 395
     NOTE_D4, NOTE_A4, NOTE_D4, NOTE_E4, NOTE_B4, 0, NOTE_A4, NOTE_E5, NOTE_G4, NOTE_A4, NOTE_E5,NOTE_A4, NOTE_E5,      //13*2=26
     NOTE_D4, NOTE_A4, NOTE_D4, NOTE_E4, NOTE_B4, 0, NOTE_A4, NOTE_E5, NOTE_G4, NOTE_A4, NOTE_E5,0, NOTE_E5,

     NOTE_A4, NOTE_C5, NOTE_D5, NOTE_E5, 0, NOTE_E5, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_E5, NOTE_D5, NOTE_CS5,0,        //55
     NOTE_A4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,NOTE_A4,NOTE_A4,NOTE_B4,NOTE_C5,NOTE_B4,NOTE_A4,
     NOTE_A4, NOTE_C5, NOTE_D5, NOTE_E5, 0, NOTE_E5, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_E5, NOTE_D5, NOTE_CS5,0,
     NOTE_A4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_A4,NOTE_G4,NOTE_A4,NOTE_A4,NOTE_C5,NOTE_A4,NOTE_G4,NOTE_A4, NOTE_A5, NOTE_A5, 

     NOTE_E5, NOTE_DS5, NOTE_E5,0 , NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_A5, NOTE_E5, NOTE_D5, NOTE_CS5,
     NOTE_E5, NOTE_E5, NOTE_E5,NOTE_E5, NOTE_D5, NOTE_D5, NOTE_D5,NOTE_C5, NOTE_C5, NOTE_C5,NOTE_B4, NOTE_B4, NOTE_B4,       //32
     NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_A4,
     
     NOTE_E5,NOTE_D5,NOTE_D5,NOTE_E5,NOTE_D5,NOTE_C5,NOTE_B4,0,                 //62
     NOTE_B4,NOTE_C5,NOTE_C5,NOTE_D5,NOTE_C5,NOTE_B4,NOTE_A4,0,
     NOTE_A4,NOTE_B4,NOTE_A4,NOTE_GS4,NOTE_A4,NOTE_G5,NOTE_D5,NOTE_E5,
     NOTE_E5,NOTE_DS5,NOTE_E5,NOTE_DS5,NOTE_E5,NOTE_DS5,
     NOTE_E5,NOTE_D5,NOTE_D5,NOTE_E5,NOTE_D5,NOTE_C5,NOTE_B4,0,
     NOTE_B4,NOTE_C5,NOTE_C5,NOTE_D5,NOTE_C5,NOTE_B4,NOTE_A4,0,
     NOTE_A4,NOTE_B4,NOTE_A4,NOTE_GS4,NOTE_A4,NOTE_C5,NOTE_B4,NOTE_A4,NOTE_A4,
     NOTE_E5,NOTE_DS5,NOTE_E5,NOTE_DS5,NOTE_E5,NOTE_DS5,

     NOTE_D4, NOTE_A4, NOTE_D4, NOTE_E4, NOTE_B4, 0, NOTE_A4, NOTE_E5, NOTE_G4, NOTE_A4, NOTE_E5,NOTE_A4, NOTE_E5,      //13*2=26
     NOTE_D4, NOTE_A4, NOTE_D4, NOTE_E4, NOTE_B4, 0, NOTE_A4, NOTE_E5, NOTE_G4, NOTE_A4, NOTE_E5,0, NOTE_E5,

     NOTE_A4, NOTE_C5, NOTE_D5, NOTE_E5, 0, NOTE_E5, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_E5, NOTE_D5, NOTE_CS5,0,        //55-2=53
     NOTE_A4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,NOTE_A4,NOTE_A4,NOTE_B4,NOTE_C5,NOTE_B4,NOTE_A4,
     NOTE_A4, NOTE_C5, NOTE_D5, NOTE_E5, 0, NOTE_E5, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_E5, NOTE_G5, NOTE_A5,0,
     NOTE_A5, NOTE_A5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_B5,NOTE_A5,NOTE_A5,NOTE_B5,NOTE_C6,NOTE_B5,NOTE_A5,NOTE_A5, 

     NOTE_DS6,NOTE_E6,NOTE_DS6,NOTE_D6,NOTE_DS6,NOTE_E6,NOTE_DS6,NOTE_D6,NOTE_DS6,NOTE_E6,NOTE_DS6,NOTE_D6,NOTE_CS6,
     NOTE_DS6,NOTE_E6,NOTE_DS6,NOTE_D6,NOTE_DS6,NOTE_E6,NOTE_DS6,NOTE_D6,NOTE_DS6,NOTE_E6,NOTE_DS6,NOTE_D6,NOTE_CS6,      //13+13+15+11=39
     NOTE_C6,NOTE_C6,NOTE_B5,NOTE_C6,NOTE_B5,NOTE_A5,NOTE_B5,NOTE_C6,NOTE_B5,NOTE_A5,NOTE_B5,NOTE_C6,NOTE_B5,NOTE_AS5,NOTE_A5,
     NOTE_A5,NOTE_A5,NOTE_A5,NOTE_A5,NOTE_A5,NOTE_E5,NOTE_D5,NOTE_D5,NOTE_C5,NOTE_B4,NOTE_A4,

     NOTE_E5, NOTE_DS5, NOTE_E5,0 , NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_A5, NOTE_E5, NOTE_D5, NOTE_CS5,
     NOTE_E5, NOTE_E5, NOTE_E5,NOTE_E5, NOTE_D5, NOTE_D5, NOTE_D5,NOTE_C5, NOTE_C5, NOTE_C5,NOTE_B4, NOTE_B4, NOTE_B4,       //32
     NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_A4,  
     
     NOTE_E5,NOTE_D5,NOTE_D5,NOTE_E5,NOTE_D5,NOTE_C5,NOTE_B4,0,                 //62-6=56+1
     NOTE_B4,NOTE_C5,NOTE_C5,NOTE_D5,NOTE_C5,NOTE_B4,NOTE_A4,0,
     NOTE_A4,NOTE_B4,NOTE_A4,NOTE_GS4,NOTE_A4,NOTE_G5,NOTE_D5,NOTE_E5,
     NOTE_E5,NOTE_DS5,NOTE_E5,NOTE_DS5,NOTE_E5,NOTE_DS5,
     NOTE_E5,NOTE_D5,NOTE_D5,NOTE_E5,NOTE_D5,NOTE_C5,NOTE_B4,0,
     NOTE_B4,NOTE_C5,NOTE_C5,NOTE_D5,NOTE_C5,NOTE_B4,NOTE_A4,0,
     NOTE_A4,NOTE_B4,NOTE_A4,NOTE_GS4,NOTE_A4,NOTE_C5,NOTE_B4,NOTE_A4,NOTE_A4,0
     

     
};
int musicduration[] = {
    4,8,8,8,8,8,4,4,8,8,8,8,8,
    4,8,8,8,8,8,4,4,8,8,8,8,8,  
    
    8,8,8,2,8,8,8,8,8,8,8,8,8,8,
    6,16,8,8,4,8,8,8,8,4,4,4,
    8,8,8,2,8,8,8,8,8,8,8,8,8,8,
    6,16,8,8,4,16,8,16,16,8,16,16,4,8,8,
  
    8,8,8,8,8,8,8,4,4,4,4,
    8,16,16,8,16,16,8,16,16,8,16,16,8,
    16,16,8,16,16,8,8,4,
    
    8,4,16,6,16,16,8,8,
    8,6,8,6,16,6,8,8,
    8,8,8,8,8,4,8,4,
    8,8,8,8,8,8,
    8,4,16,6,16,16,8,8,
    8,6,8,6,16,6,8,8,
    8,8,8,8,8,4,8,16,4,
    8,8,8,8,8,8,

    4,8,8,8,8,8,4,4,8,8,8,8,8,
    4,8,8,8,8,8,4,4,8,8,8,8,8, 

    8,8,8,2,8,8,8,8,8,8,8,8,8,8,
    6,16,8,8,4,8,8,8,8,4,4,4,
    8,8,8,2,8,8,8,8,8,8,8,8,8,8,
    6,16,8,8,4,16,8,16,16,8,8,16,16,

    8,8,8,8,8,8,8,8,8,8,8,8,4,
    8,8,8,8,8,8,8,8,8,8,8,8,4,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,4,
    8,8,16,16,8,8,8,8,8,8,2,

    8,8,8,8,8,8,8,4,4,4,4,
    8,16,16,8,16,16,8,16,16,8,16,16,8,
    16,16,8,16,16,8,8,4,
    
    8,4,16,6,16,16,8,8,
    8,6,8,6,16,6,8,8,
    8,8,8,8,8,4,8,4,
    8,8,8,8,8,8,
    8,4,16,6,16,16,8,8,
    8,6,8,6,16,6,8,8,
    8,8,8,8,8,4,8,16,4,2
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  delay(3000);
  BT.begin("mcqueen"); //bluetooth name
  pinMode(LED, OUTPUT);
  pinMode(triggerPin, OUTPUT); //發送超音波
  pinMode(echoPin, INPUT);  //讀取超音波

  pinMode(RS,INPUT);
  pinMode(LS,INPUT);
  
  ledcSetup(musicchannel,2000,8);
  ledcAttachPin(4,musicchannel);  //pin4 music buzzer
  ledcSetup(hornchannel,2000,8);
  ledcAttachPin(2,hornchannel); //pin2 horn buzzer

  servo1.attach(servo1pin);
  servo2.attach(servo2pin); 
  servo1.write(servo1pos);
  servo2.write(servo2pos);
  
}
void backward(int d) {
  digitalWrite(IN1, 1); digitalWrite(IN2, 0);
  digitalWrite(IN3, 1); digitalWrite(IN4, 0);
  delay(d);
}
void forward(int d) {
  digitalWrite(IN1, 0); digitalWrite(IN2, 1);
  digitalWrite(IN3, 0); digitalWrite(IN4, 1);
  delay(d);
}
void stopRL(int d) {
  digitalWrite(IN1, 1); digitalWrite(IN2, 1);
  digitalWrite(IN3, 1); digitalWrite(IN4, 1);
}
void turnR(int d) {
  digitalWrite(IN1, 0); digitalWrite(IN2, 1);
  digitalWrite(IN3, 1); digitalWrite(IN4, 0);
  delay(d);
}
void turnL(int d) {
  digitalWrite(IN1, 1); digitalWrite(IN2, 0);
  digitalWrite(IN3, 0); digitalWrite(IN4, 1);
  delay(d);
}
void danger(){
  //發送超音波
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);  // 給 Trig 高電位，持續 10微秒
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  //接收超音波
  duration = pulseIn(echoPin, HIGH);   // 收到高電位時的時間
  //將回傳時間換算成距離 cm
  cm = (duration/2) / 29.1;          
  Serial.print(cm);
  Serial.println("cm");
    if(cm<=4){
      dangerstate=2;
      hornntime=millis();
     if(hornntime- hornptime>= hornpausetime){
      ledcWriteTone(hornchannel,0);
      if(horni==8)horni=0;
      ledcWriteTone( hornchannel, danger2melody[horni]);
      hornpausetime=2000/danger2duration[horni];  
      horni++;
      hornptime=millis();
     }
    }
    else if(cm<=8){
      dangerstate=1;
       hornntime=millis();
     if(hornntime- hornptime>= hornpausetime){
      ledcWriteTone(hornchannel,0);
      if(horni==8)horni=0;
      ledcWriteTone( hornchannel, danger1melody[horni]);
      hornpausetime=2000/danger1duration[horni];  
      horni++;
      hornptime=millis();
     }
      }
    else dangerstate=0;
  
  }
void selfgo() {
  s1 = analogRead(RS); 
  s2 = analogRead(LS);
  Serial.print("right="); 
  Serial.println(s1); 
  Serial.print("left="); 
  Serial.println(s2);
  if(dangerstate==0){
    ledcWriteTone(hornchannel,0);
    horni=0;
    if (s1 < 1000 && s2 < 1000) {
       Serial.println("F"); 
      forward(3);
      stopRL(6);
    }
    else if (s2 > 1000) {
       Serial.println("L"); 
      turnL(3);
      stopRL(5);
    }
    else if (s1 > 1000){
       Serial.println("R"); 
      turnR(3);
      stopRL(5);
    } 
  }
  else if(dangerstate==2){
    backward(3);
     stopRL(5);
   }
  else{
    stopRL(5);
    } 
  musicntime=millis();
    if(musicntime- musicptime>=musicpausetime){
      ledcWriteTone(musicchannel,0);
      if(musici==395)musici=0;
      ledcWriteTone( musicchannel, musicmelody[musici]);
      musicpausetime=2000/musicduration[musici];  
      musici++;
      musicptime=millis();
     }
}
void disconnection(){
  Serial.println("disconnection");
  selfgostate=1;
  musicstate=0;
  hornstate=0;
  servo1pos=90;servo1.write(servo1pos);
}
void check(){
  switch (char(BT.read())) {
      case 'a': digitalWrite(LED, HIGH);Serial.println("a"); break;
      case 'b': digitalWrite(LED, LOW); Serial.println("b");break;
      case 'F': if(dangerstate==0)forward(50); Serial.println("F");  break;
      case 'B': backward(50); Serial.println("B");  break;
      case 'L': if(dangerstate==0||dangerstate==1)turnL(30); Serial.println("L"); break;
      case 'R': if(dangerstate==0||dangerstate==1)turnR(30); Serial.println("R"); break;
      case 'S': stopRL(150);Serial.println("S"); break;
      case 'D':disconnection();break;
      case 'H':hornstate=!(hornstate); Serial.println(hornstate);break;
      case 'M':musicstate=!(musicstate);Serial.println(musicstate);break;
      case 'r':servo2pos+=30;if(servo2pos>180)servo2pos=180;servo2.write(servo2pos);Serial.println("r");break;
      case 'l':servo2pos-=30;if(servo2pos<0)servo2pos=0;servo2.write(servo2pos);Serial.println("l");break;
      case 'O':servo1pos+=30;if(servo1pos>180)servo1pos=180;servo1.write(servo1pos);Serial.println("O");break;
      case 'C':servo1pos-=30;if(servo1pos<90)servo1pos=90;servo1.write(servo1pos);Serial.println("C");break; 
   }
   
  
  }
void loop() {
  danger();
 if(BT.available()) {
    selfgostate=0;
     check();  
  }

if(selfgostate==1)selfgo();
else{
   if(musicstate==1){
      musicntime=millis();
      if(musicntime- musicptime>=musicpausetime){
        ledcWriteTone(musicchannel,0);
        if(musici==395)musici=0;
        ledcWriteTone( musicchannel, musicmelody[musici]);
        musicpausetime=2000/musicduration[musici];  
        musici++;
        musicptime=millis();
     }
    }
    else ledcWriteTone(musicchannel,0);
    if(hornstate==1&&dangerstate==0){
      hornntime=millis();
      if(hornntime- hornptime>= hornpausetime){
      ledcWriteTone(hornchannel,0);
      if(horni==8)horni=0;
      ledcWriteTone( hornchannel, hornmelody[horni]);
      hornpausetime=2000/hornduration[horni];  
      horni++;
      hornptime=millis();
     } 
    }
    else if(hornstate==0&&dangerstate==0)ledcWriteTone(hornchannel,0);
    else if(dangerstate==1){
        hornntime=millis();
     if(hornntime- hornptime>= hornpausetime){
      ledcWriteTone(hornchannel,0);
      if(horni==8)horni=0;
      ledcWriteTone( hornchannel, danger1melody[horni]);
      hornpausetime=2000/danger1duration[horni];  
      horni++;
      hornptime=millis();
     }
      }
    else if(dangerstate==2){
       hornntime=millis();
     if(hornntime- hornptime>= hornpausetime){
      ledcWriteTone(hornchannel,0);
      if(horni==8)horni=0;
      ledcWriteTone( hornchannel, danger2melody[horni]);
      hornpausetime=2000/danger2duration[horni];  
      horni++;
      hornptime=millis();
     }    
      }  
  
  }

}
