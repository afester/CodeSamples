//Technical support:goodtft@163.com
// Breakout/Arduino UNO pin usage:
// LCD Data Bit :   7   6   5   4   3   2   1   0
// Uno dig. pin :   7   6   5   4   3   2   9   8
// Uno port/pin : PD7 PD6 PD5 PD4 PD3 PD2 PB1 PB0
// Mega dig. pin:  29  28  27  26  25  24  23  22
//              : PH4  PH3 PE3 PG5 PE5 PE4 PH6 PH5
#include <avr/io.h>
#include <util/delay.h>


//#define LCD_RD   50	// PB3
//#define LCD_WR   51	// PB2
//#define LCD_RS   52	// PB1
//#define LCD_CS   53     // PB0
//#define LCD_REST A0	// PF0

#define RD_MASK  0b00001000
#define RD_LOW PORTB &= ~RD_MASK
#define RD_HIGH PORTB |=  RD_MASK

#define WR_MASK  0b00000100
#define WR_LOW PORTB &= ~WR_MASK
#define WR_HIGH PORTB |=  WR_MASK

#define RS_MASK  0b00000010
#define RS_LOW PORTB &= ~RS_MASK
#define RS_HIGH PORTB |=  RS_MASK

#define CS_MASK  0b00000001
#define CS_LOW PORTB &= ~CS_MASK
#define CS_HIGH PORTB |=  CS_MASK

#define RST_MASK 0b00000001
#define RST_LOW PORTF &= ~RST_MASK
#define RST_HIGH PORTF |=  RST_MASK


void Lcd_Writ_Bus(unsigned char d)
{
  PORTL = d;
  //  PORTH &= ~(0x78);
  //  PORTH |= ((d&0xC0) >> 3) | ((d&0x3) << 5);
  //  PORTE &= ~(0x38);
  //  PORTE |= ((d & 0xC) << 2) | ((d & 0x20) >> 2);
  //  PORTG &= ~(0x20);
  //  PORTG |= (d & 0x10) << 1;  
  PORTB &= ~WR_MASK;	// 0
  PORTB |=  WR_MASK;	// 1
}


void Lcd_Write_Com(unsigned char VH)  
{   
  PORTB &= ~RS_MASK;//RS=0
  Lcd_Writ_Bus(VH);
}

void Lcd_Write_Data(unsigned char VH)
{
  PORTB |= RS_MASK;//LCD_RS=1;
  Lcd_Writ_Bus(VH);
}

void Lcd_Write_Com_Data(unsigned char com,unsigned char dat)
{
  Lcd_Write_Com(com);
  Lcd_Write_Data(dat);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
  Lcd_Write_Com(0x2a);
  Lcd_Write_Data(x1>>8);
  Lcd_Write_Data(x1);
  Lcd_Write_Data(x2>>8);
  Lcd_Write_Data(x2);

  Lcd_Write_Com(0x2b);
  Lcd_Write_Data(y1>>8);
  Lcd_Write_Data(y1);
  Lcd_Write_Data(y2>>8);
  Lcd_Write_Data(y2);

  Lcd_Write_Com(0x2c); 							 
}

void Lcd_Init(void)
{
//  digitalWrite(LCD_REST,HIGH);
  RST_HIGH;
  _delay_ms(5); 
//  digitalWrite(LCD_REST,LOW);
  RST_LOW;
  _delay_ms(15);
//  digitalWrite(LCD_REST,HIGH);
  RST_HIGH;
  _delay_ms(15);

//  digitalWrite(LCD_CS,HIGH);
  CS_HIGH;
//  digitalWrite(LCD_WR,HIGH);
  WR_HIGH;

//  digitalWrite(LCD_CS,LOW);  //CS
  CS_LOW;
  Lcd_Write_Com(0x11);
  _delay_ms(20);
  Lcd_Write_Com(0xD0);
  Lcd_Write_Data(0x07);
  Lcd_Write_Data(0x42);
  Lcd_Write_Data(0x18);

  Lcd_Write_Com(0xD1);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x07);//07
  Lcd_Write_Data(0x10);

  Lcd_Write_Com(0xD2);
  Lcd_Write_Data(0x01);
  Lcd_Write_Data(0x02);

  Lcd_Write_Com(0xC0);
  Lcd_Write_Data(0x10);
  Lcd_Write_Data(0x3B);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x02);
  Lcd_Write_Data(0x11);

  Lcd_Write_Com(0xC5);
  Lcd_Write_Data(0x03);
  Lcd_Write_Com(0x36);
  Lcd_Write_Data(0x0A);

  Lcd_Write_Com(0x3A);
  Lcd_Write_Data(0x55);

  Lcd_Write_Com(0x2A);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x01);
  Lcd_Write_Data(0x3F);

  Lcd_Write_Com(0x2B);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x01);
  Lcd_Write_Data(0xE0);
  _delay_ms(120);
  Lcd_Write_Com(0x29);
  Lcd_Write_Com(0x002c); 
}

void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)                   
{	
  unsigned int i,j;
  Lcd_Write_Com(0x02c); //write_memory_start
//  digitalWrite(LCD_RS,HIGH);
  RS_HIGH;
//  digitalWrite(LCD_CS,LOW);
  CS_LOW;
  l=l+x;
  Address_set(x,y,l,y);
  j=l*2;
  for(i=1;i<=j;i++)
  {
    Lcd_Write_Data(c);
  }
//  digitalWrite(LCD_CS,HIGH);   
  CS_HIGH;
}

void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)                   
{	
  unsigned int i,j;
  Lcd_Write_Com(0x02c); //write_memory_start
//  digitalWrite(LCD_RS,HIGH);
  RS_HIGH;
//  digitalWrite(LCD_CS,LOW);
  CS_LOW;
  l=l+y;
  Address_set(x,y,x,l);
  j=l*2;
  for(i=1;i<=j;i++)
  { 
    Lcd_Write_Data(c);
  }
//  digitalWrite(LCD_CS,HIGH);   
  CS_HIGH;
}

void Rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
{
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}

void Rectf(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
{
  unsigned int i;
  for(i=0;i<h;i++)
  {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}
//int RGB(int r,int g,int b)
//{
//  return r << 16 | g << 8 | b;
//}
void LCD_Clear(unsigned int j)                   
{	
  unsigned int i,m;
  Address_set(0,0,320,480);
  //Lcd_Write_Com(0x02c); //write_memory_start
  //digitalWrite(LCD_RS,HIGH);
//  digitalWrite(LCD_CS,LOW);
 CS_LOW;

  for(i=0;i<320;i++)
    for(m=0;m<480;m++)
    {
      Lcd_Write_Data(j>>8);
      Lcd_Write_Data(j);
    }
//  digitalWrite(LCD_CS,HIGH);   
  CS_HIGH;
}

void setup()
{/*
  for(int p=0;p<10;p++)
 {
 pinMode(p,OUTPUT);
 }*/
  //  DDRH |= 0x78;
  //  DDRE |= 0x38;
  //  DDRG |= 0x20;
  DDRL = 0b11111111;
  DDRB = 0b00001111;
  DDRF = 0b00000001;

//  pinMode(LCD_RD,OUTPUT);
//  pinMode(LCD_WR,OUTPUT);
//  pinMode(LCD_RS,OUTPUT);
//  pinMode(LCD_CS,OUTPUT);
//  pinMode(LCD_REST,OUTPUT);

  RD_HIGH;
  WR_HIGH;
  RS_HIGH;
  CS_HIGH;
  RST_HIGH;
//  digitalWrite(LCD_RD, HIGH);
//  digitalWrite(LCD_WR, HIGH);
//  digitalWrite(LCD_RS, HIGH);
//  digitalWrite(LCD_CS, HIGH);
//  digitalWrite(LCD_REST, HIGH);
  Lcd_Init();
  //LCD_Clear(0xf800);
  
//  pinMode(14, OUTPUT);
//  digitalWrite(14, LOW);
}

void loop()
{  
  LCD_Clear(0xf800);
  LCD_Clear(0x07E0);
  LCD_Clear(0x001F);

   for(int i=0;i<1000;i++)
   {
   Rect(rand()%300,rand()%300,rand()%300,rand()%300,rand()%65535); // rectangle at x, y, with, hight, color
   }

//    LCD_Clear(0xf800);
}

int main() {
   setup();
   while(1) loop();
}
