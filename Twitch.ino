    
    #include <Arduino.h>
    #include <Wire.h>
    #include "Adafruit_LEDBackpack.h"
    #include "Adafruit_GFX.h"
    #include <SoftwareServo.h>
   // #include <SoftwareSerial.h>

    #include <WaveHC.h>
    #include <WaveUtil.h> 
    
    Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
    SoftwareServo myservo;
    SoftwareServo myservo2;
    int pos = 0; 
    static const uint8_t  PROGMEM// Bitmaps are stored in program memory
    sadBlinkImg[][8] = {    // Eye animation frames
      { 
        B00100100,         // Fully open sad eye
        B01000010,
        B10000001,
        B00111100,
        B01111110,
        B11111111,
        B11111111,
        B11111111       }
      ,
      { 
        B00100100,         
        B01000010,
        B10000001,
        B00000000,
        B01111110,
        B11111111,
        B11111111,
        B11111111        }
      ,
      { 
        B00100100,         
        B01000010,
        B10000001,
        B00000000,
        B00000000,
        B01111110,
        B11111111,
        B11111111       }
      ,
      {
        B00100100,         
        B01000010,
        B10000001,
        B00000000,
        B00000000,
        B00000000,
        B01111110,
        B11111111        }
      ,
      { 
        B00100100,         // Fully closed sad eye
        B01000010,
        B10000001,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B11111111       } 
     
    };
     
    static const uint8_t  PROGMEM// Bitmaps are stored in program memory
    blinkImg[][8] = {    // Eye animation frames
      { 
        B00111100,         // Fully open eye
        B01111110,
        B11111111,
        B11111111,
        B11111111,
        B11111111,
        B01111110,
        B00111100       }
      ,
      { 
        B00000000,
        B01111110,
        B11111111,
        B11111111,
        B11111111,
        B11111111,
        B01111110,
        B00111100       }
      ,
      { 
        B00000000,
        B00000000,
        B00111100,
        B11111111,
        B11111111,
        B11111111,
        B00111100,
        B00000000       }
      ,
      { 
        B00000000,
        B00000000,
        B00000000,
        B00111100,
        B11111111,
        B01111110,
        B00011000,
        B00000000       }
      ,
      { 
        B00000000,         // Fully closed eye
        B00000000,
        B00000000,
        B00000000,
        B10000001,
        B01111110,
        B00000000,
        B00000000       } 
    };
     
    static const uint8_t  PROGMEM// Bitmaps are stored in program memory
    happyBlinkImg[][8] = {    // Eye animation frames
      { 
        B00111100,         // Fully open happy eye
        B01111110,
        B11111111,
        B11111111,
        B11111111,
        B00000000,
        B10000001,
        B01111110       }
      ,
      { 
        B00000000,         
        B01111110,
        B11111111,
        B11111111,
        B11111111,
        B00000000,
        B10000001,
        B01111110       }
      ,
      { 
        B00000000,         
        B00000000,
        B01111110,
        B11111111,
        B11111111,
        B00000000,
        B10000001,
        B01111110       }
      ,
      { 
        B00000000,         
        B00000000,
        B00000000,
        B01111110,
        B11111111,
        B00000000,
        B10000001,
        B01111110       }
      ,
      { 
        B00000000,         // Fully closed happy eye      
        B00000000,
        B00000000,
        B01111110,
        B10000001,
        B00000000,
        B10000001,
        B01111110       } 
    };
     
    static const uint8_t PROGMEM // Bitmaps are stored in program memory
    annoyedBlinkImg[][8] = {    // Eye animation frames
      { 
        B10000001,         // Fully open annoyed eye
        B01100110,
        B00000000,
        B11111111,
        B11111111,
        B11111111,
        B01111110,
        B00111100       }
      ,
      { 
        B10000001,       
        B01100110,
        B00000000,
        B11111111,
        B11111111,
        B11111111,
        B01111110,
        B00000000        }
      ,
      { 
        B10000001,       
        B01100110,
        B00000000,
        B11111111,
        B11111111,
        B01111110,
        B00000000,
        B00000000       }
      ,
      {
        B10000001,       
        B01100110,
        B00000000,
        B11111111,
        B01111110,
        B00000000,
        B00000000,
        B00000000        }
      ,
      { 
        B10000001,         // Fully closed annoyed eye
        B01100110,
        B00000000,
        B10000001,
        B01111110,
        B00000000,
        B00000000,
        B00000000       } 
     
    };
     
    uint8_t
    blinkIndex[] = { 
      1, 2, 3, 4, 3, 2, 1 },
    // Blink bitmap sequence
    blinkCountdown = 100, // Countdown to next blink (in frames)
    gazeCountdown  =  75, // Countdown to next eye movement
    gazeFrames     =  50; // Duration of eye movement (smaller = faster)
    int8_t
    eyeX = 3, eyeY = 3,   // Current eye position
    newX = 3, newY = 3,   // Next eye position
    dX   = 0, dY   = 0;   // Distance from prior to new position
     
     
   
    int tapNum = 15; // logged number of taps
    byte mood = 1; // current mood
    const byte vibration PROGMEM = A0; // vibration sensor
    const int tapLevel PROGMEM = 512;
    long previousMillis = 0;        // will store last time LED was updated
     
    
    const int decay PROGMEM = 30000;         
     
    unsigned long 
     checkMillis,
     tapMillis,
     songMillis,
     currentMillis;

  //Wav Player Part
  
  SdReader card;    // This object holds the information for the card
  FatVolume vol;    // This holds the information for the partition on the card
  FatReader root;   // This holds the information for the volumes root directory
  WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

  uint8_t dirLevel; // indent level for file/dir names    (for prettyprinting)
  dir_t dirBuf;     // buffer for directory reads
  FatReader f;      // This holds the information for the file we're play

/*
 * Define macro to put error messages in flash memory
 */
//#define error(msg) error_P(PSTR(msg))

// Function definitions (we define them here, but the code is below)
void play(FatReader &dir);
     
    void setup() {
      byte i;
      myservo.attach(6);
      myservo2.attach(9);
     
      randomSeed(analogRead(A7));
      pinMode(vibration, INPUT_PULLUP);
     
      matrix.begin(0x70);
      matrix.setRotation(3);
      matrix.setBrightness(4);
      matrix.setTextSize(1);
      matrix.setTextWrap(false);  
      matrix.setTextColor(LED_ON);
      matrix.clear();
      matrix.writeDisplay();
    //  Serial.begin(9600);


     // WAV PLAYER SETUP PART  Uncomment these lines for serial debugging
     // putstring_nl("\nWave test!");  // say we woke up!
  
     // putstring("Free RAM: ");       // This can help with debugging, running out of RAM is bad
     //Serial.println(FreeRam());

     // if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
     // error("Card init. failed!");  // Something went wrong, lets print out why
  }
  
  // enable optimize read - some cards may timeout. Disable if you're having problems
  // card.partialBlockRead(true);
  
  // Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {   // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                           // we found one, lets bail
  }
  if (part == 5) {                     // if we ended up not finding one  :(
//    error("No valid FAT partition!");  // Something went wrong, lets print out why
  }
  
  // Lets tell the user about what we found
 // putstring("Using partition ");
//  Serial.print(part, DEC);
//  putstring(", type is FAT");
 // Serial.println(vol.fatType(), DEC);     // FAT16 or FAT32?
  
  // Try to open the root directory
  if (!root.openRoot(vol)) {
//    error("Can't open root dir!");      // Something went wrong,
  }
  
  // Whew! We got past the tough parts.
 // putstring_nl("Files found (* = fragmented):");

  // Print out all of the files in all the directories.
  root.ls(LS_R | LS_FLAG_FRAGMENTED);


     
    }
     
    void loop() {
     
      while(mood==0) // sad mood
      {
     SoftwareServo::refresh(); //The software servo needs this to be calles every 50ms to update the servos
        matrix.clear();
        
        matrix.drawBitmap(0, 0,
        sadBlinkImg[
          (blinkCountdown < sizeof(blinkIndex)) ? 
        blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
        0                                       // No, show bitmap 0
        ], 8, 8, LED_ON);
        // Decrement blink counter.  At end, set random time for next blink.
        if(--blinkCountdown == 0) blinkCountdown = random(5, 180);
     
        // Add a pupil (2x2 black square) atop the blinky eyeball bitmap.
        // Periodically, the pupil moves to a new position...
        if(--gazeCountdown <= gazeFrames) {
          // Eyes are in motion - draw pupil at interim position
          matrix.fillRect(
          newX - (dX * gazeCountdown / gazeFrames),
          newY - (dY * gazeCountdown / gazeFrames),
          2, 2, LED_OFF);
          if(gazeCountdown == 0) {    // Last frame?
            eyeX = newX; 
            eyeY = newY; // Yes.  What's new is old, then...
            do { // Pick random positions until one is within the eye circle
              newX = random(0,7); 
              newY = random(5,7);
              dX   = newX-3;  
              dY   = newY-3;
            } 
            while((dX * dX + dY * dY) >= 10);        // Thank you Pythagoras
            dX            = newX - eyeX;             // Horizontal distance to move
            dY            = newY - eyeY;             // Vertical distance to move
            gazeFrames    = random(3, 15);           // Duration of eye movement
            gazeCountdown = random(gazeFrames, 120); // Count to end of next movement
          }
        }
        else {
          // Not in motion yet -- draw pupil at current static position
          matrix.fillRect(eyeX, eyeY, 2, 2, LED_OFF);
          myservo.write(eyeX*10); //move the head in relation to the eye position
          myservo2.write(eyeY*5);  //since this is the sad state, keep the head lower
        }
     
     
        // Refresh all of the matrices in one quick pass
        matrix.writeDisplay();
     
        if(millis()-checkMillis > random(10000,30000))
        {
          sadNoise();
          checkMillis = millis();
        }
     
        tapMillis = millis();
     
        while(millis()-tapMillis < 40)
        {
          checkTaps();
        
        }  
        songMillis = millis();
      }
     
      while(mood==1) // neutral mood
      {
       SoftwareServo::refresh();
       
      
        // Draw eyeball in current state of blinkyness (no pupil).
        matrix.clear();
        // When counting down to the next blink, show the eye in the fully-
        // open state.  On the last few counts (during the blink), look up
        // the corresponding bitmap index.
        matrix.drawBitmap(0, 0,
        blinkImg[
          (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
        blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
        0                                       // No, show bitmap 0
        ], 8, 8, LED_ON);
        // Decrement blink counter.  At end, set random time for next blink.
        if(--blinkCountdown == 0) blinkCountdown = random(5, 180);
     
        // Add a pupil (2x2 black square) atop the blinky eyeball bitmap.
        // Periodically, the pupil moves to a new position...
        if(--gazeCountdown <= gazeFrames) {
          // Eyes are in motion - draw pupil at interim position
          matrix.fillRect(
          newX - (dX * gazeCountdown / gazeFrames),
          newY - (dY * gazeCountdown / gazeFrames),
          2, 2, LED_OFF);
          if(gazeCountdown == 0) {    // Last frame?

            eyeX = newX; 
            eyeY = newY; // Yes.  What's new is old, then...
            do { // Pick random positions until one is within the eye circle
              newX = random(7); 
              newY = random(7);
              dX   = newX - 3;  
              dY   = newY - 3;
            } 
            while((dX * dX + dY * dY) >= 10);      // Thank you Pythagoras
            dX            = newX - eyeX;             // Horizontal distance to move
            dY            = newY - eyeY;             // Vertical distance to move
            gazeFrames    = random(3, 15);           // Duration of eye movement
            gazeCountdown = random(gazeFrames, 120); // Count to end of next movement
          }
        } 
        else {
          // Not in motion yet -- draw pupil at current static position
          matrix.fillRect(eyeX, eyeY, 2, 2, LED_OFF);
          myservo.write(eyeX*10);  //move the servos in relation to eye position
          myservo2.write(eyeY*10);
        }
     
     
        // Refresh all of the matrices in one quick pass
        matrix.writeDisplay();
       
         if(millis()-checkMillis > random(30000,50000))
        {
          neutralNoise();
          checkMillis = millis();
        }
     
     
        tapMillis = millis();
     
        while(millis()-tapMillis < 40)
        {
          checkTaps();
        }  
        songMillis = millis();
       
  
      }
     
      while(mood == 2) // happy mood
      {
        
        // Draw eyeball in current state of blinkyness (no pupil).
        matrix.clear();
        // When counting down to the next blink, show the eye in the fully-
        // open state.  On the last few counts (during the blink), look up
        // the corresponding bitmap index.
        matrix.drawBitmap(0, 0,
        happyBlinkImg[
          (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
        blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
        0                                       // No, show bitmap 0
        ], 8, 8, LED_ON);
        // Decrement blink counter.  At end, set random time for next blink.
        if(--blinkCountdown == 0) blinkCountdown = random(5, 180);
     
        // Add a pupil (2x2 black square) atop the blinky eyeball bitmap.
        // Periodically, the pupil moves to a new position...
        if(--gazeCountdown <= gazeFrames) {
          // Eyes are in motion - draw pupil at interim position
          matrix.fillRect(
          newX - (dX * gazeCountdown / gazeFrames),
          newY - (dY * gazeCountdown / gazeFrames),
          2, 2, LED_OFF);
          if(gazeCountdown == 0) {    // Last frame?
            eyeX = newX; 
            eyeY = newY; // Yes.  What's new is old, then...
            do { // Pick random positions until one is within the eye circle
              newX = random(7); 
              newY = random(4);
              dX   = newX - 3;  
              dY   = newY - 3;
            } 
            while((dX * dX + dY * dY) >= 10);      // Thank you Pythagoras
            dX            = newX - eyeX;             // Horizontal distance to move
            dY            = newY - eyeY;             // Vertical distance to move
            gazeFrames    = random(3, 15);           // Duration of eye movement
            gazeCountdown = random(gazeFrames, 120); // Count to end of next movement

                     }
        } 
        else {
          // Not in motion yet -- draw pupil at current static position
          matrix.fillRect(eyeX, eyeY, 2, 2, LED_OFF);
          myservo.write(eyeX*10);
          myservo2.write(eyeY*10);
          SoftwareServo::refresh();
        }
     
     
        // Refresh all of the matrices in one quick pass
        matrix.writeDisplay();
     
        if(millis()-checkMillis > random(10000,30000))
        {
          happyNoise();
          checkMillis = millis();
        }
     
        tapMillis = millis();
     
        while(millis()-tapMillis < 40)
        {
          checkTaps();
        }  
     
      }
     
      while(mood==3) // annoyed mood
      {
        SoftwareServo::refresh();
                // Draw eyeball in current state of blinkyness (no pupil).
        matrix.clear();
        // When counting down to the next blink, show the eye in the fully-
        // open state.  On the last few counts (during the blink), look up
        // the corresponding bitmap index.
        matrix.drawBitmap(0, 0,
        annoyedBlinkImg[
          (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
        blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
        0                                       // No, show bitmap 0
        ], 8, 8, LED_ON);
        // Decrement blink counter.  At end, set random time for next blink.
        if(--blinkCountdown == 0) blinkCountdown = random(5, 180);
     
        // Add a pupil (2x2 black square) atop the blinky eyeball bitmap.
        // Periodically, the pupil moves to a new position...
        if(--gazeCountdown <= gazeFrames) {
          // Eyes are in motion - draw pupil at interim position
          matrix.fillRect(
          newX - (dX * gazeCountdown / gazeFrames),
          newY - (dY * gazeCountdown / gazeFrames),
          2, 2, LED_OFF);
          if(gazeCountdown == 0) {    // Last frame?
            eyeX = newX; 
            eyeY = newY; // Yes.  What's new is old, then...
            do { // Pick random positions until one is within the eye circle
              newX = random(7); 
              newY = random(3,7);
              dX   = newX - 3;  
              dY   = newY - 3;
            } 
            while((dX * dX + dY * dY) >= 10);      // Thank you Pythagoras
            dX            = newX - eyeX;             // Horizontal distance to move
            dY            = newY - eyeY;             // Vertical distance to move
            gazeFrames    = random(3, 15);           // Duration of eye movement
            gazeCountdown = random(gazeFrames, 120); // Count to end of next movement
          }
        }
        else {
          // Not in motion yet -- draw pupil at current static position
          matrix.fillRect(eyeX, eyeY, 2, 2, LED_OFF);
          myservo.write(eyeX*10);
          myservo2.write(eyeY*10);
        }
     
        // Refresh all of the matrices in one quick pass
        matrix.writeDisplay();
        
        if(millis()-checkMillis > random(10000,30000))
        {
         angryNoise();
          checkMillis = millis();
        }
     
        tapMillis = millis();
     
        while(millis()-tapMillis < 40)
        {
          checkTaps();
        }    
      }
    }
     
    #define isdigit(n) (n >= '0' && n <= '9')
     
   
    void checkTaps()
    {
     
     
      if(analogRead(vibration)<tapLevel) //these sounds play when the touch sensor is activated
      {
        tapNum++;
      byte i;
       i = random(8);
       if (i == 0) {
           playcomplete("1.WAV");
       } else if (i == 1) {
           playcomplete("2.WAV");
       } else if (i == 2) {
           playcomplete("3.WAV");      
       } else if (i == 3) {
           playcomplete("4.WAV");
       } else if (i == 4) {
           playcomplete("5.WAV");              
       } else if (i == 5) {
           playcomplete("6.WAV");
       } else if (i == 6) {
           playcomplete("7.WAV");
        } else if (i == 7) {
           playcomplete("8.WAV");
              }
        }
     
      if(tapNum <= 10) mood = 0;    //here you can change how frequently you need to activate the sensor to change the mood.
      else if(tapNum <= 20) mood = 1;
      else if(tapNum <= 30) mood = 2;
      else if(tapNum > 30) mood = 3;
     
      currentMillis = millis();
     
      if(currentMillis - previousMillis > decay) {
        // save the last time you blinked the LED 
        previousMillis = currentMillis;   
        tapNum--;
        if(tapNum < 0) tapNum = 0;
        if(tapNum == 30) tapNum = 15;
        if(tapNum > 40) tapNum = 40;
      }
     
      currentMillis = millis();
     
     if(currentMillis - songMillis > random(30000,60000) && mood == 2)
      {
        matrix.clear();
     
        matrix.drawBitmap(0, 0, happyBlinkImg[0], 8, 8, LED_ON);
        eyeX = 3;
        eyeY = 3;
        matrix.fillRect(eyeX, eyeY, 2, 2, LED_OFF);
        matrix.writeDisplay();
     
       // play_rtttl(songs[random(7)]);
        previousMillis = millis();
        checkMillis = millis();
        songMillis = millis();
      }
    }

    //WAV PLAYER PIECES

    void error_P(const char *str) {
  PgmPrint("Error: ");
  SerialPrint_P(str);
  sdErrorCheck();
  while(1);
}
/*
 * print error message and halt if SD I/O error, great for debugging!
 */
void sdErrorCheck(void) {
  if (!card.errorCode()) return;
  PgmPrint("\r\nSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  PgmPrint(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}

void play(FatReader &dir) {
  FatReader file;
  while (dir.readDir(dirBuf) > 0) {    // Read every file in the directory one at a time
  
    // Skip it if not a subdirectory and not a .WAV file
    if (!DIR_IS_SUBDIR(dirBuf)
         && strncmp_P((char *)&dirBuf.name[8], PSTR("WAV"), 3)) {
      continue;
    }

    Serial.println();            // clear out a new line
    
    for (uint8_t i = 0; i < dirLevel; i++) {
       Serial.write(' ');       // this is for prettyprinting, put spaces in front
    }
    if (!file.open(vol, dirBuf)) {        // open the file in the directory
//      error("file.open failed");          // something went wrong
    }
    
    if (file.isDir()) {                   // check if we opened a new directory
    //  putstring("Subdir: ");
      printEntryName(dirBuf);
      Serial.println();
      dirLevel += 2;                      // add more spaces
      // play files in subdirectory
      play(file);                         // recursive!
      dirLevel -= 2;    
    }
    else {
      // Aha! we found a file that isnt a directory
    //  putstring("Playing ");
      printEntryName(dirBuf);              // print it out
      if (!wave.create(file)) {            // Figure out, is it a WAV proper?
      //  putstring(" Not a valid WAV");     // ok skip it
      } else {
        Serial.println();                  // Hooray it IS a WAV proper!
        wave.play();                       // make some noise!
        
        uint8_t n = 0;
        while (wave.isplaying) {// playing occurs in interrupts, so we print dots in realtime
        //  putstring(".");
          if (!(++n % 32))Serial.println();
          delay(100);
        }       
        sdErrorCheck();                    // everything OK?
        // if (wave.errors)Serial.println(wave.errors);     // wave decoding errors
      }
    }
  }
}

// Plays a full file from beginning to end with no pause.
void playcomplete(char *name) {
  // call our helper to find and play this name
  playfile(name);
  while (wave.isplaying) {  //you can comment this out to allow the sensor to be activated and the eye to blink or move while it is playing sounds.  It caused servo jitter when I did it
  // do nothing while its playing
  }
  // now its done playing
}

void playfile(char *name) {
  // see if the wave object is currently doing something
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  }
  // look in the root directory and open the file
  if (!f.open(root, name)) {
  //  putstring("Couldn't open file "); Serial.print(name); return;
  }
  // OK read the file and turn it into a wave object
  if (!wave.create(f)) {
   // putstring_nl("Not a valid WAV"); return;
  }
  
  // ok time to play! start playback
  wave.play();
}

     
    void happyNoise() //these are the happy noises.  You can add more in the commented out lines.  watch for out of memory errors
     {
    byte i;
       i = random(10);  //if you add more files, increase this number accordingly.
       if (i == 0) {
           playcomplete("70.WAV");
       } else if (i == 1) {
           playcomplete("71.WAV");
       } else if (i == 2) {
           playcomplete("72.WAV");      
       } else if (i == 3) {
           playcomplete("73.WAV");
       } else if (i == 4) {
           playcomplete("74.WAV");              
       } else if (i == 5) {
           playcomplete("75.WAV");
       } else if (i == 6) {
           playcomplete("76.WAV");
        } else if (i == 7) {
          playcomplete("77.WAV");
        } else if (i == 8) {
          playcomplete("78.WAV");
        } else if (i == 9) {
      //     playcomplete("79.WAV");
       // } else if (i == 10) {
       //    playcomplete("80.WAV");
       // } else if (i == 11) {
       //    playcomplete("81.WAV");
       // } else if (i == 12) {
       //    playcomplete("82.WAV");
        }
     }
     
    void sadNoise()
    {
    byte i;
       i = random(10);
       if (i == 0) {
           playcomplete("50.WAV");
       } else if (i == 1) {
           playcomplete("51.WAV");
       } else if (i == 2) {
           playcomplete("52.WAV");      
       } else if (i == 3) {
           playcomplete("53.WAV");
       } else if (i == 4) {
           playcomplete("54.WAV");              
       } else if (i == 5) {
           playcomplete("55.WAV");
       } else if (i == 6) {
           playcomplete("56.WAV");
        } else if (i == 7) {
          playcomplete("57.WAV");
       // } else if (i == 8) {
       //   playcomplete("58.WAV");
      //  } else if (i == 9) {
      //     playcomplete("59.WAV");
      //  } else if (i == 10) {
       //    playcomplete("60.WAV");
       // } else if (i == 11) {
       //    playcomplete("61.WAV");
      //  } else if (i == 12) {
       //    playcomplete("62.WAV");
        }
     }
     
    void angryNoise()
     {
    byte i;
       i = random(10);
       if (i == 0) {
           playcomplete("30.WAV");
       } else if (i == 1) {
           playcomplete("31.WAV");
       } else if (i == 2) {
           playcomplete("32.WAV");      
       } else if (i == 3) {
           playcomplete("33.WAV");
       } else if (i == 4) {
           playcomplete("34.WAV");              
       } else if (i == 5) {
           playcomplete("35.WAV");
       } else if (i == 6) {
           playcomplete("36.WAV");
        } else if (i == 7) {
          playcomplete("37.WAV");
        } else if (i == 8) {
          playcomplete("38.WAV");
        } else if (i == 9) {
           playcomplete("39.WAV");
       // } else if (i == 10) {
        //   playcomplete("40.WAV");
       // } else if (i == 11) {
       //    playcomplete("41.WAV");
       // } else if (i == 12) {
        //   playcomplete("42.WAV");
        }
     }
    void neutralNoise()
    {
    byte i;
       i = random(10);
       if (i == 0) {
           playcomplete("10.WAV");
       } else if (i == 1) {
           playcomplete("11.WAV");
       } else if (i == 2) {
           playcomplete("12.WAV");      
       } else if (i == 3) {
           playcomplete("13.WAV");
       } else if (i == 4) {
           playcomplete("14.WAV");              
       } else if (i == 5) {
           playcomplete("15.WAV");
       } else if (i == 6) {
           playcomplete("16.WAV");
        } else if (i == 7) {
          playcomplete("17.WAV");
        } else if (i == 8) {
          playcomplete("18.WAV");
        } else if (i == 9) {
           playcomplete("19.WAV");
       // } else if (i == 10) {
        //   playcomplete("20.WAV");
       // } else if (i == 11) {
         //  playcomplete("21.WAV");
       // } else if (i == 12) {
        //   playcomplete("22.WAV");
        }
    }
