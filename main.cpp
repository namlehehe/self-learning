#include <Arduino.h>
//ledPin
const int x1=8;
const int v1=9;
const int d1=10;
const int x2=11;
const int v2=12;
const int d2=13;
//74HC595 Pin
const int SCLK=7;
const int RCLK=6;
const int DIO=5;
//nút bấm
const int BT_AUTO=A0;
const int BT_OFF=A1;
const int BT_FLASH=A2;
const int BT_UUTIEN_1=A3;
const int BT_UUTIEN_2=A4;


//******************************************************************************************************************************************************************
unsigned char maled[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90}; //led 7 đoạn anode(dương chung)


int time_x1=10 , time_v1=3 ; //cài đặt thời gian đếm ngược cho đèn xanh là 10s, vàng là 3s (trục1)
int time_x2=10 , time_v2=3 ;//cài đặt thời gian đếm ngược cho đèn xanh là 10s, vàng là 3s (trục2)
int time_truc1 , time_truc2 ;
int t=0;//biến đếm t
unsigned long time_delay = 0;
int Mode=0;

//khai báo các hàm
//******************************************************************************************************************************************************************
void x1_sang();
void v1_sang();
void d1_sang();
void x2_sang();
void v2_sang();
void d2_sang();
void Truc1_tat();
void Truc2_tat();
void Mode_Off();
void Led7_tat();
void Mode_Auto();
void DK_LED_MODE_AUTO();
void DK_TIME_MODE_AUTO();
void hienthi(int duong1, int duong2);
void Mode_Flash();
void Mode_UuTienTruc1();
void Mode_UuTienTruc2();
//******************************************************************************************************************************************************************

//trạng thái trục đèn led1
//******************************************************************************************************************************************************************
void x1_sang() 
{
  digitalWrite(x1,HIGH); 
  digitalWrite(v1,LOW); 
  digitalWrite(d1,LOW);
}

void v1_sang()
{
  digitalWrite(x1,LOW); 
  digitalWrite(v1,HIGH); 
  digitalWrite(d1,LOW);
} 

void d1_sang() 
{
  digitalWrite(x1,LOW); 
  digitalWrite(v1,LOW); 
  digitalWrite(d1,HIGH);
}
//******************************************************************************************************************************************************************

//trạng thái trục đèn led2
//******************************************************************************************************************************************************************
void x2_sang()
{
  digitalWrite(x2,HIGH); 
  digitalWrite(v2,LOW); 
  digitalWrite(d2,LOW);
} 

void v2_sang()
{
  digitalWrite(x2,LOW); 
  digitalWrite(v2,HIGH); 
  digitalWrite(d2,LOW);
} 

void d2_sang()
{
  digitalWrite(x2,LOW); 
  digitalWrite(v2,LOW); 
  digitalWrite(d2,HIGH);
}
//******************************************************************************************************************************************************************
 void setup()
{
  pinMode(x1,OUTPUT);  //thiết lập các gpio là ngõ ra
  pinMode(v1,OUTPUT);
  pinMode(d1,OUTPUT);
  pinMode(x2,OUTPUT);
  pinMode(v2,OUTPUT);
  pinMode(d2,OUTPUT);
  pinMode(SCLK,OUTPUT); //74HC595 PIN
  pinMode(RCLK,OUTPUT);
  pinMode(DIO,OUTPUT);

  pinMode(BT_AUTO,INPUT_PULLUP);
  pinMode(BT_OFF,INPUT_PULLUP);
  pinMode(BT_FLASH,INPUT_PULLUP);
  pinMode(BT_UUTIEN_1,INPUT_PULLUP);
  pinMode(BT_UUTIEN_2,INPUT_PULLUP);
  time_delay=millis(); //thiết lập hàm ngắt mềm
}

//******************************************************************************************************************************************************************
 void loop()
{
  if(digitalRead(BT_AUTO)==LOW)
  {
    Mode=1;
  }
  if(digitalRead(BT_OFF)==LOW)
  {
    Mode=0;
  }
  if(digitalRead(BT_FLASH)==LOW)
  {
    Mode=2;
  }
  if(digitalRead(BT_UUTIEN_1)==LOW)
  {
    Mode=3;
  }
  if (digitalRead(BT_UUTIEN_2)==LOW)
  {
    Mode=4;
  }
  switch (Mode)
  {
  case 0:
    Mode_Off();
    break;
  case 1:
    Mode_Auto();
    break;
  case 2:
    Mode_Flash();
    break;
  case 3:
    Mode_UuTienTruc1();
    break;
  case 4:
    Mode_UuTienTruc2();
    break;
  
  default:Mode=0;
    break;
  }
}
//******************************************************************************************************************************************************************
 void Mode_Auto()
{
  if((unsigned long)(millis()-time_delay)>=1000)
  {
    t++;
    time_truc1--;
    time_truc2--;
    time_delay=millis();
  }
  if(t>=time_x1 + time_v1 + time_v2 + time_x2)
  {
    t=0;
  }
  DK_LED_MODE_AUTO();
  DK_TIME_MODE_AUTO();
  hienthi(time_truc1,time_truc2);
}
//******************************************************************************************************************************************************************
void DK_LED_MODE_AUTO()
{
  if(t>=0 && t<time_x1)
  {
    x1_sang();
    d2_sang();
  }

  if(t>=time_x1 && t<time_x1 + time_v1)
  {
    v1_sang();
    d2_sang();
  }
  
  if(t>=time_x1 + time_v1 && t<time_x1 + time_v1 + time_x2)
  {
    d1_sang();
    x2_sang();
  }

  if(t>=time_x1 + time_v1 + time_x2 && t<time_x1 + time_v1 + time_x2 +time_v2)
  {
    d1_sang();
    v2_sang();
  }
}

//******************************************************************************************************************************************************************
void DK_TIME_MODE_AUTO()
{
  if(t==0)
  {
    time_truc1=time_x1;
    time_truc2=time_x1 + time_v1;
  }
  
  if(t==time_x1)
  {
    time_truc1=time_v1;
    time_truc2=time_v1;
  }
  
  if(t==time_x1 + time_v1)
  {
    time_truc1=time_x2 + time_v2;
    time_truc2=time_x2;
  }
  
  if(t==time_x1 + time_v1 + time_x2)
  {
    time_truc1=time_v2;
    time_truc2=time_v2;
  }
}

//******************************************************************************************************************************************************************
void hienthi(int duong2,int duong1)
{
   shiftOut(DIO, SCLK, MSBFIRST, maled[duong2%10]);
   shiftOut(DIO, SCLK, MSBFIRST, maled[duong2/10]);
   shiftOut(DIO, SCLK, MSBFIRST, maled[duong1%10]);
   shiftOut(DIO, SCLK, MSBFIRST, maled[duong1/10]);
   digitalWrite(RCLK,LOW);
   digitalWrite(RCLK,HIGH);
}
//******************************************************************************************************************************************************************

//Mode Off
//******************************************************************************************************************************************************************
void Mode_Off()
{
  Truc1_tat();
  Truc2_tat();
  Led7_tat();
}
 
void Led7_tat()
{
  shiftOut(DIO,SCLK,MSBFIRST,0xFF); //tắt 2 led 7 đoạn
  shiftOut(DIO,SCLK,MSBFIRST,0xFF);
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);
}

//Trạng thái trục
//******************************************************************************************************************************************************************
void Truc1_tat()
{
  digitalWrite(x1,LOW); 
  digitalWrite(v1,LOW); 
  digitalWrite(d1,LOW);
} 

void Truc2_tat()
{
  digitalWrite(x2,LOW); 
  digitalWrite(v2,LOW); 
  digitalWrite(d2,LOW);
}
//******************************************************************************************************************************************************************

//Mode Flash
//******************************************************************************************************************************************************************
void Mode_Flash()
{
  if((unsigned long)(millis()-time_delay)>=1000)
  {
    t++;
    time_delay=millis();
  }

  if(t>=2)
  {
    t=0;
  }
  if(t>=0 && t<1)
  {
    v1_sang();
    v2_sang();
  }
  else
  {
    Truc1_tat();
    Truc2_tat();
  }
  Led7_tat();
}
//******************************************************************************************************************************************************************

//Mode ưu tiên 1
//******************************************************************************************************************************************************************
void Mode_UuTienTruc1()
{
  d2_sang();
  Led7_tat();
  v1_sang();
}

//Mode ưu tiên 2
//******************************************************************************************************************************************************************
void Mode_UuTienTruc2()
{
  d1_sang();
  Led7_tat();
  v2_sang();
}