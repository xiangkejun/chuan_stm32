#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "timer.h"
 
 //USART_REC_LEN = 14
union floatData  //ʵ��char������֮���ת��
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
	u8 lcd_id[12];			//���LCD ID�ַ���
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();
	TIM3_PWM_Init(19999,71); 		// 50hz 20ms 
	TIM2_Int_Init(19999,71);  // ����������
	TIM2_IRQHandler();
	 
	TIM3->CCR2 = 1500;  // PB5
	TIM3->CCR3 = 1500;   // PB0
	 
	POINT_COLOR=RED;
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//��LCD ID��ӡ��lcd_id���顣 
	 
		POINT_COLOR=RED;	  
	LCD_ShowString(30,40,210,24,24,"Elite STM32F1 ^_^"); 
	LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,lcd_id);		//��ʾLCD ID	      					 
	LCD_ShowString(30,130,200,12,12,"2015/1/14");	  
 while(1) 
	{		
	  //��������
   // go(1,1);		 // 1618  1900
		
		// ����
		if(USART_RX_STA&0x8000)
		{					   
		//	len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
		//	printf("\r\n�����͵���ϢΪ:\r\n\r\n");
			for(t=0;t<4;t++)  //�����ٶ���Ϣ
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
			
			  if(tuolian_state_rec.f == 6666)  //��������
				{
					tuolian_state = 1;
				}
		//	printf("linear_vel_rec=%f\n",linear_vel_rec.f / 1000);  // ��С1000��
		//	printf("angular_vel_rec=%f\n",angular_vel_rec.f / 1000);
			
		//	printf("\r\n\r\n");//���뻻��
			USART_RX_STA=0;
		}else
		{
			times++;
			
			  //go(0,0); //
//			if(times%5000==0)
//			{
//				printf("\r\n��ӢSTM32������ ����ʵ��\r\n");
//				printf("����ԭ��@ALIENTEK\r\n\r\n");
//			}
		//	if(times%200==0)printf("����������,�Իس�������\n");  
			if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
			delay_ms(10);   
		}
		
		// ˢ����ʾ
		LCD_ShowString(30,200,200,16,16,"linear_vel_rec=                ");
		LCD_ShowString(30,300,200,16,16,"angular_vel_rec=               ");
		LCD_ShowString(30,400,200,16,16,"tuolian_state_rec=             ");

	} 
}
 