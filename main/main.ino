const int PIN_YES_BUTTON = 12;
const int PIN_NO_BUTTON = 13;

const int PIN_JOYSTICK_X = A4;
const int PIN_JOYSTICK_Y = A3;


const int PIN_YES_LED = 8;
const int PIN_NO_LED = 9;


// Hold duration for lights in milliseconds
const long HOLD_DURATION = 1000;


int latchedYesState = LOW;
long timeOfLastYes = 0;

int latchedNoState = LOW;
long timeOfLastNo = 0;


void setup() {
  // Initialize button pins as input:
  pinMode(PIN_YES_BUTTON, INPUT);
  pinMode(PIN_NO_BUTTON, INPUT);

  // Initialize the LED pins as output:
  pinMode(PIN_YES_LED, OUTPUT);
  pinMode(PIN_NO_LED, OUTPUT);

  // initialize serial communication:
  Serial.begin(9600);
}


  void loop() {
    handleButtons();
    handleJoystick();

    delay(50); 
  }


  void handleButtons(){
    // read inputs
    int yesButtonState = digitalRead(PIN_YES_BUTTON);
    int noButtonState = digitalRead(PIN_NO_BUTTON);

    

    // Edge Detection
    if(yesButtonState == HIGH && latchedYesState == LOW){
      timeOfLastYes = millis();
      latchedYesState = HIGH;
    } 

    if(yesButtonState == LOW && latchedYesState == HIGH && (millis() - timeOfLastYes > HOLD_DURATION)){
      latchedYesState = LOW;
    }

    // Edge Detection
    if(noButtonState == HIGH && latchedNoState == LOW){
      timeOfLastNo = millis();
      latchedNoState = HIGH;
    } 

    if(noButtonState == LOW && latchedNoState == HIGH && (millis() - timeOfLastNo > HOLD_DURATION)){
      latchedNoState = LOW;
    }

    //Serial.print("NoBtn: "); //Serial.println(noButtonState);
    //Serial.print(" LatchedNo: "); //Serial.println(latchedNoState);
    //Serial.print(" TimeOfLastNo: "); //Serial.println(timeOfLastNo);



    digitalWrite(PIN_YES_LED, latchedYesState);
    digitalWrite(PIN_NO_LED, latchedNoState);
  }

  void handleJoystick(){

    const int DEADBAND_RANGE = 150;

    // We have 8 directions, so each one should have margin of 22.5 above and below
    // That way, each one occupies 45 degrees - 8 directions
    const double JOYSTICK_ANGLE_ERROR_MARGIN = 45 / 2;

    int joystickX = analogRead(PIN_JOYSTICK_X);
    int joystickY = analogRead(PIN_JOYSTICK_Y);

    // Converting to Cartesian coordinates with 0 as center
    joystickX -= 500;
    joystickY -= 500;

    //Serial.print("x: "); //Serial.println(joystickX);
    //Serial.print("y: "); //Serial.println(joystickY);

    // Deadbanding
    if (epsilonEquals(0, abs(joystickX), DEADBAND_RANGE) && 
        epsilonEquals(0, abs(joystickY), DEADBAND_RANGE)){
        Serial.print('P');
      return;
    } 

    int angle = (int) (atan2(joystickY, joystickX) * 180 / PI);
    //Serial.print("Angle: "); //Serial.println(angle);

    //Serial.print("Direction: ");

   
    // Case statement for the potential angles
    if (epsilonAngleEquals(angle, 0, JOYSTICK_ANGLE_ERROR_MARGIN)){
      Serial.print('R');
    } else if (epsilonAngleEquals(angle, 45, JOYSTICK_ANGLE_ERROR_MARGIN)){
      Serial.print('X');
    } else if (epsilonAngleEquals(angle, 90, JOYSTICK_ANGLE_ERROR_MARGIN)){
      Serial.print('F');
    } else if (epsilonAngleEquals(angle, 135, JOYSTICK_ANGLE_ERROR_MARGIN)){
      Serial.print('W');
    } else if (epsilonAngleEquals(angle, 180, JOYSTICK_ANGLE_ERROR_MARGIN)){
      Serial.print('L');
    } else if (epsilonAngleEquals(angle, 225, JOYSTICK_ANGLE_ERROR_MARGIN)){
      Serial.print('Z');
    } else if (epsilonAngleEquals(angle, 270, JOYSTICK_ANGLE_ERROR_MARGIN)){
      Serial.print('B');
    } else if (epsilonAngleEquals(angle, 315, JOYSTICK_ANGLE_ERROR_MARGIN)){
      Serial.print('Y');
    }


  }

  bool epsilonEquals(double currentVal, double targetVal, double epsilon){
    return abs(targetVal - currentVal) < epsilon;
  }

  bool epsilonAngleEquals(double currentDegrees, double targetDegrees, double epsilon){
    return abs(relativeAngularDifference(currentDegrees, targetDegrees)) < epsilon;
  }
  


  // Thanks Aviva for the angle code <3
  // Iron Pants 5026!

   /**
   * Modulo on a negative number returns a negative number. This function always returns positive
   * numbers, even for negative angles, without rotation.
   *
   * @param degrees
   * @return normalized degrees
   */
  int normalizeDegrees(int degrees) {
    return (degrees % 360 + 360) % 360;
  }

  /**
   * This function finds the degree difference between angles, the shortest path. Useful for pid
   * control of drivebase rotation. Returns a value between -180 and 180 on a 360 reference field.
   * Values greater than 180 won't exist, because the other direction is shorter.
   *
   * @param currentAngle Current Angle Degrees
   * @param newAngle Target Angle Degrees
   * @return Shortest angular difference in degrees
   */
  int relativeAngularDifference(int currentAngle, int newAngle) {
    int a = normalizeDegrees(currentAngle - newAngle);
    int b = normalizeDegrees(newAngle - currentAngle);
    return a < b ? a : -b;
  }



