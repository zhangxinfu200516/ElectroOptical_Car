/**
 * @file drv_uart.c
 * @author yssickjgd (1345578933@qq.com)
 * @brief ����SCUT-Robotlab��д��UARTͨ�ų�ʼ������������
 * @version 0.1
 * @date 2023-08-29 0.1 23��������
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "drv_uart.h"
#include "string.h"
/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

Struct_UART_Manage_Object UART1_Manage_Object = {0};
Struct_UART_Manage_Object UART2_Manage_Object = {0};
Struct_UART_Manage_Object UART3_Manage_Object = {0};
Struct_UART_Manage_Object UART4_Manage_Object = {0};
Struct_UART_Manage_Object UART5_Manage_Object = {0};
Struct_UART_Manage_Object UART6_Manage_Object = {0};
Struct_UART_Manage_Object UART7_Manage_Object = {0};
Struct_UART_Manage_Object UART8_Manage_Object = {0};

/* Private function declarations ---------------------------------------------*/

/* function prototypes -------------------------------------------------------*/

/**
 * @brief ��ʼ��UART
 *  HAL_UARTEx_ReceiveToIdle_DMA()�˺�������Ϊ�����¿������ڿ����жϺ�DMA����
 * @param huart UART���
 * @param Callback_Function ����ص�����
 */
void UART_Init(UART_HandleTypeDef *huart, UART_Call_Back Callback_Function, uint16_t Rx_Buffer_Length)
{
    if (huart->Instance == USART1)
    {
        UART1_Manage_Object.UART_Handler = huart;
        UART1_Manage_Object.Callback_Function = Callback_Function;
        UART1_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART1_Manage_Object.Rx_Buffer, UART1_Manage_Object.Rx_Buffer_Length);
    }
//    else if (huart->Instance == USART2)
//    {
//        UART2_Manage_Object.UART_Handler = huart;
//        UART2_Manage_Object.Callback_Function = Callback_Function;
//        UART2_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
//        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART2_Manage_Object.Rx_Buffer, UART2_Manage_Object.Rx_Buffer_Length);
//    }
//    else if (huart->Instance == USART3)
//    {
//        UART3_Manage_Object.UART_Handler = huart;
//        UART3_Manage_Object.Callback_Function = Callback_Function;
//        UART3_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
//        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART3_Manage_Object.Rx_Buffer, UART3_Manage_Object.Rx_Buffer_Length);
//        __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
//    }
//    else if (huart->Instance == UART4)
//    {
//        UART4_Manage_Object.UART_Handler = huart;
//        UART4_Manage_Object.Callback_Function = Callback_Function;
//        UART4_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
//        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART4_Manage_Object.Rx_Buffer, UART4_Manage_Object.Rx_Buffer_Length);
//    }
//    else if (huart->Instance == UART5)
//    {
//        UART5_Manage_Object.UART_Handler = huart;
//        UART5_Manage_Object.Callback_Function = Callback_Function;
//        UART5_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
//        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART5_Manage_Object.Rx_Buffer, UART5_Manage_Object.Rx_Buffer_Length);
//    }
    else if (huart->Instance == USART6)
    {
        UART6_Manage_Object.UART_Handler = huart;
        UART6_Manage_Object.Callback_Function = Callback_Function;
        UART6_Manage_Object.Rx_Buffer_Length = Rx_Buffer_Length;
        
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART6_Manage_Object.Rx_Buffer, UART6_Manage_Object.Rx_Buffer_Length);
        // __HAL_DMA_DISABLE_IT(&hdma_usart6_rx, DMA_IT_HT);
        // memset(UART6_Manage_Object.Rx_Buffer, 0, sizeof(UART6_Manage_Object.Rx_Buffer));
    }
}

/**
 * @brief ��������֡
 *
 * @param huart UART���
 * @param Data �����͵�����ָ��
 * @param Length ����
 * @return uint8_t ִ��״̬
 */
uint8_t UART_Send_Data(UART_HandleTypeDef *huart, uint8_t *Data, uint16_t Length)
{
    return (HAL_UART_Transmit_DMA(huart, Data, Length));
}

/**
 * @brief UART��TIM��ʱ���жϷ��ͻص�����
 *
 */
void TIM_UART_PeriodElapsedCallback()
{
    // UART1����ͨѶ
    //UART_Send_Data(&huart1, UART1_Manage_Object.Tx_Buffer,19);
}

/**
 * @brief HAL��UART����DMA�����ж�
 *
 * @param huart UART���
 * @param Size ����
 */

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{    
    //ֹͣDMA���� �����������
    //HAL_UART_DMAStop(huart);
    //ѡ��ص�����
    if (huart->Instance == USART1)
    {
        UART1_Manage_Object.Rx_Length = Size;
        UART1_Manage_Object.Callback_Function(UART1_Manage_Object.Rx_Buffer, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART1_Manage_Object.Rx_Buffer, UART1_Manage_Object.Rx_Buffer_Length);
    }
//    else if (huart->Instance == USART2)
//    {
//        UART2_Manage_Object.Rx_Length = Size;
//        UART2_Manage_Object.Callback_Function(UART2_Manage_Object.Rx_Buffer, Size);
//        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART2_Manage_Object.Rx_Buffer, UART2_Manage_Object.Rx_Buffer_Length);
//    }
//    else if (huart->Instance == USART3)
//    {
//        UART3_Manage_Object.Rx_Length = Size;
//        UART3_Manage_Object.Callback_Function(UART3_Manage_Object.Rx_Buffer, Size);
//        memset(UART3_Manage_Object.Rx_Buffer, 0, UART3_Manage_Object.Rx_Buffer_Length);
//        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART3_Manage_Object.Rx_Buffer, UART3_Manage_Object.Rx_Buffer_Length);
//        __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
//    }
//    else if (huart->Instance == UART4)
//    {
//        UART4_Manage_Object.Rx_Length = Size;
//        UART4_Manage_Object.Callback_Function(UART4_Manage_Object.Rx_Buffer, Size);
//        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART4_Manage_Object.Rx_Buffer, UART4_Manage_Object.Rx_Buffer_Length);
//    }
//    else if (huart->Instance == UART5)
//    {
//        UART5_Manage_Object.Rx_Length = Size;
//        UART5_Manage_Object.Callback_Function(UART5_Manage_Object.Rx_Buffer, Size);
//        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART5_Manage_Object.Rx_Buffer, UART5_Manage_Object.Rx_Buffer_Length);
//    }
    else if (huart->Instance == USART6)
    {
         UART6_Manage_Object.Rx_Length = Size;
        UART6_Manage_Object.Callback_Function(UART6_Manage_Object.Rx_Buffer, Size);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, UART6_Manage_Object.Rx_Buffer, UART6_Manage_Object.Rx_Buffer_Length);

    }
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
