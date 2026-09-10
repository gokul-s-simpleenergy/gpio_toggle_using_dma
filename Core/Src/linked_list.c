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

#define NUM_FRAMES 32

DMA_NodeTypeDef ExecutionNode_TX[NUM_FRAMES];
DMA_NodeTypeDef ExecutionNode_RX[NUM_FRAMES];
DMA_NodeTypeDef ExecutionNode_Trig[NUM_FRAMES];
DMA_QListTypeDef QueueExecution0;
DMA_NodeTypeDef EntryNode1;
DMA_QListTypeDef QueueEntry1;
DMA_NodeTypeDef EntryNode2;
DMA_NodeTypeDef Node1;
DMA_NodeTypeDef ExitNode2;
DMA_QListTypeDef QueueExit2;
DMA_NodeTypeDef ExitNode3;
DMA_NodeTypeDef KillNode;
DMA_QListTypeDef QueueKillswitch;

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
uint8_t rx_buffer[32][3];

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
  pNodeConfig.TriggerConfig.TriggerSelection = GPDMA1_TRIGGER_TIM4_TRGO;
  pNodeConfig.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;
  pNodeConfig.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
  pNodeConfig.SrcAddress = &src_buffer_gpio_control_rising;
  pNodeConfig.DstAddress = (uint32_t)&GPIOB->BSRR;
  pNodeConfig.DataSize = 4;

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

  ret |= HAL_DMAEx_List_SetCircularModeConfig(&QueueExit2, &ExitNode2);

   return ret;
}

/**
  * @brief  DMA Linked-list QueueKillswitch configuration
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef MX_QueueKillswitch_Config(void)
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
  pNodeConfig.TriggerConfig.TriggerSelection = GPDMA1_TRIGGER_TIM5_TRGO;
  pNodeConfig.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;
  pNodeConfig.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
  pNodeConfig.SrcAddress = &src_buffer_timer_stop;
  pNodeConfig.DstAddress = (uint32_t)&TIM3->CR1;
  pNodeConfig.DataSize = 4;

  /* Build KillNode Node */
  ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &KillNode);

  /* Insert KillNode to Queue */
  ret |= HAL_DMAEx_List_InsertNode_Tail(&QueueKillswitch, &KillNode);

  ret |= HAL_DMAEx_List_SetCircularMode(&QueueKillswitch);

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

  for (int i = 0; i < NUM_FRAMES; i++)
  {
    /* Set node configuration for TX Node i ####################################*/
    pNodeConfig.NodeType = DMA_GPDMA_LINEAR_NODE;
    pNodeConfig.Init.Request = GPDMA1_REQUEST_SPI2_TX;
    pNodeConfig.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
    pNodeConfig.Init.Direction = DMA_MEMORY_TO_PERIPH;
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
    pNodeConfig.SrcAddress = (uint32_t)&src_buffer_node1[i][0];
    pNodeConfig.DstAddress = (uint32_t)&SPI2->TXDR;
    pNodeConfig.DataSize = 3;

    /* Build ExecutionNode_TX Node */
    ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &ExecutionNode_TX[i]);

    /* Insert ExecutionNode_TX to Queue */
    ret |= HAL_DMAEx_List_InsertNode_Tail(&QueueExecution0, &ExecutionNode_TX[i]);

    /* Set node configuration for RX Node i ####################################*/
    pNodeConfig.NodeType = DMA_GPDMA_LINEAR_NODE;
    pNodeConfig.Init.Request = GPDMA1_REQUEST_SPI2_RX;
    pNodeConfig.Init.Direction = DMA_PERIPH_TO_MEMORY;
    pNodeConfig.Init.SrcInc = DMA_SINC_FIXED;
    pNodeConfig.Init.DestInc = DMA_DINC_INCREMENTED;
    pNodeConfig.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    pNodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_MASKED;
    pNodeConfig.SrcAddress = (uint32_t)&SPI2->RXDR;
    pNodeConfig.DstAddress = (uint32_t)&rx_buffer[i][0];
    pNodeConfig.DataSize = 3;

    /* Build ExecutionNode_RX Node */
    ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &ExecutionNode_RX[i]);

    /* Insert ExecutionNode_RX to Queue */
    ret |= HAL_DMAEx_List_InsertNode_Tail(&QueueExecution0, &ExecutionNode_RX[i]);

    /* Set node configuration for Timer 4 Start Node i #########################*/
    pNodeConfig.NodeType = DMA_GPDMA_LINEAR_NODE;
    pNodeConfig.Init.Request = DMA_REQUEST_SW;
    pNodeConfig.Init.Direction = DMA_MEMORY_TO_MEMORY;
    pNodeConfig.Init.SrcInc = DMA_SINC_FIXED;
    pNodeConfig.Init.DestInc = DMA_DINC_FIXED;
    pNodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_WORD;
    pNodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_WORD;
    pNodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_MASKED;
    pNodeConfig.SrcAddress = (uint32_t)&src_buffer_timer4_ctrl;
    pNodeConfig.DstAddress = (uint32_t)&TIM4->CR1;
    pNodeConfig.DataSize = 4;

    /* Build ExecutionNode_Trig Node */
    ret |= HAL_DMAEx_List_BuildNode(&pNodeConfig, &ExecutionNode_Trig[i]);

    /* Insert ExecutionNode_Trig to Queue */
    ret |= HAL_DMAEx_List_InsertNode_Tail(&QueueExecution0, &ExecutionNode_Trig[i]);
  }

  ret |= HAL_DMAEx_List_SetCircularModeConfig(&QueueExecution0, &ExecutionNode_TX[0]);

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

