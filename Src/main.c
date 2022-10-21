#include "main.h"
/*
	Ilgili kod D12'ye bagli LED'i 250ms araliklarla yakip söndüren bir kod projesi.
	Ders boyuncaki tüm projelerde ve ödevlerde buradaki taslak kullanilacaktir.
	Bu proje kasnagindan haric herhangi farkli bir kod kasnagi olusturanlar bu durumun tüm ayrintilarini mutlaka anlatmaktan sorumludur.
*/

static void LED_GPIO_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
	LED_GPIO_Init();
  while((Display_Pin_Init() != HAL_OK))
	{	}
	LCD_InitStruct LCD_Init;	
	LCD_Init.Display_Funtion_Set.Data_Length 							= BIT_4;
	LCD_Init.Display_Funtion_Set.Row_Count		 						= ROW_2;
	LCD_Init.Display_Funtion_Set.Font_Size	 							= HIGH_QUALITY;
	LCD_Init.Display_Control.Display 											= DSP_ON;
	LCD_Init.Display_Control.Cursor												= CRS_ON;
	LCD_Init.Display_Control.Cursor_Blink 								= CRS_BLNK_OFF;
	LCD_Init.Display_Entry_Mode.Inc_Dec_State							= INCREMENT;
	LCD_Init.Display_Entry_Mode.Shift_of_Entire_Dsp_State = HIGH;
	Display_Init(LCD_Init);
	//LCD_Set_Cursor(1, 1);
	LCD_Communication(0x44, CMMON_WRITE_DATA);
  while (1)
  {
#ifdef _TEST_MODE
		//Test_Function();
#endif /* _TEST_MODE */
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void LED_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin 	= GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 	= GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}


