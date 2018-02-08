#define LED_RED_ONE 3
#define LED_RED_TWO 5
#define LED_RED_THREE 6
#define LED_GREEN_ONE 9
#define LED_GREEN_TWO 10
#define LED_GREEN_THREE 11
#define POTENTIOMETER 0
#define BUTTON 2

/*
 * flags[0]: first LED group
 * flags[1]: second LED group
 * flags[2]: third LED group
 * flags[3]: all LED ON or OFF
 * flags[4]: taster clicked
 * flags[5]: formula has been added
 */
boolean flags[6] = {false, false, false, false, false, false};

float a = 0, b = 0, c = 0;
int group[3] = {0,0,0};

void setup() {
  pinMode(LED_RED_ONE, OUTPUT);
  pinMode(LED_RED_TWO, OUTPUT);
  pinMode(LED_RED_THREE, OUTPUT);
  pinMode(LED_GREEN_ONE, OUTPUT);
  pinMode(LED_GREEN_TWO, OUTPUT);
  pinMode(LED_GREEN_THREE, OUTPUT);

  pinMode(TASTER, INPUT);
  
  Serial.begin(9600);
}

int formulaVal = 0;

void loop() {
  int pVal = analogRead(POTENTIOMETER);
  int delayVal = pVal;
  
  if(flags[0] && flags[1] && flags[2] && flags[3] || flags[0] && flags[1] && !flags[2] && flags[3] || flags[0] && !flags[1] && !flags[2] && flags[3]) {
    if(flags[2]) {
      analogWrite(LED_RED_THREE, --group[2]);
      analogWrite(LED_GREEN_THREE, group[2]);
    } else if(flags[1]) {
      analogWrite(LED_RED_TWO, --group[1]);
      analogWrite(LED_GREEN_TWO, group[1]);
    } else if(flags[0]) {
      analogWrite(LED_RED_ONE, --group[0]);
      analogWrite(LED_GREEN_ONE, group[0]);
    }
    if(group[2] == 0  && flags[2]) {
      flags[2] = false;
    } else if(group[1] == 0 && flags[1]) {
      flags[1] = false;
    } else if(group[0] == 0 && flags[0]) {
      flags[0] = false;
    }

    if(!flags[0] && !flags[1] && !flags[2]){
      flags[3] = false;
      Serial.println("##########################");
      Serial.println("On phase finished");   
      Serial.println("##########################");
      delay(1000);
    }
      
  } else {
    if(!flags[0]) {
      analogWrite(LED_RED_ONE, ++group[0]);
      analogWrite(LED_GREEN_ONE, group[0]);
    } else if(!flags[1]) {
      analogWrite(LED_RED_TWO, ++group[1]);
      analogWrite(LED_GREEN_TWO, group[1]);
    } else if(!flags[2]) {
      analogWrite(LED_RED_THREE, ++group[2]);
      analogWrite(LED_GREEN_THREE, group[2]);
    }
    if(group[0] == 100 && !flags[0]) {
      flags[0] = true;
    }else if(group[1] == 100 && !flags[1]) {
      flags[1] = true;
    }else if(group[2] == 100 && !flags[2]) {
      flags[2] = true;
    }
    if(flags[0] && flags[1] && flags[2]){
      flags[3] = true;
      Serial.println("##########################");
      Serial.println("Off phase finished");
      Serial.println("##########################");
      delay(1000);
    }
  }
  if(Serial.available()){
    a = Serial.parseFloat();
    Serial.read();
    b = Serial.parseFloat();
    for(int i = 0; i < 3; i++)
      Serial.read();
    c = Serial.parseFloat();
    flags[5] = true;
  }
  int valTaster = digitalRead(TASTER);

  if(flags[4] == false && valTaster == HIGH)
    flags[4] = true;
  else if(flags[4] == true && valTaster == LOW){
    Serial.println("==========================");
    Serial.print("First group: ");
    Serial.println(group[0]);
    Serial.print("Second group: ");
    Serial.println(group[1]);
    Serial.print("Third group: ");
    Serial.println(group[2]);
    Serial.print("Potentiometer value: ");
    Serial.println(pVal);
    Serial.println("==========================");
    flags[4] = false;
  }
  
  if(flags[5])
  {
    formulaVal = a + b * pVal / c;

    Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~");
    Serial.print("Result of formula: ");
    Serial.println(formulaVal);
    Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~");
    delay(formulaVal);
  }
  else
    delay(delayVal);
}
