#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 3);

//ร่าง 1
byte one11[8] = {B00011, B00100, B00011, B00000, B00001, B00110, B01000, B01000};
byte one12[8] = {B11111, B00000, B11111, B00000, B11111, B00000, B00000, B00000};
byte one13[8] = {B11000, B00100, B11000, B00000, B10000, B01100, B00010, B00010};
//หน้าปกติ
byte one21[8] = {B10010, B10010, B10000, B10000, B01011, B01000, B00110, B00001};
byte one22[8] = {B10000, B10000, B00000, B00000, B10000, B00000, B00000, B11111};
byte one23[8] = {B00001, B00001, B00001, B00001, B00010, B00010, B01100, B10000};
byte one21_2[8] = {B10000, B10000, B10000, B10000, B01000, B01000, B00110, B00001};
byte one22_2[8] = {B00101, B00101, B00000, B00000, B00111, B00000, B00000, B11111};
//:(
byte bad21[8] = {B10010, B10010, B10000, B10000, B01011, B01100, B00110, B00001};
byte bad22[8] = {B10000, B10000, B00000, B00000, B10000, B01000, B00000, B11111};
byte bad23[8] = {B00001, B00001, B00001, B00001, B00010, B00010, B01100, B10000};
byte bad21_2[8] = {B10000, B10000, B10000, B10000, B01000, B01000, B00110, B00001};
byte bad22_2[8] = {B00101, B00101, B00000, B00000, B01110, B10001, B00000, B11111};
//:)
byte good21[8] = {B10010, B10010, B10000, B10100, B01011, B01000, B00110, B00001};
byte good22[8] = {B10000, B10000, B00000, B01000, B10000, B00000, B00000, B11111};
byte good23[8] = {B00001, B00001, B00001, B00001, B00010, B00010, B01100, B10000};
byte good21_2[8] = {B10000, B10000, B10000, B10000, B01000, B01000, B00110, B00001};
byte good22_2[8] = {B00101, B00101, B00000, B10001, B01110, B00000, B00000, B11111};
//โลงศพ RIP
byte tomb21[8] = {B00011, B00111, B01111, B01111, B11111, B11111, B11111, B11111};
byte tomb22[8] = {B11000, B11100, B11110, B11110, B11111, B11111, B11111, B11111};
byte tomb11[8] = {B00000, B00000, B00000, B00000, B00001, B00011, B00001, B00001};
byte tomb12[8] = {B00000, B00000, B00000, B00000, B10000, B11000, B10000, B10000};

int hp = 30; // hp
int count = 0;

float hpdisplaytime = 0;
bool showHpmessage = false;

float dialogdisplaytime = 0;
bool showDialogmessage = false;

int ballPosition = 5;
int direction = 1;
const int MIN_POS = 0;
const int MAX_POS = 10;
unsigned long lastAnimationTime = 0;

const int BTN = 13;//ปุ่ม
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;

//เลือดลดเท่าไหร่
int minusHp = 1;

//stage
int stage = 1;
int counttime = 0;

// เพิ่มตัวแปรจับเวลา
unsigned long startTime = 0;

unsigned long lastCountTime = 0;
unsigned long lastStageTime = 0;
unsigned long lastMinusOverTime = 0;

int food = 2;

bool isDead = false;

// LED RGB
const int GREEN_PIN = 11;
const int YELLOW_PIN = 10;
const int RED_PIN = 9;

void setup() {

  Serial.begin(9600);

  pinMode(BTN, INPUT_PULLUP);

  startTime = millis();
  
  dht.begin();
  lcd.backlight();
  lcd.init();

  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);

  lcd.setBacklight(50);
}

void led(){
  if (20 <= hp && hp <= 30){
    analogWrite(GREEN_PIN, 255);
    analogWrite(RED_PIN, 0);
    analogWrite(YELLOW_PIN, 0);
  } else if (10 <= hp && hp <= 19){
    analogWrite(GREEN_PIN, 0);
    analogWrite(RED_PIN, 0);
    analogWrite(YELLOW_PIN, 50);
  } else if (1 <= hp && hp <= 9){
    analogWrite(GREEN_PIN, 0);
    analogWrite(RED_PIN, 50);
    analogWrite(YELLOW_PIN, 0);
}
}

void loadNeutralface() {
  //บน
  lcd.createChar(0, one11);
  lcd.createChar(1, one12);
  lcd.createChar(2, one13);
//ล่าง
  lcd.createChar(3, one21);
  lcd.createChar(4, one22);
  lcd.createChar(5, one23);
//ล่างมองขวา
  lcd.createChar(6, one21_2);
  lcd.createChar(7, one22_2);
}

void loadBadface(){
  //บน
  lcd.createChar(0, one11);
  lcd.createChar(1, one12);
  lcd.createChar(2, one13);
//ล่าง
  lcd.createChar(3, bad21);
  lcd.createChar(4, bad22);
  lcd.createChar(5, bad23);
//ล่างมองขวา
  lcd.createChar(6, bad21_2);
  lcd.createChar(7, bad22_2);
}

void loadGoodface(){
  //บน
  lcd.createChar(0, one11);
  lcd.createChar(1, one12);
  lcd.createChar(2, one13);
//ล่าง
  lcd.createChar(3, good21);
  lcd.createChar(4, good22);
  lcd.createChar(5, good23);
//ล่างมองขวา
  lcd.createChar(6, good21_2);
  lcd.createChar(7, good22_2);
}

void loadTombStone(){
  //บน
  lcd.createChar(0, tomb11);
  lcd.createChar(1, tomb12);
  //ล่าง
  lcd.createChar(2, tomb21);
  lcd.createChar(3, tomb22);
}

void drawTombStone(){
  lcd.clear();
  
  loadTombStone();
  
  lcd.setCursor(7, 1);
  lcd.write(0);
  lcd.write(1);
  lcd.setCursor(7, 2);
  lcd.write(2);
  lcd.write(3);
  
  // Write RIP text
  lcd.setCursor(11, 1);
  lcd.print("RIP");

  unsigned long currentTime = (millis() - startTime) / 1000;
  lcd.setCursor(15, 1);
  lcd.print(currentTime / 60);
  lcd.print(":");
  if((currentTime % 60) < 10) lcd.print("0");
  lcd.print(currentTime % 60);

  lcd.setCursor(11, 2);
  lcd.print("RETRY?");
}

void drawMikael1(int pos, int dir){
  //ลบมอนเก่า
  lcd.setCursor(0, 2);
  for(int i = 0; i < 13; i++) {
    lcd.print(" ");
  }
  lcd.setCursor(0, 3);
  for(int i = 0; i < 13; i++) {
    lcd.print(" ");
  }

  // วาดมอนใหม่
  lcd.setCursor(pos, 2);
  lcd.write(0);
  lcd.write(1);
  lcd.write(2);
  
  lcd.setCursor(pos, 3);
  if (dir == 1) {  //มองขวา
    lcd.write(6);
    lcd.write(7);  
  } else {  //มองซ้าย
    lcd.write(3);
    lcd.write(4);  
  }
  lcd.write(5);

}

void resetStat(){
  hp = 30; // hp
  count = 0;

  hpdisplaytime = 0;
  showHpmessage = false;

  dialogdisplaytime = 0;
  showDialogmessage = false;

  ballPosition = 5; 
  direction = 1;

  lastAnimationTime = 0;

  lastButtonState = HIGH;
  lastDebounceTime = 0;

  minusHp = 1;

  stage = 1;
  counttime = 0;

  startTime = 0;

  lastCountTime = 0;
  lastStageTime = 0;
  lastMinusOverTime = 0;
  food = 2;
}
void loop() {
  if (isDead) {
    bool reading = digitalRead(BTN);
    if (reading != lastButtonState && (millis() - lastDebounceTime) > 50) {
      if (reading == HIGH) { 
        resetStat();
        isDead = false;
        lcd.clear();
        startTime = millis();
      }
      lastDebounceTime = millis();
    }
    lastButtonState = reading;
    return;
  }

  led();
  float temperature = dht.readTemperature();

  // อัปเดตเวลา
  unsigned long currentTime = (millis() - startTime) / 1000;
  lcd.setCursor(0, 0);
  lcd.print(currentTime / 60);
  lcd.print(":");
  if((currentTime % 60) < 10) lcd.print("0");
  lcd.print(currentTime % 60);

  lcd.setCursor(13, 2);
  lcd.print("Food:");
  lcd.print(food);

  lcd.setCursor(5, 0);
  lcd.print("ST");
  lcd.print(stage);

  //แสดงอารมณ์
  if (hp >= 10 && hp < 20){
    loadNeutralface();
  } else if (hp < 10) {
    loadBadface();
  } else {
    loadGoodface();
  }

  bool reading = digitalRead(BTN);

  lcd.setCursor(13, 0);
  lcd.print(temperature);
  lcd.write(byte(223));
  lcd.print("C");

  //HP
  lcd.setCursor(12, 1);
  lcd.print("HP:   ");
  lcd.setCursor(12, 1);
  lcd.print("HP:");
  lcd.print(hp);
  lcd.setCursor(17, 1);
  lcd.print("/30");
  if (currentTime % 5 == 0){
    count++;
  }

  // เวลา animation 
  if (millis() - lastAnimationTime >= 1000) {
    drawMikael1(ballPosition, direction);
    ballPosition += direction;
    if (ballPosition >= MAX_POS || ballPosition <= MIN_POS) {
      direction = -direction;
    }
    lastAnimationTime = millis();
    //Serial.print("Temperature: ");
    //Serial.print(temperature);
    //Serial.print(" °C\t");
    //Serial.println(currentTime);
  }

  //เพิ่มเลือด กินข้าว
  if (food != 0){
    if (reading != lastButtonState && (millis() - lastDebounceTime) > 50) {
      if (reading == HIGH) {  // เมื่อปล่อยปุ่ม
        hp += 10;
        showHpmessage = true;
        hpdisplaytime = millis();
        lcd.setCursor(15, 3);
        lcd.print("+10HP");
        showDialogmessage = true;
        dialogdisplaytime = millis();
        lcd.setCursor(0, 1);
        lcd.print("\"YUM! YUM!\"");

        food--;
        
      }
      lastDebounceTime = millis();
    }
    lastButtonState = reading;
  }

  // //ถ้าอุณหภูมิอยู่ระหว่าง 24 - 24.49 หรือ 25.51 - 26 เลือดจะไม่เพิ่ม
  if (currentTime % 5 == 0 && currentTime != lastCountTime) {
    if (temperature > 26 || temperature < 24) { //อุณหภูมิอยู่ระหว่าง temp < 24 หรือ temp > 26 เลือดจะลดตามเวลาที่ผ่านไป ทีละ 5 วิ
      hp -= stage;
      lcd.setCursor(16, 3);
      showHpmessage = true;
      hpdisplaytime = millis();
      lcd.print("-");
      lcd.print(stage);
      lcd.print("HP");
    } else if (24.5 <= temperature && temperature <= 25.5) { //อุณหภูมิอยู่ระหว่าง 24.5-25.5 เลือดจะเพิ่ม +5 ทีละ 5 วิ
      // เพิ่มเลือดเมื่ออุณหภูมิอยู่ในช่วง 24.5 - 25.5 องศา
      hp += 5;
      lcd.setCursor(16, 3);
      showHpmessage = true;
      hpdisplaytime = millis();
      lcd.print("+5HP");
    }
    lastCountTime = currentTime;
  }

  if (currentTime % 30 == 0 && currentTime != lastStageTime) {
    stage++;
    food += 2;
    lastStageTime = currentTime;
  }

  if (currentTime % 2 == 0 && currentTime != lastMinusOverTime && stage >= 2) {
    hp -= stage - 1;
    lcd.setCursor(16, 3);
    showHpmessage = true;
    hpdisplaytime = millis();
    lcd.print("-");
    lcd.print(stage - 1);
    lcd.print("HP");
    lastMinusOverTime = currentTime;
  }

  //ถ้าเกิน 30 จะปรับให้เป็น 30 เท่าเดิม
  if (hp >= 30){
    hp = 30;
  }

  //ถ้าต่ำกว่า 0 จะปรับให้เหลือ 0
  if (hp <= 0){
    hp = 0;
    if (!isDead) { 
      isDead = true;
      drawTombStone();
    }
    return;
  }

  //ทำให้ขวาล่างหาย
  if (showHpmessage && millis() - hpdisplaytime >= 500){
    lcd.setCursor(15, 3);
    lcd.print("      ");
    showHpmessage = false;
  }

  //ทำให้คำพูดด้านบนหาย
  if (showDialogmessage && millis() - dialogdisplaytime >= 500){
    lcd.setCursor(0, 1);
    lcd.print("     ");
    lcd.setCursor(6, 1);
    lcd.print("     ");
    showDialogmessage = false;
  }
    
}
