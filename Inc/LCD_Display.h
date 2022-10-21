/**
	*	@aut : bhtyr.byr
	*	@dat : 14.02.2022
	* 
	*	1206A (12x6 Led Dislpay) Librariy for STM32F4 Disc.
	*		
	* Pin Connect Diagram:
	*
	*		-----------------------------------------------------
	*		|LCD_Pin	|	STM32F4 Disc 	|	LCD_Pin	|	STM32F4 Disc 	|
	*		|---------------------------------------------------|
	*		|		VSS 	->			GND			|		D2  	-> 		-NC-			|
	*		|		VDD 	->			5V			|		D3  	-> 		-NC-			|
	*		|		V0  	->		GND&E14		|		D4  	-> 		E7				|
	*		|		RS  	->			E8			|		D5  	-> 		E9				|
	*		|		WR  	->			E10			|		D6  	-> 		E11				|
	*		|		EN  	->			E12			|		D7  	-> 		E13				|
	*		|		D0  	->			-NC-		|		AN  	-> 		5V				|
	*		|		D1  	->			-NC-		|		KT  	-> 		GND				|
	*		-----------------------------------------------------
	*
	*/

#ifndef __LCD_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

/* INCLUDE BEGIN -------------------------------------------------------*/
#include "stm32f4xx_hal.h"
/* INCLUDE END ---------------------------------------------------------*/

/* DEFINE MACRO BEGIN --------------------------------------------------*/

// Display Command -------------------------------------------------------
#define		LCD_RTRN_HOME						0x03
#define		LCD_CMD_CLEAR						0x01

// Port Select -----------------------------------------------------------
#define		LCD_PORT								GPIOE
#define 	__LCD_CLK_EN						__HAL_RCC_GPIOE_CLK_ENABLE()

// Control Pins ----------------------------------------------------------
#define 	LCD_PIN_RS							GPIO_PIN_8
#define 	LCD_PIN_WR							GPIO_PIN_10
#define 	LCD_PIN_EN							GPIO_PIN_12
#define 	LCD_PIN_KN							GPIO_PIN_14

// Data Pins -------------------------------------------------------------
#define 	LCD_PIN_D4							GPIO_PIN_7
#define 	LCD_PIN_D5							GPIO_PIN_9
#define 	LCD_PIN_D6							GPIO_PIN_11
#define 	LCD_PIN_D7							GPIO_PIN_13
#define 	LCD_DISPLAY_DTA_PINS		PIN_D4 | PIN_D5 | PIN_D6 | PIN_D7
#define 	__LCD_ALL_DTA_PIN_RST		HAL_GPIO_WritePin(LED_PORT, LCD_DISPLAY_DTA_PINS, GPIO_PIN_RESET)

#define		LCD_ALL_PINS						LCD_PIN_RS | LCD_PIN_WR | LCD_PIN_EN | LCD_PIN_KN | LCD_PIN_D4 | LCD_PIN_D5 | LCD_PIN_D6 | LCD_PIN_D7

/* DEFINE MACRO END ----------------------------------------------------*/

/* DEFINE VARIABLE BEGIN -----------------------------------------------*/

typedef enum{
	WRITE													= GPIO_PIN_RESET,
	READ													= GPIO_PIN_SET	
}WR_State;				
				
typedef enum{				
	REGISTER_INSTRUCTION					= GPIO_PIN_RESET,
	REGISTER_DATA 								= GPIO_PIN_SET
}RS_State;				
				
typedef enum{				
	CMMON_SEND_COMMAND 						= 0,
	CMMON_BUSY_FLAG,				
	CMMON_WRITE_DATA,				
	CMMON_READ_DATA				
}CMMON_State;				
				
typedef enum{				
	BIT_4													= 0x00,
	BIT_8													= 0x10
}Commun_Data_Length;				
				
typedef enum{				
	ROW_1													= 0x00,
	ROW_2													= 0x08
}Display_Row_Count;				
				
typedef enum{				
	LOW_QUALITY										= 0x00,
	HIGH_QUALITY									= 0x04
}Font_Size;				
				
typedef struct{				
	Commun_Data_Length						Data_Length;
	Display_Row_Count							Row_Count;
	Font_Size											Font_Size;
}Function_Set;				
				
typedef enum{				
	DSP_OFF												= 0x00,
	DSP_ON												= 0x04
}Display_Control;				
				
typedef enum{				
	CRS_OFF 											= 0x00,
	CRS_ON 												= 0x02
}Cursor_Control;				
				
typedef enum{				
	CRS_BLNK_OFF									= 0x00,
	CRS_BLNK_ON										= 0x01
}Cursor_Blink_Control;				
				
typedef struct{				
	Display_Control								Display;
	Cursor_Control								Cursor;
	Cursor_Blink_Control					Cursor_Blink;
}Display_On_Off;

typedef enum{
	INCREMENT 										= 0x02,
	DECREMENT 										= 0x00
}Inc_Dec_State;

typedef enum{
	LOW 													= 0x00,
	HIGH 													= 0x01
}Shift_of_Entire_Dsp_State;

typedef struct{
	Inc_Dec_State									Inc_Dec_State;
	Shift_of_Entire_Dsp_State			Shift_of_Entire_Dsp_State;
}Entry_Mode_Set;	
	
typedef struct{	
	Function_Set									Display_Funtion_Set;
	Display_On_Off								Display_Control;
	Entry_Mode_Set								Display_Entry_Mode;
}LCD_InitStruct;


/* DEFINE VARIABLE END -------------------------------------------------*/

/* DEFINE FUNCTION BEGIN -----------------------------------------------*/

HAL_StatusTypeDef Display_Pin_Init(void);
HAL_StatusTypeDef Display_Init(LCD_InitStruct InitStruct);
HAL_StatusTypeDef LCD_Set_Cursor(uint8_t Row, uint8_t Column);
HAL_StatusTypeDef LCD_Communication(uint8_t DATA, CMMON_State CMMON_STATE);
HAL_StatusTypeDef LCD_Pin_Write(uint8_t DATA, RS_State PIN_RS_STATE, WR_State PIN_WR_STATE);

/* DEFINE FUNCTION END -------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

