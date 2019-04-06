#include <SoftwareSerial.h> // Bluetooth için seri iletişim kütüphanesi
#include <Servo.h>          // Servo motor için kütüphane

SoftwareSerial bt_iletisim(0, 1); //  Bluetooth sırasıyla RX | TX pinlerini tanıtıyoruz
Servo servoMotor;

//Kullanılan program = ControlJoystick veya Arduino Car

//IŞIKLANDIRMA
const int onFar         = 8 ;// ÖN FAR PİN
const int arkaStop      = 7 ;// ARKA STOP PİN

//Buzzer
const int buzzer        = 6;//Buzzer Pin

//Mesafe Sensörü
const int mesafeSensoruEcho = 3;
const int mesafeSensoruTrig = 2;
long sure;
double mesafe;

//MOTOR PİNLERİ
const int out3          = 10;
const int out4          = 11;
//const int hizKontrol    = A0 ;

//FONSİYONLARIN ÖN TANIMLANMASI
void motorIleri();
void motorGeri();

void servoSagaDondur();
void servoSolaDondur();
void servoOrtala();

void mesafeOlc();
void mesafeKontrol();

void onFarYak();
void arkaStopYak();
void dortluYak();

void kornaCal();

char komut;//Bluetooth ile gelen komut

void setup()
{
  Serial.begin(9600);
 
  //BLUETOOTH SETUP
  bt_iletisim.begin(9600);
  
  //SERVO Setup
  servoMotor.attach(5);

  //Mesafe Sensörü Setup
  pinMode(mesafeSensoruTrig, OUTPUT);
  pinMode(mesafeSensoruEcho, INPUT);

  //IŞIKLANDIRMA PİN TANITIMI
  pinMode(onFar, OUTPUT);
  pinMode(arkaStop, OUTPUT);

  //Buzzer Tanıtımı
  pinMode(buzzer, OUTPUT);

  //MOTOR PİN TANITIMI
  pinMode(out3, OUTPUT);
  pinMode(out4, OUTPUT);
  //pinMode(hizKontrol,OUTPUT);
}
void loop()
{
  //mesafeOlc();
  //mesafeKontrol();
  if (Serial.available() > 0)
  {
    komut = Serial.read();
    Serial.println(komut);
        
    //MOTOR KONTROLLERİ
    if (komut == 'F')
    {
      motorIleri();
      servoOrtala();
    }
    if (komut == 'B')
    {
      motorGeri();
      servoOrtala();
    }
    if (komut == 'S' || komut == 'D')
    {
      motorDur();
      servoOrtala();
    }
    //SERVO KONTROLLERİ
    if (komut == 'L')
    {
      servoSolaDondur();
      motorDur();
    }
    if (komut == 'R')
    {
      servoSagaDondur();
      motorDur();
    }
    //ORTA YÖNLER
    if (komut == 'G')
    {
      servoSolaDondur();
      motorIleri();
    }
    if (komut == 'I')
    {
      servoSagaDondur();
      motorIleri();
    }
    if (komut == 'H')
    {
      servoSolaDondur();
      motorGeri();
    }
    if (komut == 'J')
    {
      servoSagaDondur();
      motorGeri();
    }
    //KORNA
    if (komut == 'V' || komut == 'v')
    {
      kornaCal();
    }
    //FAR KONTROLLERİ
    if (komut == 'W' || komut == 'w')
    {
      onFarYak();
    }
    if (komut == 'U' || komut == 'u')
    {
      arkaStopYak();
    }
    if (komut == 'X' || komut == 'x')
    {
      Serial.println("BOŞ KOD");
      //dortluYak();
    }
  }
  else
  {
    motorDur();
    servoOrtala();
  }
}
// #########################   MOTOR FONKSİYONLARI  ####################
// #  Hız aralığı 99-255
void motorIleri()
{
  digitalWrite(out3, HIGH);
  digitalWrite(out4, LOW);
  //analogWrite(hizKontrol,200);
}
void motorGeri()
{
  digitalWrite(out3, LOW);
  digitalWrite(out4, HIGH);
  //analogWrite(hizKontrol,200);
}
void motorDur()
{
  digitalWrite(out3, HIGH);
  digitalWrite(out4, HIGH);
}
// #########################   SERVO FONKSİYONLARI  ########################
//SERVO DİREKSİYON ARALIKLARI (minimum:77 maksimum:99 orta:88)

void servoSagaDondur()
{
  servoMotor.write(99);
}
void servoSolaDondur()
{
  servoMotor.write(77);
}
void servoOrtala()
{
  servoMotor.write(88);
}

// ######################### IŞIKLANDIRMA FONKSİYONLARI #####################

void onFarYak()
{
  digitalWrite(onFar, !digitalRead(onFar)); // ÖN FAR YAK - SONDUR
}
void arkaStopYak()
{
  digitalWrite(arkaStop, !digitalRead(arkaStop)); // ARKA STOP YAK -SONDUR
}
void dortluYak()
{
  digitalWrite(arkaStop, !digitalRead(arkaStop));
  delay(500);
  digitalWrite(arkaStop, !digitalRead(arkaStop));
  delay(500);
}

// ######################### BUZZER FONKSİYONLARI ####################

void kornaCal()
{
  digitalWrite(buzzer, !digitalRead(buzzer));
}

// #################### MESAFE FONKSİYONLARI ########################

void mesafeOlc()
{
  digitalWrite(mesafeSensoruTrig, HIGH);  
  delayMicroseconds(1000);
  digitalWrite(mesafeSensoruTrig, LOW);

  sure = pulseIn(mesafeSensoruEcho, HIGH);
  //Gidiş geliş olduğu için geçen süreyi ikiye bölüyoruz ve 
  // sonucunuda ses hızına bölüyoruz(ses hizi=343 m/s)
  mesafe = (sure/2) / 29.1;

  Serial.print("Mesafe=");
  Serial.println(mesafe);
}
void mesafeKontrol()
{
  if(mesafe<10)
  {
    motorGeri();
    delay(50);
    motorDur();
  }   
}