
// Includes ------------------------------------------------------------------//
#include "button.h"
#include "typedef.h"
#include "tim.h"
#include "usart.h"

// Exported types -------------------------------------------------------------//
//static struct KEY_MACHINE_t Key_Machine;

// Prototypes --------------------------------------------------------------------------------//

//Private defines ----------------------------------------------------------------------------//


// Private variables -------------------------------------------------------------------------//
uint8_t count_autorepeat = 0; //подсчёт удержания кнопки

//--------------------------------------------------------------------------------------------//
uint16_t scan_keys (void)
{
	//uint8_t key_state = KEY_STATE_OFF; // начальное состояние кнопки - не нажата
	uint16_t key_code = NO_KEY;
	if(LL_GPIO_IsInputPinSet(ENC1_BTN_GPIO_Port, ENC1_BTN_Pin) == OFF)	//если кнопка была нажата - получение кода нажатой кнопки
	{
	//	key_state =  KEY_STATE_ON; //переход в режим нажатия кнопки
		key_code = KEY_ENC_SHORT;
	}
	else
	{
		if (LL_GPIO_IsInputPinSet(CENTER_BTN_GPIO_Port, CENTER_BTN_Pin) == OFF)
		{	key_code = KEY_NULL_SHORT;	}
		else
		{
			if (LL_GPIO_IsInputPinSet(MODE_BTN_GPIO_Port, MODE_BTN_Pin) == OFF)
			{	key_code 	= 	KEY_PEDAL_SHORT;	}
						else
			{
				if (LL_GPIO_IsInputPinSet(MODE_BTN_GPIO_Port, MODE_BTN_Pin) == OFF)
				{
					key_code = KEY_PEDAL_SHORT;
				}
			}
		}
	}
	return key_code;
}

//--------------------------------------------------------------------------------------------//
uint16_t scan_keys_long (uint16_t key_code)
{
	switch (key_code)
	{
		case KEY_PEDAL_SHORT:
			key_code = KEY_PEDAL_LONG;	
			break;
						
		case KEY_ENC_SHORT:
			key_code = KEY_ENC_LONG;	
			break;
						
		case KEY_NULL_SHORT:
			key_code = KEY_NULL_LONG;	
			break;	
						
		default:
			break;	
	}
	return key_code;
}


//-------------------------------------------------------------------------------------------------//
uint8_t scan_buttons_GPIO (uint16_t key_code)
{
	uint8_t pin_status = OFF;
	
	switch (key_code)
	{						
		case KEY_NULL_SHORT:
			if ((LL_GPIO_IsInputPinSet(CENTER_BTN_GPIO_Port, CENTER_BTN_Pin))	== ON)
				pin_status = ON;
			break;
						
		case KEY_NULL_LONG:
			if ((LL_GPIO_IsInputPinSet(CENTER_BTN_GPIO_Port, CENTER_BTN_Pin))	== ON)
				pin_status = ON;
			break;	
						
		case KEY_ENC_SHORT:
			if ((LL_GPIO_IsInputPinSet(ENC1_BTN_GPIO_Port, ENC1_BTN_Pin))	== ON)
				pin_status = ON;
			break;
						
		case KEY_ENC_LONG:
			if ((LL_GPIO_IsInputPinSet(ENC1_BTN_GPIO_Port, ENC1_BTN_Pin))	== ON)
				pin_status = ON;
			break;
		
		case KEY_PEDAL_SHORT:
			if ((LL_GPIO_IsInputPinSet(MODE_BTN_GPIO_Port, MODE_BTN_Pin))	== ON)
				pin_status = ON;
			break;
						
		case KEY_PEDAL_LONG:
			if ((LL_GPIO_IsInputPinSet(MODE_BTN_GPIO_Port, MODE_BTN_Pin))	== ON)
				pin_status = ON;
			break;
		
		default:
			break;	
	}	
	return pin_status;			
}

