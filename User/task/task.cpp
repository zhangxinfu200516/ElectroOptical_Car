#include "task.h"
#include "ita_car.h"  
uint8_t CanRxFifoBuffer[7][8];
Class_EoCar EoCar;
void Chassis_Device_CAN1_Callback(Struct_CAN_Rx_Buffer *CAN_RxMessage)
{
    switch (CAN_RxMessage->Header.StdId)
    {
    case (0x142):
    {
        EoCar.Gimbal.LK_Motor.CAN_RxCpltCallback(CAN_RxMessage->Data);
    }
    break;
    default:
    {
        EoCar.N100.CAN_RxCpltCallback(CAN_RxMessage->Data);
        // static uint8_t acc_flag = 0;
        // static uint8_t data_num = 0;
        // static uint8_t last_count = 0;
        // if (CAN_RxMessage->Data[0] == 0xFC && last_count == 0xFD)
        // {
        //     data_num = 1;
        //     if (CAN_RxMessage->Data[1] == 0x41 && CAN_RxMessage->Data[2] == 0x30)
        //         acc_flag = 1;
        // }
        // last_count = CAN_RxMessage->Data[7];

        // if (data_num)
        // {
        //     for (auto i = 0; i < 8; i++)
        //     {
        //         CanRxFifoBuffer[data_num - 1][i] = CAN_RxMessage->Data[i];
        //     }
        //     data_num++;
        // }

        // if (acc_flag == 1 && data_num == 8)
        // {
        //     data_num = 0;
        //     acc_flag = 0;

        //     for (auto i = 0; i < 7; i++)
        //     {
        //         for (auto j = 0; j < 8; j++)
        //         {
        //             reinterpret_cast<uint8_t *>(&EoCar.N100.Frame_Format)[i * 8 + j] = CanRxFifoBuffer[i][j];
        //         }
        //     }
        // }
    }
    break;
    }
}
void Task_UART3_Callback(uint8_t *Buffer, uint16_t Length)
{
}
void Task1ms_TIM8_Callback()
{
    EoCar.TIM_Calculate_PeriodElapsedCallback();
    EoCar.N100.Data_Process();
    //can
    TIM_CAN_PeriodElapsedCallback();

    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3) == GPIO_PIN_SET)
    {
        EoCar.Gimbal.Set_Gimbal_Control_Type(Gimbal_Control_Type_Enable);
    }
    else
    {
        EoCar.Gimbal.Set_Gimbal_Control_Type(Gimbal_Control_Type_Disable);
    }
}

extern "C" void Task_Init(void)
{
    //集中总线can1/can2
    CAN_Init(&hcan1, Chassis_Device_CAN1_Callback);
    //定时器循环任务
    TIM_Init(&htim8, Task1ms_TIM8_Callback);
    //串口初始化
     UART_Init(&huart3, Task_UART3_Callback, 15); 
    /********************************* 交互层初始化 *********************************/
    EoCar.Init();
    /********************************* 使能调度时钟 *********************************/
    HAL_TIM_Base_Start_IT(&htim8);
    //PWM初始化
    HAL_TIM_PWM_Start(&htim10,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim11,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
}

void Task_Loop(void)
{
}