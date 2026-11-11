
//******************************************************************************/
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"

u8 UseHorizontalMode = 0;
u16 LcdWidth = LCD_W;
u16 LcdHeight = LCD_H;

int main(void)
{
	u8 i,j;
	float t=0;
	delay_init();
	LED_Init();//LED初始化
	LCD_Init();//LCD初始化
	LED=0;
	while(1)
	{
		LCD_Fill(0,0,LcdWidth,LcdHeight,RED);
		delay_ms(200);
		LCD_Fill(0,0,LcdWidth,LcdHeight,GREEN);
		delay_ms(200);
		LCD_Fill(0,0,LcdWidth,LcdHeight,BLUE);
		delay_ms(200);
		LCD_Fill(0,0,LcdWidth,LcdHeight,WHITE);
		delay_ms(200);
		switch(UseHorizontalMode)
		{
			default:
			case 0:
				LCD_ShowChinese(10,5,"广东深圳",RED,WHITE,16,0);
				LCD_ShowString(4,32,"LCD_W:",RED,WHITE,16,0);
				LCD_ShowIntNum(48,32,LCD_W,3,RED,WHITE,16);
				LCD_ShowString(4,62,"LCD_H:",RED,WHITE,16,0);
				LCD_ShowIntNum(52,62,LCD_H,3,RED,WHITE,16);
				LCD_ShowFloatNum1(24,92,t,3,RED,WHITE,16);
				t+=0.11;
				LCD_ShowPicture(20,112,40,40,gImage_1);
				UseHorizontalMode = 1;
				LcdWidth = LCD_W;//172;//
				LcdHeight = LCD_H;//320;//
				break;
			case 1:
				LCD_ShowChinese(10,5,"广东深圳",RED,WHITE,16,0);
				LCD_ShowString(4,32,"LCD_W:",RED,WHITE,16,0);
				LCD_ShowIntNum(48,32,LCD_W,3,RED,WHITE,16);
				LCD_ShowString(4,62,"LCD_H:",RED,WHITE,16,0);
				LCD_ShowIntNum(52,62,LCD_H,3,RED,WHITE,16);
				LCD_ShowFloatNum1(24,92,t,3,RED,WHITE,16);
				t+=0.11;
				LCD_ShowPicture(20,112,40,40,gImage_1);
				UseHorizontalMode = 2;
				LcdWidth = LCD_H;//320;//
				LcdHeight = LCD_W;//172;//
				break;
			case 2:
				LCD_ShowChinese(40,2,"广东省深圳",RED,WHITE,16,0);
				LCD_ShowString(10,20,"LCD_W:",RED,WHITE,16,0);
				LCD_ShowIntNum(58,20,LCD_W,3,RED,WHITE,16);
				LCD_ShowString(10,40,"LCD_H:",RED,WHITE,16,0);
				LCD_ShowIntNum(58,40,LCD_H,3,RED,WHITE,16);
				LCD_ShowFloatNum1(48,60,t,3,RED,WHITE,16);
				t+=0.11;
				LCD_ShowPicture(100,26,40,40,gImage_1);
				UseHorizontalMode = 3;
				LcdWidth = LCD_H;//320;//
				LcdHeight = LCD_W;//172;//
				break;
			case 3:
				LCD_ShowChinese(40,2,"广东省深圳",RED,WHITE,16,0);
				LCD_ShowString(10,20,"LCD_W:",RED,WHITE,16,0);
				LCD_ShowIntNum(58,20,LCD_W,3,RED,WHITE,16);
				LCD_ShowString(10,40,"LCD_H:",RED,WHITE,16,0);
				LCD_ShowIntNum(58,40,LCD_H,3,RED,WHITE,16);
				LCD_ShowFloatNum1(48,60,t,3,RED,WHITE,16);
				t+=0.11;
				LCD_ShowPicture(100,26,40,40,gImage_1);
				UseHorizontalMode = 0;
				LcdWidth = LCD_W;//172;//
				LcdHeight = LCD_H;//320;//
				break;
		}
		
		LCD_WR_REG(0x36); 
		if(UseHorizontalMode==0)LCD_WR_DATA8(0x08);
		else if(UseHorizontalMode==1)LCD_WR_DATA8(0xC8);
		else if(UseHorizontalMode==2)LCD_WR_DATA8(0x78);
		else LCD_WR_DATA8(0xA8);
		delay_ms(500);
		delay_ms(200);
		
	}
}

