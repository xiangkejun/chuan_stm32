#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "timer.h"
 
 //USART_REC_LEN = 14
union floatData  //实现char与数组之间的转换
{
	float f;
	unsigned char data[4];
}linear_vel_rec,angular_vel_rec,tuolian_state_rec;

float chuan_vx ,chuan_vw;
u8 tuolian_state = 0;


 int main(void)
 {	 
	 u8 t;  
	u16 len;
	u16 times=0;	 
	u8 lcd_id[12];			//存放LCD ID字符串
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			     //LED端口初始化
	LCD_Init();
	TIM3_PWM_Init(19999,71); 		// 50hz 20ms 
	TIM2_Int_Init(19999,71);  // 处理拖链用
	TIM2_IRQHandler();
	 
	TIM3->CCR2 = 1500;  // PB5
	TIM3->CCR3 = 1500;   // PB0
	 
	POINT_COLOR=RED;
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。 
	 
		POINT_COLOR=RED;	  
	LCD_ShowString(30,40,210,24,24,"Elite STM32F1 ^_^"); 
	LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,lcd_id);		//显示LCD ID	      					 
	LCD_ShowString(30,130,200,12,12,"2015/1/14");	  
 while(1) 
	{		
	  //函数测试
   // go(1,1);		 // 1618  1900
		
		// 接收
		if(USART_RX_STA&0x8000)
		{					   
		//	len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
		//	printf("\r\n您发送的消息为:\r\n\r\n");
			for(t=0;t<4;t++)  //接收速度信息
			{
				linear_vel_rec.data[t] = USART_RX_BUF[t];
				angular_vel_rec.data[t] = USART_RX_BUF[t+4];
				tuolian_state_rec.data[t] = USART_RX_BUF[t+8];
			}
				LCD_ShowxNum_int(180,200,linear_vel_rec.f,5,16,1);
				LCD_ShowxNum_int(180,300,angular_vel_rec.f,5,16,1);
				LCD_ShowxNum_int(180,400,tuolian_state_rec.f,5,16,1);
     
				chuan_vx = linear_vel_rec.f/1000;
				chuan_vw = angular_vel_rec.f/1000;
			  
			  //go(vx,vw);
			
			  if(tuolian_state_rec.f == 6666)  //启动拖链
				{
					tuolian_state = 1;
				}
		//	printf("linear_vel_rec=%f\n",linear_vel_rec.f / 1000);  // 缩小1000倍
		//	printf("angular_vel_rec=%f\n",angular_vel_rec.f / 1000);
			
		//	printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
		}else
		{
			times++;
			
			  //go(0,0); //
//			if(times%5000==0)
//			{
//				printf("\r\n精英STM32开发板 串口实验\r\n");
//				printf("正点原子@ALIENTEK\r\n\r\n");
//			}
		//	if(times%200==0)printf("请输入数据,以回车键结束\n");  
			if(times%30==0)LED0=!LED0;//闪烁LED,提示系统正在运行.
			delay_ms(10);   
		}
		
		// 刷新显示
		LCD_ShowString(30,200,200,16,16,"linear_vel_rec=                ");
		LCD_ShowString(30,300,200,16,16,"angular_vel_rec=               ");
		LCD_ShowString(30,400,200,16,16,"tuolian_state_rec=             ");

	} 
}
 