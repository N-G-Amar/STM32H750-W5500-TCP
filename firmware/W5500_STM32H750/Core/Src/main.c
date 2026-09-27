/* USER CODE BEGIN Header */

/**

  ******************************************************************************

  * @file           : main.c

  * @brief          : Main program body

  ******************************************************************************

  * @attention

  *

  * Copyright (c) 2026 STMicroelectronics.

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

#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/

/* USER CODE BEGIN PD */

#define W5500_S0_REG_BSB       0x01

#define W5500_S0_TX_BSB        0x02

#define W5500_S0_RX_BSB        0x03

#define W5500_Sn_MR            0x0000

#define W5500_Sn_CR            0x0001

#define W5500_Sn_IR            0x0002

#define W5500_Sn_SR            0x0003

#define W5500_Sn_PORT          0x0004

#define W5500_Sn_TX_WR         0x0024

#define W5500_Sn_RX_RSR        0x0026

#define W5500_Sn_RX_RD         0x0028

#define W5500_Sn_MR_TCP        0x01

#define W5500_Sn_CR_OPEN       0x01

#define W5500_Sn_CR_LISTEN     0x02

#define W5500_Sn_CR_CLOSE      0x10

#define W5500_Sn_CR_SEND       0x20

#define W5500_Sn_CR_RECV       0x40

#define W5500_SOCK_CLOSED      0x00

#define W5500_SOCK_INIT        0x13

#define W5500_SOCK_LISTEN      0x14

#define W5500_SOCK_ESTABLISHED 0x17

#define W5500_SOCK_CLOSE_WAIT  0x1C

#define W5500_S0_TX_BUF_SIZE    2048

#define W5500_S0_RX_BUF_SIZE    2048

#define W5500_TCP_RX_BUFFER_SIZE 256

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

volatile uint8_t w5500_version = 0;

volatile uint8_t w5500_ok = 0;

volatile uint8_t w5500_phycfgr = 0;

volatile uint8_t w5500_net_ok = 0;

volatile uint8_t w5500_mac[6] = {0};

volatile uint8_t w5500_ip[4] = {0};

volatile uint8_t w5500_subnet[4] = {0};

volatile uint8_t w5500_gateway[4] = {0};

volatile uint8_t w5500_readback_ok = 0;

volatile uint8_t w5500_tcp_status = 0;

volatile uint8_t w5500_tcp_connected = 0;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);

static void MPU_Config(void);

static void MX_GPIO_Init(void);

static void MX_SPI1_Init(void);

/* USER CODE BEGIN PFP */

static uint8_t W5500_BlockWrite(uint8_t bsb,

                                uint16_t address,

                                const uint8_t *data,

                                uint16_t length);

static uint8_t W5500_BlockRead(uint8_t bsb,

                               uint16_t address,

                               uint8_t *data,

                               uint16_t length);

static uint8_t W5500_TCP_ServerInit(uint16_t port);

static void W5500_TCP_ServerTask(void);

static uint8_t W5500_SocketRead16(uint16_t address, uint16_t *value);

static uint8_t W5500_SocketWrite16(uint16_t address, uint16_t value);

static uint8_t W5500_TCP_Send(const uint8_t *data, uint16_t length);

static uint8_t W5500_ReadVersion(void);

static uint8_t W5500_ReadPHYCFGR(void);

static HAL_StatusTypeDef W5500_WriteCommon(uint16_t address,

                                            const uint8_t *data,

                                            uint16_t length);

static uint8_t W5500_ReadCommon(uint16_t address,

                                uint8_t *data,

                                uint16_t length);

static uint8_t W5500_ConfigureNetwork(void);



/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**

  * @brief  The application entry point.

  * @retval int

  */

int main(void)

{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/

  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */

  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */

  MX_GPIO_Init();

  MX_SPI1_Init();

  /* USER CODE BEGIN 2 */

  w5500_version = W5500_ReadVersion();

  if (w5500_version == 0x04)

  {

      w5500_ok = 1;

  }

  else

  {

      w5500_ok = 0;

  }

  w5500_phycfgr = W5500_ReadPHYCFGR();

  w5500_net_ok = W5500_ConfigureNetwork();

  w5500_readback_ok = 1;

  if (!W5500_ReadCommon(0x0009, (uint8_t *)w5500_mac, 6))

      w5500_readback_ok = 0;

  if (!W5500_ReadCommon(0x000F, (uint8_t *)w5500_ip, 4))

      w5500_readback_ok = 0;

  if (!W5500_ReadCommon(0x0005, (uint8_t *)w5500_subnet, 4))

      w5500_readback_ok = 0;

  if (!W5500_ReadCommon(0x0001, (uint8_t *)w5500_gateway, 4))

      w5500_readback_ok = 0;

  /* Start TCP server only if network configuration and read-back passed */

  if (w5500_net_ok && w5500_readback_ok)

  {

      W5500_TCP_ServerInit(5000);

  }

  /* USER CODE END 2 */



  /* Infinite loop */

  /* USER CODE BEGIN WHILE */

  while (1)

  {

    /* USER CODE END WHILE */

&#x9;    W5500_TCP_ServerTask();

    /* USER CODE BEGIN 3 */

  }

  /* USER CODE END 3 */

}

/**

  * @brief System Clock Configuration

  * @retval None

  */

void SystemClock_Config(void)

{

  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable

  */

  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage

  */

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters

  * in the RCC_OscInitTypeDef structure.

  */

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;

  RCC_OscInitStruct.HSEState = RCC_HSE_ON;

  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;

  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;

  RCC_OscInitStruct.PLL.PLLN = 160;

  RCC_OscInitStruct.PLL.PLLP = 2;

  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLR = 2;

  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;

  RCC_OscInitStruct.PLL.PLLFRACN = 0;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)

  {

    Error_Handler();

  }

  /** Initializes the CPU, AHB and APB buses clocks

  */

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK

                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2

                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;

  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;

  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;

  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;

  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;

  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)

  {

    Error_Handler();

  }

}

/**

  * @brief SPI1 Initialization Function

  * @param None

  * @retval None

  */

static void MX_SPI1_Init(void)

{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */

  /* SPI1 parameter configuration*/

  hspi1.Instance = SPI1;

  hspi1.Init.Mode = SPI_MODE_MASTER;

  hspi1.Init.Direction = SPI_DIRECTION_2LINES;

  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;

  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;

  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;

  hspi1.Init.NSS = SPI_NSS_SOFT;

  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;

  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;

  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;

  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;

  hspi1.Init.CRCPolynomial = 0x0;

  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;

  hspi1.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;

  hspi1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;

  hspi1.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;

  hspi1.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;

  hspi1.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;

  hspi1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;

  hspi1.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;

  hspi1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;

  hspi1.Init.IOSwap = SPI_IO_SWAP_DISABLE;

  if (HAL_SPI_Init(&hspi1) != HAL_OK)

  {

    Error_Handler();

  }

  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**

  * @brief GPIO Initialization Function

  * @param None

  * @retval None

  */

static void MX_GPIO_Init(void)

{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */

  __HAL_RCC_GPIOH_CLK_ENABLE();

  __HAL_RCC_GPIOC_CLK_ENABLE();

  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */

  HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */

  HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : W5500_RST_Pin */

  GPIO_InitStruct.Pin = W5500_RST_Pin;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct.Pull = GPIO_NOPULL;

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(W5500_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : W5500_CS_Pin */

  GPIO_InitStruct.Pin = W5500_CS_Pin;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct.Pull = GPIO_NOPULL;

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(W5500_CS_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */

}

/* USER CODE BEGIN 4 */

static uint8_t W5500_ReadVersion(void)

{

    uint8_t command[3];

    uint8_t version = 0;

    /* Hardware reset */

    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_RESET);

    HAL_Delay(2);

    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);

    HAL_Delay(50);

    /*

     * W5500 VERSIONR:

     * Address  = 0x0039

     * Control  = 0x00

     * Expected = 0x04

     */

    command[0] = 0x00;   // Address high byte

    command[1] = 0x39;   // Address low byte

    command[2] = 0x00;   // Read, Common Register, VDM

    /* Select W5500 */

    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET);

    /* Send address + control byte */

    HAL_SPI_Transmit(&hspi1, command, 3, HAL_MAX_DELAY);

    /* Receive VERSIONR */

    HAL_SPI_Receive(&hspi1, &version, 1, HAL_MAX_DELAY);

    /* Deselect W5500 */

    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_SET);

    return version;

}

static HAL_StatusTypeDef W5500_WriteCommon(uint16_t address,

                                            const uint8_t *data,

                                            uint16_t length)

{

    uint8_t command[3];

    HAL_StatusTypeDef status;

    command[0] = (uint8_t)(address >> 8);

    command[1] = (uint8_t)(address & 0xFF);

    command[2] = 0x04;   // Write, Common Register, VDM

    HAL_GPIO_WritePin(W5500_CS_GPIO_Port,

                      W5500_CS_Pin,

                      GPIO_PIN_RESET);

    status = HAL_SPI_Transmit(&hspi1,

                              command,

                              3,

                              HAL_MAX_DELAY);

    if (status == HAL_OK)

    {

        status = HAL_SPI_Transmit(&hspi1,

                                  (uint8_t *)data,

                                  length,

                                  HAL_MAX_DELAY);

    }

    HAL_GPIO_WritePin(W5500_CS_GPIO_Port,

                      W5500_CS_Pin,

                      GPIO_PIN_SET);

    return status;

}

static uint8_t W5500_ReadCommon(uint16_t address,

                                uint8_t *data,

                                uint16_t length)

{

    uint8_t command[3];

    command[0] = (uint8_t)(address >> 8);

    command[1] = (uint8_t)(address & 0xFF);

    command[2] = 0x00;   // Read, Common Register, VDM

    HAL_GPIO_WritePin(W5500_CS_GPIO_Port,

                      W5500_CS_Pin,

                      GPIO_PIN_RESET);

    if (HAL_SPI_Transmit(&hspi1,

                         command,

                         3,

                         HAL_MAX_DELAY) != HAL_OK)

    {

        HAL_GPIO_WritePin(W5500_CS_GPIO_Port,

                          W5500_CS_Pin,

                          GPIO_PIN_SET);

        return 0;

    }

    if (HAL_SPI_Receive(&hspi1,

                        data,

                        length,

                        HAL_MAX_DELAY) != HAL_OK)

    {

        HAL_GPIO_WritePin(W5500_CS_GPIO_Port,

                          W5500_CS_Pin,

                          GPIO_PIN_SET);

        return 0;

    }

    HAL_GPIO_WritePin(W5500_CS_GPIO_Port,

                      W5500_CS_Pin,

                      GPIO_PIN_SET);

    return 1;

}

static uint8_t W5500_ConfigureNetwork(void)

{

    uint8_t mac[6] =

    {

        0x02, 0x08, 0xDC, 0x34, 0x56, 0x78

    };

    uint8_t gateway[4] =

    {

        0, 0, 0, 0

    };

    uint8_t subnet[4] =

    {

        255, 255, 0, 0

    };

    uint8_t ip[4] =

    {

        169, 254, 228, 251

    };

    if (W5500_WriteCommon(0x0009, mac, 6) != HAL_OK)

        return 0;

    if (W5500_WriteCommon(0x0001, gateway, 4) != HAL_OK)

        return 0;

    if (W5500_WriteCommon(0x0005, subnet, 4) != HAL_OK)

        return 0;

    if (W5500_WriteCommon(0x000F, ip, 4) != HAL_OK)

        return 0;

    return 1;

}

static uint8_t W5500_ReadPHYCFGR(void)

{

    uint8_t command[3];

    uint8_t phycfgr = 0;

    command[0] = 0x00;

    command[1] = 0x2E;

    command[2] = 0x00;

    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET);

    HAL_SPI_Transmit(&hspi1, command, 3, HAL_MAX_DELAY);

    HAL_SPI_Receive(&hspi1, &phycfgr, 1, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_SET);

    return phycfgr;

}

static uint8_t W5500_BlockWrite(uint8_t bsb,

                                uint16_t address,

                                const uint8_t *data,

                                uint16_t length)

{

    uint8_t command[3];

    command[0] = (uint8_t)(address >> 8);

    command[1] = (uint8_t)(address & 0xFF);

    /* Write + Variable Data Length Mode */

    command[2] = (uint8_t)((bsb << 3) | 0x04);

    HAL_GPIO_WritePin(W5500_CS_GPIO_Port,

                      W5500_CS_Pin,

                      GPIO_PIN_RESET);

    if (HAL_SPI_Transmit(&hspi1,

                         command,

                         3,

                         HAL_MAX_DELAY) != HAL_OK)

    {

        HAL_GPIO_WritePin(W5500_CS_GPIO_Port,

                          W5500_CS_Pin,

                          GPIO_PIN_SET);

        return 0;

    }

    if (HAL_SPI_Transmit(&hspi1,

                         (uint8_t *)data,

                         length,

                         HAL_MAX_DELAY) != HAL_OK)

    {

        HAL_GPIO_WritePin(W5500_CS_GPIO_Port,

                          W5500_CS_Pin,

                          GPIO_PIN_SET);

        return 0;

    }

    HAL_GPIO_WritePin(W5500_CS_GPIO_Port,

                      W5500_CS_Pin,

                      GPIO_PIN_SET);

    return 1;

}

static uint8_t W5500_BlockRead(uint8_t bsb,

        uint16_t address,

        uint8_t *data,

        uint16_t length)

{

uint8_t command[3];

command[0] = (uint8_t)(address >> 8);

command[1] = (uint8_t)(address & 0xFF);

/* Read + Variable Data Length Mode */

command[2] = (uint8_t)(bsb << 3);

HAL_GPIO_WritePin(W5500_CS_GPIO_Port,

W5500_CS_Pin,

GPIO_PIN_RESET);

if (HAL_SPI_Transmit(&hspi1,

  command,

  3,

  HAL_MAX_DELAY) != HAL_OK)

{

HAL_GPIO_WritePin(W5500_CS_GPIO_Port,

   W5500_CS_Pin,

   GPIO_PIN_SET);

return 0;

}

if (HAL_SPI_Receive(&hspi1,

 data,

 length,

 HAL_MAX_DELAY) != HAL_OK)

{

HAL_GPIO_WritePin(W5500_CS_GPIO_Port,

   W5500_CS_Pin,

   GPIO_PIN_SET);

return 0;

}

HAL_GPIO_WritePin(W5500_CS_GPIO_Port,

W5500_CS_Pin,

GPIO_PIN_SET);

return 1;

}

static uint8_t W5500_TCP_ServerInit(uint16_t port)

{

    uint8_t value;

    uint8_t port_bytes[2];

    uint8_t status;

    /* TCP mode */

    value = W5500_Sn_MR_TCP;

    if (!W5500_BlockWrite(W5500_S0_REG_BSB,

                          W5500_Sn_MR,

                          &value,

                          1))

    {

        return 0;

    }

    /* Local TCP port */

    port_bytes[0] = (uint8_t)(port >> 8);

    port_bytes[1] = (uint8_t)(port & 0xFF);

    if (!W5500_BlockWrite(W5500_S0_REG_BSB,

                          W5500_Sn_PORT,

                          port_bytes,

                          2))

    {

        return 0;

    }

    /* OPEN socket */

    value = W5500_Sn_CR_OPEN;

    if (!W5500_BlockWrite(W5500_S0_REG_BSB,

                          W5500_Sn_CR,

                          &value,

                          1))

    {

        return 0;

    }

    HAL_Delay(2);

    /* LISTEN */

    value = W5500_Sn_CR_LISTEN;

    if (!W5500_BlockWrite(W5500_S0_REG_BSB,

                          W5500_Sn_CR,

                          &value,

                          1))

    {

        return 0;

    }

    HAL_Delay(2);

    /* Read socket status */

    if (!W5500_BlockRead(W5500_S0_REG_BSB,

                         W5500_Sn_SR,

                         &status,

                         1))

    {

        return 0;

    }

    w5500_tcp_status = status;

    return 1;

}



static void W5500_TCP_ServerTask(void)

{

    uint8_t status;

    uint8_t rx_data[W5500_TCP_RX_BUFFER_SIZE];

    uint16_t rx_size;

    uint16_t rx_rd;

    uint16_t i;

    if (!W5500_BlockRead(W5500_S0_REG_BSB,

                         W5500_Sn_SR,

                         &status,

                         1))

    {

        return;

    }

    w5500_tcp_status = status;

    /* ---------------------------------------------------------

       CLOSED: reopen the TCP server

       --------------------------------------------------------- */

    if (status == W5500_SOCK_CLOSED)

    {

        w5500_tcp_connected = 0;

        W5500_TCP_ServerInit(5000);

        return;

    }

    /* ---------------------------------------------------------

       ESTABLISHED: TCP connection is active

       --------------------------------------------------------- */

    if (status == W5500_SOCK_ESTABLISHED)

    {

        w5500_tcp_connected = 1;

        /*

         * Read number of bytes waiting in RX buffer.

         */

        if (!W5500_SocketRead16(W5500_Sn_RX_RSR, &rx_size))

        {

            return;

        }

        if (rx_size == 0)

        {

            return;

        }

        /*

         * Limit one read to our local buffer size.

         */

        if (rx_size > W5500_TCP_RX_BUFFER_SIZE)

        {

            rx_size = W5500_TCP_RX_BUFFER_SIZE;

        }

        /*

         * Read current RX read pointer.

         */

        if (!W5500_SocketRead16(W5500_Sn_RX_RD, &rx_rd))

        {

            return;

        }

        /*

         * Read received data from W5500 RX memory.

         *

         * The default socket RX buffer is 2 KB.

         * Split the read if it crosses the 2 KB boundary.

         */

        uint16_t offset = rx_rd & (W5500_S0_RX_BUF_SIZE - 1);

        uint16_t first_part = W5500_S0_RX_BUF_SIZE - offset;

        if (first_part > rx_size)

        {

            first_part = rx_size;

        }

        if (!W5500_BlockRead(W5500_S0_RX_BSB,

                             offset,

                             rx_data,

                             first_part))

        {

            return;

        }

        if (first_part < rx_size)

        {

            if (!W5500_BlockRead(W5500_S0_RX_BSB,

                                 0,

                                 &rx_data[first_part],

                                 rx_size - first_part))

            {

                return;

            }

        }

        /*

         * Move RX read pointer forward.

         */

        rx_rd += rx_size;

        if (!W5500_SocketWrite16(W5500_Sn_RX_RD, rx_rd))

        {

            return;

        }

        /*

         * Tell W5500 that the received data has been consumed.

         */

        uint8_t command = W5500_Sn_CR_RECV;

        if (!W5500_BlockWrite(W5500_S0_REG_BSB,

                              W5500_Sn_CR,

                              &command,

                              1))

        {

            return;

        }

        /*

         * Echo the received data back to the PC.

         */

        W5500_TCP_Send(rx_data, rx_size);

        /*

         * Prevent unused-variable warning if the compiler

         * optimizes this loop in the future.

         */

        for (i = 0; i < rx_size; i++)

        {

            (void)rx_data[i];

        }

    }

    /* ---------------------------------------------------------

       CLOSE_WAIT: remote side closed the connection

       --------------------------------------------------------- */

    else if (status == W5500_SOCK_CLOSE_WAIT)

    {

        w5500_tcp_connected = 0;

        uint8_t command = W5500_Sn_CR_CLOSE;

        W5500_BlockWrite(W5500_S0_REG_BSB,

                         W5500_Sn_CR,

                         &command,

                         1);

        HAL_Delay(2);

        W5500_TCP_ServerInit(5000);

    }

    else

    {

        w5500_tcp_connected = 0;

    }

}

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)

{

  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */

  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected

  */

  MPU_InitStruct.Enable = MPU_REGION_ENABLE;

  MPU_InitStruct.Number = MPU_REGION_NUMBER0;

  MPU_InitStruct.BaseAddress = 0x0;

  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;

  MPU_InitStruct.SubRegionDisable = 0x87;

  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;

  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;

  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;

  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;

  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enables the MPU */

  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

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

#ifdef USE_FULL_ASSERT

/**

  * @brief  Reports the name of the source file and the source line number

  *         where the assert_param error has occurred.

  * @param  file: pointer to the source file name

  * @param  line: assert_param error line source number

  * @retval None

  */

void assert_failed(uint8_t *file, uint32_t line)

{

  /* USER CODE BEGIN 6 */

  /* User can add his own implementation to report the file name and line number,

     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* USER CODE END 6 */

}

#endif /* USE_FULL_ASSERT */