unsigned long currentMillis = millis();
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
