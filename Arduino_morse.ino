#define DEBUG_CHAR false // char to vidco translation
#define DEBUG_WORD false // ascii to vidco translation
#define DEBUG_LED false // vidco to LED translation
#define DEBUG_L false // L############ command
#define USR_FRIENDLY false // user friendly serial monitor information

#define IR_PIN 2 // infrared pin
#define POT_PIN A0 // potentiometer pin
#define MAX_POT 1023 // maximum potentiometer value
#define ERROR_CHAR "#" // error char for characters unable to translate

// LED pins
#define RED_LED 5
#define GREEN_LED 10
#define BLUE_LED 11
#define ORANGE_LED 6
#define YELLOW_LED 9
#define MAX_LED 255 // maximum LED value


bool readPOT = false; // controls pot to LED function
bool L_LED = false; // controls L############ to LED brightnesses

char alpha[26] = {
'A','B','C','D','E','F','G',
'H','I','J','K','L','M','N','O','P',
'Q','R','S','T','U','V','W','X','Y','Z'};

String vidco[26] = {
"-","!!-","!-*","-!","!","!-!","--",
"!--","!!","-!!","-*","!!*","!-","!*","*-","*!",
"-!*","!*-","!*!","*","**","--*","--!","-!*","!**","---"};

char punc[21] = {
' ','.',',','\'','?','!','+','-','*','/','=',
'1','2','3','4','5',
'6','7','8','9','0'};
  
String vidco_punc[21] = {
" ","*!!","*!-","*-!","*--","*-*","**!","**-","***","*!*","*---*",
"*!!-","*!!*","*!-!","*!--","*!-*",
"*!*!","*!*-","*!**","*-!!","*!!!"};



void setup() {
  Serial.begin(9600);
  // setting pin modes
  pinMode(IR_PIN, INPUT);
  pinMode(POT_PIN, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(ORANGE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);

#if USR_FRIENDLY
  Serial.println("Listening...");
#endif

}

String removePrecedingSpaces(String s) {
  bool space = true;
  String output = "";
  for (int i = 0; i < s.length(); i++) {
    if (space && s[i] != ' ') { // if no more spaces found
      space = false;
    }
    if (!space) { // after no more preceding spaces
      output += s[i]; // append rest of string
    }
  }
  return output;
}

String charToVidco(char s) {

#if DEBUG_CHAR
  Serial.println(s);
#endif

  for (int i = 0; i < 26; i++) { // loop through alphabetical
    if (alpha[i] == toupper(s)) { // if character index found
      return vidco[i]; // return vidco equivalent
    }
  }

#if DEBUG_CHAR
  Serial.println("not alpha");
#endif

  for (int i = 0; i < 21; i++) { // loop through numeric & punctuation
    if (punc[i] == s) { // if character index found
      return vidco_punc[i]; // return vidco equivalent
    }
  }

#if DEBUG
  Serial.print("Didnt find ");
  Serial.print(s);
  Serial.print(" (");
  Serial.print(toupper(s));
  Serial.print(") in charToVidco");
#endif

  return ERROR_CHAR; // error character if nothing returned previous
}

String asciiToVidco(String s) {
  String output = "";
  String letter = "";

#if DEBUG_WORD
  Serial.println(s.length());
#endif

  for (int i = 0; i < s.length(); i++) {
    letter = charToVidco(s[i]);
    if ((i != 0) && (output[output.length() - 1] != ' ') && (letter != " ")) {
      // if not the first character, and previous & current are not spaces
      
#if DEBUG_WORD
      Serial.print("no spaces previous and current");
      Serial.print(" | ");
      Serial.print(output[output.length() - 1]);
      Serial.print(" | ");
      Serial.print(letter);
      Serial.println(" | ");
      Serial.println(output[output.length() - 1] == ' ');
      Serial.println("added /");
#endif

      output += "/"; // append "/"
    }
    output += letter; // append letter

#if DEBUG_WORD
    Serial.print("added ");
    Serial.println(letter);
    Serial.println(output);
#endif

  }
  return output;
}

char vidcoToChar(String s) {
  for (int i = 0; i < 26; i++) { // loop through alphabetical
    if (vidco[i] == s) { // if character index found

#if DEBUG_CHAR
      Serial.println("Vidco is alpha");
#endif

      return alpha[i]; // return alpha equivalent
    }
  }
  for (int i = 0; i < 21; i++) { // loop through numerical and punctuation
    if (vidco_punc[i] == s) { // if vidco punctuation index found

#if DEBUG_CHAR
      Serial.println("Vidco is punc");
#endif

      return punc[i]; // return punctuation equivalent
    }
  }

#if DEBUG_CHAR
  Serial.println("Something went wrong");
  Serial.println(s);
#endif

  return ERROR_CHAR; // error character if nothing returned previous
}

String vidcoToAscii(String s) {
  String letter = "";
  String translated = "";
  for (int i = 0; i < s.length(); i++) {
    if (s[i] == '/') { // end of letter

#if DEBUG_WORD
      Serial.println("Read '/'");
#endif

      translated += vidcoToChar(letter); // translate and append to output

#if DEBUG_WORD
      Serial.print("Got ");
      Serial.println(translated[translated.length() - 1]);
#endif

      letter = ""; // empty letter variable for next index
    }
    else if (s[i] == ' ') { // end of word
      if (letter != "") { // letter would be empty if previous character was also a space (double space)
        translated += vidcoToChar(letter);

#if DEBUG_WORD
        Serial.print("Got ");
        Serial.println(translated[translated.length() - 1]);
        Serial.println("Read ' '");
#endif

        translated += ' ';
        letter = "";
      }
      else { // if letter is empty (previous was a space)
        translated += s[i]; // s[i] is a space in this case
      }
    }
    else { // not end of letter

#if DEBUG_WORD
      Serial.print("Read ");
      Serial.println(s[i]);
#endif

      letter += s[i]; // append to letter string
    }
  }
  if (letter != "") { // translates last letter if string doesnt end with a space
    translated += vidcoToChar(letter);

#if DEBUG_WORD
    Serial.print("Got ");
    Serial.println(translated[translated.length() - 1]);
    Serial.println("Read ' '");
#endif

  }
  return translated;
}

void wait(int amount) {
  unsigned long prev = millis(); // millis() counts from initial runtime in milliseconds (can get really large)
  while (millis() - prev < amount) { // count wait amount
  }
}

void vidco2LED(String s, int LED_NUM) {
  const int timeLength = (analogRead(POT_PIN) / (MAX_POT / 380)) + 20; // math for pot range 0-1023 -> 20-400(ms)
  wait(timeLength); // delay for possible LEDs that turned off too quick for human eye

#if DEBUG_LED
  Serial.print("Transmitting: ");
  Serial.println(s);
#endif

  for (int i = 0; i < s.length(); i++) {
    if (s[i] == '!') {

#if DEBUG_LED
      Serial.print("(!,1): ");
#endif

      analogWrite(LED_NUM, 1023);
      wait(timeLength); // LED on for 1x time length
    }
    else if (s[i] == '-') {

#if DEBUG_LED
      Serial.print("(-,2): ");
#endif

      analogWrite(LED_NUM, 1023);
      wait(timeLength * 2); // LED on for 2x time lengths
    }
    else if (s[i] == '*') {

#if DEBUG_LED
      Serial.print("(*,4): ");
#endif

      analogWrite(LED_NUM, 1023);
      wait(timeLength * 4); // LED on for 4x time lengths
    }
    else if (s[i] == '/') {

#if DEBUG_LED
      Serial.print("(/,2): ");
#endif

      analogWrite(LED_NUM, 0);
      wait(timeLength); // LED off for 1+1 time lengths between letters of same word -> +1 from out of if statement
    }
    else if (s[i] == ' ') {

#if DEBUG_LED
      Serial.print("( ,5): ");
#endif

      wait(timeLength * 4); // LED off for 4+1 time lengths between words -> +1 from out of if statement
    }
    else if (s[i] == '#') {

#if USR_FRIENDLY
      Serial.println("Error character found [#]; LED will not correspond this output");
#endif

    }
    else {
      Serial.print(" = error woops ");
    }
    // end of if's
    analogWrite(LED_NUM, 0);
    wait(timeLength); // 1x time length outside if statement regardless of character

#if DEBUG_LED
    Serial.println(s[i]);
#endif

  }
}

bool checkDigits(String s) {

  for (int i = 1; i < 13; i++) { // characters 1-13 (digits for L command)
    if (L_LED && isdigit(s[i])) {

#if DEBUG_L
      Serial.println(s[i]);
      Serial.println(digits[i - 1]);
#endif

    }
    else { // if any character is not a digit
      return false;
    }
  }
  return true; // if no character returned false
}


void sendLToLED(String digits) {

#if DEGUB_L
  Serial.println(digits);
  Serial.println("Made it");
#endif

  int greenRGB = digits.substring(1, 4).toInt();
  int orangeRGB = digits.substring(4, 7).toInt();
  int yellowRGB = digits.substring(7, 10).toInt();
  int redRGB = digits.substring(10, 13).toInt();

#if DEBUG_L
  Serial.println(greenRGB);
  Serial.println(orangeRGB);
  Serial.println(yellowRGB);
  Serial.println(redRGB);
#endif

  if (!(greenRGB > 255 or orangeRGB > 255 or yellowRGB > 255 or redRGB > 255)) {

#if USR_FRIENDLY
    Serial.print("Green: ");
    Serial.print(greenRGB);
    Serial.print(" Orange: ");
    Serial.print(orangeRGB);
    Serial.print(" Yellow: ");
    Serial.print(yellowRGB);
    Serial.print(" RED: ");
    Serial.println(redRGB);
#endif

    analogWrite(GREEN_LED, greenRGB);
    analogWrite(ORANGE_LED, orangeRGB);
    analogWrite(YELLOW_LED, yellowRGB);
    analogWrite(RED_LED, redRGB);
  }
}



void loop() {
  String s = "";

  if (Serial.available()) { // if serial monitor received input
    s = removePrecedingSpaces(Serial.readString());
    s.toUpperCase();
    // COMMANDS
    if (s.equals("PR")) {

#if USR_FRIENDLY
      Serial.println("Potentiometer reading toggled");
#endif

      if (readPOT) { // turns off red LED if potentiometer was in control
        analogWrite(RED_LED, 0);
      }
      readPOT = !readPOT; // toggle potentiometer read
    }
    else if (s.equals("IRRR")) {
      if (digitalRead(IR_PIN) == 1) {

#if USR_FRIENDLY
        Serial.print("IR: ");
#endif
        Serial.println("HIGH");
      }
      else if (digitalRead(IR_PIN) == 0) {

#if USR_FRIENDLY
        Serial.print("IR: ");
#endif
        Serial.println("LOW");
      }
    }
    else if (s.length() == 13 && s[0] == 'L' && checkDigits(s)) {
      sendLToLED(s);
      readPOT = false; // disable potentiometer value changing red LED value

    }
    else { // no commands; normal translation
      sendLToLED("L000000000000"); // turn off all LEDs
      readPOT = false; // disable potentiometer preventing turning off the red LED

#if USR_FRIENDLY
      Serial.print("IN: ");
      Serial.println(s);
#endif

      if (s[0] != '-' and s[0] != '*' and s[0] != '!') { // VIdCo check
        String vidco = asciiToVidco(s);

#if USR_FRIENDLY
        Serial.print("OUT: ");

#endif
        Serial.println(vidco);

        vidco2LED(vidco, GREEN_LED);
      }
      else { // if not VIdCo -> treated as ASCII translation

#if USR_FRIENDLY
        Serial.print("OUT: ");
#endif
        Serial.println(vidcoToAscii(s));

        vidco2LED(s, BLUE_LED);
      }
    }

#if USR_FRIENDLY
    Serial.println("Listening...");
#endif

  }
  // outside user input if statement
  if (readPOT) { // potentiometer LED updater
    analogWrite(RED_LED, analogRead(POT_PIN) / (MAX_POT / MAX_LED)); //1023/255 adjusts 0-1023 to 0-255 for LED brightness
  }
}
