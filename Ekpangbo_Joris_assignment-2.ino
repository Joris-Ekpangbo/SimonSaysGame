/*
   Simon Says game with sound effects.
   Made by Joris Ekpangbo
   Latest version 2/24/2016
*/

#include <CapacitiveSensor.h> // To allow the use of a capacitor sensor added to the circuit 
#include <LiquidCrystal.h> // To allow the use of the LCD

// Defintion of all the PINS used to govern all the inputs and the outputs of the circuit

LiquidCrystal lcd (13, 12, 9, 8, 7, 6);
CapacitiveSensor capSensor = CapacitiveSensor(A4, A5);

const int piezoPIN = 11;

const int LEDRED = 2;
const int LEDYELLOW = 3;
const int LEDGREEN = 4;
const int LEDBLUE = 5;

const int REDSENSORPIN = A0;
const int YELLOWSENSORPIN = A1;
const int GREENSENSORPIN = A2;
const int BLUESENSORPIN = A3;


int velocityArray[] = {1000, 750, 500}; // array to help calibrate the speed of the LEDs pattern according to the level reached by the player
int noteArray[] = {1000, 4000, 3500, 6000}; // array to help play distinct sounds thanks to the piezo
int level[] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; // defintion of all the levels reachable by the player, assuming that the level 10 (13 LEDs pattern) would be the most difficult
int ledArray[] = {LEDRED, LEDYELLOW, LEDGREEN, LEDBLUE};
int patternArray[100]; // array to hold the LEDs patterns
int guessArray[100]; // array to hold the players guesses

// Photo resistor state checker for each LED

int redSensorValue = 0;
int yellowSensorValue = 0;
int greenSensorValue = 0;
int blueSensorValue = 0;


  //Sensor Values Thresholds In the lab
  int thresholdStateRED = 650;
  int thresholdStateYELLOW = 650;
  int thresholdStateGREEN = 630;
  int thresholdStateBLUE = 650;


/*
// Sensor Values Thresholds

int thresholdStateRED = 350;
int thresholdStateYELLOW = 350;
int thresholdStateGREEN = 330;
int thresholdStateBLUE = 350;
*/
int buttonCheck = 0; // to check if the number of player guesses is equal to the number of times that the LEDs light per pattern
int levelIncrement = 0; // to change the level game according to the number of the player wins or the number of the player losses
int turnCounter = 0;

boolean victoryFlag = false; // flag to check victory condition
boolean startGame = false; // flag to check if the game has started
boolean gameLaunched = false; // flag to check if the game has been launched by the capacitive sensor


void setup() {

  randomSeed(analogRead(0)); // Added to generate "more randomness" with the patternArray for the output function displayPattern
  lcd.begin(16, 2); // to define the limits of the LCD Screen
  Serial.begin(9600);  // needed for serial debugging

  //setup of PINMODES

  for (int i = 0; i < 4; i++)
  {
    pinMode(ledArray[i], OUTPUT);
  }

  // Printing Welcoming message to the player
  lcdPrint("Bonjour ;)", "Good Morning!");
  delay(3000);
  lcdPrint("***SIMON SAYS***", "WELCOME TO YOU!");
  delay(3000);

}// end of setup

void detectTouch()
{
  // Launching of the game thanks to a pressure on the capacitive sensor
  lcdPrint("Press the touch", "screen to start!");

  Serial.println(gameLaunched);
  Serial.println("\n");
  if (gameLaunched == false) {
    long sensorValue = capSensor.capacitiveSensor(30);

    Serial.println("\n");
    Serial.println(sensorValue);
    Serial.println("\n");

    // Launch the game if the capacitive sensor is pressed (sensorValue==0 in that case)
    if (sensorValue == 0)
    {
      gameLaunched = true;
    }

    initializeGame(); // initialize the game if the game is properly launched
  }

}// end of detectTouch

void initializeGame() {
  // Initialization of the game by flashing LEDs in a particular pattern with distinct sounds playing for each LED on
   
  Serial.println("Level\n:");
  Serial.println(levelIncrement);
  Serial.println("\n");

  // The initialization starts if the game has not already started, the capacitive sensor has been pressed and if the game level is not 10 (highest level reachable) 
  if (startGame == false && gameLaunched == true && levelIncrement != 10) 
  {
    startGame = true;
    for (int i = 0; i < 4; i++)
    {
      digitalWrite(ledArray[i], HIGH);
      tone(piezoPIN, noteArray[i], 100);
      delay(250);
      digitalWrite(ledArray[i], LOW);
    }

    for (int i = 3; i >= 0; i--)
    {
      digitalWrite(ledArray[i], HIGH);
      tone(piezoPIN, noteArray[i], 100);
      delay(250);
      digitalWrite(ledArray[i], LOW);
    }
    noTone(piezoPIN);

    lcd.clear();
    lcd.print(" --- LEVEL");
    lcd.print(levelIncrement);
    lcd.print(" ---");
    delay(2000);

    Serial.println("Initializing Game Phase started...");
    delay(500);

  }
  gameLaunched = false;

}// end of initializeGame

void displayPattern() {
  
  // Displaying the pattern randomly determined that the player should match

  lcdPrint("Watch carefully", "this pattern...");

  if (startGame == true)
  {

    while (turnCounter != level[levelIncrement])
    {

      Serial.print("\nTurn:\t"); // Some serial output to follow along the number of times that the LEDs have been lit on
      Serial.print(turnCounter);
      Serial.println("\n");
      patternArray[turnCounter] = random (1, 5); // Assigning a random number (1-4) to the patternArray[y], y being the turn counter

      for (int x = turnCounter; x <= turnCounter; x++)
      {

        Serial.print(patternArray[x]);

        for (int y = 0; y < 4; y++)
        {

          // if statements to display the stored values in the patternArray
          
          if (patternArray[x] == 1 && ledArray[y] == LEDRED) //if the pattern array value is 1 the RED LED is lit on
          {
            digitalWrite(ledArray[y], HIGH);
            tone(piezoPIN, noteArray[y], 100); // A special sound id played by the piezo according to the LED colour

            // The LED flashing speed is defined according to the level reached by the player (level 0-2 => 1000ms; level 3-6 => 750ms; level 7-10 => 500ms)
             
            if (levelIncrement >= 0 && levelIncrement <= 2)
              delay(velocityArray[0]);
            else if (levelIncrement >= 3 && levelIncrement <= 6)
              delay(velocityArray[1]);
            else if (levelIncrement >= 7 && levelIncrement <= 10)
              delay(velocityArray[2]);

            digitalWrite(ledArray[y], LOW);
            delay(100);
            break;
          }

          if (patternArray[x] == 2 && ledArray[y] == LEDYELLOW)
          {
            digitalWrite(ledArray[y], HIGH);
            tone(piezoPIN, noteArray[y], 100);

            if (levelIncrement >= 0 && levelIncrement <= 2)
              delay(velocityArray[0]);
            else if (levelIncrement >= 3 && levelIncrement <= 6)
              delay(velocityArray[1]);
            else if (levelIncrement >= 7 && levelIncrement <= 10)
              delay(velocityArray[2]);

            digitalWrite(ledArray[y], LOW);
            delay(500);
            break;
          }

          if (patternArray[x] == 3 && ledArray[y] == LEDGREEN)
          {
            digitalWrite(ledArray[y], HIGH);
            tone(piezoPIN, noteArray[y], 100);

            if (levelIncrement >= 0 && levelIncrement <= 2)
              delay(velocityArray[0]);
            else if (levelIncrement >= 3 && levelIncrement <= 6)
              delay(velocityArray[1]);
            else if (levelIncrement >= 7 && levelIncrement <= 10)
              delay(velocityArray[2]);

            digitalWrite(ledArray[y], LOW);
            delay(500);
            break;
          }

          if (patternArray[x] == 4 && ledArray[y] == LEDBLUE)
          {
            digitalWrite(ledArray[y], HIGH);
            tone(piezoPIN, noteArray[y], 100);

            if (levelIncrement >= 0 && levelIncrement <= 2)
              delay(velocityArray[0]);
            else if (levelIncrement >= 3 && levelIncrement <= 6)
              delay(velocityArray[1]);
            else if (levelIncrement >= 7 && levelIncrement <= 10)
              delay(velocityArray[2]);

            digitalWrite(ledArray[y], LOW);
            delay(500);
            break;
          }
        }
      }
      turnCounter++;

    } // for loop to display pattern to match

    Serial.println("\n");
    Serial.println("Displaying the randomly generated pattern done!");
    Serial.println("The turn counter for this level is\t:");
    Serial.println(turnCounter);
    Serial.println("\n");
    delay(250);

    Serial.println("The pattern array displayed previously is the following: \n");
    for (int i = 0; i < level[levelIncrement]; i++)
    {
      Serial.println(patternArray[i]);
    }
    getUserInput();
  }


}// end of displayPattern

void getUserInput() {
  
  // Waiting for the user inputs to be entered and recording them

  lcdPrint("Try to reproduce", "it...");

  Serial.println("Checking User Inputs...");

  while (buttonCheck != level[levelIncrement])
  {

    int x = 0;
    //Statement controlled by turn counter
    do
    {

      // Reading all the photoresistors (connected to the LEDs, paying attention to the colors) 
      redSensorValue = analogRead(REDSENSORPIN);
      delay(1);
      yellowSensorValue = analogRead(YELLOWSENSORPIN);
      delay(1);
      greenSensorValue = analogRead(GREENSENSORPIN);
      delay(1);
      blueSensorValue = analogRead(BLUESENSORPIN);

      Serial.print("\n");
      Serial.print("Raw Sensor Values\t Red: ");
      Serial.print(redSensorValue);
      delay(100);
      Serial.print("\t Yellow: ");
      Serial.print(yellowSensorValue);
      delay(100);
      Serial.print("\t Green: ");
      Serial.print(greenSensorValue);
      delay(100);
      Serial.print("\t Blue: ");
      Serial.print(blueSensorValue);
      delay(100);
      Serial.print("\n");


      // Checking for sensor value changes
      
      if (redSensorValue <= thresholdStateRED) // Light on the LED if the sensor Value is inferior to a certain threshold arbitrarily defined
      {

        guessArray[x] = 1;
        digitalWrite(ledArray[0], HIGH);
        tone(piezoPIN, noteArray[0], 100);
        delay(100);
        digitalWrite(ledArray[0], LOW);
        delay(50);
        Serial.print("RED ON\n");
        Serial.println("Button Index:\t");
        Serial.println(x);
        Serial.print("\n");
        Serial.print(guessArray[x]);
        Serial.print("\t");
        Serial.print(patternArray[x]);
        Serial.print("\n");
        buttonCheck++;
        x++;
      }
      else if (yellowSensorValue <= thresholdStateYELLOW)
      {

        guessArray[x] = 2;
        digitalWrite(ledArray[1], HIGH);
        tone(piezoPIN, noteArray[1], 100);
        delay(100);
        digitalWrite(ledArray[1], LOW);
        delay(50);
        Serial.print("YELLOW ON\n");
        Serial.println("Button Index:\t");
        Serial.println(x);
        Serial.print("\n");
        Serial.print(guessArray[x]);
        Serial.print("\t");
        Serial.print(patternArray[x]);
        Serial.print("\n");
        buttonCheck++;
        x++;
      }

      else if (greenSensorValue <= thresholdStateGREEN)
      {

        guessArray[x] = 3;
        digitalWrite(ledArray[2], HIGH);
        tone(piezoPIN, noteArray[2], 100);
        delay(100);
        digitalWrite(ledArray[2], LOW);
        delay(50);
        Serial.print("GREEN ON\n");
        Serial.println("Button Index:\t");
        Serial.println(x);
        Serial.print("\n");
        Serial.print(guessArray[x]);
        Serial.print("\t");
        Serial.print(patternArray[x]);
        Serial.print("\n");
        buttonCheck++;
        x++;
      }

      else if (blueSensorValue <= thresholdStateBLUE)
      {

        guessArray[x] = 4;
        digitalWrite(ledArray[3], HIGH);
        tone(piezoPIN, noteArray[3], 100);
        delay(100);
        digitalWrite(ledArray[3], LOW);
        delay(50);
        Serial.print("BLUE ON\n");
        Serial.println("Button Index:\t");
        Serial.println(x);
        Serial.print("\n");
        Serial.print(guessArray[x]);
        Serial.print("\t");
        Serial.print(patternArray[x]);
        Serial.print("\n");
        buttonCheck++;
        x++;
      }

    } while (x != turnCounter);
  }
  compareUserAnswersToAnswerArray();

  Serial.println("Checking User Inputs done");

  buttonCheck = 0; // reset the button check when all the inputs are checked

}// end of getUserInputs

void compareUserAnswersToAnswerArray() {
  // All the users inputs being recorded, compare them to the answerArray

  Serial.println("\nButton Check:\t");
  Serial.println(buttonCheck);

  Serial.println("\nStarting pattern  and guess arrays comparison...\n");
  for (int i = 0; i < 4; i++)
  {
    Serial.println(patternArray[i]);
  }
  Serial.println("\n");
  for (int i = 0; i < 4; i++)
  {
    Serial.println(guessArray[i]);
  }
  for (int x = 0; x < level[levelIncrement]; x++)
  {

    if (guessArray[x] != (patternArray[x])) // set the victory flag to false if the player guesses and the pattern array are not identical
    {
      Serial.print("NO YOU GOT IT WRONG!!!\n");
      Serial.print(guessArray[x]);
      Serial.println("\t");
      Serial.print(patternArray[x]);
      Serial.println("\n");
      victoryFlag = false;
      displayVictory();
      break;

    }

    else 
    {
      Serial.println("YES YOU GOT IT RIGHT!!!\n");
      Serial.print(guessArray[x]);
      Serial.println("\t");
      Serial.print(patternArray[x]);
      Serial.println("\n");


      if (x == level[levelIncrement] - 1) // set the victory flag to true if the player guesses and the pattern array are identical
      {
        victoryFlag = true;
        displayVictory();
        break;
      }

    }
  }
  Serial.println("Comaparison input-pattern done!");
  
}// end of compareUserAnswersToAnswerArray

void displayVictory() {


  // The victory condition met, display a victory message on the LCD
  startGame = false;
  if (victoryFlag == true) {
    Serial.println("CONGRATULATIONS!!! YOU WIN!!!!!");

    // increment the game level if the player was right
    if (levelIncrement >= 0 && (levelIncrement < 10))
    {
      levelIncrement++;
    }
    victoryFlag = false;

    lcdPrint("CONGRATULATIONS!!!", "YOU WIN!!!!!");
    delay(500);

    lcdPrint("BRAVO CHAMPION !!!", ":)");
    delay(700);
  }
  else {

    for (int y = 0; y <= 2; y++)
    { //Flashing lights for failure

      // decrement the game level if the player was wrong
      
      if (levelIncrement > 0 && (levelIncrement <= 10))
      {
        levelIncrement--;
      }
      digitalWrite(ledArray[0], HIGH);
      digitalWrite(ledArray[1], HIGH);
      digitalWrite(ledArray[2], HIGH);
      digitalWrite(ledArray[3], HIGH);
      delay(200);
      digitalWrite(ledArray[0], LOW);
      digitalWrite(ledArray[1], LOW);
      digitalWrite(ledArray[2], LOW);
      digitalWrite(ledArray[3], LOW);
      delay(200);
    }
    delay(500);

    Serial.println("OOPS! YOU LOSE... TRY AGAIN!");
    lcdPrint("OOPS! YOU LOSE...", "TRY AGAIN!");
    delay(500);
  }

  turnCounter = 0; //Resets turn value so the game starts over without need for a reset button

}// end of displayVictory

void lcdPrint(String message0, String message1)
{

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message0);
  lcd.setCursor(0, 1);
  lcd.print(message1);

}// end of lcdPrint

void loop() {

  detectTouch();
  displayPattern();

}
