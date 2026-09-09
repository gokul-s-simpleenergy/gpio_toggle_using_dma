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

DMA_NodeTypeDef ExecutionNode0;
DMA_QListTypeDef QueueExecution0;
DMA_NodeTypeDef EntryNode1;
DMA_QListTypeDef QueueEntry1;
DMA_NodeTypeDef EntryNode2;
DMA_NodeTypeDef Node1;
DMA_NodeTypeDef ExitNode1;
DMA_QListTypeDef QueueExit2;
DMA_NodeTypeDef ExitNode2;
DMA_NodeTypeDef ExitNode3;

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

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/**
  * @brief  DMA Linked-list QueueExit2 configuration
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef MX_QueueExit2_Config(void)
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
  pNodeConfig.Init.DestInc = DMA_DINC_FIXED;
  pNodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_WORD;
  pNodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_WORD;
  pNodeConfig.Init.SrcBurstLength = 1;
  pNodeConfig.Init.DestBurstLength = 1;
  pNodeConfig.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT0;
  pNodeConfig.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  pNodeConfig.TriggerConfig.TriggerMode = DMA_TRIGM_SINGLE_BURST_TRANSFER ;
  pNodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_RISING;
  pNodeConfig.TriggerConfig.TriggerSelection = GPDMA1_TRIGGER_GPDMA1_CH0_TCF;   // the loop is broken in this case as we will not get tcf as only half of the DMA transfer is ony completed
  pNodeConfig.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;
  pNodeConfig.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
  pNodeConfig.SrcAddress = &src_buffer_timer4_ctrl;
  pNodeConfig.DstAddress = (uint32_t)&TIM4->CR1;
  pNodeConfig.DataSize = 4;

  /* Build ExitNode1 Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &ExitNode1);

  /* Insert ExitNode1 to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&QueueExit2, &ExitNode1);

  /* Set node configuration ################################################*/
  pNodeConfig.TriggerConfig.TriggerSelection = GPDMA1_TRIGGER_TIM4_TRGO;
  pNodeConfig.SrcAddress = &src_buffer_gpio_control_rising;
  pNodeConfig.DstAddress = (uint32_t)&GPIOB->BSRR;

  /* Build ExitNode2 Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &ExitNode2);

  /* Insert ExitNode2 to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&QueueExit2, &ExitNode2);

  /* Set node configuration ################################################*/
  pNodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_MASKED;
  pNodeConfig.SrcAddress = &src_buffer_timer3_ctrl;
  pNodeConfig.DstAddress = (uint32_t)&TIM3->CR1;

  /* Build ExitNode3 Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &ExitNode3);

  /* Insert ExitNode3 to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&QueueExit2, &ExitNode3);

  ret |= HAL_DMAEx_List_SetCircularModeConfig(&QueueExit2, &ExitNode1);

   return ret;
}

/**
  * @brief  DMA Linked-list QueueExecution0 configuration
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef MX_QueueExecution0_Config(void)
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
  pNodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
  pNodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
  pNodeConfig.Init.SrcBurstLength = 1;
  pNodeConfig.Init.DestBurstLength = 1;
  pNodeConfig.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT0;
  pNodeConfig.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  pNodeConfig.TriggerConfig.TriggerMode = DMA_TRIGM_BLOCK_TRANSFER;
  pNodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_RISING;
  pNodeConfig.TriggerConfig.TriggerSelection = GPDMA1_TRIGGER_TIM2_TRGO;
  pNodeConfig.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;
  pNodeConfig.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
  pNodeConfig.SrcAddress = src_buffer_node1;
  pNodeConfig.DstAddress = (uint32_t)&SPI2->TXDR;
  pNodeConfig.DataSize = 3;

  /* Build ExecutionNode0 Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &ExecutionNode0);

  /* Insert ExecutionNode0 to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&QueueExecution0, &ExecutionNode0);

  ret |= HAL_DMAEx_List_SetCircularMode(&QueueExecution0);

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

  /* Set node configuration ################################################*/
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
  pNodeConfig.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT0;
  pNodeConfig.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  pNodeConfig.TriggerConfig.TriggerMode = DMA_TRIGM_SINGLE_BURST_TRANSFER ;
  pNodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_RISING;
  pNodeConfig.TriggerConfig.TriggerSelection = GPDMA1_TRIGGER_TIM3_TRGO;
  pNodeConfig.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;
  pNodeConfig.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
  pNodeConfig.SrcAddress = &src_buffer_gpio_control_falling;
  pNodeConfig.DstAddress = (uint32_t)&GPIOB->BSRR;
  pNodeConfig.DataSize = 4;

  /* Build EntryNode1 Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &EntryNode1);

  /* Insert EntryNode1 to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&QueueEntry1, &EntryNode1);

  /* Set node configuration ################################################*/
  pNodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
  pNodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
  pNodeConfig.Init.TransferEventMode = DMA_TCEM_EACH_LL_ITEM_TRANSFER;
  pNodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_MASKED;
  pNodeConfig.SrcAddress = &src_buffer_timer2_ctrl;
  pNodeConfig.DstAddress = (uint32_t)&TIM2->CR1;

  /* Build EntryNode2 Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &EntryNode2);

  /* Insert EntryNode2 to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&QueueEntry1, &EntryNode2);

  ret |= HAL_DMAEx_List_SetCircularModeConfig(&QueueEntry1, &EntryNode1);

   return ret;
}

