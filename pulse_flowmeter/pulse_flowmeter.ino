const int pulsepin1 = 2; //pin to connect the pulse pin of flowmeter

unsigned long pulsecount = 0;
int pulseNew = 0;
int pulseOld = 0;

float flow_inst_volume = 0.0; //in cubic m
float flowrate = 0.0; //in litres per min
float total_volume = 0.0; //in cubic m
float ppl = 41.2; // pulses per litre

float flow_calc_freq = 1.0; //Freq of calculating Flowrate and Volume in a second
/*
 * 4.0--> 4 times in a second
 * 2.0--> 2 times a second
 * 1.0--> 1 time every second
 * 0.5-->1 times in every 2 seconds
 * 0.25-->1 time in every 4 seconds
 */
hw_timer_t *timer = NULL;
bool flow_calc_start = 0;

void IRAM_ATTR pulse1ISR() {
  pulsecount++;  // Set the flag indicating the pulse is received
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  Serial.println("PULSE FLOWMETER CALCULATIONS:");

  pinMode(pulsepin1, INPUT);
  attachInterrupt(digitalPinToInterrupt(pulsepin1), pulse1ISR, FALLING); //NPN

  flow_calc_setup(flow_calc_freq);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(flow_calc_start == 1)
  { 
    flowmeter_calculations();
    flow_calc_start = 0;
    flow_calc_setup(flow_calc_freq);
   }

   //flow_inst_volume (in cubic m),flowrate(litres per min) and total_volume(cubic m) can be used for other purposes
  
}

void IRAM_ATTR onTimer(){
  flow_calc_start = 1;
}

void flow_calc_setup(float freq)
{
  uint64_t alarm_value = (uint64_t)(1000000.0/freq); //in micro secs
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, alarm_value, true);
  timerAlarmEnable(timer); //Just Enable
  }

void flowmeter_calculations()
{
  pulseNew = pulsecount;
  int no_of_pulses = pulseNew - pulseOld;
  float interval = 1.0/flow_calc_freq;

  float inst_volume = ((float)no_of_pulses * interval)/ppl; //in litres

  flowrate = inst_volume * 60.0; //in litres per min

  flow_inst_volume = (inst_volume * 0.001); //inst volume in cubic m 
  total_volume = total_volume + flow_inst_volume; //total volume in cubic m

  Serial.print("Current Pulsecount = ");
  Serial.println(pulseNew);
  Serial.print("No of pulses = ");
  Serial.println(no_of_pulses);
  Serial.print("Interval in seconds = ");
  Serial.println(interval);
  Serial.print("Inst volume in litres = ");
  Serial.println(inst_volume);
  

  Serial.print("Instant Volume = ");
  Serial.print(flow_inst_volume);
  Serial.println(" cubic m");

  Serial.print("Flowrate = ");
  Serial.print(flowrate);
  Serial.println(" litres per min");

  Serial.print("Total Volume = ");
  Serial.print(total_volume);
  Serial.println(" cubic m");
  
  Serial.println("_____________________________");
  
  pulseOld = pulseNew;
  }
