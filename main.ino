//Current notes, need to find better alternitive for salinity sensor, currently locks it up for ~1.5 seconds with delay
//Spell Check?
//Finish the damm thing

//Update 2-not the day that i am supposed to have this done-18
//need to do salnity math but finished logic for it

//static vars
const long interval = 1000; // how often we check the salnity.
const int tempthreshold = 25; //5 degrees c or whatever if i'm too lazy to fix it
const int TempUCL = 28; //Upper control limit
const int TempLCL = 23; //Lower control limit
const int SalnTarget = .1; //in percent
const int salnthreshold = .03; //0.3% i think we agreed on
const int SalnUCL = 0.13; //upper control limit
const int SalnLCL = 0.07; //Lower control limit

//changing vars
unsigned long previousMillis = 0;
int temp = 0;
int sal = 0;
int salState = 0; // can be -2,-1,0,1,2. 2 means needs major corrections, 1 means minor, 0 means none
int SalTimeStart = 0;
int SalTimeEnd = 0;
int Valve1OpenTime = 0;
int deadtime = 0;
int idk = 0;

//pins go where
//7,8,9 are relays
//a0 analog input
void setup() {
  // put your setup code here, to run once:
  pinMode(7, OUTPUT); //relay output, volvo 1 di?
  pinMode(8, OUTPUT); //relay output, volvo 2, salty?
  pinMode(9, OUTPUT); //relay output, thermresitor
  delay(100);
  digitalWrite(7, HIGH); //testing relays fun clicks
  delay(50);
  digitalWrite(8, HIGH);
  delay(50);
  digitalWrite(9, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  delay(50);
  digitalWrite(8, LOW);
  delay(50);
  digitalWrite(9, LOW);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) //Checks for temp and sal
  {
    previousMillis = currentMillis;
    CheckSaln();
    if (sal > SalnUCL || sal < SalnLCL)
    {
      if (sal > SalnUCL)
      {
        if (sal > SalnUCL*2)
        {
          salState = 2; //should do calculations for valve open
          Valve1OpenTime = SalTimeCalc();
        }
        else
        {
          salState = 1;
          Valve1OpenTime = SalTimeCalc();
        }
      }
      if (sal < SalnLCL)
      {
        if (sal < SalnLCL*2)
        {
          salState = -2;
          Valve1OpenTime = SalTimeCalc();
        }
        else
        {
          salState = -1;
          Valve1OpenTime = SalTimeCalc();
        }
      }
    }
  }
  else
  {
    salState = 0;
  }
  if (salState == 2)          //Beginning of the salt correction block for to much salt
  {
    digitalWrite(7, HIGH); //starts adding di cuz too much salt
    SalTimeStart = currentMillis; //set to when the valve is open
    Valve1OpenTime = SalTimeCalc();//run calc again for stage 1
    deadtime = 1000;//do dead time calc or if you want static idk what you people want
  }
  if (salState == 2 && currentMillis == Valve1OpenTime + SalTimeStart) //check to stop valve open
  {
    digitalWrite(7, LOW); //close valve
    salState = 1; //set state to 1
    SalTimeEnd = currentMillis;
    deadtime = 2000; //2 seconds
  }
  if (salState == 1 && currentMillis == SalTimeEnd + deadtime)
  {
    Valve1OpenTime = SalTimeCalc();//run calc again for stage 1
    digitalWrite(7, HIGH);
    SalTimeStart = currentMillis; //set to when the valve is open
  }
  if (salState == 1 && currentMillis == Valve1OpenTime + SalTimeStart) //stahp
  {
    digitalWrite(7, LOW); //close valve
    salState = 0; //set state to 0
    SalTimeEnd = currentMillis;
    deadtime = 0; //set dead time to 0
  }
  if (salState == -2)          //Beginning of the salt correction block for too little salt
  {
    digitalWrite(8, HIGH); //starts adding di cuz too much salt
    SalTimeStart = currentMillis; //set to when the valve is open
    Valve1OpenTime = SalTimeCalc();//run calc again for stage 1
    deadtime = 1000;//do dead time calc or if you want static idk what you people want
  }
  if (salState == -2 && currentMillis == Valve1OpenTime + SalTimeStart) //check to stop valve open
  {
    digitalWrite(8, LOW); //close valve
    salState = 1; //set state to 1
    SalTimeEnd = currentMillis;
    deadtime = 2000; //2 seconds
  }
  if (salState == -1 && currentMillis == SalTimeEnd + deadtime)
  {
    Valve1OpenTime = SalTimeCalc();//run calc again for stage 1
    digitalWrite(8, HIGH);
    SalTimeStart = currentMillis; //set to when the valve is open
  }
  if (salState == -1 && currentMillis == Valve1OpenTime + SalTimeStart) //stahp
  {
    digitalWrite(8, LOW); //close valve
    salState = 0; //set state to 0
    SalTimeEnd = currentMillis;
    deadtime = 0; //set dead time to 0
  }
  //######Begin Heater Logic#######
  CheckTemp();
  if(temp < TempLCL)
  {
    digitalWrite(9,HIGH);
  }
  if(temp > TempUCL) //Shutoff when heater is above threshold
  {
    digitalWrite(9,LOW);
  }
}

// note have to use float to return a value.
void CheckTemp()
{
  temp = analogRead(A0); //math stuff
  //https://learn.adafruit.com/thermistor/using-a-thermistor
  // values from https://www.digikey.com/product-detail/en/cantherm/MF52A2103J3470/317-1258-ND/1191033
  temp = 1023 / temp - 1;
  temp = 10000 / temp;
  float steinhart;
  steinhart = temp / 10000;
  steinhart = log(steinhart);
  steinhart /= 3470;
  steinhart += 1.0 / (25 + 273.15);
  steinhart = 1.0 / steinhart;
  steinhart -= 273.15;
  return;
}
void CheckSaln()
{
  sal = analogRead(A1); //more math stuff
  return;
}
float SalTimeCalc()
{
  //math
  delay(1);//has to do math to find out how long to open valve
  return idk;
}
void HeatTimeCalc()
{
  delay(1);
  return;
}

