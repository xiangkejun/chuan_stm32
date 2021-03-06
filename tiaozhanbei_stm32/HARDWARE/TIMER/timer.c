#include "timer.h"
#include "led.h"
#include "usart.h"

//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!

void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
//		{
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
//		LED1=!LED1;
//		}
}

//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
   	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_1|GPIO_Pin_0; //TIM_CH2,TIM_CH3,TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC3

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC4
	
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR3上的预装载寄存器
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR4上的预装载寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIM3 
}

void TIM2_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //瓯?箨1?�
	
	//?�?鱐IM33?�??�
	TIM_TimeBaseStructure.TIM_Period = arr; //殍???�??�????麯?�??t装�?????�??�??装????'??�?�???�	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //殍??�?�'作?aTIMx瓯?�?�?�3y陏�??し??�?�
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //殍??瓯?蠓???:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM?蜷???陏??�?
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //?�?Y???ǖ?2?陏3?�??疶IMx�?瓯???y�???
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //�1?�???ǖ?TIM3?D??,?闐�?麯??D??

	//?D??�??�??NVIC殍??
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3?D??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //?�??�??�??0??
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //'篌??�??3??
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ悫掂�?�1?�
	NVIC_Init(&NVIC_InitStructure);  //3?�??疦VIC??'??�

	TIM_Cmd(TIM2, ENABLE);  //�1?黅IMx					 
}


float length = 0.8; // 左右电机的距离 0.8m
float motor_left_vel,motor_right_vel;  //
int pwm_count_left=0 ,pwm_count_right=0;
void go(float vx, float vw)
{
	 motor_left_vel = vx - 0.5*vw*length;  // 假设-1~1   -0.2~0.2
	 motor_right_vel = vx + 0.5*vw*length; //-1~1
	 //  左电机
	 if(motor_left_vel >0) 
	 {
		 pwm_count_left = (1900-1525)*motor_left_vel*5+1525;
		 if(pwm_count_left >= 1900) pwm_count_left=1900;
	 }else if(motor_left_vel < 0)
	 {
		 pwm_count_left = (1475-1100)*motor_left_vel*5 +1475;
		 if(pwm_count_left <=1100) pwm_count_left = 1100;
	 }else if(motor_left_vel == 0)
	 {
		 pwm_count_left = 1500;
	 }
	 
	 //右电机
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
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //?�2門IM3?麯??D??�?辁�?�?
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //??3yTIMx?麯??D??标?? 
		
		go(chuan_vx,chuan_vw);
		
		if(tuolian_state == 1)
		{
			GPIO_ResetBits(GPIOE,GPIO_Pin_5); //启动拖链
			i++;  //20ms加一次
			if(i == 500)  //10s
			{
						// 关闭拖链
				GPIO_SetBits(GPIOE,GPIO_Pin_5);
				tuolian_state =0;  //复位
				i  = 0;
			}
		}
	}
}

