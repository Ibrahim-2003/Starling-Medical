// Define pin connections & motor's steps per revolution
const int dirPin = 2;
const int stepPin = 3;
//const int stepsPerRevolution = 5000;
const int togglePin = 4;

//const int dirPin = 27;
//const int stepPin = 33;
//const int togglePin = 15;
int state = 0;

void setup()
{
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(togglePin, INPUT);
  Serial.begin(250000);
}

void loop()
{
  // Call moveRev()
  if (digitalRead(togglePin) == 0)
  {
    if (state == 0)
    {
      // Program in the refill volume
      int steps = 3500;
      accelUp(1, 8000, 5500, 500);
      moveRev(1, 5500, steps - 1000);
      accelDown(1, 5500, 8000, 500);
      delay(500);
    }
    if (state == 1)
    {
      // Program in the calibration run:
      moveRev(0, 30000, 550); // Calibrate rate of 4000
      // moveRev(0, 7500, 550); // 3500
      moveRev(0, 28000, 550); // 3100
      // moveRev(0, 6800, 550); // 2800
      
      moveRev(0, 26000, 550); // 2500
      accelUp(0, 26000, 10000, 50); // These acceleration steps kick in to try to smooth out any binding
      // moveRev(0, 6200, 550); // 2200
      // accelUp(0, 6200, 6000, 106);
      
      moveRev(0, 10000, 250); // 2000
      accelUp(0, 10000, 4800, 350);
      // moveRev(0, 5800, 550); // 1800
      // accelUp(0, 5800, 5600, 106);

      
      moveRev(0, 4800, 550); // 1600
      accelUp(0, 4800, 4600, 75);
      // moveRev(0, 5400, 550); // 1400
      // accelUp(0, 5400, 5200, 106);
      
      moveRev(0, 4600, 550); // 1200
      // accelUp(0, 5200, 5000, 104);
      // moveRev(0, 5000, 550); // 1000
      
      
      delay(500);
    }

    // A purge for air to leave the chamber before the trial begins
    /*
    if (state == 2) 
    {
      // Place a purge that is of volume of the tube
    }
    */
    
    switch(state)
    {
      case 0:
        state = 1;
        // state = 2;
        break;
      case 1:
        state = 0;
        break;
      case 2:
        state = 1;
        break;
    }
  }
}

int moveRev(int dir, int per, int num_step)
{
  // dir: the direction to spin: 0 for CW, 1 for CCW
  // per: period of square wave output in microseconds
  // num_step: number of steps to go 

  if (dir == 0)
  {
    digitalWrite(dirPin, HIGH);
    for (int i = 0; i < num_step; i++)
    {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(round(per/2));
      digitalWrite(stepPin, LOW);
      delayMicroseconds(round(per/2));
    }
  }

  if (dir == 1)
  {
    digitalWrite(dirPin, LOW);
    for (int i = 0; i < num_step; i++)
    {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(round(per/2));
      digitalWrite(stepPin, LOW);
      delayMicroseconds(round(per/2));
    }
  }
  return(0);
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void accelUp(int dir, float init, float fin, int num_step)
{
  // This function is marked as 'Attempt' due to the unsuccessful attempt to add higher or lower order polynomial
  // modelling to the uroflow curves
  // dir: the direction to spin: 0 for CW, 1 for CCW
  // init: initial period of square wave output in microseconds
  // fin: final period of square wave output in microseconds
  // num_step: the number of steps to run the acceleration curve through.
  // order: the polynomial order with which to fill the acceleration curve with
  // float scaled = pow(float(abs(init - fin)/num_step), order);
  if (dir == 0)
  {
    digitalWrite(dirPin, HIGH);
    float addedSpeed = 0.0;
    for (int i = 1; i < num_step; i++)
    {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds((init - addedSpeed)/2);
      digitalWrite(stepPin, LOW);
      delayMicroseconds((init - addedSpeed)/2);
      addedSpeed = addedSpeed + float(abs(init - fin)/num_step);
      // Serial.println(addedSpeed);
    }
  }
  if (dir == 1)
  {
    digitalWrite(dirPin, LOW);
    float addedSpeed = 0.0;
    for (int i = 0; i < num_step; i++)
    {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds((init - addedSpeed)/2);
      digitalWrite(stepPin, LOW);
      delayMicroseconds((init - addedSpeed)/2);
      addedSpeed = addedSpeed + float(abs(init - fin)/num_step);
      //Serial.println(addedSpeed);
    }
  }
  //return(0);
}

void accelUpAttempt(int dir, float init, float fin, int num_step, int order)
{
  // This function is marked as 'Attempt' due to the unsuccessful attempt to add higher or lower order polynomial
  // modelling to the uroflow curves
  // dir: the direction to spin: 0 for CW, 1 for CCW
  // init: initial period of square wave output in microseconds
  // fin: final period of square wave output in microseconds
  // num_step: the number of steps to run the acceleration curve through.
  // order: the polynomial order with which to fill the acceleration curve with
  // float scaled = pow(float(abs(init - fin)/num_step), order);
  if (dir == 0)
  {
    digitalWrite(dirPin, HIGH);
    float addedSpeed = 0.0;
    
    if (order == 1)
    {
      for (int i = 1; i < num_step; i++)
      {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds((init - addedSpeed)/2);
      digitalWrite(stepPin, LOW);
      delayMicroseconds((init - addedSpeed)/2);
      addedSpeed = addedSpeed + pow(float(abs(init - fin)/num_step), order);
      // Serial.println(addedSpeed);
      }
    }

    if (order > 1)
    {
      int count = 1;
      // The plan is to generate an array of size of num_steps where
      // each element 
      float curveArray[num_step];
      for (int i = 1; i < num_step; i++)
      {
        // scaled = pow(scaled, order);        
        curveArray[count - 1] = pow(count, order);
        count = count + 1;
        Serial.println(count);
        Serial.println(curveArray[count - 1]);
      }

      // Map the example order array to the initial and final speed curves
      float mappedArray[num_step];
      for (int i = 0; i < (num_step - 1); i++)
      {
        mappedArray[i] = mapf(curveArray[i], 0, curveArray[num_step - 1], 0, abs(init - fin));
        Serial.println(count);
        Serial.println(curveArray[count - 1]);
      }
      
      for (int i = 0; i < (num_step - 1); i++)
      {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds((init - mappedArray[i])/2);
        digitalWrite(stepPin, LOW);
        delayMicroseconds((init - mappedArray[i])/2);
        // The intuition here is to scale the straight line curve and then
        // scale it down with respect to the original scale of curve.
        // Based on just playing with the numbers, this appears to follow
        // a recurrence of x^order*(1/x^(order - 1))
        /*
        addedSpeed = scaled * (1/pow(count, order));
        Serial.println(addedSpeed);
        // Use counter to scale the increments of each step with respect to
        // the straight line,
        count = count + 1;
        scaled = count*(float(abs(init - fin)/num_step));
        scaled = pow(scaled, order);
        Serial.println(count);
        Serial.println(scaled);
        */
      }
    }
  }

  if (dir == 1)
  {
    digitalWrite(dirPin, LOW);
    float addedSpeed = 0.0;
    for (int i = 0; i < num_step; i++)
    {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds((init - addedSpeed)/2);
      digitalWrite(stepPin, LOW);
      delayMicroseconds((init - addedSpeed)/2);
      addedSpeed = addedSpeed + float(abs(init - fin)/num_step);
      //Serial.println(addedSpeed);
    }
  }
  //return(0);
}

void accelDown(int dir, float init, float fin, int num_step)
{
  // dir: the direction to spin: 0 for CW, 1 for CCW
  // per: period of square wave output in microseconds
  // num_step: number of steps to go 

  if (dir == 0)
  {
    digitalWrite(dirPin, HIGH);
    float addedSpeed = 0.0;
    for (int i = 0; i < num_step; i++)
    {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds((init + addedSpeed)/2);
      digitalWrite(stepPin, LOW);
      delayMicroseconds((init + addedSpeed)/2);
      addedSpeed = addedSpeed + float(abs(init - fin)/num_step);
      //Serial.println(addedSpeed);
    }
  }

  if (dir == 1)
  {
    digitalWrite(dirPin, LOW);
    float addedSpeed = 0.0;
    for (int i = 0; i < num_step; i++)
    {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds((init + addedSpeed)/2);
      digitalWrite(stepPin, LOW);
      delayMicroseconds((init + addedSpeed)/2);
      addedSpeed = addedSpeed + float(abs(init - fin)/num_step);
      //Serial.println(addedSpeed);
    }
  }
  //return(0);
}

void flowAccelUp(int dir, float init, float fin, int vol)
{
  // dir: the direction to spin: 0 for CW, 1 for CCW
  // init: the number of steps per second initially
  // fin: the number of steps per second at the end
  // vol: amount of liquid volume to move through
  // order: order polynomial to fit the accel curve to
  int i_per = ((1/(init/0.0329)) * pow(10, 6));
  int f_per = ((1/(fin/0.0329)) * pow(10, 6));
  int steps = (vol * 30.395);
  if (dir == 0)
  {
    /*
    if (order == 1)
    {
       accelUp(0, i_per, f_per, steps, 1);
    }

    if (order != 1)
    {
      accelUp(0, i_per, f_per, steps, 2);
    }
    */
    accelUp(0, i_per, f_per, steps);
  }

  if (dir == 1)
  {
    /*
    if (order == 1)
    {
       accelUp(1, i_per, f_per, steps, 1);
    }

    if (order != 1)
    {
      accelUp(1, i_per, f_per, steps, 2);
    }
    */

    accelUp(1, i_per, f_per, steps);
  }
}

void flowAccelDown(int dir, float init, float fin, int vol)
{
  // dir: the direction to spin: 0 for CW, 1 for CCW
  // init: the initial flow rate
  // fin: the final flowrate
  // vol: amount of liquid volume to move through
  int i_per = ((1/(init/0.0329)) * pow(10, 6));
  int f_per = ((1/(fin/0.0329)) * pow(10, 6));
  int steps = (vol * 30.395);
  if (dir == 0)
  {
    accelDown(0, i_per, f_per, steps);
  }

  if (dir == 1)
  {
    accelDown(1, i_per, f_per, steps);
  }
}

void moveVol(int dir, float rate, int vol)
{
  int per = ((1/(rate/0.0329)) * pow(10, 6));
  int steps = (vol * 30.395);
  if (dir == 0)
  {
    moveRev(0, per, steps);
  }

  if (dir == 1)
  {
    moveRev(1, per, steps);
  }
}
