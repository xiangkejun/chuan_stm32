#include "timer.h"
#include "led.h"
#include "usart.h"

//Õ®”√∂® ±∆˜÷–∂œ≥ı ºªØ
//’‚¿Ô ±÷”—°‘ÒŒ™APB1µƒ2±∂£¨∂¯APB1Œ™36M
//arr£∫◊‘∂Ø÷ÿ◊∞÷µ°£
//psc£∫ ±÷”‘§∑÷∆µ ˝
//’‚¿Ô π”√µƒ «∂® ±∆˜3!

void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // ±÷” πƒ‹

	TIM_TimeBaseStructure.TIM_Period = arr; //…Ë÷√‘⁄œ¬“ª∏ˆ∏¸–¬ ¬º˛◊∞»ÎªÓ∂Øµƒ◊‘∂Ø÷ÿ◊∞‘ÿºƒ¥Ê∆˜÷‹∆⁄µƒ÷µ	 º∆ ˝µΩ5000Œ™500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //…Ë÷√”√¿¥◊˜Œ™TIMx ±÷”∆µ¬ ≥˝ ˝µƒ‘§∑÷∆µ÷µ  10Khzµƒº∆ ˝∆µ¬   
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //…Ë÷√ ±÷”∑÷∏Ó:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMœÚ…œº∆ ˝ƒ£ Ω
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //∏˘æ›TIM_TimeBaseInitStruct÷–÷∏∂®µƒ≤Œ ˝≥ı ºªØTIMxµƒ ±º‰ª˘ ˝µ•Œª
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); // πƒ‹÷∏∂®µƒTIM3÷–∂œ,‘ –Ì∏¸–¬÷–∂œ

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3÷–∂œ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //œ»’º”≈œ»º∂0º∂
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //¥””≈œ»º∂3º∂
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÕ®µ¿±ª πƒ‹
	NVIC_Init(&NVIC_InitStructure);  //∏˘æ›NVIC_InitStruct÷–÷∏∂®µƒ≤Œ ˝≥ı ºªØÕ‚…ËNVICºƒ¥Ê∆˜

	TIM_Cmd(TIM3, ENABLE);  // πƒ‹TIMxÕ‚…Ë
							 
}
//∂® ±∆˜3÷–∂œ∑˛ŒÒ≥Ã–Ú
void TIM3_IRQHandler(void)   //TIM3÷–∂œ
{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //ºÏ≤È÷∏∂®µƒTIM÷–∂œ∑¢…˙”Î∑Ò:TIM ÷–∂œ‘¥ 
//		{
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //«Â≥˝TIMxµƒ÷–∂œ¥˝¥¶¿ÌŒª:TIM ÷–∂œ‘¥ 
//		LED1=!LED1;
//		}
}

//TIM3 PWM≤ø∑÷≥ı ºªØ 
//PWM ‰≥ˆ≥ı ºªØ
//arr£∫◊‘∂Ø÷ÿ◊∞÷µ
//psc£∫ ±÷”‘§∑÷∆µ ˝
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	// πƒ‹∂® ±∆˜3 ±÷”
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  // πƒ‹GPIOÕ‚…Ë∫ÕAFIO∏¥”√π¶ƒ‹ƒ£øÈ ±÷”
   	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3≤ø∑÷÷ÿ”≥…‰  TIM3_CH2->PB5    
 
   //…Ë÷√∏√“˝Ω≈Œ™∏¥”√ ‰≥ˆπ¶ƒ‹, ‰≥ˆTIM3 CH2µƒPWM¬ˆ≥Â≤®–Œ	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_1|GPIO_Pin_0; //TIM_CH2,TIM_CH3,TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //∏¥”√Õ∆ÕÏ ‰≥ˆ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//≥ı ºªØGPIO
 
   //≥ı ºªØTIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //…Ë÷√‘⁄œ¬“ª∏ˆ∏¸–¬ ¬º˛◊∞»ÎªÓ∂Øµƒ◊‘∂Ø÷ÿ◊∞‘ÿºƒ¥Ê∆˜÷‹∆⁄µƒ÷µ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //…Ë÷√”√¿¥◊˜Œ™TIMx ±÷”∆µ¬ ≥˝ ˝µƒ‘§∑÷∆µ÷µ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //…Ë÷√ ±÷”∑÷∏Ó:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMœÚ…œº∆ ˝ƒ£ Ω
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //∏˘æ›TIM_TimeBaseInitStruct÷–÷∏∂®µƒ≤Œ ˝≥ı ºªØTIMxµƒ ±º‰ª˘ ˝µ•Œª
	
	//≥ı ºªØTIM3 Channel2 PWMƒ£ Ω	 
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //—°‘Ò∂® ±∆˜ƒ£ Ω:TIM¬ˆ≥ÂøÌ∂»µ˜÷∆ƒ£ Ω2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //±»Ωœ ‰≥ˆ πƒ‹
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // ‰≥ˆº´–‘:TIM ‰≥ˆ±»Ωœº´–‘∏ﬂ
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //∏˘æ›T÷∏∂®µƒ≤Œ ˝≥ı ºªØÕ‚…ËTIM3 OC2

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //—°‘Ò∂® ±∆˜ƒ£ Ω:TIM¬ˆ≥ÂøÌ∂»µ˜÷∆ƒ£ Ω2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //±»Ωœ ‰≥ˆ πƒ‹
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // ‰≥ˆº´–‘:TIM ‰≥ˆ±»Ωœº´–‘∏ﬂ
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //∏˘æ›T÷∏∂®µƒ≤Œ ˝≥ı ºªØÕ‚…ËTIM3 OC3

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //—°‘Ò∂® ±∆˜ƒ£ Ω:TIM¬ˆ≥ÂøÌ∂»µ˜÷∆ƒ£ Ω2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //±»Ωœ ‰≥ˆ πƒ‹
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // ‰≥ˆº´–‘:TIM ‰≥ˆ±»Ωœº´–‘∏ﬂ
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //∏˘æ›T÷∏∂®µƒ≤Œ ˝≥ı ºªØÕ‚…ËTIM3 OC4
	
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  // πƒ‹TIM3‘⁄CCR2…œµƒ‘§◊∞‘ÿºƒ¥Ê∆˜
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  // πƒ‹TIM3‘⁄CCR3…œµƒ‘§◊∞‘ÿºƒ¥Ê∆˜
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  // πƒ‹TIM3‘⁄CCR4…œµƒ‘§◊∞‘ÿºƒ¥Ê∆˜

	TIM_Cmd(TIM3, ENABLE);  // πƒ‹TIM3 
}

void TIM2_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //Í±?ÛÍ1?¸
	
	//?®Í±?˜TIM33?Í??Ø
	TIM_TimeBaseStructure.TIM_Period = arr; //ÈË???˙??Ú????¸D?Í??t◊∞Ë????Øµ?◊??Ø??◊∞????'??˜?¸?˙µ??µ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //ÈË??Û?‡'◊˜?aTIMxÍ±?Û?µ?Í3yÍyµ??§∑??µ?µ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ÈË??Í±?Û∑???:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM?ÚÈ???Íy??Í?
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //?˘?Y???®µ?2?Íy3?Í??ØTIMxµ?Í±???˘Íyµ???
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //Í1?¸???®µ?TIM3?D??,?ÍDÌ?¸D??D??

	//?D??Û??Ë??NVICÈË??
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3?D??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //?Ë??Û??Ë??0??
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //'ÛÛ??Ë??3??
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÌ®µ‡±?Í1?¸
	NVIC_Init(&NVIC_InitStructure);  //3?Í??ØNVIC??'??˜

	TIM_Cmd(TIM2, ENABLE);  //Í1?¸TIMx					 
}


float length = 0.8; // ◊Û”“µÁª˙µƒæ‡¿Î 0.8m
float motor_left_vel,motor_right_vel;  //
int pwm_count_left=0 ,pwm_count_right=0;
void go(float vx, float vw)
{
	 motor_left_vel = vx - 0.5*vw*length;  // ºŸ…Ë-1~1   -0.2~0.2
	 motor_right_vel = vx + 0.5*vw*length; //-1~1
	 //  ◊ÛµÁª˙
	 if(motor_left_vel >0) 
	 {
		 pwm_count_left = (1900-1525)*motor_left_vel*5+1525;
		 if(pwm_count_left >= 1700) pwm_count_left=1900;
	 }else if(motor_left_vel < 0)
	 {
		 pwm_count_left = (1475-1100)*motor_left_vel*5 +1475;
		 if(pwm_count_left <=1100) pwm_count_left = 1100;
	 }else if(motor_left_vel == 0)
	 {
		 pwm_count_left = 1500;
	 }
	 
	 //”“µÁª˙
	 	 if(motor_right_vel >0) 
	 {
		 pwm_count_right = (1900-1525)*motor_right_vel*5+1525;
		 if(pwm_count_right >= 1900) pwm_count_right=1900;
	 }else if(motor_right_vel < 0)
	 {
		 pwm_count_right = (1475-1100)*motor_right_vel*5 +1475;
		 if(pwm_count_right <=1100) pwm_count_right = 1100;
	 }else if(motor_right_vel == 0)
	 {
		 pwm_count_right = 1500;
	 }
	 
	 
		TIM3->CCR2 = pwm_count_left;  // PB5
		TIM3->CCR3 = pwm_count_right;   // PB0	
}


u16 i;
extern u8 tuolian_state;
extern float chuan_vx ,chuan_vw;
void TIM2_IRQHandler(void)   //TIM3?D??
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //?Ï2ÈTIM3?¸D??D??∑?È˙Û?∑?
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //??3yTIMx?¸D??D??±Í?? 
		
		go(chuan_vx,chuan_vw);
		
		if(tuolian_state == 1)
		{
			GPIO_ResetBits(GPIOE,GPIO_Pin_5); //∆Ù∂ØÕœ¡¥
			i++;  //20msº”“ª¥Œ
			if(i == 500)  //10s
			{
						// πÿ±’Õœ¡¥
				GPIO_SetBits(GPIOE,GPIO_Pin_5);
				tuolian_state =0;  //∏¥Œª
				i  = 0;
			}
		}
	}
}

