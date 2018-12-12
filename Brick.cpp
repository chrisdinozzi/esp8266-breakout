#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
class Brick {
  public:
    int x=0;
    int y=0;
    int w=14;
    int h=6;
    bool alive = true;

    void setXY(int x,int y){
      this->x = x;
      this->y = y;
    }

    void display(Adafruit_SSD1306 &d){
      d.fillRect(x,y,w,h,WHITE);//draw a rectangle 
    }
};//end class
