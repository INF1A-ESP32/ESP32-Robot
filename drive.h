int count = 0;

void straight()
{
  analogWrite(BackRight, LOW);
  analogWrite(BackLeft, LOW);
  analogWrite(FrontRight, v);
  analogWrite(FrontLeft, v);
}

void left()
//drive left
{
  analogWrite(FrontLeft, LOW);
  analogWrite(BackRight, LOW);
  analogWrite(FrontRight, v);
  analogWrite(BackLeft, vTurn);
}

void right()
//drive right
{
  analogWrite(FrontRight, LOW);
  analogWrite(BackLeft, LOW);
  analogWrite(FrontLeft, v);
  analogWrite(BackRight, vTurn);
}

void back()
//drive back
{
  analogWrite(FrontLeft, LOW);
  analogWrite(FrontRight, LOW);
  analogWrite(BackLeft, v);
  analogWrite(BackRight, v);
}

void off()
//turn off
{
  analogWrite(FrontLeft, LOW);
  analogWrite(FrontRight, LOW);
  analogWrite(BackLeft, LOW);
  analogWrite(BackRight, LOW);
}


// *************************
//         Maze game
// *************************


unsigned long currentMillis = millis();
int previousMillis = 0;
int timer = 500;

void Stop()
{
    int wait = 1000;
    if (currentMillis - previousMillis <= wait){
      off();
    }
    previousMillis = currentMillis;
}        
        
void goLeft()
{
    int goLeft = 500;
    currentMillis = previousMillis;
    if (previousMillis <= goLeft){
      left();
    }
}

void goRight()
{
    int goRight = 500;
    currentMillis = previousMillis;
    if (previousMillis <= goRight){
      right();
    }
}
