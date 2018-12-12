//Class for the ball in the game
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


class Ball {
  public:
    int x = 0;
    int y = 0;
    int r = 1; //radius
    float x_speed = 4;//ball starts moving to the right
    float y_speed = -4;//ball starts moving up, stop unfair loses

    Ball(int ball_x, int ball_y, int ball_r){ //constructor, pass in starting x and y values and the radius
        x = ball_x;
        y = ball_y;
        r = ball_r;
    }//end constructor

    void update(Adafruit_SSD1306 &d) { //displays the ball
        d.fillCircle(x, y, r, WHITE);
        x = x + x_speed;
        y = y + y_speed;
        x_speed+=0.1;
        y_speed+=0.2;
    }//end of func

    void wallHit(Adafruit_SSD1306 &d){ //check if the ball hits one of the three walls, if it does, reverse the speed
        if (x <=0 || x>=d.width()){ //check if ball is going off left or right wall
            x_speed = x_speed * -1; //reverse the speed
        }//end if


        if (y <=0){ //check if the ball is hitting the roof
            y_speed = y_speed * -1; //reverse the speed
        }
    }//end of func

    void paddleHit(int &px, int &py, int &pw){ //paddle x, paddle y, paddle width
           if (x>=px && x<=px+pw/2 && y>=py-this->r){//hitting the left side of the paddle
            
            if(x_speed>0){//x_speed positive, moving to the right, hitting the left side causes it to go back the way it came
              y_speed =y_speed*-1;
              x_speed=x_speed*-1;
            } else{ //if its moving to the left, hitting the left side causes it to only change y_speed
              y_speed =y_speed*-1;
            }
                 
           }else if (x>=px+pw/2 && x<=px+pw && y>=py-this->r
           
           ){//hitting the rightside of the paddle
              
              if(x_speed>0){//x_speed positive, moving to the right, hitting the right side causes it to only change y_speed
              y_speed =y_speed*-1;
            } else{ //if its moving to the left, hitting the right side causes it to go back the way it came
              y_speed =y_speed*-1;
              x_speed=x_speed*-1;
            }
            
           }
    }//end of func
    
    bool deathCheck(){
        if (y>64+this->r){
            return true;
        } else{
            return false;
        }
    }

    void respawn(int &lives, Adafruit_SSD1306 &d){
        y = d.height()/2;
        x = (rand()%128);
        y_speed = -4;
        x_speed = 4;
        lives--;
    }
};//end class
