void playMaze(){
  // Set the default speed of each wheel
  int speedLeft = 180;
  int speedRight = 180;
  
  // Set the default values for the prediction
  char* predictL = "UNKNOWN";
  char* predictR = "UNKNOWN";

  // Read the IR Sensors
  int sLeft = analogRead(39);
  int sRight = analogRead(34);

  if (sLeft < 60) {
    predictL = "white";
  } else if (sLeft < 250) {
    predictL = "gray";
  }  else if (sLeft >= 800){
    predictL = "black";
  } else {
    predictL = "?";
  }

  if (sRight < 60) {
    predictR = "white";
  }else if (sRight < 250) {
    predictR = "gray";
  }  else if (sRight >= 800){
    predictR = "black";
  } else {
    predictR = "?";
  }
  
  display.clearDisplay();  // clears display
  display.invertDisplay(false);
  display.setTextSize(2);  // sets text size
  display.setTextColor(WHITE);
  display.setCursor(0, 6);  // sets cursor
  display.print("R: ");
  display.print(predictR);  // displays text
  display.setCursor(0, 26);
  display.print("L: ");
  display.print(predictL);
  display.setCursor(0, 46);
  display.print("Sl :");
  display.println(sLeft);
  display.display();

  if(predictR == "gray" && predictL == "white"){
    analogWrite(FrontRight, 180);
    analogWrite(FrontLeft, 150);
    delay(20);
 
  }else if(predictR == "white" && predictL == "gray"){
    analogWrite(FrontRight, 150);
    analogWrite(FrontLeft, 180);
    delay(20);

  }else if(predictR == "black" || predictL == "black"){
     analogWrite(FrontRight, 0);
      analogWrite(FrontLeft, 0);
      analogWrite(BackLeft, 0);
      delay(100);
    if(predictR== "black"){
      analogWrite(FrontRight, 180);
      analogWrite(FrontLeft, 180);
      delay(400);
      analogWrite(FrontRight, 160);
      analogWrite(FrontLeft, 0);
      analogWrite(BackLeft, 180);
      delay(380);
      analogWrite(FrontRight, 0);
      analogWrite(FrontLeft, 0);
      analogWrite(BackLeft, 0);
      delay(500);
    } else if(predictL== "black"){
      analogWrite(FrontRight, 180);
      analogWrite(FrontLeft, 180);
      delay(400);
      analogWrite(FrontRight, 0);
      analogWrite(FrontLeft, 160);
      analogWrite(BackRight, 180);
      delay(390);
      analogWrite(FrontRight, 0);
      analogWrite(FrontLeft, 0);
      analogWrite(BackRight, 0);
      delay(500);
    }else{
      analogWrite(FrontRight, 0);
      analogWrite(FrontLeft, 0);
      analogWrite(BackLeft, 0);
      analogWrite(BackRight, 0);
      delay(5000);
    }
  }else{
    analogWrite(FrontRight, speedRight);
    analogWrite(FrontLeft, speedLeft);
    delay(100);
    analogWrite(FrontRight, 0);
    analogWrite(FrontLeft, 0);
    analogWrite(BackLeft, 0);
    delay(100); 
  }
  if (predictR == "white" || predictL == "white") {
    delay(100);
    analogWrite(FrontRight, 0);
    analogWrite(FrontLeft, 0);
    analogWrite(BackLeft, 0);
    delay(100);    
  }
}
