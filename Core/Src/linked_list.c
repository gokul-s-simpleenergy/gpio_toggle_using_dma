/* USER CODE BEGIN Header */
// /**
//   ******************************************************************************
//   * File Name          : linked_list.c
//   * Description        : This file provides code for the configuration
//   *                      of the LinkedList.
//   ******************************************************************************
//   * @attention
//   *
//   * Copyright (c) 2026 STMicroelectronics.
//   * All rights reserved.
//   *
//   * This software is licensed under terms that can be found in the LICENSE file
//   * in the root directory of this software component.
//   * If no LICENSE file comes with this software, it is provided AS-IS.
//   *
//   ******************************************************************************
//   */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "linked_list.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

DMA_NodeTypeDef EntryNode1;
DMA_QListTypeDef QueueEntry1;
DMA_NodeTypeDef EntryNode2;
DMA_NodeTypeDef ExecutionNode21;
DMA_NodeTypeDef ExecutionNode22;
DMA_NodeTypeDef ExecutionNode23;
DMA_NodeTypeDef ExitNode3;
DMA_NodeTypeDef ExitNode4;
DMA_NodeTypeDef CopyNodeTx;
DMA_QListTypeDef QueueTx;
DMA_NodeTypeDef CopyNodeRx;
DMA_QListTypeDef QueueRx;

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint32_t src_buffer_gpio_control_falling = 0x10000000; 
uint32_t src_buffer_gpio_control_rising = 0x1000; 
uint32_t src_buffer_timer2_ctrl = 0x89;
uint32_t src_buffer_timer3_ctrl = 0x89;
uint32_t src_buffer_timer4_ctrl = 0x89;
uint32_t src_buffer_timer_stop = 0x00;
uint32_t rx_buffer;
uint32_t rx_buffer_dump[32];

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/**
  * @brief  DMA Linked-list QueueTx configuration
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef MX_QueueTx_Config(void)
{
  HAL_StatusTypeDef ret = HAL_OK;
  /* DMA node configuration declaration */
  DMA_NodeConfTypeDef pNodeConfig;

  /* Set node configuration ################################################*/
  pNodeConfig.NodeType = DMA_GPDMA_LINEAR_NODE;
  pNodeConfig.Init.Request = DMA_REQUEST_SW;
  pNodeConfig.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
  pNodeConfig.Init.Direction = DMA_MEMORY_TO_MEMORY;
  pNodeConfig.Init.SrcInc = DMA_SINC_INCREMENTED;
  pNodeConfig.Init.DestInc = DMA_DINC_FIXED;
  pNodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_WORD;
  pNodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_WORD;
  pNodeConfig.Init.SrcBurstLength = 1;
  pNodeConfig.Init.DestBurstLength = 1;
  pNodeConfig.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT1|DMA_DEST_ALLOCATED_PORT1;
  pNodeConfig.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  pNodeConfig.TriggerConfig.TriggerMode = DMA_TRIGM_SINGLE_BURST_TRANSFER ;
  pNodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_RISING;
  pNodeConfig.TriggerConfig.TriggerSelection = GPDMA1_TRIGGER_TIM15_TRGO;
  pNodeConfig.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;
  pNodeConfig.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
  pNodeConfig.SrcAddress = src_buffer_lut;
  pNodeConfig.DstAddress = (uint32_t)&src_buffer_node1;
  pNodeConfig.DataSize = 4*32;

  /* Build CopyNodeTx Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &CopyNodeTx);

  /* Insert CopyNodeTx to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&QueueTx, &CopyNodeTx);

   return ret;
}

/**
  * @brief  DMA Linked-list QueueRx configuration
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef MX_QueueRx_Config(void)
{
  HAL_StatusTypeDef ret = HAL_OK;
  /* DMA node configuration declaration */
  DMA_NodeConfTypeDef pNodeConfig;

  /* Set node configuration ################################################*/
  pNodeConfig.NodeType = DMA_GPDMA_LINEAR_NODE;
  pNodeConfig.Init.Request = DMA_REQUEST_SW;
  pNodeConfig.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
  pNodeConfig.Init.Direction = DMA_MEMORY_TO_MEMORY;
  pNodeConfig.Init.SrcInc = DMA_SINC_FIXED;
  pNodeConfig.Init.DestInc = DMA_DINC_INCREMENTED;
  pNodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_WORD;
  pNodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_WORD;
  pNodeConfig.Init.SrcBurstLength = 1;
  pNodeConfig.Init.DestBurstLength = 1;
  pNodeConfig.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT1|DMA_DEST_ALLOCATED_PORT1;
  pNodeConfig.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  pNodeConfig.TriggerConfig.TriggerMode = DMA_TRIGM_SINGLE_BURST_TRANSFER ;
  pNodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_RISING;
  pNodeConfig.TriggerConfig.TriggerSelection = GPDMA1_TRIGGER_GPDMA1_CH1_TCF;
  pNodeConfig.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;
  pNodeConfig.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
  pNodeConfig.SrcAddress = (uint32_t)&rx_buffer;
  pNodeConfig.DstAddress = rx_buffer_dump;
  pNodeConfig.DataSize = 4*32;

  /* Build CopyNodeRx Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &CopyNodeRx);

  /* Insert CopyNodeRx to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&QueueRx, &CopyNodeRx);

   return ret;
}

/**
  * @brief  DMA Linked-list QueueEntry1 configuration
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef MX_QueueEntry1_Config(void)
{
  HAL_StatusTypeDef ret = HAL_OK;
  /* DMA node configuration declaration */
  DMA_NodeConfTypeDef pNodeConfig;

  /* ================= 1. EntryNode1: CS LOW ================= */
  pNodeConfig.NodeType = DMA_GPDMA_LINEAR_NODE;
  pNodeConfig.Init.Request = DMA_REQUEST_SW;
  pNodeConfig.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
  pNodeConfig.Init.Direction = DMA_MEMORY_TO_MEMORY;
  pNodeConfig.Init.SrcInc = DMA_SINC_FIXED;
  pNodeConfig.Init.DestInc = DMA_DINC_FIXED;
  pNodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_WORD;
  pNodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_WORD;
  pNodeConfig.Init.SrcBurstLength = 1;
  pNodeConfig.Init.DestBurstLength = 1;
  pNodeConfig.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT0;
  pNodeConfig.Init.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  pNodeConfig.TriggerConfig.TriggerMode = DMA_TRIGM_BLOCK_TRANSFER;
  pNodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_RISING;
  pNodeConfig.TriggerConfig.TriggerSelection = GPDMA1_TRIGGER_TIM15_TRGO;
  pNodeConfig.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;
  pNodeConfig.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
  pNodeConfig.SrcAddress = (uint32_t)&src_buffer_gpio_control_falling;
  pNodeConfig.DstAddress = (uint32_t)&GPIOB->BSRR;
  pNodeConfig.DataSize = 4;

  /* Build EntryNode1 Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &EntryNode1);
  ret |= HAL_DMAEx_List_InsertNode_Tail(&QueueEntry1, &EntryNode1);


  /* ================= 3. ExecutionNode21: SPI TX (3 Bytes) ================= */
  pNodeConfig.Init.Request = GPDMA1_REQUEST_SPI2_TX;
  pNodeConfig.Init.Direction = DMA_MEMORY_TO_PERIPH;
  pNodeConfig.Init.SrcInc = DMA_SINC_INCREMENTED;
  pNodeConfig.Init.DestInc = DMA_DINC_FIXED;
  pNodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
  pNodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
  pNodeConfig.TriggerConfig.TriggerMode = DMA_TRIGM_BLOCK_TRANSFER;
  pNodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_MASKED;
  pNodeConfig.SrcAddress = (uint32_t)&src_buffer_node1;
  pNodeConfig.DstAddress = (uint32_t)&SPI2->TXDR;
  pNodeConfig.DataSize = 3;

  /* Build ExecutionNode21 Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &ExecutionNode21);
  ret |= HAL_DMAEx_List_InsertNode_Tail(&QueueEntry1, &ExecutionNode21);

  /* ================= 4. ExecutionNode22: SPI RX (3 Bytes) ================= */
  pNodeConfig.Init.Request = GPDMA1_REQUEST_SPI2_RX;
  pNodeConfig.Init.Direction = DMA_PERIPH_TO_MEMORY;
  pNodeConfig.Init.SrcInc = DMA_SINC_FIXED;
  pNodeConfig.Init.DestInc = DMA_DINC_INCREMENTED;
  pNodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
  pNodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
  pNodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_MASKED;
  pNodeConfig.SrcAddress = (uint32_t)&SPI2->RXDR;
  pNodeConfig.DstAddress = (uint32_t)&rx_buffer;
  pNodeConfig.DataSize = 3;

  /* Build ExecutionNode22 Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &ExecutionNode22);
  ret |= HAL_DMAEx_List_InsertNode_Tail(&QueueEntry1, &ExecutionNode22);


  /* ================= 6. ExitNode3: CS HIGH ================= */
  pNodeConfig.Init.Request = DMA_REQUEST_SW;
  pNodeConfig.Init.Direction = DMA_MEMORY_TO_MEMORY;
  pNodeConfig.Init.SrcInc = DMA_SINC_FIXED;
  pNodeConfig.Init.DestInc = DMA_DINC_FIXED;
  pNodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_WORD;
  pNodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_WORD;
  pNodeConfig.TriggerConfig.TriggerMode = DMA_TRIGM_BLOCK_TRANSFER;
  pNodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_MASKED;
  pNodeConfig.Init.TransferEventMode = DMA_TCEM_EACH_LL_ITEM_TRANSFER;
  pNodeConfig.SrcAddress = (uint32_t)&src_buffer_gpio_control_rising;
  pNodeConfig.DstAddress = (uint32_t)&GPIOB->BSRR;
  pNodeConfig.DataSize = 4;

  /* Build ExitNode3 Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &ExitNode3);
  ret |= HAL_DMAEx_List_InsertNode_Tail(&QueueEntry1, &ExitNode3);

  ret |= HAL_DMAEx_List_SetCircularModeConfig(&QueueEntry1, &EntryNode1);

  return ret;
}

