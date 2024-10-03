
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

// Includes -------------------------------------------------------------------------//
#include "main.h"

// Exported types ------------------------------------------------------------------//

//----------------------------------------------------------------------------------//
typedef struct 
{
	int32_t 	prevCounter_SetClick; 			//���������� ��������� ��������
	int32_t 	currCounter_SetClick; 			//������� ��������� ��������
	int32_t delta;
} encoder_data_t;

//----------------------------------------------------------------------------------//
#define 	MAX_NUMBER_COIL 					3

#pragma anon_unions(1)
#pragma pack(1) 
typedef struct 
{
	union
	{
		struct 
		{
			uint8_t number_coil;
			uint16_t set_coil[MAX_NUMBER_COIL]; 		
			uint16_t remains_coil[MAX_NUMBER_COIL]; 		
		};
		uint8_t coil_buffer[MAX_NUMBER_COIL*MAX_NUMBER_COIL*2+1];
	};
	struct coil_data_t *next;
} coil_data_t;

//----------------------------------------------------------------------------------//
typedef union
{
	struct
	{
		uint8_t tool_mode			: 1;
		uint8_t left_flag			: 1;
		uint8_t right_flag		: 1;
		uint8_t raw						:	5;
	};
	uint8_t flag;
}STATUS_FLAG_t;

//----------------------------------------------------------------------------------//
typedef struct 
{
	uint16_t value_Compare; //�������� ���������
	uint16_t value_Period; //������ ��������
	uint16_t turn_number; //���������� ����������� ��������
	uint8_t microstep; //�������� �������� RCR
	uint8_t count_start_PWM_TIM; //���������� �������� � ����� �������
} PWM_data_t;

#pragma pack()
#pragma anon_unions()

//��� ������� ������----------------------------------------------------------------//
typedef enum 
{
	NO_KEY 						= 	0x00,			//������ �� ������	
	KEY_PEDAL_SHORT 	= 	0x01,			//�������� ������� ����������� ������
	KEY_PEDAL_LONG 	= 		0x02,			//������� ������� ����������� ������
	KEY_ENC_SHORT			= 	0x03,			//�������� ������� ������ ��������
	KEY_ENC_LONG			=		0x04,			//������� ������� ������ ��������
	KEY_NULL_SHORT		=		0x05,			//�������� ������� ������ ������
	KEY_NULL_LONG			=		0x06,			//�������� ������� ������ ������
} KEY_CODE_t; 					

//-----------------------------------------------------------------------------------//
typedef enum 
{
	KEY_STATE_OFF 				= 0	,				//����� - ������ �� ������
	KEY_STATE_ON							,				//����� - ������ ������
	KEY_STATE_BOUNCE					, 			//����� -  ������� ������
	KEY_STATE_AUTOREPEAT			,	 			//����� - ����� �������� (�����������) ������� ������
	KEY_STATE_WAIT_TURNOFF
} KEY_STATE_t; 									//������ ������������ ����������

//-----------------------------------------------------------------------------------//
typedef struct 
{
	KEY_CODE_t 		key_code;
	KEY_STATE_t 	key_state;
	uint8_t count_autorepeat; //������� ������ ��������� ������
} KEY_MACHINE_t;

// Defines ----------------------------------------------------------------------//
#define 	I2C_REQUEST_WRITE                       0x00
#define 	I2C_REQUEST_READ                        0x01
#define 	SLAVE_OWN_ADDRESS                       0xA0 //������ EEPROM ����������

#define   REDUCER 						40 			//�������� ���������
#define 	PULSE_IN_TURN				1600 		//���������� ���������� � ����� ������ ������� (360 ��) � ������ �������� ��������
#define 	PULSE_IN_TIM_RCR		100
#define 	STEP_IN_TURN				200		 //���������� ����� (1,8��) � ����� ������ ������� (360 ��)
#define 	STEP_DIV 						(PULSE_IN_TURN/STEP_IN_TURN)		//���������� ���������� (8) � ����� ���� ��������� (1,8��)
#define 	STEP_TURNOVER				(PULSE_IN_TURN*REDUCER) //���������� ���������� � ����� ������ ������� (360 ��) � ������ �������� �������� � ��������� 

#define 	STEP18_IN_SEC					6480 							//���������� ������ � ����� ���� ��������� (1,8��)
#define 	CIRCLE_IN_SEC					(STEP18_IN_SEC*CIRCLE_IN_STEP)	//���������� ������ � ����� ������ ������� ��������� (360 ��)
#define 	SECOND_PER_MINUTE 		60
#define 	SECOND_PER_DEGREE 		3600

#define 	ON 												1
#define 	OFF 											0
#define 	FORWARD 									1
#define 	BACKWARD 									0
#define 	DISP_CLEAR 								1
#define 	DISP_NOT_CLEAR 						0
//#define 	EEPROM_NUMBER_BYTES 			4
#define 	CPU_CLOCK_VALUE						(72000000UL)		//������� ����������� 
#define 	TICKS_PER_SECOND					1000 
// Private variables -----------------------------------------------------------//

#ifdef __cplusplus
}
#endif

#endif 

