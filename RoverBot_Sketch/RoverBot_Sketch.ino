//Standard PWM DC control
int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;    //M1 Direction Control
int M2 = 7;    //M1 Direction Control

///For previous Romeo, please use these pins.
//int E1 = 6;     //M1 Speed Control
//int E2 = 9;     //M2 Speed Control
//int M1 = 7;    //M1 Direction Control
//int M2 = 8;    //M1 Direction Control

//Motor Speeds
char fwbw = 100;
char RL = 75;

//Counter
int m = 0;

//Button Control Parameters
int  adc_key_val[5] ={
  30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;

void blink(int n)
{
  for(int j = 0; j < n; j++)
  {
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(200);              // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second
  }
}

// Convert ADC value to key number
int get_key(unsigned int input)
{   
  int k;
  for (k = 0; k < NUM_KEYS; k++)
  {
    if (input < adc_key_val[k])
    {  
      return k;  
    }
  }
  if (k >= NUM_KEYS)
    k = -1;     // No valid key pressed
  return k;
}

void stop(void)                    //Stop
{
  digitalWrite(E1,LOW);   
  digitalWrite(E2,LOW);      
}   
void advance(char a,char b)          //Move forward
{
  analogWrite (E1,a);      //PWM Speed Control
  digitalWrite(M1,HIGH);    
  analogWrite (E2,b);    
  digitalWrite(M2,HIGH);
}  
void back_off (char a,char b)          //Move backward
{
  analogWrite (E1,a);
  digitalWrite(M1,LOW);   
  analogWrite (E2,b);    
  digitalWrite(M2,LOW);
}
void turn_L (char a,char b)             //Turn Left
{
  analogWrite (E1,a);
  digitalWrite(M1,LOW);    
  analogWrite (E2,b);    
  digitalWrite(M2,HIGH);
}
void turn_R (char a,char b)             //Turn Right
{
  analogWrite (E1,a);
  digitalWrite(M1,HIGH);    
  analogWrite (E2,b);    
  digitalWrite(M2,LOW);
}
void setup(void) 
{ 
  int i;
  for(i=4;i<=7;i++)
  {
    pinMode(i, OUTPUT);  
  }
  pinMode(13, OUTPUT);
  Serial.begin(9600);      //Set Baud Rate for Debug
} 
void loop(void) 
{
  adc_key_in = analogRead(0);    // read the value from the sensor  
  digitalWrite(13, HIGH);
  /* get the key */
  key = get_key(adc_key_in);    // convert into key press
  if (key != oldkey)
  {   // if keypress is detected
    delay(50);      // wait for debounce time
    adc_key_in = analogRead(0);    // read the value from the sensor  
    key = get_key(adc_key_in);    // convert into key press
    if (key != oldkey)
    {         
      oldkey = key;
      switch(key)
      {
        case 0: //Move Forward
          Serial.println("Fwd");
          advance (fwbw, fwbw);
          delay(1000);
          stop();
          break;
        case 1: //Turn Left
          Serial.println("L"); 
          turn_L(RL,RL);
          delay(250);
          stop();
          break;
        case 2: //Move Backward
          Serial.println("Bwd");
          back_off(fwbw,fwbw);
          delay(1000);
          stop();
          break;
        case 3: //Turn Right
          Serial.println("R");
          turn_R(RL,RL);
          delay(250);
          stop();
          break;
        case 4: //Preprogrammed Pathway
          Serial.println("PP");
          blink(3);
          delay(1000);
          advance (fwbw, fwbw);
          delay(1000);
          turn_L(RL,RL);
          delay(250);
          advance(fwbw,fwbw);
          delay(2000);
          turn_R(RL,RL);
          delay(250);
          back_off(fwbw,fwbw);
          delay(2000);
          stop();
          break;  
        default:
          stop();
          break;
      }
    }
  }
}
