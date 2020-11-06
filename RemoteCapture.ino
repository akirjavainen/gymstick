 /*
******************************************************************************************************************************************************************
*
* Gymstick protocol remote control capture
* Compatible with Gymstick WalkingPad remotes
* 
* Code by Antti Kirjavainen (antti.kirjavainen [_at_] gmail.com)
* 
* Use this code to capture the commands from your remotes. Outputs to serial
* (Tools -> Serial Monitor). Captures the 25 bit command sequence.
* 
* 
* HOW TO USE
* 
* Plug a 433.92MHz receiver to digital pin 2 and start pressing buttons
* from your original remotes (copy pasting them to Gymstick.ino).
*
******************************************************************************************************************************************************************
*/



// Plug your 433.92MHz receiver to digital pin 2:
#define RECEIVE_PIN   2

// Enable debug mode if there's no serial output or if you're modifying this code for
// another protocol/device. However, note that serial output delays receiving, causing
// data bit capture to fail. So keep debug disabled unless absolutely required:
#define DEBUG         false
#define ADDITIONAL    false    // Display some additional info after capture



// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup()
{
  pinMode(RECEIVE_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("Starting up...");
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop()
{
  int i = 0;
  unsigned long t = 0;
  String command = "";



  if (DEBUG) {
    Serial.print("Radio silence: ");
    Serial.println(t);
    //return; // If modifying this code for another protocol, stop here
  }


  
  // *********************************************************************
  // Command bits, locate them simply by HIGH waveform spikes:
  // *********************************************************************
  // 0 = 250-500 us
  // 1 = 800-1200 us
  // *********************************************************************

  while (i < 25) {
    t = pulseIn(RECEIVE_PIN, HIGH, 1000000); // Waits for HIGH and times it
  
    if (DEBUG) {
      Serial.print(t);
      Serial.print(": ");
    }
  
    if (t > 250 && t < 500) { // Found 0
      command += "0";
      if (DEBUG) Serial.println("0");
        
    } else if (t > 800 && t < 1200) { // Found 1
      command += "1";
      if (DEBUG) Serial.println("1");
        
    } else { // Unrecognized bit, finish
      if (ADDITIONAL) {
        Serial.print("INVALID TIMING: ");
        Serial.println(t);
      }
      
      i = 0;
      break;
    }
  
    i++;
  }


 
  // *********************************************************************
  // Done! Display results:
  // *********************************************************************  

  // Correct data bits length is 25 bits, dismiss bad captures:
  if (command.length() != 25) {
  
    if (ADDITIONAL) {
      Serial.print("Bad capture, invalid command length ");
      Serial.println(command.length());
      Serial.println();
    }
    
  } else {
      Serial.println("Command is: " + command);

      // We'll pause here to avoid multiple command repeats:
      Serial.println("Pausing for 1 second...");
      delay(1000);
      Serial.println("OK, receiving again!");
      Serial.println();
  }
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
