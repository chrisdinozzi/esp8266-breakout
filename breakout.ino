#include <Streaming.h>
#include <iomanip>
#include <SPI.h>
#include <Wire.h>
#include "Ball.cpp"
#include "Brick.cpp"
using namespace std;

//INSTRUCTIONS TO ATTATCH KNOB
//left most pin -> 3v3
//middle pin -> A0
//right most pin -> GND
//if the knob seems to be the wrong way around it's because the 3v3 and GND pins are the wrong way around.

//DISPLAY
#define OLED_RESET -1
#define OLED_SCREEN_I2C_ADDRESS 0x3C
Adafruit_SSD1306 display(OLED_RESET);

//EXPANSION BOARD
  //STB->D5
  //CLK->D6
  //DIO->D7
#include <TM1638.h>
#include <TM16XX.h>
#define TM1638_STB D5
#define TM1638_CLK D6
#define TM1638_DIO D7
TM1638 module(TM1638_DIO, TM1638_CLK, TM1638_STB);
  ////////////////////////
 //declareing variables//
////////////////////////
int sensorValue = 0;
  //paddle
int paddle_x;
int paddle_y = 62;
int paddle_w = 32;//width
int paddle_h = 2;//height
int paddle_r = 1;//rounding

  //game
int lives = 3;
int score = 0;
  //ball
int ball_x = paddle_x+(paddle_w/2);
int ball_r = 2;
int ball_y = paddle_y - paddle_h - ball_r;
Ball ball(ball_x, ball_y, ball_r);

//brick
Brick b[24];

  ////////////////////////////////////
 ///////////////setup////////////////
////////////////////////////////////
void setup()
{
Serial.begin(115200);
//DISPLAY
display.begin(SSD1306_SWITCHCAPVCC, OLED_SCREEN_I2C_ADDRESS);

//set up display
display.clearDisplay();
display.setCursor(20,0);
display.setTextSize(2); 
display.setTextColor(WHITE);
display << "WEMOS \nBREAKOUT";
display.display();
delay(3000);//wait for 3 seconds
display.clearDisplay();

//setup score display
module.setDisplayToString("SCORE");
module.setupDisplay(true,7);
updateScoreDisplay(score);


//prototype
bool brickHit(Brick);
void gameOver(Adafruit_SSD1306);
void updateScoreDisplay(int);
void updateLivesDisplay();
void playAgain();
void setupWall();
void displayWall();

//calling initial setup functions
setupWall(); //creates wall array
displayWall(); //displays wall array

}//end setup function

  ////////////////////////////////////
 /////////////main loop//////////////
////////////////////////////////////
void loop(){
/////////////
//gameloop//
///////////
while((lives>0 && score<24)){
  srand(score); //seed the randomiser with the score
  display.clearDisplay();//empties the display, ready to be drawn on again with updated data
  sensorValue=analogRead(A0);//get the value from the knob
  paddle_x = sensorValue/8; //knob works from 0-1024. dividing by 8 brings it down to 0-128, the same as the length of the display. unfortunaly leaves a deadzone as the high end of the knob
  
  //display lives(LEDs)
  if (lives==3){
    module.setLEDs(0x7);
  } else if(lives==2){
    module.setLEDs(0x3);
  } else if(lives==1){
    module.setLEDs(0x1);
  }
  
  if (paddle_x>display.width()-paddle_w){//to stop the paddle sneaking off the right side of the screen
    paddle_x=display.width()-paddle_w;
  }
  
  //check brick hit
  for (int i=0;i<24;i++){
    if(brickHit(b[i]) == true){
      updateScoreDisplay(score);
      displayWall();
    }//end if
  }//end for
  
  display.fillRoundRect(paddle_x,paddle_y,paddle_w,paddle_h,paddle_r,WHITE); //display the paddle
  ball.update(display); //update ball
  ball.wallHit(display); //check for wall collisions
  ball.paddleHit(paddle_x, paddle_y, paddle_w); //check for paddle collisions
  
  if (ball.deathCheck()==true){ //
    ball.respawn(lives, display);
  }
  delay(1);//stop it crashing
  display.display();//refresh display
 }//end while
//end game loop

while(lives==0 || score==24){
gameOver();
}

}//end loop


  ///////////////
 ///FUNCTIONS///
///////////////
void setupWall(){
  int xInc = 0; //move each brick along by 16px, leaving a 2px gap between each.
  //row one
  for (int i=0;i<8;i++){
    b[i].setXY(xInc,1);
    b[i].alive=true;
    xInc+=16;
  }
  //row two
  xInc=0;
  for (int i=8;i<16;i++){
    b[i].setXY(xInc,8);
    b[i].alive=true;
    xInc+=16;
  }
  //row three
  xInc=0;
  for (int i=16;i<24;i++){
    b[i].setXY(xInc,15);
    b[i].alive=true;
    xInc+=16;
  }
}//end function

void displayWall(){
  for (int i=0;i<24;i++){ 
    if(b[i].alive==true){
      b[i].display(display);
    }//end if
  }//end for
}//end function

bool brickHit (Brick &b){
  if ((ball.x >= b.x && ball.x <= b.x+b.w) &&(ball.y>=b.y && ball.y<=b.y+b.h)){
    b.alive = false;
    if ((ball.x == b.x || ball.x == b.x+b.w) && (ball.y <=b.y+b.h && ball.y>=b.y)){
      ball.x_speed = ball.x_speed * -1;
    } else{
      ball.y_speed *=-1;
    }
    b.x = 1000;//effectivly dumps the brick into the middle of no where. Perhaps the worst piece of code I've written in my life but I refuse to change it because it works and I love it.
    b.y = 1000;
    score++;
     return true;
  }//end if
}//end function

void gameOver() {
    display.clearDisplay();
    delay(1000);
    display.setCursor(36,0);
    display.setTextSize(2);
    display << "GAME";
    display << '\n';
    display.setCursor(36,16);
    display << "OVER";
    display.display();
    delay(1000);
    Serial << "Score: "<<score<<endl;
    if (score==24){ //winner
      display.clearDisplay();
      display.setCursor(30,32);
      display << "WINNER";
      display.display();
      Serial << "WINNER"<<endl;
      delay(1000);
    }
    playAgain();
    
}

void updateScoreDisplay(int score){
  module.setDisplayDigit(score,7,false);
  if(score>=10&&score<=19){
    module.setDisplayDigit(1,6,false);
    module.setDisplayDigit(score-10,7,false);
  }
  if (score>=20&&score<=29){
    module.setDisplayDigit(2,6,false);
    module.setDisplayDigit(score-20,7,false);
  }
}

void updateLivesDisplay(){
  if (lives==3){
    module.setLEDs(0x7);
  } else if(lives==2){
    module.setLEDs(0x3);
  } else if(lives==1){
    module.setLEDs(0x1);
  }else if(lives==0){
    module.setLEDs(0x0);
  }
}

void playAgain(){
  byte select;
 do{
  display.clearDisplay();
  display.setCursor(34,28);
  display.setTextSize(1);
  display << "Play Again?";
  display.setCursor(22, 56);
  display << "YES";
  display.setCursor(100, 56);
  display << "NO";
  int arrow=analogRead(A0)/1024;//makes the input only 1 or 0.
  select = 2;//no selection made yet
  if (arrow==0){ 
    display.fillTriangle(16,57,16,61,20,59,WHITE);//draws triangle beside "yes"
    if(module.getButtons()){//check for button press
      select=0;//wants to play again
    }
  }else if(arrow==1){
    display.fillTriangle(94,57,94,61,98,59,WHITE);//draw triangle besi "no"
    if(module.getButtons()){//check for button press
      select=1;//does not want to play again
    }
  }
  display.display();
  delay(1);
  
}while(select == 2);

 if(select==0){//play again
  resetGame();
 }else if (select==1){//doesnt want to play again
  while(1){
    display.fillScreen(WHITE);
    delay(1);
  }
 }
}//end function

void resetGame(){
  module.clearDisplay();
  lives =3;
  score=0;
  ball.y_speed = -4; //send the ball upwards
  ball.x_speed = 4;
  setupWall();
}//end function
