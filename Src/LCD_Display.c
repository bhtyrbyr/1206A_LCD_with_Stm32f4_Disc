#include "LCD_Display.h"

HAL_StatusTypeDef Display_Pin_Init(void)
{
  GPIO_InitTypeDef LED_Pin_InitTypeDef = {0};

  /* GPIO Ports Clock Enable */
  __LCD_CLK_EN;

  /*Configure GPIO pins*/
  LED_Pin_InitTypeDef.Pin 	= LCD_ALL_PINS;
  LED_Pin_InitTypeDef.Mode 	= GPIO_MODE_OUTPUT_PP;
  LED_Pin_InitTypeDef.Pull 	= GPIO_NOPULL;
  LED_Pin_InitTypeDef.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(LCD_PORT, &LED_Pin_InitTypeDef);
	
	__LCD_ALL_DTA_PIN_RST;
	
  return HAL_OK;
}

HAL_StatusTypeDef Display_Init(LCD_InitStruct InitStruct)
{
	HAL_Delay(50);
	// Configure LCD Function Set to 4-Bit, 2 line and high quality font size
	uint8_t Function_Set = (((0x20 | InitStruct.Display_Funtion_Set.Data_Length) | InitStruct.Display_Funtion_Set.Row_Count) | InitStruct.Display_Funtion_Set.Font_Quality );
#ifdef _TEST_MODE
	if(Function_Set == 0x2C)
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
#endif
	LCD_Communication(Function_Set, CMMON_SEND_COMMAND);
	HAL_Delay(1);
	
	// Configure LCD Display On / Off to Display: On, Cursor: On, Cursor Blink: On
	uint8_t Display_Control = (((0x08 | InitStruct.Display_Control.Display) | InitStruct.Display_Control.Cursor) | InitStruct.Display_Control.Cursor_Blink );
#ifdef _TEST_MODE
	if(Display_Control == 0x0D)
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
#endif
	LCD_Communication(Display_Control, CMMON_SEND_COMMAND);
	HAL_Delay(1);
		
	// Clear LCD
	LCD_Communication(LCD_CMD_CLEAR, CMMON_SEND_COMMAND);
	HAL_Delay(2);
	
	// Configure Entry Mode to I/D: Incrament mode and SH: Entire shif on
	uint8_t Entry_Mode = ((0x04 | InitStruct.Display_Entry_Mode.Inc_Dec_State) | InitStruct.Display_Entry_Mode.Shift_of_Entire_Dsp_State );
#ifdef _TEST_MODE
	if(Entry_Mode == 7)
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
#endif
	LCD_Communication(Entry_Mode, CMMON_SEND_COMMAND);
	HAL_Delay(1);
	
	return HAL_OK;
}

HAL_StatusTypeDef LCD_Write_String(int8_t * Message)
{
	while(*Message)
	{
		LCD_Communication(*Message++, CMMON_WRITE_DATA);
	}
	return HAL_OK;
}

HAL_StatusTypeDef LCD_Set_Cursor(uint8_t Row, uint8_t Column)
{
	switch(Row)
	{
		case 1: {
			uint8_t temp = 0x80 | (Column);
			LCD_Communication(temp, CMMON_SEND_COMMAND);
		} break;
		case 2: {
			uint8_t temp = 0xC0 | (Column);
			LCD_Communication(temp, CMMON_SEND_COMMAND);
		} break;
		default: {
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
		} break;
	}
	return HAL_OK;
}

HAL_StatusTypeDef LCD_Communication(uint8_t DATA, CMMON_State CMMON_STATE)
{
	switch(CMMON_STATE)
	{
		case CMMON_SEND_COMMAND : {
			LCD_Pin_Write(((DATA >> 4) & 0x0F), REGISTER_INSTRUCTION, WRITE);
			LCD_Pin_Write((DATA & 0x0F), 				REGISTER_INSTRUCTION, WRITE);
		}break;
		case CMMON_BUSY_FLAG : {
			LCD_Pin_Write(((DATA >> 4) & 0x0F), REGISTER_INSTRUCTION, READ);
			LCD_Pin_Write((DATA & 0x0F), 				REGISTER_INSTRUCTION, READ);
		}break;
		case CMMON_WRITE_DATA : {
			LCD_Pin_Write(((DATA >> 4) & 0x0F), REGISTER_DATA, WRITE);
			LCD_Pin_Write((DATA & 0x0F), 				REGISTER_DATA, WRITE);
		}break;
		case CMMON_READ_DATA : {
			LCD_Pin_Write(((DATA >> 4) & 0x0F), REGISTER_DATA, READ);
			LCD_Pin_Write((DATA & 0x0F), 				REGISTER_DATA, READ);
		}break;
	}
	HAL_Delay(2);
	return HAL_OK;
}

HAL_StatusTypeDef LCD_Pin_Write(uint8_t DATA, uint8_t PIN_RS_STATE, uint8_t PIN_WR_STATE)
{
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_RS, PIN_RS_STATE);
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_WR, PIN_WR_STATE);
	
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_D7, ((DATA >> 3) & 0x01));
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_D6, ((DATA >> 2) & 0x01));
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_D5, ((DATA >> 1) & 0x01));
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_D4, ((DATA >> 0) & 0x01));
	
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_EN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_EN, GPIO_PIN_RESET);
	return HAL_OK;
}
