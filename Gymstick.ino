/*
******************************************************************************************************************************************************************
*
* Gymstick WalkingPad 433.92MHz
* PSAA*
* 
* Code by Antti Kirjavainen (antti.kirjavainen [_at_] gmail.com)
* 
* Since I only have one of these products, I'm not able to tell if the remotes have unique IDs or if they all send identical
* commands. My guess would be that the first 21 bits of the 25 bit command is the (unique) remote ID.
* 
* 
* HOW TO USE
* 
* Capture the binary commands from your remote with RemoteCapture.ino and copy paste the 25 bit commands to the #defines below.
* Then you can control your projection screen with sendGymstickCommand(GYMSTICK_START_STOP), sendGymstickCommand(GYMSTICK_PLUS) etc.
* 
* 
* PROTOCOL DESCRIPTION
* 
* Tri-state bits are used.
* A single command is: (no AGC/preamble) 25 command bits + radio silence
*
* All sample counts below listed with a sample rate of 44100 Hz (sample count / 44100 = microseconds).
* 
* Pulse length:
* SHORT: approx. 15 samples = 340 us
* LONG: approx. 43 samples = 975 us
* 
* Data bits:
* Data 0 = short HIGH, long LOW (wire 100)
* Data 1 = long HIGH, short LOW (wire 110)
* 
* Command is as follows:
* 21 bits for (possibly unique) remote control ID (hard coded in remotes?)
* 4 bits for command: START/STOP = 0010, MINUS = 0110, PLUS = 0100, MODE = 1000
* = 25 bits in total
* 
* End with LOW radio silence of (minimum) 290 samples = 6600 us
* 
******************************************************************************************************************************************************************
*/



// Example commands (capture your own remotes with the rc-switch library):
#define GYMSTICK_START_STOP     "0111010010100100000000010"
#define GYMSTICK_MODE           "0111010010100100000001000"
#define GYMSTICK_PLUS           "0111010010100100000000100"
#define GYMSTICK_MINUS          "0111010010100100000000110"



// Timings in microseconds (us). Get sample count by zooming all the way in to the waveform with Audacity.
// Calculate microseconds with: (samples / sample rate, usually 44100 or 48000) - ~15-20 to compensate for delayMicroseconds overhead.
// Sample counts listed below with a sample rate of 44100 Hz:
#define GYMSTICK_RADIO_SILENCE            6600   // 290 samples
#define GYMSTICK_PULSE_SHORT              340    // 15 samples
#define GYMSTICK_PULSE_LONG               975   // 43 samples (approx. PULSE_SHORT * 3)
#define GYMSTICK_COMMAND_BIT_ARRAY_SIZE   25     // Command bit count


#define TRANSMIT_PIN                      13     // We'll use digital 13 for transmitting
#define REPEAT_COMMAND                    4      // How many times to repeat the same command: original remote repeats 4 times
#define DEBUG                             false  // Some extra info on serial


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600); // Used for error messages even with DEBUG set to false
  
  if (DEBUG) Serial.println("Starting up...");
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {
  sendGymstickCommand(GYMSTICK_START_STOP);
  delay(7000);
  sendGymstickCommand(GYMSTICK_PLUS);
  delay(7000);
  sendGymstickCommand(GYMSTICK_MINUS);
  delay(7000);
  sendGymstickCommand(GYMSTICK_MODE);
  delay(7000);
  sendGymstickCommand(GYMSTICK_START_STOP);
  delay(7000);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void sendGymstickCommand(char* command) {

  if (command == NULL) {
    errorLog("sendGymstickCommand(): Command array pointer was NULL, cannot continue.");
    return;
  }

  // Prepare for transmitting and check for validity
  pinMode(TRANSMIT_PIN, OUTPUT); // Prepare the digital pin for output
  
  if (strlen(command) < GYMSTICK_COMMAND_BIT_ARRAY_SIZE) {
    errorLog("sendGymstickCommand(): Invalid command (too short), cannot continue.");
    return;
  }
  if (strlen(command) > GYMSTICK_COMMAND_BIT_ARRAY_SIZE) {
    errorLog("sendGymstickCommand(): Invalid command (too long), cannot continue.");
    return;
  }
  
  // Repeat the command:
  for (int i = 0; i < REPEAT_COMMAND; i++) {
    doGymstickSend(command);
  }

  // Disable output to transmitter to prevent interference with
  // other devices. Otherwise the transmitter will keep on transmitting,
  // which will disrupt most appliances operating on the 433.92MHz band:
  digitalWrite(TRANSMIT_PIN, LOW);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void doGymstickSend(char* command) {

  // Transmit command:
  for (int i = 0; i < GYMSTICK_COMMAND_BIT_ARRAY_SIZE; i++) {

    // If current command bit is 0, transmit HIGH-LOW-LOW (wire 100):
    if (command[i] == '0') {
      transmitHigh(GYMSTICK_PULSE_SHORT);
      transmitLow(GYMSTICK_PULSE_LONG);
    }

    // If current command bit is 1, transmit HIGH-HIGH-LOW (wire 110):
    if (command[i] == '1') {
      transmitHigh(GYMSTICK_PULSE_LONG);
      transmitLow(GYMSTICK_PULSE_SHORT);
    }
   }

  // Radio silence at the end.
  // It's better to rather go a bit over than under required length.
  transmitLow(GYMSTICK_RADIO_SILENCE);
  
  if (DEBUG) {
    Serial.println();
    Serial.print("Transmitted ");
    Serial.print(GYMSTICK_COMMAND_BIT_ARRAY_SIZE);
    Serial.println(" bits.");
    Serial.println();
  }
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void transmitHigh(int delay_microseconds) {
  digitalWrite(TRANSMIT_PIN, HIGH);
  //PORTB = PORTB D13high; // If you wish to use faster PORTB calls instead
  delayMicroseconds(delay_microseconds);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void transmitLow(int delay_microseconds) {
  digitalWrite(TRANSMIT_PIN, LOW);
  //PORTB = PORTB D13low; // If you wish to use faster PORTB calls instead
  delayMicroseconds(delay_microseconds);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void errorLog(String message) {
  Serial.println(message);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
