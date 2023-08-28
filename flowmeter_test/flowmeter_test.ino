const int pulsepin1 = 2; //NPN
const int pulsepin2 = 3; //PNP

volatile bool pulsetrig1 = false;
volatile bool pulsetrig2 = false;

void IRAM_ATTR pulse1ISR() {
  pulsetrig1 = true;  // Set the flag indicating the button was pressed
}

void IRAM_ATTR pulse2ISR() {
  pulsetrig2 = true;  // Set the flag indicating the button was pressed
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  Serial.println("Connect pulse pin of flowmeter to pin 2 for NPN, pin 3 for PNP testing");

  pinMode(pulsepin1, INPUT);
  pinMode(pulsepin2, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(pulsepin1), pulse1ISR, FALLING); //NPN
  attachInterrupt(digitalPinToInterrupt(pulsepin2), pulse2ISR, RISING); //PNP
}

void loop() {
  // put your main code here, to run repeatedly:
  if(pulsetrig1 == true)
  { 
    Serial.println("The meter is NPN!");
    pulsetrig1 = false;
    }

  if(pulsetrig2 == true)
  { 
    Serial.println("The meter is PNP!");
    pulsetrig2 = false;
    }

}

