int backCount = 0;

void straight()
//drive straight
{
  backCount = 0;
  analogWrite(BackRight, LOW);
  analogWrite(BackLeft, LOW);
  analogWrite(FrontRight, v);
  analogWrite(FrontLeft, v);
}

void left()
//drive left
{
  backCount = 0;
  analogWrite(FrontLeft, LOW);
  analogWrite(BackRight, LOW);
  analogWrite(FrontRight, v);
  analogWrite(BackLeft, v/1.2);
}

void right()
//drive right
{
  backCount = 0;
  analogWrite(FrontRight, LOW);
  analogWrite(BackLeft, LOW);
  analogWrite(FrontLeft, v);
  analogWrite(BackRight, v/1.2);
}

void back()
//drive back
{
  backCount++;
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

