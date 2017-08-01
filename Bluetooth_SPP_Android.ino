/*
  Written By: Jeff Kim,
  Date: 29th July 2017,
  As Part of Operation Fadell,
  And the Bloody Code is Awesome!
*/

const int redPin = 5; //Red LED    //test repo
const int greenPin = 9; //Green LED 
const int bluePin = 10; //Blue LED
const int power_ctrl = 6; //Power Control For Dimming Functions


#define fadeSpeed 2353  //Resolution for Fading, should translate to 1 hour per cycle
static int state = 0; //For different colour states
static int intensity = 255; //To track increase or decrease in light intensity
int flag_up = 0; //Volume down button pressed
int flag_down = 0; //Volume up button pressed
int flag_power = 0; //Track whether Power is ON or OFF
int flag_sequencer = 0; //Track sequencing between colours
int flag_cycling = 0; 
int exit_cycle = 0;

char command;
String string;
boolean ledon = false;
#define led 11 //Debug Led

  void setup()
  {
  Serial.begin(9600);
  // Initialize Lamps as Outputs:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(power_ctrl, OUTPUT);
  pinMode(led, OUTPUT);
  
  // Pin High/Low configuration
  digitalWrite(redPin, HIGH); //Turn OFF.(PNP Transistor)
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, HIGH);
  digitalWrite(power_ctrl, LOW); //Turn OFF. (NPN Transistor)
  digitalWrite(led, LOW); //Debug Led
    
  }

  void loop()
  {
    delay(10);
    
    if (Serial.available() > 0) 
    {
      string = "";
    }
    
    while(Serial.available() > 0)
    {
      command = ((byte)Serial.read());
      
      if(command == ':')
      {
        break;
      }
      
      else
      {
        string += command;
      }
      
      delay(1);
    }
    
    if(flag_power == 0)
    {
      if(string == "0x4FB4AB5")
      {
        Power();
      }
    }
    
    else if(string == "StairsOn")
    {
      ledOn();
    }
    
    else if(string == "StairsOff")
    {
      ledOff();
    }
    
    else if(flag_power == 1)
    {
      Colour_Options();
    }
    
    else{}
    
  
   /* 
    if ((string.toInt() >=0 ) && (string.toInt()<= 255)) //If Analog Command Received.
    {
      if (ledon == true) //If Lamp was 100% On
      {
        analogWrite(led, string.toInt()); //Dim
        Serial.println(string);
        delay(10);
      }
    }
    */
 }
 
void ledOn()
   {
      digitalWrite(led, HIGH); //DigitalWrite(led, ON)
      delay(10);
   }
 
 void ledOff()
 {
      digitalWrite(led, LOW);
      delay(10);
 }
 
 
//Different Colour Modes
void Colour_Options()
 {  
  if(string == "0x4FB00FF")
  {
    Red();
  }
  
  else if(string  == "0x4FB08F7")
  {
    Green();
  }
  
   
  else if(string  == "0x4FB8877")
  {
    Blue();
  } 
  
  else if(string  == "0x4FBC837")
  {
    Yellow();
  } 
   
  else if(string  == "0x4FB28D7")
  {
    Purple();
  } 
  
  else if(string  == "0x4FBA857")
  {
    Aqua();
  } 
  
  else if(string  == "0x4FBE817")
  {
    Gold();
  } 
  
  else if(string  == "0x4FB18E7")
  {
    Gray();
  } 
          
  else if(string  == "0x4FB9867")
  {
    Crimson();
  } 
   
  else if(string  == "0x4FBD827")
  {
    Orange();
  }       
    
  else if(string  == "0x4FBCA35") //Mute Button
  {
    On();
  }       
  
  /* 
  else if(string  == "0x4FB52AD") //Ok Button
  {
     //For Debug
      off(); 
      delay(20);
      Power();
      intensity = 255; //Set Light intensity Variable to Max, so that Flickering 
                       // doesn't occur when I come out of it. 
     
      while(exit_cycle == 0) //While no other Button Pressed
      {
       Cycling();
      }
      exit_cycle = 0;
      break;    
  }      
  */     
   
     
  else if(string  == "0x4FB40BF") //Vol up to increase brightness
  {
    flag_up = 1;
    light_intensity();
  }              
     
  else if(string  == "0x4FB906F") //Vol Down to decrease brightness
  {
    flag_down = 1;
    light_intensity();
  }    
   
  //Prog up to change Colours    
  else if(string  == "0x4FBC03F") 
  {
    flag_sequencer = 0;
    state ++; //Increment state counter 
    
    if(state >= 14)
     {
      state = 1;
     }   
    
     Sequencer();
   }      
   
  //Prog down to change Colours  
  else if(string  == "0x4FB827D")
  {
    flag_sequencer = 0;    
    state--; //Decrement state counter                
    Sequencer();   
    if(state <= 1 )
    {
      state = 14;
    }  
  }   
      
  else if(string  == "0x4FB4AB5")
  {
    off();
  }                   
 } 

/*
void Cycling()
{    
  int r, g, b;
  
  // fade from blue to violet
  for (r = 0; r < 256; r++)
  {
   if(exit_cycle == 0)
   {    
    analogWrite(redPin, r);
    delay(fadeSpeed);
   
    if(irrecv.decode(&results))
    { 
      if(results.value == 0xFFFFFFFF)   // if repeat command (button held down)
      {
       results.value = last_cycle_Code;      // replace FFFFFFFF with last good code
      }
      
      if(results.value != 0x4FB52AD) //Button press not OK
      {
        exit_cycle = 1;
        Colour_Options();
      }
     
      irrecv.resume(); //receive the next value       
    }      
    Serial.println(results.value, HEX);
   }
  } 
  
  // fade from violet to red
  for (b = 255; b > 0; b--) 
  { 
   if(exit_cycle == 0)
   {    
    analogWrite(bluePin, b);
    delay(fadeSpeed);
        
    if(irrecv.decode(&results))
    {
   
      if(results.value == 0xFFFFFFFF)   // if repeat command (button held down)
      {
       results.value = last_cycle_Code;      // replace FFFFFFFF with last good code
      }
      
      if(results.value != 0x4FB52AD) //Button press not OK
      {
        exit_cycle = 1;
        Colour_Options();
      }
      irrecv.resume(); //receive the next value        
    }        
    Serial.println(results.value, HEX); 
   }
  }
          
  // fade from red to yellow
  for (g = 0; g < 256; g++) 
  {  
   if(exit_cycle == 0)
   {
    analogWrite(greenPin, g);
    delay(fadeSpeed);
    
    if(irrecv.decode(&results))
    { 
      if(results.value == 0xFFFFFFFF)   // if repeat command (button held down)
      {
       results.value = last_cycle_Code;      // replace FFFFFFFF with last good code
      }
      
      if(results.value != 0x4FB52AD) //Button press not OK
      {
        exit_cycle = 1;
        Colour_Options();
      }
      irrecv.resume(); //receive the next value       
    }
    
    Serial.println(results.value, HEX); 
   }
  }  
     
  // fade from yellow to green
  for (r = 255; r > 0; r--) 
  {  
    if(exit_cycle == 0)
    {
     analogWrite(redPin, r);
     delay(fadeSpeed);
     
     if(irrecv.decode(&results))
     {
      if(results.value == 0xFFFFFFFF)   // if repeat command (button held down)
      {
       results.value = last_cycle_Code;      // replace FFFFFFFF with last good code
      }
       
       if(results.value != 0x4FB52AD) //Button press not OK
      {
        exit_cycle = 1;
        Colour_Options();
      }
       irrecv.resume(); //receive the next value       
     }
     
     Serial.println(results.value, HEX); 
    }
  } 

  // fade from green to teal
  for (b = 0; b < 256; b++) 
  {  
    if(exit_cycle == 0)
    {
     analogWrite(bluePin, b);
     delay(fadeSpeed);
     
     if(irrecv.decode(&results))
     {     
      if(results.value == 0xFFFFFFFF)   // if repeat command (button held down)
      {
       results.value = last_cycle_Code;      // replace FFFFFFFF with last good code
      }
      
       if(results.value != 0x4FB52AD) //Button press not OK
      {
        exit_cycle = 1;
        Colour_Options();
      }
       irrecv.resume(); //receive the next value       
     }
     
     Serial.println(results.value, HEX); 
    }
  }  
   
  // fade from teal to blue
  for (g = 255; g > 0; g--) 
  {  
   if(exit_cycle == 0)
   { 
    analogWrite(greenPin, g);
    delay(fadeSpeed);
    
    if(irrecv.decode(&results))
    {
      if(results.value == 0xFFFFFFFF)   // if repeat command (button held down)
      {
       results.value = last_cycle_Code;      // replace FFFFFFFF with last good code
      }
      
      if(results.value != 0x4FB52AD) //Button press not OK
      {
        exit_cycle = 1;
        Colour_Options();
      }
      irrecv.resume(); //receive the next value       
    }
     Serial.println(results.value, HEX); 
   }
  }  
}
*/



//A function to take colours as Arguments
void setColour(int red, int green, int blue) //For digital colours
{
  //Performing an Inverse coz of PNP Transistor
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
  
  digitalWrite(redPin, red); //Pin No, followed by intensity 
  digitalWrite(greenPin, green);
  digitalWrite(bluePin, blue);  

}

void Colour(int red, int green, int blue) //For analog colours
{
  analogWrite(redPin, red); //Pin No, followed by intensity 
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}


void On() //Lights Fully On without fading in or out
{
  digitalWrite(power_ctrl, HIGH); //Turn ON Main switch
  delay(1);
  setColour(255, 255, 255);
}


void Red()
{
  digitalWrite(power_ctrl, HIGH); //Turn ON Main switch
  delay(1);
  setColour(255, 0, 0);
}

void Pink_1()
{
  digitalWrite(power_ctrl, HIGH); //Turn ON Main switch
  delay(1);
  Colour(0, 255, 128);
}

void Pink_2()
{
  digitalWrite(power_ctrl, HIGH); //Turn ON Main switch
  delay(1);
  Colour(0, 255, 51);
}

void Green()
{
  digitalWrite(power_ctrl, HIGH); //Turn ON Main switch
  delay(1);
  setColour(0, 255, 0);
}


void Blue()
{
  digitalWrite(power_ctrl, HIGH); //Turn ON Main switch
  delay(1);
  setColour(0, 0, 255);
}


void Yellow()
{
  digitalWrite(power_ctrl, HIGH); //Turn ON Main switch
  delay(1);
  setColour(255, 255, 0);
}

void Purple()
{
  digitalWrite(power_ctrl, HIGH); //Turn ON Main switch
  delay(1);
  Colour(100, 255, 205);
}

void Aqua()
{
  digitalWrite(power_ctrl, HIGH); //Turn ON Main switch
  delay(1);
  setColour(0, 255, 255);
}

void Gold()
{
  digitalWrite(power_ctrl, HIGH); //Turn ON Main switch
  delay(1);
  Colour(255, 215, 0);
}

void Gray()
{
  digitalWrite(power_ctrl, HIGH); //Turn ON Main switch
  delay(1);
  Colour(128, 128, 128);
}

void Maroon()
{
  digitalWrite(power_ctrl, HIGH); //Turn ON Main switch 
  delay(1);
  Colour(128, 0, 0);
}

void Orange()
{
  digitalWrite(power_ctrl, HIGH); //Turn ON Main switch 
  delay(1);
  Colour(200, 50, 200);
}


void Crimson()
{
  digitalWrite(power_ctrl, HIGH); //Turn ON Main switch
  delay(1);
  Colour(20, 150, 255);
}


//Creating a Colour sequence state 
void Sequencer()
{
  switch(state){
    
  case 1:
  Red();
  break;
  
  case 2:
  Green();
  break;
   
  case 3:
  Blue();
  break;
  
  case 4:
  Yellow();
  break;
  
  case 5:
  Pink_1();
  break;
  
  case 6:
  Pink_2();
  break;
  
  case 7:
  Purple();
  break;
     
  case 8:
  Aqua();
  break;
  
  case 9:
  Gold();
  break;
   
  case 10:
  Gray();
  break;
     
  case 11:
  Maroon();
  break;  
    
  case 12:
  Orange();
  break;
  
  case 13:
  Crimson();
  break;  
  
  case 14:
  On();
  break; 
  
  default:
  break;
  }
}

void light_intensity()
{
  if(flag_up == 1)
  {
     flag_up = 0;
        
     intensity += 10;
     
     if(intensity >= 255)
     {
       intensity = 255;
     }  
     analogWrite(power_ctrl, intensity);
  }
  
  if(flag_down == 1)
  {
     flag_down = 0;
      
     intensity -= 10;
     
     if(intensity <= 0)
     {
       intensity = 0;
     }  
     
     analogWrite(power_ctrl, intensity);          
  }
    
  else{}  
}


//Creating Colour Effects
void off()
{
  if(intensity == 255) //If Lamp at full brightness,
  {
    for(int i = 255; i >= 0; i--)
    {
     analogWrite(power_ctrl, i); //Turn OFF Main switch Slowly
     delay(6);
     if(i <= 0)
     {
       i = 0;
     }
    }
  }
  
  else //If Lamp was dimmed, Turn off From the Dimming Point.
  {
    for(int i = intensity; i >= 0; i--) 
    {
     analogWrite(power_ctrl, i); //Turn OFF Main switch Slowly
     delay(5);
     if(i <= 0)
     {
       i = 0;
     }
    }
  }
  
 
  setColour(0, 0, 0); // LEDs
  
  flag_power = 0; //Reset Power flag
  
  state = state - 1; //So as to go to the same state as before when button is pressed.
}

void Power()
{
  setColour(255, 255, 255);
  intensity = 255; //Set Light intensity variable to max to clear previous sessions settings
  for(int i = 0; i < 255; i++)
  {
     analogWrite(power_ctrl, i); //Turn ON Main switch Slowly
     delay(4);
     if(i >= 255)
     {
       i = 255;
     }
  }
  
  flag_power = 1;
}


    

