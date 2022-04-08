int backCount = 0;

void straight()
// drive straight
{
  backCount = 0;
  analogWrite(BackRight, 0);
  analogWrite(BackLeft, 0);
  analogWrite(FrontRight, v);
  analogWrite(FrontLeft, v);
}

void left()
// drive left
{
  backCount = 0;
  analogWrite(FrontLeft, 0);
  analogWrite(BackRight, 0);
  analogWrite(FrontRight, v);
  analogWrite(BackLeft, v / 1.2);
}

void right()
// drive right
{
  backCount = 0;
  analogWrite(FrontRight, 0);
  analogWrite(BackLeft, 0);
  analogWrite(FrontLeft, v);
  analogWrite(BackRight, v / 1.2);
}

void back()
// drive back
{
  backCount++;
  analogWrite(FrontLeft, 0);
  analogWrite(FrontRight, 0);
  analogWrite(BackLeft, v);
  analogWrite(BackRight, v);
}

void off()
// turn off
{
  analogWrite(FrontLeft, 0);
  analogWrite(FrontRight, 0);
  analogWrite(BackLeft, 0);
  analogWrite(BackRight, 0);
}
