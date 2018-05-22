#define PORTRAIT  0
#define LANDSCAPE 1
#define TOUCH_ORIENTATION  PORTRAIT
#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;       // hard-wired for UNO shields anyway.
#include <TouchScreen.h>
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define BUTTON_X 30
#define BUTTON_Y 123
#define BUTTON_W 51
#define BUTTON_H 50
#define BUTTON_SPACING_X 07
#define BUTTON_SPACING_Y 07
#define BUTTON_TEXTSIZE 2
char buttonlabels[16][5] = { "1", "2", "3","+", "4", "5", "6","-", "7", "8", "9","/","=","0","DEL","x"};
uint16_t buttoncolors[16] = { BLUE,BLUE,BLUE,GREEN,BLUE,BLUE,BLUE,GREEN,BLUE,BLUE,BLUE,GREEN,GREEN,BLUE,RED,BLUE };
Adafruit_GFX_Button buttons[16];
int col=0,row=0,z=0;
#define TEXT_X 5
#define TEXT_Y 25
#define TEXT_W (tft.width()-10)
#define TEXT_H 70
#define TEXT_TSIZE 3
#define TEXT_TCOLOR BLACK
char q;
const int XP=7,XM=A1,YP=A2,YM=6; //240x320 ID=0x7789
const int TS_LEFT=337,TS_RT=653,TS_TOP=797,TS_BOT=495;
// Calibrate values
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 800
#define TS_MAXY 905
uint32_t num1,num2;
char operator_type=-1;
#define TEXT_LEN 10
char textfield[TEXT_LEN+1] = "";
uint8_t textfield_i=0;
uint32_t result=0;
#define MINPRESSURE 400
#define MAXPRESSURE 800
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 350);
TSPoint tp;
boolean n=false;
char test=0;
uint16_t identifier;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  tft.reset();
  identifier = tft.readID();
  tft.begin(identifier);
  tft.fillScreen(WHITE);
  tft.setRotation(0);
  start_screen();
 
}
void loop() 
{
 get_button();
    if(q> 0 && q<=16)
        {
          q--;  
    if(get_number(q)!=-1)
      {
        test++;
        if(test==1)
        {
          num1=get_number(q);
        pressed_button();
        animate();
         display_text();
        }
        else if(test==2 && operator_type!=-1){
        num2=get_number(q);
        pressed_button();
        animate(); 
         display_text();
        test=0;
        }
      }
    else
    if(get_opertator(q)!=-1)
    {
      operator_type=get_opertator(q);
      pressed_button();
      animate();
       display_text();
         
    }else 
    if(q==12)
    {display_text();
      switch (operator_type)
      {
        case 0:result=num1+num2;break;
        case 1:result=num1-num2;break;
        case 2:result=num1/num2;break;
        case 3:result=num1*num2;break;
      }
      result_display();
      operator_type=-1;
      test=0;
      num1=0;
     num2=0;
     char textfield[TEXT_LEN+1] = "";
     textfield_i=0;
    n=true;
    }
    else 
    if(q==14 && test!=0)
    {
       clr_button();
    test--;
    animate();
     display_text();
    }
    q=0;
   }
}

void get_button()
{for (uint8_t b=0; b<16; b++) 
    { 
      if (buttons[b].contains(tp.x, tp.y)) 
      {
        buttons[b].press(true);  // tell the button it is pressed
       q=b+1;
      } 
      else 
       {
        buttons[b].press(false);  // tell the button it is NOT pressed
       }
    }
    }
void animate()
  {
      if (buttons[q].justPressed()) 
      {
        buttons[q].drawButton(true);  // draw invert!
        delay(10);
        buttons[q].drawButton(); 
      } 
  }
void clr_button()
{
          
    textfield[textfield_i] = 0;
    if (textfield > 0) 
    {
      textfield_i--;
      textfield[textfield_i] = ' ';
    }
}


int get_number(unsigned int num)
{
  switch (num)
  {
    case 0x00: return 1; break;
    case 0x01: return 2; break;
    case 0x02: return 3; break;
    case 0x04: return 4; break;
    case 0x05: return 5; break;
    case 0x06: return 6; break;
    case 0x08: return 7; break;
    case 0x09: return 8; break;
    case 0x0A: return 9; break;
    case 0x0D: return 0; break;
    default: return -1;break;
  }
}
int get_opertator(const char num)
{
  switch (num)
  {
    case 0x03: return 0;break;
    case 0x07: return 1;break;
    case 0x0B: return 2;break;
    case 0x0F: return 3;break;
    default: return -1;break;
  }
}

void display_text()
{
  tft.setCursor(TEXT_X + 2, TEXT_Y+10);
  tft.setTextColor(TEXT_TCOLOR, WHITE);
  tft.setTextSize(TEXT_TSIZE);
  tft.print(textfield);
}
void  clear_result()
{
   tft.setCursor(TEXT_X + 2, TEXT_Y+40);
  tft.setTextColor(TEXT_TCOLOR, WHITE);
  tft.setTextSize(TEXT_TSIZE);
  tft.print("     ");
  tft.setCursor(TEXT_X + 2, TEXT_Y+10);
  tft.print("     ");
   
  n=false;
}
void result_display()
{
  tft.setCursor(TEXT_X + 2, TEXT_Y+40);
  tft.setTextColor(TEXT_TCOLOR, WHITE);
  tft.setTextSize(TEXT_TSIZE);
  tft.print('=');
  tft.print(result);
}
void pressed_button()
{
    textfield[textfield_i] = buttonlabels[q][0];    
    textfield_i++;    
    textfield[textfield_i] = 0; // zero terminate
}

void button_ini()
{
  for(int i=0;i<4;i++)
 { for(int j=0;j<4;j++)
   creat_button(i,j);  
 }
}
//function to creat buttons
void creat_button(int x,int y)
{row=x;col=y;

  buttons[z].initButton(&tft, BUTTON_X+col*(BUTTON_W+BUTTON_SPACING_X), 
                 BUTTON_Y+row*(BUTTON_H+BUTTON_SPACING_Y),    // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, BLACK, buttoncolors[z],BLACK,
                  buttonlabels[z], BUTTON_TEXTSIZE); 
  buttons[z].drawButton();
  z++;
}

void start_screen()
{
  
  tft.fillScreen(WHITE);
  tft.setCursor ((tft.width()/2)-60, 5);
  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.println("CALCULATOR");
  tft.drawRoundRect(TEXT_X, TEXT_Y, TEXT_W, TEXT_H, 10, BLACK);
    button_ini();
    waitfortouch();
}

void drawBorder () 
{// Draw a border
  uint16_t width = tft.width() - 1;
  uint16_t height = tft.height() - 1;
  uint8_t border = 10;
  tft.fillScreen(RED);
  tft.fillRect(border, border, (width - border * 2), (height - border * 2), WHITE); 
}
TSPoint waitfortouch() 
{  
  do {
    tp= ts.getPoint(); 
    pinMode(XM, OUTPUT); //Pins configures again for TFT control
    pinMode(YP, OUTPUT);
     } while((tp.z < MINPRESSURE )|| (tp.z > MAXPRESSURE));

tp.x = map(tp.x, TS_MINX, TS_MAXX, 0, tft.width());
tp.y = map(tp.y, TS_MINY, TS_MAXY, 0, tft.height());
  return tp;
  
}
