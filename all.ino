#include <Servo.h>
#include <TM1637TinyDisplay.h>
#include "DHT.h"
#include <Adafruit_MLX90614.h>

#define RED 5
#define GREEN 6
#define BLUE 11
#define SERVO_PIN 8
#define PIEZO_BUZZER 3
#define CLK 9
#define DIO 10
#define BUTTON_1 4
#define BUTTON_2 7
#define VR_PIN A0
#define BRIGHT_PIN A1
#define DHTPIN 2
#define DHTTYPE DHT11

Servo sv;
TM1637TinyDisplay display(CLK, DIO);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  sv.attach(SERVO_PIN);
  display.setBrightness(BRIGHT_7);
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  mlx.begin();
  dht.begin();
}

void loop() {
  if (Serial.available() > 0)
  {
    String strRead = Serial.readStringUntil('\n');
    if (strRead.indexOf("RGB=") != -1)
    {
      int index1 = strRead.indexOf(",");
      int index2 = strRead.indexOf(",", index1 + 1);

      int redV = strRead.substring(4, index1).toInt();
      int greenV = strRead.substring(index1 + 1, index2).toInt();
      int blueV = strRead.substring(index2 + 1, strRead.length()).toInt();

      ledSet(redV, greenV, blueV);
      Serial.println("OKRGB");
    }
    else if (strRead.indexOf("SERVO=") != -1)
    {
      int servoDigree = strRead.substring(6, strRead. length()).toInt();
      if (servoDigree <= 180)
      {
        sv.write(servoDigree);
        Serial.println("OKSERVO");
      }
      else Serial.println("error digree");
    }
    else if (strRead.indexOf("BUZZER=") != -1)
    {
      float bzFreq = strRead.substring(7, strRead.length()).toInt();
      setBuzzer(bzFreq);
    }
    else if (strRead.indexOf("FND=") != -1)
    {
      float fndValue = strRead.substring(4, strRead.length()).toFloat();
      display.showNumber(fndValue);
      Serial.println("OKFND");
    }
    else if (strRead.indexOf("VR=?") != -1)
    {
      Serial.print("VR=");
      Serial.println(analogRead(VR_PIN));
    }
    else if (strRead.indexOf("BRIGHT=?") != -1)
    {
      Serial.print("BRIGHT=");
      Serial.println(analogRead(BRIGHT_PIN));
    }
    else if (strRead.indexOf("TEMPERATURE=?") != -1)
    {
      sendTemperature();
    }
    else if (strRead.indexOf("HUMIDITY=?") != -1)
    {
      sendHumidity();
    }
    else if (strRead.indexOf("OBJECT=?") != -1)
    {
      float objectTemp = mlx.readObjectTempC();
      Serial.print("OBJECT=");
      Serial.println(objectTemp);
    }
    else if (strRead.indexOf("AMBIENT=?") != -1)
    {
      float ambientTemp = mlx.readAmbientTempC();
      Serial.print("AMBIENT=");
      Serial.println(ambientTemp);
    }
  }

  if(btn1() == 1) Serial.println("BUTTON1=CLICK");
  if(btn2() == 1) Serial.println("BUTTON2=CLICK");

}

void ledSet(int red, int green, int blue)
{
  analogWrite(RED, red);
  analogWrite(GREEN, green);
  analogWrite(BLUE, blue);
}

void setBuzzer(int freq)
{
  if (freq > 31)
  {
    tone(PIEZO_BUZZER, freq);
    Serial.println("OKBUZZER");
  }
  else
  {
    noTone(PIEZO_BUZZER);
    Serial.println("OKBUZZER");
  }
}
int btn1()
{
  static int currBtn = 0;
  static int prevBtn = 0;

  currBtn = digitalRead(BUTTON_1);

  if(currBtn != prevBtn)
  {
    prevBtn = currBtn;
    if(currBtn == 1)
    {
      return 1;
    }
    delay(50);
  }

  return 0;
}

int btn2()
{
  static int currBtn = 0;
  static int prevBtn = 0;

  currBtn = digitalRead(BUTTON_2);

  if(currBtn != prevBtn)
  {
    prevBtn = currBtn;
    if(currBtn == 1)
    {
      return 1;
    }
    delay(50);
  }

  return 0;
}

void sendTemperature()
{
  float temperature = dht.readTemperature();
  if (!isnan(temperature))
  {
    Serial.print("TEMPERATURE=");
    Serial.println(temperature);
  }
  else
  {
    Serial.print("TEMPERATURE=");
    Serial.println(0);
  }
}

void sendHumidity()
{
  float humidity = dht.readHumidity();
  if (!isnan(humidity))
  {
    Serial.print("HUMIDITY=");
    Serial.println(humidity);
  }
  else
  {
    Serial.print("HUMIDITY=");
    Serial.println(0);
  }
}