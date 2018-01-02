//Rotation Mechanism Program for Arduino Uno
//Made by Dimitrios Alexandridis - 10/8/16

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int com_pin = 3; //Pulse Width Modulation (PWM) pin that sends data to servo
int feed_pin = 4; //Analog pin that receives data from servo

int pos = 1000; //instructed position of servo in μs
int lastpos = 1000; //last position of servo in μs
int feedback = 0; //feedback from the servo to ensure it was operated properly
int UPD = 13.36; //"μs per degree", estimated relation between angle and pulse width

void setup()
{
  Serial.begin(9600);
  myservo.attach(3);
}

void loop() {
  //if command is send from computer, record it.
   if (Serial.available() > 0) {
    lastpos = pos;
    pos = Serial.parseInt();

    //if command is valid, instruct servo to move
    if (pos > 560) {
      slow_move(pos, lastpos);
    }
    else {
      Serial.println("WARNING: Bad angle!");
    }
   }

   //Provide status data to computer
   feedback = analogRead(feed_pin);
   Serial.print("Current position: ");
   Serial.print(pos);
   Serial.println(" μs");
   Serial.print("Feedback: ");
   Serial.println(feedback);
   Serial.print("Expected angle: ");
   Serial.println(angle_calc(pos));
   Serial.println("");
  delay(2000);

}

//Rough estimation of current angle
int angle_calc(int input) {
  if (input > 1440) {
    return ((input - 1440) / UPD + 90);
  }
  else {
    return (90 - (1440 - input) / UPD);
  }
}

//Function that moves the PV cell slowly and steadily
void slow_move(int go_pos, int current_pos) {
  if ((go_pos - current_pos) > 0) {
    while (go_pos > current_pos) {
      current_pos += 10;
      myservo.writeMicroseconds(current_pos);
      delay(30);
    }
  }
  else {
    while (current_pos > go_pos) {
      current_pos -= 10;
      myservo.writeMicroseconds(current_pos);
      delay(40);
    }
  }
}
