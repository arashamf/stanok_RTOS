
// Includes ------------------------------------------------------------------//
#include "drive.h"
#include "gpio.h"
#include "tim.h"
#include "menu.h"
#include "usart.h"
#include "typedef.h"
#include "button.h"
#include "eeprom.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Functions --------------------------------------------------------------------------------------//
static void dr1_rotate_step (uint8_t );
static void dr2_one_full_turn (void);
//static void PWM_turn_drive (void);

// Variables --------------------------------------------------------------------------------------//
__IO float step_unit = ((float)STEP18_IN_SEC/(float)STEP_DIV); //количество секунд в одном микрошаге(1,8гр/100=6480/100=64,8)
PWM_data_t Drive2_PWM = {0};
uint8_t flag_turn_end = 1;

//----------------------------поворот вала двигател€ 1 на один микрошаг----------------------------//
static void dr1_rotate_step (uint8_t micro_step)
{
	for (uint8_t count = 0; count < micro_step; count++) //количество микрошагов (импульсов)
	{
		//	STEP_DRIVE1(ON);
			delay_us (3); //3 мкс
	//		STEP_DRIVE1(OFF);
			delay_us (3); //3 мкс
	}
}

//------------------------------------------------------------------------------------------------//
void dr1_one_full_turn (void) 
{
	DRIVE1_ENABLE(ON);
	DIR_DRIVE1 (FORWARD); //направление вращени€
	delay_us (6);	
	
	for (uint16_t count = 0; count < STEP_IN_TURN; count++)
	{
		dr1_rotate_step (STEP_DIV);
		delay_us (750);
	}
	DRIVE1_ENABLE(OFF);
}

//--------------------------------------------------------------------------------------------------------------//
void init_drive_turn (coil_data_t * HandleCoilData)
{
	Drive2_PWM.turn_number = (HandleCoilData->remains_coil[0]);
	while (HandleCoilData->remains_coil[0] > 0) //пока количество оставшихс€ витков обмотки больше нул€
//	do
	{
		if (flag_turn_end == 1) //если двигатель не активен
		{
			HandleCoilData->remains_coil[0] = (Drive2_PWM.turn_number-1);
			flag_turn_end = 0; //двигатель активен
			dr2_one_full_turn();
			turn_drive_menu (HandleCoilData);
		}
	}	
}


//--------------------------инициализаци€ и запуск вращени€ полного оборота двигател€--------------------------//
static void dr2_one_full_turn (void) 
{
	DRIVE2_ENABLE(ON);
	DIR_DRIVE2 (BACKWARD); //направление вращени€
	delay_us (6);	
	
	Drive2_PWM.value_Compare = 499;
	Drive2_PWM.value_Period = 999;
//	Drive2_PWM.microstep = PULSE_IN_TIM_RCR-1;
	Drive2_PWM.count_start_PWM_TIM = PULSE_IN_TURN/PULSE_IN_TIM_RCR;
	
	#ifdef __USE_DBG
		sprintf (DBG_buffer,  "full_turn_start\r\n");
		DBG_PutString(DBG_buffer);
	#endif
	
	PWM_start (&Drive2_PWM);
}

//------------------------------------------------------------------------------------------------//
uint8_t read_encoder1_rotation (encoder_data_t * HandleEncData) 
{
	int32_t currCounter = 0; //текущее показание энкодера
	int32_t currDelta = 0; //разница между полученным показани€м энкодера и предыдущим
	//int32_t changeAng = 0; //изменение угла уставки (щелчки энкодера * угол хода вала)
	
	currCounter = Get_Encoder1_data (); //получение текущего показани€ энкодера
	HandleEncData->currCounter_SetClick = (32767 - ((currCounter-1) & 0xFFFF))/2; //деление на 2, счЄт щелчков (щелчок = 2 импульса)
	
	if(HandleEncData->currCounter_SetClick != HandleEncData->prevCounter_SetClick) //если показани€ энкодера изменились
	{
		currDelta = (HandleEncData->currCounter_SetClick - HandleEncData->prevCounter_SetClick); //разница между полученным и предыдущим показанием энкодера
		HandleEncData->prevCounter_SetClick = HandleEncData->currCounter_SetClick; //сохранение текущего показани€ энкодера    	
		if (currDelta != 0) //если изменилось положение энкодера
		{		
			HandleEncData->delta += currDelta; //сохранение изменени€ показаний энкодера (кол-во щелчков энкодера)
		/*	if (currDelta > 0) //если повернулс€ по часовой стрелке
			{	
				step = currDelta;
			}
			else
			{
				if (currDelta < 0) //если энкодер повернулс€ против часовой стрелке
				{	
					step = (-1)*currDelta;
				}
			}*/
			return ON;
		}
	}
	return OFF; 
}

//------------------------------------сброс показаний энкодера------------------------------------//
void encoder_reset (encoder_data_t * HandleEncData) 
{
	int32_t encCounter = 0; //переменна€ дл€ хранени€ данных энкодера
	encCounter = LL_TIM_GetCounter(TIM3); //сохранение текущего показани€ энкодера
	HandleEncData->currCounter_SetClick = (32767 - ((encCounter-1) & 0xFFFF))/2; //преобразование полученного показани€ энкодера в формат от -10 до 10
	HandleEncData->prevCounter_SetClick = HandleEncData->currCounter_SetClick; //сохранение преобразованного текущего показани€ энкодера в структуру установки шага поворота	
	HandleEncData->delta = 0; //показани€ от энкодера обнул€ютс€
}

//---------------------------------------------------------------------------------------------------------------//
void TIM_PWM1_Callback(void)
{
	//PWM_turn_drive ();
}

//---------------------------------------------------------------------------------------------------------------//
void TIM_counter_PWM_Callback (void)
{
	#ifdef __USE_DBG
		sprintf (DBG_buffer,  "full_turn_end\r\n");
		DBG_PutString(DBG_buffer);
	#endif
	
	DRIVE2_ENABLE(OFF);
	flag_turn_end = 1; 				//двигатель не активен
	if (Drive2_PWM.turn_number > 0)
	{	Drive2_PWM.turn_number--; }
}

//---------------------------------------------------------------------------------------------------------------//
