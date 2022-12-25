
// ----- INCLUDE FILES
#include			"main.h"
#include			"sStd.h"
#include			"sCMD.h"
#include			<stdint.h>
#include			<stdlib.h>


// ----- STM32 STUFF
UART_HandleTypeDef huart1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);


// ----- FUNCTION DECLARATIONS
void UARTout(const char* buffer, const uint16_t len);
void cmdHelp(const char** args, const uint8_t argCnt);
void cmdStatus(const char** args, const uint8_t argCnt);
void cmdPeriod(const char** args, const uint8_t argCnt);
void cmdTest(const char** args, const uint8_t argCnt);
void cmdNA(const char** args, const uint8_t argCnt);
void cmdEnableNull(const char** args, const uint8_t argCnt);


// ----- VARIABLES
uint8_t status = 0;
uint32_t flashTick = 0;
uint16_t period = 100;
uint8_t enabledNull = 0;


// ----- ARRAY WITH COMMANDS
// List of commands. Every command is paried with own handler function. Parser is case sensitive
const CMDList cmdList[] = {
	{ "help", cmdHelp },
	{ "status", cmdStatus },
	{ "period", cmdPeriod },
	{ "test", cmdTest },
	{ "enable_null", cmdEnableNull }
};


// ----- OBJECTS
sStd::Logger<256> Serial = sStd::Logger<256>(UARTout);
CMDHandler<4> cmd(cmdList, AL(cmdList), cmdNA); // Uses default delimiters (; , ,). Handler can parse maximum 4 arguments


// ----- FUNCTION DEFINITIONS
void UARTout(const char* buffer, const uint16_t len)
{
	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	HAL_UART_Transmit(&huart1, (uint8_t*)&buffer[0], len, 500);
}

// Called on every help command
void cmdHelp(const char** args, const uint8_t argCnt)
{
	Serial.print("\n----- HELP\n- status: [0 = off; 1 = on; 2 = flash]\n- period: [10-1000ms]\n-----\n");
}

// This is called on every status command
void cmdStatus(const char** args, const uint8_t argCnt)
{
	if (!argCnt) Serial.print("Argument missing!\n");
	else
	{
		if (*(args[0]) > '2' && *(args[0]) < '0') Serial.print("Argument can be only 0, 1 or 2!\n");
		else
		{
			status = *(args[0]) - '0';

			switch (status)
			{
				case 0:
				{
					Serial.print("LED is off!\n");
					HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
					break;
				}

				case 1:
				{
					Serial.print("LED is on!\n");
					HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
					break;
				}

				case 2:
				{
					Serial.printf("LED is flashing with %dms period!\n", period);
					HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
					flashTick = HAL_GetTick();
					break;
				}
			}
		}
	}
}

// This is called on every period command
void cmdPeriod(const char** args, const uint8_t argCnt)
{
	if (!argCnt) Serial.print("Argument missing!\n");
	else
	{
		uint16_t tmp = atoi(args[0]);

		if (tmp < 10 || tmp > 1000)
		{
			Serial.print("Period have to be between 10 and 1000ms!\n");
			return;
		}

		period = tmp;
		Serial.printf("New flash period is %dms\n", period);
	}
}

// This is called on every test command
void cmdTest(const char** args, const uint8_t argCnt)
{
	Serial.print("Test command:\n");

	for (uint8_t i = 0; i < argCnt; i++)
	{
		Serial.printf("- Argument %d: '%s'\n", i, args[i]);
	}

	Serial.print("\n");
}

// Enable NULL as argument. When enabled, command "cmd,1,,," will have 3 arguments, not one
void cmdEnableNull(const char** args, const uint8_t argCnt)
{
	enabledNull = !enabledNull;
	Serial.printf("NULL as argument is %s\n", enabledNull ? "enabled" : "disabled");
}

// This is called when CMD handler finds unknown command
void cmdNA(const char** args, const uint8_t argCnt)
{
	Serial.printf("Command '%s' not found!\n", args);
}



int main(void)
{
	// STM32 STUFF
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART1_UART_Init();

	// sCMD STUFF
	uint8_t UARTChar = '\0';
	uint8_t UARTStr[64] = { '\0' };
	uint8_t UARTIdx = 0;

	Serial.print("sCMD Example\nType \"help\" to see list of commands.\n");

	while (1)
	{
		// UART RX Part
		HAL_UART_Receive(&huart1, &UARTChar, 1, 1);

		if (UARTChar == '\n') // End of line
		{
			UARTStr[UARTIdx] = '\0';
			UARTIdx = 0;
			UARTChar = '\0';

			// Command parser and executer. String have to end with NULL char.
			cmd.exe((char*)UARTStr, enabledNull);
		}
		else if (UARTChar) // Every char other than NULL char
		{
			if (UARTIdx > 63) UARTIdx = 0;

			UARTStr[UARTIdx] = UARTChar;
			UARTIdx++;
		}


		// LED FLASHING
		if (HAL_GetTick() - flashTick >= period && status == 2)
		{
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			flashTick = HAL_GetTick();
		}
	}
}

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
															|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

static void MX_USART1_UART_Init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
}

static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin : LED_Pin */
	GPIO_InitStruct.Pin = LED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : PC14 PC15 */
	GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PD0 PD1 */
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
													|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
													|GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
													|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
													|GPIO_PIN_15|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
													|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure peripheral I/O remapping */
	__HAL_AFIO_REMAP_PD01_ENABLE();

}


void Error_Handler(void)
{
	__disable_irq();
	while (1)
	{
	}
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif /* USE_FULL_ASSERT */
