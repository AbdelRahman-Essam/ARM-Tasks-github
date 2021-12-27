
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"

UART_HandleTypeDef huart2;


void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

uint8_t convert_to_capital(uint8_t data);

char *msg = "HelloWorld\n";



  uint8_t rcvd_data;
  uint8_t data_buffer[100];
  uint32_t count=0;

int main(void)
{


  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();



  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  __HAL_UART_ENABLE(&huart2);

  HAL_UART_IRQHandler(&huart2);




  while (1)
  {
	  //wait till button is pressed
	  		while( ! HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) );

	  		//to avoid button de-bouncing related issues 200ms of delay
	  		delay();

	  		HAL_UART_Transmit(&huart2 , (uint8_t*)msg , strlen(msg) , HAL_MAX_DELAY );


	  		//First lets enable the reception in interrupt mode
	  		//this code enables the receive interrupt
	  		HAL_UART_Receive_IT(&huart2 , &rcvd_data,1);


  }





  	return 0;
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

}


/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{


  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIOBtn ,GPIOLed , usart_gpio;

  /* GPIO Ports Clock Enable */

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();


  GPIOBtn.Pin = GPIO_PIN_0;
  GPIOBtn.Mode = MODE_INPUT;
  GPIOBtn.Pull = GPIO_NOPULL;
  GPIOBtn.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOA, &GPIOBtn);


  GPIOLed.Mode = GPIO_MODE_OUTPUT_OD;
  GPIOLed.Pin = GPIO_PIN_12;
  GPIOLed.Pull = GPIO_NOPULL;
  GPIOLed.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOD, &GPIOLed);

  usart_gpio.Mode = GPIO_MODE_AF_PP;
  usart_gpio.Pull = GPIO_PULLUP;
  usart_gpio.Speed = GPIO_SPEED_FREQ_LOW ;
  usart_gpio.Alternate =7;
  usart_gpio.Pin  = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOA, &usart_gpio);





}

uint8_t convert_to_capital(uint8_t data)
{
	if( data >= 'a' && data <= 'z')
	{
		data = data - ( 'a'- 'A');
	}

	return data;

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(rcvd_data == '\r')
		  	{

			data_buffer[count++]= '\r'; data_buffer[count++]= '\n' ;
		  	HAL_UART_Transmit(&huart2,data_buffer,count,HAL_MAX_DELAY);
		  	count = 0;
		  	}
		  	else
		  	{
		  		data_buffer[count++]= convert_to_capital(rcvd_data);
		  		HAL_UART_Receive_IT(&huart2 , &rcvd_data,1);
		  	}

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
