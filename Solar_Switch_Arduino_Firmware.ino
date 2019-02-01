/*
 Firmware v0.1
*/

#define RELAY_PIN 2

#define LED_G_PIN 3
#define LED_R_PIN 5 
#define LED_B_PIN 6

#define SOLAR_PRIORITY 7 //LOW == Switch on (pin pulled high w/ pull-up resistor, pulled low when shorted)
#define MAINS_PRIORITY 8

#define BATT_IN_PIN A0

#define BATT_CALIBRATE 65 //

#define ABS_MIN_VOLTAGE 3.0
#define ABS_MAX_VOLTAGE 16.0
#define NEED_CHARGE_VOLTAGE 13.1
#define FULL_VOLTAGE 14.0

bool source_current; //1 - Solar | 0 - Mains
bool source_prev;  //needed?

bool charge_flag = 0;
int battery_voltage;


void setup() {
  // setup io
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  
  pinMode(BATT_IN_PIN, INPUT);
  pinMode(SOLAR_PRIORITY, INPUT);
  pinMode(MAINS_PRIORITY, INPUT);

  //start serial monitor for debug
  Serial.begin(9600);
}

//***MAIN LOOP***//
void loop() {
  //check if there is a source priority before main program
  while (digitalRead(SOLAR_PRIORITY) == 0){solar_priority();}
  while (digitalRead(MAINS_PRIORITY) == 0){mains_priority();}

  //no priority - normal operation
  normal_operation();
}

void solar_priority(){
  Serial.println("Solar Priority - Relay OFF");
  delay(250);
  
  digitalWrite(RELAY_PIN, LOW);

  digitalWrite(LED_R_PIN, LOW);
  digitalWrite(LED_G_PIN, HIGH);
  digitalWrite(LED_B_PIN, LOW);

  //allows low battery voltage check
  battery_voltage = check_voltage();
  
  delay(100);
}

void mains_priority(){
  Serial.println("Mains Priority - Relay ON");
  delay(250);
  
  digitalWrite(RELAY_PIN, HIGH);

  digitalWrite(LED_R_PIN, LOW);
  digitalWrite(LED_G_PIN, LOW);
  digitalWrite(LED_B_PIN, HIGH);
  
  delay(100);
}

void normal_operation(){
  battery_voltage = check_voltage();

  //battery less than NEEDS_CHARGE_VOLTAGE, so switch to mains and let battery charge
  if(battery_voltage <= (NEED_CHARGE_VOLTAGE * 100)){
    Serial.println("Battery is charging - Mains supply");
    delay(250);
    
    charge_flag = 1;
    digitalWrite(RELAY_PIN, HIGH);

    digitalWrite(LED_R_PIN, LOW);
    digitalWrite(LED_G_PIN, LOW);
    digitalWrite(LED_B_PIN, HIGH);
    
    delay(100);
  }
  
  else if(battery_voltage > (NEED_CHARGE_VOLTAGE * 100)){
    
    if(charge_flag == 0){
      //battery normal range and charged - solar power
      Serial.println("Battery Normal - Solar power");
      delay(250);
      
      digitalWrite(RELAY_PIN, LOW);

      digitalWrite(LED_R_PIN, LOW);
      digitalWrite(LED_G_PIN, HIGH);
      digitalWrite(LED_B_PIN, LOW);
      
      delay(100);
    }

    //above empty battery, but needs to bring up to float voltage
    else if(charge_flag == 1){
      if(battery_voltage < (FULL_VOLTAGE * 100)){
        Serial.print("Above ");
        Serial.print(NEED_CHARGE_VOLTAGE);
        Serial.print(" but still charging to ");
        Serial.print(FULL_VOLTAGE);

        delay(250);
      }
      //battery fully charged
      else if(battery_voltage >= (FULL_VOLTAGE * 100)){

        Serial.println("Done charging, switching to solar");
        
        charge_flag = 0;

        //done charging, switch to solar
        digitalWrite(RELAY_PIN, LOW);

        digitalWrite(LED_R_PIN, LOW);
        digitalWrite(LED_G_PIN, HIGH);
        digitalWrite(LED_B_PIN, LOW);
      }
    }

      //battery just finished charging - solar power
    else if( charge_flag == 1 && battery_voltage >= (FULL_VOLTAGE * 100)){
      Serial.println("Battery Charged - Solar power");
      delay(250);
      charge_flag = 0;
      digitalWrite(RELAY_PIN, LOW);

      digitalWrite(LED_R_PIN, LOW);
      digitalWrite(LED_G_PIN, HIGH);
      digitalWrite(LED_B_PIN, LOW);
      
      delay(100);
    }
    
    //if charge flag previously on, battery needs more time to charge (float voltage)
    else{ //(charge_flag == 1 && battery_voltage < (FULL_VOLTAGE * 100)){
      //do nothing, battery still needs charging
      delay(100);
    }
  }
}

float check_voltage(){
  float battery_in = analogRead(BATT_IN_PIN);
  Serial.print("Raw Analog In: ");
  Serial.println(battery_in);
  
  float battery_voltage = (battery_in / BATT_CALIBRATE)*100; //avoiding float math

  Serial.print("Integer voltage: ");
  Serial.println(battery_voltage);

  Serial.print("Battery Voltage: ");
  Serial.println(battery_voltage / 100);
  //Serial.print(".");
  //Serial.println(battery_voltage % 100);
  delay(250);
  //error state
  if(battery_voltage <= (ABS_MIN_VOLTAGE * 100) || battery_voltage >= (ABS_MAX_VOLTAGE * 100)){
    
    while(1){ //force reset
    
      digitalWrite(LED_G_PIN, LOW);      
      digitalWrite(LED_R_PIN, LOW);
      digitalWrite(LED_B_PIN, HIGH);
      delay(250);
      digitalWrite(LED_R_PIN, HIGH);
      digitalWrite(LED_B_PIN, LOW);
      delay(250);
    }
  }
  return battery_voltage;
}
