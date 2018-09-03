
#define SERIES_RESISTOR 10000
#define NOMINAL_RESISTANCE 100000
#define NOMINAL_TEMPERATURE 25
#define BCOEFFICIENT 3950
#define THERMISTORPIN A1
#define LED_READY 2
#define LED_WARMING 1
#define START_BUTTON 10
#define HBP_PIN 6
#define BACKLIGHT_PIN 3
#define TEMP_UP 7
#define TEMP_DOWN 11

#include <LiquidCrystal_I2C.h>
#include <Wire.h> //Built-in Arduino library needed for using the I2C communication protocol

LiquidCrystal_I2C  lcd(0x3F, 2, 1, 0, 4, 5, 6, 7);

boolean startButtonPushed = false;
int startButtonLastState = LOW;
long startButtonDebounceTime = 0;

boolean upButtonPushed = false;
int upButtonLastState = LOW;
long upButtonDebounceTime = 0;

boolean downButtonPushed = false;
int downButtonLastState = LOW;
long downButtonDebounceTime = 0;

long debounceDelay = 250;

int state = 0;
float thermistor_reading;
int iteration = 0;
float temperature = 0;
float temp_set = 70;
int flag = 1;

void setup() {
  Serial.begin(9600);
  pinMode(LED_WARMING, OUTPUT);
  pinMode(HBP_PIN,OUTPUT);
  pinMode(LED_READY, OUTPUT);
  pinMode(TEMP_UP, INPUT);
  pinMode(TEMP_DOWN,INPUT);

  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);

  lcd.begin(20, 4); //Initializes the lcd with 4 rows and 20 columns
  lcd.setCursor(0, 0);
  lcd.print("   HBP Controller ");
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("    Press Button   ");
  lcd.setCursor(0, 1);
  lcd.print("      To Begin    ");
}


//Status Leds
//HBP SSR Turn On
//Output Voltage Read


void loop() {

  if (iteration == 0) {
    readButtons();
  }
  
  if (startButtonPushed)
  {
    read_thermistor();
    calculateTemp();
    status_LED();
   // readTempButton();
    HBP_Control();
    display_data();
    delay(250);
  }
}

void display_data()
{
  lcd.setCursor(0, 0);
  lcd.print("TEMP_SET: ");
  lcd.print(temp_set);
  lcd.write(0xDF);
  lcd.print("C     ");
  
  lcd.setCursor(0, 1);
  lcd.print("TEMP.:");
  lcd.print(temperature);
  lcd.write(0xDF);
  lcd.print("C      ");
}

void calculateTemp()
{
  temperature = (thermistor_reading) / NOMINAL_RESISTANCE; // (R/Ro)
  temperature = log(temperature); // ln(R/Ro)
  temperature /= BCOEFFICIENT; // 1/B * ln(R/Ro)
  temperature += 1.0 / (25 + 273.15); // + (1/To)
  temperature = 1.0 / temperature; // Invert
  temperature -= 276.15; // convert to C

}
void status_LED()
{
  if (temperature >= 0 && temperature <= 68) {
    digitalWrite(LED_WARMING, HIGH);
    digitalWrite(LED_READY, LOW);
    state = 1;
  }

  else if (temperature > 68) {
    digitalWrite(LED_WARMING, LOW);
    digitalWrite(LED_READY, HIGH);
    state = 2;
  }
  else{
    state = 0;
  }
}

void HBP_Control()
{
  readTempButton();

  if(temperature < temp_set and flag == 1)
  {
    digitalWrite(HBP_PIN, HIGH);
    if(temperature >= temp_set - 1)
    {
      flag = 0;
    }
  }
  else if ((temperature >= temp_set - 1 and temperature <= temp_set + 1 ) and flag == 0) {
    digitalWrite(HBP_PIN, HIGH);
  }
  else if ((temperature >= temp_set + 1.5) and flag == 0) {
    digitalWrite(HBP_PIN, LOW);
  }
}

void readTempButton()
{
  int reading_up;
  int reading_down;
  int upButtonState = LOW;
  int downButtonState = LOW;
  
  reading_up = digitalRead(TEMP_UP);
  reading_down = digitalRead(TEMP_DOWN);

  Serial.print("Reading UP: ");
  Serial.println(reading_up);
  Serial.print("Reading Down: ");
  Serial.println(reading_down);
  if (reading_up != upButtonLastState)
  {
    upButtonDebounceTime = millis();
  }

  if ((millis() - upButtonDebounceTime) > debounceDelay)
  {
    upButtonState = reading_up;
    upButtonDebounceTime = millis();
  }

  upButtonLastState = reading_up;

  if (reading_down != downButtonLastState)
  {
    downButtonDebounceTime = millis();
  }

  if ((millis() - downButtonDebounceTime) > debounceDelay)
  {
    downButtonState = reading_down;
    downButtonDebounceTime = millis();
  }

  downButtonLastState = reading_down;

  //Reporting which button was pushed
  delay(200);

  if (upButtonState == HIGH) {
    upButtonPushed = true;
    temp_set += 1;
  }
  else if(downButtonState == HIGH){
    downButtonPushed = true;
    temp_set -= 1;
  }
  else 
  {
    upButtonPushed = false;
    downButtonPushed = false;
  }
}

void read_thermistor()
{
  int LED = 0;
  Serial.println(LED);
  thermistor_reading = analogRead(THERMISTORPIN);
  // convert the value to resistance
  thermistor_reading = (1023 / thermistor_reading)  - 1;
  thermistor_reading = SERIES_RESISTOR / thermistor_reading;

}
void readButtons() {

  int reading;
  int startButtonState = LOW;

  //Check if start button pushed
  reading = digitalRead(START_BUTTON);
  //Serial.println(reading);
  if (reading != startButtonLastState)
  {
    startButtonDebounceTime = millis();
  }

  if ((millis() - startButtonDebounceTime) > debounceDelay)
  {
    startButtonState = reading;
    startButtonDebounceTime = millis();
  }

  startButtonLastState = reading;

  //Reporting which button was pushed
  delay(200);

  if (startButtonState == HIGH) {
    startButtonPushed = true;
    iteration = 1;
  } else {
    startButtonPushed = false;
  }
}

