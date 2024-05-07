#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>;
#include <LiquidCrystal_I2C.h>;
LiquidCrystal_I2C lcd(0x27,16,2);
int led = 8 ;                // the pin that the LED is atteched to
int sensor1 = 2;              // the pin that the sensor1 is atteched to
int state1 = LOW;             // by default, no motion detected
int val1 = 0;

int maxspeed = 190;
int sensor2 = 4;              // the pin that the sensor is atteched to
int state2 = LOW;             // by default, no motion detected
int val2 = 0; 


int enA = 5;
int in1 = 6;
int in2 = 7;

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int Number_of_people = 0;

void setup() 
{

  // Set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);

	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);


  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();

  lcd.init();       //initalize LCD 
  lcd.backlight();

}
void loop() 
{
  lcd.setCursor(0,0);
  lcd.print("Number of people in the room is: ");
  lcd.setCursor(0,8);
  lcd.print(Number_of_people);
  val2 = digitalRead(sensor2);   // read sensor value
  val1 = digitalRead(sensor1);   // read sensor1 value
  Serial.print("Sensor 1: ");
  Serial.println(val2);
  Serial.print("Sensor 2: ");
  Serial.println(val1);
  if (val2 == HIGH) {           // check if the sensor is HIGH
    //digitalWrite(led, HIGH);   // turn LED ON
   // delay 100 milliseconds 
   
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Insert Card!");
  delay(2000);
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if ((content.substring(1) == "53 35 A9 5D" || content.substring(1) == "83 CB 08 98") && val2 == HIGH) //change here the UID of the card/cards that you want to give access
  {
    Number_of_people += 1;
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Welcome!");
    digitalWrite(led, HIGH);
    directionControl();
	  delay(1000);
	  speedControl();
	  delay(1000);
    Serial.println("Welcome!");
    Serial.println();
    delay(3000);
  }
  else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Access denied!");
      Serial.println(" Access denied");
      delay(800);
  }
  if (state2 == LOW) {
      Serial.println("Motion2 detected!"); 
      state2 = HIGH;       // update variable state to HIGH
    }
    else {
        digitalWrite(led, LOW); // turn LED OFF
        delay(200);             // delay 200 milliseconds 
        
        if (state2 == HIGH){
          Serial.println("Motion2 stopped!");
          state2 = LOW;       // update variable state to LOW
      }
      delay(50);
    }
  }
  delay(2000);
  if (val1 == HIGH) {           // check if the sensor1 is HIGH
    if(Number_of_people<=0){
      Number_of_people = 0;
    }
    else
    {
      Number_of_people -= 1;
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("See you again");
      digitalWrite(led, HIGH);
        // turn LED ON
      delay(1000);                // delay 100 milliseconds 
      directionControl();
      delay(1000);
      speedControl();
      delay(100);
    }
    if (state1 == LOW) {
      Serial.println("Motion1 detected!"); 
      state1 = HIGH;       // update variable state1 to HIGH
    }
  } 
  else {
      digitalWrite(led, LOW); // turn LED OFF
      delay(200);             // delay 200 milliseconds 
      
      if (state1 == HIGH){
        Serial.println("Motion1 stopped!");
        state1 = LOW;       // update variable state1 to LOW
    }
    delay(500);
  }



  
 //else   {
   // lcd.clear();
    //lcd.setCursor(3,0);
    //lcd.print("Access denied!");
    //Serial.println(" Access denied");
    //delay(3000);
  //}
} 
void directionControl() {
	// Set motors to maximum speed
	// For PWM maximum possible values are 0 to 255
	
	// Turn on motor A & B
  for (int i = 0;i<maxspeed;i++){
    analogWrite(enA, i);
    digitalWrite(in1, LOW);
	  digitalWrite(in2, HIGH);
    delay(2);
  }
  for (int i = maxspeed;i>=0;i--){
    analogWrite(enA, i);
    digitalWrite(in1, LOW);
	  digitalWrite(in2, HIGH);
    delay(2);
  }
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
  delay(3000);
	// Now change motor directions
  for (int i = 0;i<maxspeed;i++){
    analogWrite(enA, i);
    digitalWrite(in1, HIGH);
	  digitalWrite(in2, LOW);
    delay(2);
  }
	for (int i = maxspeed;i>=0;i--){
    analogWrite(enA, i);
    digitalWrite(in1, HIGH);
	  digitalWrite(in2, LOW);
    delay(2);
  }
	// Turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
}

// This function lets you control speed of the motors
void speedControl() {
	// Turn on motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);


  digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
}