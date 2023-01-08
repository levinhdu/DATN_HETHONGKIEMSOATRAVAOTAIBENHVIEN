#include <LiquidCrystal_I2C.h> 
#include <Adafruit_MLX90614.h>
#include <SoftwareSerial.h>

#define Rx1 12
#define Tx1 13
#define Rx2 11
#define Tx2 10
#define Buzz 3
#define Pum 4
#define HN1 A1
#define HN2 A2
#define Interrup 2

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
LiquidCrystal_I2C lcd(0x27,16,2);
SoftwareSerial my1(Rx1 ,Tx1);
SoftwareSerial my2(Rx2 ,Tx2);
String Data = "";
int Mode;
int percent = 1;
int a = 0;
void SetTimePumb()
{ 
  if(digitalRead(Interrup) == 0){
    a++;
    if(a==1)
    {
      Mode = 4;  
    }
    else if(a==2){
      Mode = 5;
      a = 0;
   }
  } 
}

void setup() {
  // put your setup code here, to run once:
  pinMode(Buzz,OUTPUT);
  pinMode(Pum,OUTPUT);
  pinMode(HN1,INPUT);
  pinMode(HN2,INPUT);
  pinMode(Interrup, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Interrup),SetTimePumb, CHANGE);
  mlx.begin();
  lcd.init();
  lcd.backlight();
  my2.begin(9600);
  my1.begin(9600);
  Mode = 1;
  displayStart();
}

void loop() {
 if(Mode == 1){
    ReadID();
 }
 else if(Mode == 2){
    ReadTemperture();
 }
 else if(Mode == 3){
    Pumb(percent);
    }
  else if(Mode == 4){
    ReadAnalog();
  }
  else if(Mode == 5){
    displayStart();
    Mode = 1;
  }
}
void ReadID(){
  if(my1.available())
  {
    String Id = my1.readString();
    Data = "?id=" + Id;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ma nhan vien");
    lcd.setCursor(3,1);
    lcd.print(Id);
    delay(2000);
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Moi do nhiet do");
    Mode = 2;
    }
}


void ReadTemperture(){
  if(digitalRead(HN1) == LOW){
    digitalWrite(Buzz,HIGH);
    float temp = mlx.readObjectTempC(); 
    Data = Data + "&temp=" + String(temp+1.5); 
    my2.print(Data);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nhiet do cua ban");
    lcd.setCursor(3,1);
    lcd.print(temp+1.5);
    lcd.setCursor(8,1);
    lcd.print("*C");
    digitalWrite(Buzz,LOW);
    if(temp > 37.5){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Ban dang bi sot");
      digitalWrite(Buzz,HIGH);
      delay(5000);
      digitalWrite(Buzz,LOW);
      Mode = 3;
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("Moi sat");
      lcd.setCursor(2,1);
      lcd.print("khuan tay");
    }else{
      delay(1000);
      Mode = 3;
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("Moi sat");
      lcd.setCursor(2,1);
      lcd.print("khuan tay");
    }    
  }
}

void Pumb(int percent){
  if(digitalRead(HN2) == LOW){
        digitalWrite(Pum,HIGH);
        delay(percent*100);
        digitalWrite(Pum,LOW);
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("Cam on ban!!!!");
        delay(2000);
        displayStart();
        Mode = 1;
    } 
}

void ReadAnalog(){
  percent = map(analogRead(A3),0,1024,1,10);
  String t = String((percent*10)) + "%";
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Muc bom: ");
  lcd.setCursor(9,0);
  lcd.print(t);
  delay(500);
}

void displayStart(){   
    lcd.clear();                  
    lcd.setCursor(0,0);
    lcd.print("    Moi quet");
    lcd.setCursor(1,1);
    lcd.print("ma nhan vien");
}
