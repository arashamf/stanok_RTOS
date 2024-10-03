/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "typedef.h"
#include "menu.h"
#include "button.h"
#include "usart.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/

/* USER CODE BEGIN PTD */
typedef StaticQueue_t osStaticMessageQDef_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
coil_data_t Coil1 = {0};
KEY_MACHINE_t btn_info = {0};

//------------------------------------------------------------------------------------------//
osTimerId_t ButtonTimerHandle;
const osTimerAttr_t ButtonTimer_attributes = {
  .name = "ButtonTimer"
};

//------------------------------------------------------------------------------------------//
osThreadId TaskMenuDisplay_Handle;
const osThreadAttr_t TaskMenuDisplay_attributes = 
{
  .name = "MenuDisplayTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

//------------------------------------------------------------------------------------------//
osThreadId TaskSelectMode_Handle;
const osThreadAttr_t TaskSelectMode_attributes = 
{
  .name = "SelectModeTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

//------------------------------------------------------------------------------------------//
osThreadId TaskScanKeys_Handle;
const osThreadAttr_t TaskScanKeys_attributes = 
{
  .name = "ScanKeysTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

//------------------------------------------------------------------------------------------//
osMessageQueueId_t BtnQueueHandle;
uint8_t BtnQueueBuffer[ 2 * sizeof( uint16_t ) ];
osStaticMessageQDef_t BtnQueueControlBlock;
const osMessageQueueAttr_t BtnQueue_attributes = 
{
  .name = "BtnQueue",
  .cb_mem = &BtnQueueControlBlock,
  .cb_size = sizeof(BtnQueueControlBlock),
  .mq_mem = &BtnQueueBuffer,
  .mq_size = sizeof(BtnQueueBuffer)
};

//------------------------------------------------------------------------------------------//
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void Select_Mode (void * argument);
void Show_Display (void * argument);
void Scan_Keys (void * argument);
void ButtonTimerCallback(void *argument);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	ButtonTimerHandle = osTimerNew(ButtonTimerCallback, osTimerOnce, NULL, &ButtonTimer_attributes);
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of BtnQueue */

  /* USER CODE BEGIN RTOS_QUEUES */
	BtnQueueHandle = osMessageQueueNew (2, sizeof(uint16_t), &BtnQueue_attributes);
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	TaskMenuDisplay_Handle = osThreadNew(Show_Display, NULL, &TaskMenuDisplay_attributes); 	
	TaskSelectMode_Handle = osThreadNew(Select_Mode, NULL, &TaskSelectMode_attributes);
	TaskScanKeys_Handle = osThreadNew(Scan_Keys, NULL, &TaskScanKeys_attributes);
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of EventBtnTimer */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
//------------------------------------------------------------------------------------------//
void Show_Display (void * argument)
{
	for(;;)
	{
	//	main_menu ();	
		osDelay(1000);
	}
}

//------------------------------------------------------------------------------------------//
void Select_Mode  (void * argument)
{
	uint8_t key_code = NO_KEY;
	for(;;)
	{
		if (osMessageQueueGet(BtnQueueHandle, &key_code, NULL, 10) == osOK)
		{
			#ifdef __USE_DBG
			sprintf (DBG_buffer,  "key_code=%u\r\n",key_code);
			DBG_PutString(DBG_buffer);
			#endif	
			
			switch (key_code) //обработка кода нажатой кнопки
			{	
				case KEY_PEDAL_SHORT:
					dbg_button_menu (KEY_PEDAL_SHORT);
					break;
				
				case KEY_ENC_SHORT:
				//	setup_menu (&Pos_Enc1, &Coil1);
					dbg_button_menu (KEY_ENC_SHORT);
					break;
				
				case KEY_PEDAL_LONG:
				//	init_drive_turn (&Coil1);
					dbg_button_menu (KEY_PEDAL_LONG);
					break;
				
				case KEY_ENC_LONG:
				//	setup_menu (&Pos_Enc1, &Coil1);
					dbg_button_menu (	KEY_ENC_LONG);
					break;
				
				default:
					break;
			}
			key_code = NO_KEY;
			osThreadYield ();
		}
		osDelay(10);
	}
}
//------------------------------------------------------------------------------------------//
void Scan_Keys (void * argument)
{
	KEY_MACHINE_t btn_info = {0};

	for(;;)
	{
		if(btn_info.key_state == KEY_STATE_OFF) //если кнопка была отпущена - ожидание нажатия
		{
			if ((btn_info.key_code = scan_keys ()) != NO_KEY)
			{	
				btn_info.key_state =  KEY_STATE_ON; //переход в режим нажатия кнопки
			}
		}
		
		if (btn_info.key_state == KEY_STATE_ON)  //режим нажатия кнопки
		{
			btn_info.key_state = KEY_STATE_BOUNCE;
			osDelay (KEY_BOUNCE_TIME);
		}
		
		if(btn_info.key_state == KEY_STATE_BOUNCE) //режим окончания дребезга
		{
			if(scan_buttons_GPIO(btn_info.key_code) == ON)	 // если кнопка отпущена (нажатие менее 50 мс это дребезг)
			{
				btn_info.key_state = KEY_STATE_OFF; //переход в начальное состояние ожидания нажатия кнопки
			}	
			else //если кнопка продолжает удерживаться
			{	
				btn_info.key_state = KEY_STATE_AUTOREPEAT;
				btn_info.count_autorepeat = 0; //обнуление счётчика автоповтора
				osDelay (KEY_AUTOREPEAT_TIME);
			}
		}
		
		if (btn_info.key_state == KEY_STATE_AUTOREPEAT) //если активен режим автоповтора
		{
			if(scan_buttons_GPIO(btn_info.key_code) == ON) // если кнопка была отпущена (короткое нажатие кнопки < 150 мс)
			{
				btn_info.key_state = KEY_STATE_OFF; //переход в начальное состояние ожидания нажатия кнопки
				osMessageQueuePut ( BtnQueueHandle, &btn_info.key_code, 0, 20);
			}
			else
			{
				if (btn_info.count_autorepeat >= COUNT_REPEAT_BUTTON)	//если кнопка удерживалась более 650 мс
				{		
					btn_info.key_code = scan_keys_long (btn_info.key_code);					
					osMessageQueuePut ( BtnQueueHandle, &btn_info.key_code, 0, 20);
					btn_info.key_state = KEY_STATE_WAIT_TURNOFF; //стадия ожидания отпускания кнопки
					osDelay (KEY_AUTOREPEAT_TIME);
				}
				else
				{
					{
						btn_info.count_autorepeat++;
						osDelay (KEY_AUTOREPEAT_TIME);
					}
				}
			}
		}
		
		if (btn_info.key_state == KEY_STATE_WAIT_TURNOFF) //ожидание отпускания кнопки
		{	
			if(scan_buttons_GPIO(btn_info.key_code) == ON)	 // если кнопка была отпущена (короткое нажатие кнопки < 150 мс)
			{				
				btn_info.key_code = NO_KEY;
				btn_info.key_state = KEY_STATE_OFF; //переход в начальное состояние ожидания нажатия кнопки
			}
			else
			{
				//if (osTimerIsRunning(ButtonTimerHandle) == OFF) //если таймер не запущен
				{
				//	osTimerStart (ButtonTimerHandle, KEY_AUTOREPEAT_TIME); //установка таймера ожидания отключения кнопки
					osDelay (KEY_AUTOREPEAT_TIME);
				}
			}
		}
		osDelay (10);
	}
}	

//------------------------------------------------------------------------------------------//
void ButtonTimerCallback (void *argument)
{
	//Task_action('c');
	if (btn_info.key_state == KEY_STATE_ON)
	{	btn_info.key_state = KEY_STATE_BOUNCE;	}
	else
	{
		if (btn_info.key_state == KEY_STATE_BOUNCE)
		{
			btn_info.key_state = KEY_STATE_AUTOREPEAT; //переход в режим автоповтора 
			btn_info.count_autorepeat++;
		}
		else
		{
			if (btn_info.key_state == KEY_STATE_AUTOREPEAT)
			{	btn_info.count_autorepeat++; }
		}
	}
}
/* USER CODE END Application */

