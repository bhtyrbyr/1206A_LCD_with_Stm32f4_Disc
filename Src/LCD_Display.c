#include "main.h"

#ifdef _TEST_MODE
void Test_Function(void)
{
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
}
#endif /* _TEST_MODE */

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
	
	//__LCD_SET_KONS_;
  return HAL_OK;
}

HAL_StatusTypeDef Display_Init(LCD_InitStruct InitStruct)
{
	HAL_Delay(40);
	// Configure LCD Function Set to 4-Bit, 2 line and high quality font size
	uint8_t Function_Set = (((0x20 | InitStruct.Display_Funtion_Set.Data_Length) | InitStruct.Display_Funtion_Set.Row_Count) | InitStruct.Display_Funtion_Set.Font_Size );
#ifdef _TEST_MODE
	if(Function_Set == 0x2C)
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
#endif
	LCD_Communication(Function_Set, CMMON_SEND_COMMAND);
	HAL_Delay(1);
	
	// Configure LCD Display On / Off to Display: On, Cursor: On, Cursor Blink: On
	uint8_t Display_Control = (((0x08 | InitStruct.Display_Control.Display) | InitStruct.Display_Control.Cursor) | InitStruct.Display_Control.Cursor_Blink );
#ifdef _TEST_MODE
	if(Display_Control == 14)
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

HAL_StatusTypeDef LCD_Set_Cursor(uint8_t Row, uint8_t Column)
{
	switch(Row)
	{
		case 1: {
			uint8_t temp = 0x80 | (Column - 1);
			LCD_Communication(temp, CMMON_SEND_COMMAND);
		} break;
		case 2: {
			uint8_t temp = 0xC0 | (Column - 1);
			LCD_Communication(temp, CMMON_SEND_COMMAND);
		} break;
		default: {
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
		} break;
	}
	HAL_Delay(2);
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
	return HAL_OK;
}

HAL_StatusTypeDef LCD_Pin_Write(uint8_t DATA, RS_State PIN_RS_STATE, WR_State PIN_WR_STATE)
{
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_RS, PIN_RS_STATE);
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_RS, PIN_WR_STATE);
	
	//HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_EN, GPIO_PIN_SET);
	//HAL_Delay(1);
	//Data Write Area
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_D7, ((DATA >> 3) & 0x01));
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_D6, ((DATA >> 2) & 0x01));
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_D5, ((DATA >> 1) & 0x01));
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_D4, (DATA & 0x01));
	
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_EN, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_PORT, LCD_PIN_EN, GPIO_PIN_RESET);
	HAL_Delay(1);
	
	return HAL_OK;
}

///*
// * LCD1602.c
// *
// *  Created on: 21-Jan-2020
// *      Author: Controllerstech
// */




///*********** Define the LCD PINS below ****************/

//#define RS_Pin GPIO_PIN_1
//#define RS_GPIO_Port GPIOA
//#define RW_Pin GPIO_PIN_2
//#define RW_GPIO_Port GPIOA
//#define EN_Pin GPIO_PIN_3
//#define EN_GPIO_Port GPIOA
//#define D4_Pin GPIO_PIN_4
//#define D4_GPIO_Port GPIOA
//#define D5_Pin GPIO_PIN_5
//#define D5_GPIO_Port GPIOA
//#define D6_Pin GPIO_PIN_6
//#define D6_GPIO_Port GPIOA
//#define D7_Pin GPIO_PIN_7
//#define D7_GPIO_Port GPIOA

///****************** define the timer handler below  **************/
//#define timer htim6

///*
//extern TIM_HandleTypeDef timer;
//void delay (uint16_t us)
//{
//	__HAL_TIM_SET_COUNTER(&timer, 0);
//	while (__HAL_TIM_GET_COUNTER(&timer) < us);
//}
//*/
///****************************************************************************************************************************************************************/

//void send_to_lcd (char data, int rs)
//{
//	HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, rs);  // rs = 1 for data, rs=0 for command

//	/* write the data to the respective pin */
//	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, ((data>>3)&0x01));
//	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, ((data>>2)&0x01));
//	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, ((data>>1)&0x01));
//	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, ((data>>0)&0x01));

//	/* Toggle EN PIN to send the data
//	 * if the HCLK > 100 MHz, use the  20 us delay
//	 * if the LCD still doesn't work, increase the delay to 50, 80 or 100..
//	 */
//	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, 1);
//	HAL_Delay (1);
//	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, 0);
//	HAL_Delay (1);
//}

//void lcd_send_cmd (char cmd)
//{
//    char datatosend;

//    /* send upper nibble first */
//    datatosend = ((cmd>>4)&0x0f);
//    send_to_lcd(datatosend,0);  // RS must be 0 while sending command

//    /* send Lower Nibble */
//    datatosend = ((cmd)&0x0f);
//	send_to_lcd(datatosend, 0);
//}

//void lcd_send_data (char data)
//{
//	char datatosend;

//	/* send higher nibble */
//	datatosend = ((data>>4)&0x0f);
//	send_to_lcd(datatosend, 1);  // rs =1 for sending data

//	/* send Lower nibble */
//	datatosend = ((data)&0x0f);
//	send_to_lcd(datatosend, 1);
//}

//void lcd_clear (void)
//{
//	lcd_send_cmd(0x01);
//	HAL_Delay(2);
//}

//void lcd_put_cur(int row, int col)
//{
//    switch (row)
//    {
//        case 0:
//            col |= 0x80;
//            break;
//        case 1:
//            col |= 0xC0;
//            break;
//    }

//    lcd_send_cmd (col);
//}


//void lcd_init (void)
//{
//	// 4 bit initialisation
//	HAL_Delay(50);  // wait for >40ms
//	lcd_send_cmd (0x30);
//	HAL_Delay(5);  // wait for >4.1ms
//	lcd_send_cmd (0x30);
//	HAL_Delay(1);  // wait for >100us
//	lcd_send_cmd (0x30);
//	HAL_Delay(10);
//	lcd_send_cmd (0x20);  // 4bit mode
//	HAL_Delay(10);

//  // dislay initialisation
//	lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
//	HAL_Delay(1);
//	lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
//	HAL_Delay(1);
//	lcd_send_cmd (0x01);  // clear display
//	HAL_Delay(1);
//	HAL_Delay(1);
//	lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
//	HAL_Delay(1);
//	lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
//}

//void lcd_send_string (char *str)
//{
//	while (*str) lcd_send_data (*str++);
//}

