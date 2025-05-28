/**
 * @file dvc_LKmotor.cpp
 * @author lez
 * @brief lk������������
 * @version 0.1
 * @date 2024-07-1 0.1 24��������
 *
 * @copyright ZLLC 2024
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_lkmotor.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

//������������Ϣ
uint8_t LK_Motor_CAN_Message_Clear_Error[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb};
//ʹ�ܵ��
uint8_t LK_Motor_CAN_Message_Enter[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc};
//ʧ�ܵ��
uint8_t LK_Motor_CAN_Message_Exit[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd};
//���浱ǰ���λ��Ϊ���
uint8_t LK_Motor_CAN_Message_Save_Zero[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe};

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief ����CAN���ͻ�����
 *
 * @param hcan CAN���
 * @param __CAN_ID CAN ID
 * @return uint8_t* ������ָ��
 */
uint8_t *allocate_tx_data(CAN_HandleTypeDef *hcan, Enum_LK_Motor_ID __CAN_ID)
{
    uint8_t *tmp_tx_data_ptr;
    if (hcan == &hcan1)
    {
        switch (__CAN_ID)
        {
        case (LK_Motor_ID_0x141):
        {
            tmp_tx_data_ptr = CAN1_0x141_Tx_Data;
        }
        break;
        case (LK_Motor_ID_0x142):
        {
            tmp_tx_data_ptr = CAN1_0x142_Tx_Data;
        }
        break;
        case (LK_Motor_ID_0x143):
        {
            tmp_tx_data_ptr = CAN1_0x143_Tx_Data;
        }
        break;
        case (LK_Motor_ID_0x144):
        {
            tmp_tx_data_ptr = CAN1_0x144_Tx_Data;
        }
        break;
        case (LK_Motor_ID_0x145):
        {
            tmp_tx_data_ptr = CAN1_0x145_Tx_Data;
        }
        break;
        case (LK_Motor_ID_0x146):
        {
            tmp_tx_data_ptr = CAN1_0x146_Tx_Data;
        }
        break;
        case (LK_Motor_ID_0x147):
        {
            tmp_tx_data_ptr = CAN1_0x147_Tx_Data;
        }
        break;
        case (LK_Motor_ID_0x148):
        {
            tmp_tx_data_ptr = CAN1_0x148_Tx_Data;
        }
        break;
        }
    }
//    else if (hcan == &hcan2)
//    {
//        switch (__CAN_ID)
//        {
//        case (LK_Motor_ID_0x141):
//        {
//            tmp_tx_data_ptr = CAN2_0x141_Tx_Data;
//        }
//        break;
//        case (LK_Motor_ID_0x142):
//        {
//            tmp_tx_data_ptr = CAN2_0x142_Tx_Data;
//        }
//        break;
//        case (LK_Motor_ID_0x143):
//        {
//            tmp_tx_data_ptr = CAN2_0x143_Tx_Data;
//        }
//        break;
//        case (LK_Motor_ID_0x144):
//        {
//            tmp_tx_data_ptr = CAN2_0x144_Tx_Data;
//        }
//        break;
//        case (LK_Motor_ID_0x145):
//        {
//            tmp_tx_data_ptr = CAN2_0x145_Tx_Data;
//        }
//        break;
//        case (LK_Motor_ID_0x146):
//        {
//            tmp_tx_data_ptr = CAN2_0x146_Tx_Data;
//        }
//        break;
//        case (LK_Motor_ID_0x147):
//        {
//            tmp_tx_data_ptr = CAN2_0x147_Tx_Data;
//        }
//        break;
//        case (LK_Motor_ID_0x148):
//        {
//            tmp_tx_data_ptr = CAN2_0x148_Tx_Data;
//        }
//        break;
//        }
//    }
    return (tmp_tx_data_ptr);
}

/**
 * @brief �����ʼ��
 *
 * @param hcan �󶨵�CAN����
 * @param __CAN_ID �󶨵�CAN ID
 * @param __Control_Method ������Ʒ�ʽ, Ĭ�ϽǶ�
 * @param __Position_Offset ������ƫ��, Ĭ��0
 * @param __Omega_Max ����ٶ�, ������������
 * @param __Torque_Max ���Ť��, ������������
 */
void Class_LK_Motor::Init(CAN_HandleTypeDef *hcan, Enum_LK_Motor_ID __CAN_ID,  float __Omega_Max, int32_t __Position_Offset, float __Current_Max, Enum_LK_Motor_Control_Method __Control_Method)
{
    if (hcan->Instance == CAN1)
    {
        CAN_Manage_Object = &CAN1_Manage_Object;
    }
    else if (hcan->Instance == CAN2)
    {
        CAN_Manage_Object = &CAN2_Manage_Object;
    }
    CAN_ID = __CAN_ID;
    LK_Motor_Control_Method = __Control_Method;
    Position_Offset = __Position_Offset;
    Omega_Max = __Omega_Max;
    Current_Max = __Current_Max;
    CAN_Tx_Data = allocate_tx_data(hcan, __CAN_ID);
}

/**
 * @brief ���ݴ������
 *
 */
void Class_LK_Motor::Data_Process()
{
    //���ݴ������
    int32_t delta_encoder;
    uint16_t tmp_encoder;
    int16_t tmp_omega,tmp_current;
    Struct_LK_Motor_CAN_Rx_Data *tmp_buffer = (Struct_LK_Motor_CAN_Rx_Data *)CAN_Manage_Object->Rx_Buffer.Data;
    
    tmp_encoder = tmp_buffer->Encoder_Reverse;
    tmp_omega = tmp_buffer->Omega_Reverse;
    tmp_current = tmp_buffer->Current_Reverse;

    //����Ȧ�����ܽǶ�ֵ
    if(Start_Flag==0)
    {
        delta_encoder = tmp_encoder - Data.Pre_Encoder;
        if (delta_encoder < -(Position_Max / 2))
        {
            //������ת����һȦ
            Data.Total_Round++;
        }
        else if (delta_encoder > (Position_Max / 2))
        {
            //������ת����һȦ
            Data.Total_Round--;
        }        
    }
    Data.Total_Encoder = Data.Total_Round * Position_Max + tmp_encoder + Position_Offset;

    //������������Ϣ
    Data.CMD_ID = tmp_buffer->CMD_ID;
    //Data.Now_Angle = (float)Data.Total_Encoder / (float)Position_Max *360.0f; 
    Data.Now_Angle = (float)tmp_encoder / (float)Position_Max * 360.0f;
    Data.Now_Radian = Data.Now_Angle * DEG_TO_RAD;
    Data.Now_Omega_Angle = (float)tmp_omega / 6.0f * RPM_TO_DEG;//dps->rpm->��/s
    Data.Now_Omega_Radian = (float)tmp_omega  / 6.0f * RPM_TO_RADPS; 
    Data.Now_Current = tmp_current; 
    Data.Now_Temperature = tmp_buffer->Temperature_Centigrade;  

    //�洢Ԥ����Ϣ
    Data.Pre_Encoder = tmp_encoder;
    if(Start_Flag==0)   Start_Flag = 1;
}
void Class_LK_Motor::Cale_Now_Omage()
{
    static float Pre_Now_Angle = 0.0f;

    Transform_Omega = (Data.Now_Angle - Pre_Now_Angle) / 0.001f;
    Pre_Now_Angle = Data.Now_Angle;
}
void Class_LK_Motor::Output(void)
{
    switch(LK_Motor_Control_ID)
    {
        case(LK_Motor_Control_Torque):
            CAN_Tx_Data[0] = LK_Motor_Control_Torque;
            CAN_Tx_Data[4] = (int16_t)Out; 
            CAN_Tx_Data[5] = (int16_t)Out >> 8;
        break;
        case(LK_Motor_Control_Run):
            CAN_Tx_Data[0] = LK_Motor_Control_Run;
        break;
        case(LK_Motor_Control_Stop):
            CAN_Tx_Data[0] = LK_Motor_Control_Stop;
        break;
        case(LK_Motor_Control_Shut_Down):
            CAN_Tx_Data[0] = LK_Motor_Control_Shut_Down;
        break;
        case(LK_Motor_Control_Request_Data):
            CAN_Tx_Data[0] = LK_Motor_Control_Request_Data;
        break;
        case(LK_Motor_Control_OneRound_Position_PIDControler):
        {
            CAN_Tx_Data[0] = LK_Motor_Control_OneRound_Position_PIDControler;
            
            // 1. ����ת�����򣨼��� spinDirection �����Ա������
            CAN_Tx_Data[1] = spinDirection; // uint8_t ֱ�Ӹ�ֵ
            
            // 2. �ֽ� maxSpeed��uint16_t �� DATA[2]��DATA[3]��
            uint16_t maxSpeed = (uint16_t)Max_Speed; // ʵ��ֵ��Ӳ������Ա������ȡ
            CAN_Tx_Data[2] = *(uint8_t*)(&maxSpeed);    // ��8λ
            CAN_Tx_Data[3] = *((uint8_t*)(&maxSpeed) + 1); // ��8λ
            
            // 3. �ֽ� angleControl��uint32_t �� DATA[4]-DATA[7]��
            uint32_t angleControl = (uint32_t)(Target_Angle * 100.0f); // ʵ��ֵ��Ӳ������Ա������ȡ
            CAN_Tx_Data[4] = *(uint8_t*)(&angleControl);      // bit0-7
            CAN_Tx_Data[5] = *((uint8_t*)(&angleControl) + 1); // bit8-15
            CAN_Tx_Data[6] = *((uint8_t*)(&angleControl) + 2); // bit16-23
            CAN_Tx_Data[7] = *((uint8_t*)(&angleControl) + 3); // bit24-31
        }
        break;
        default:
        break;
    }   
}

/**
 * @brief CANͨ�Ž��ջص�����
 *
 * @param Rx_Data ���յ�����
 */
void Class_LK_Motor::CAN_RxCpltCallback(uint8_t *Rx_Data)
{
    //��������, �жϵ���Ƿ�����
    this->Flag += 1;

    Data_Process();
}

/**
 * @brief TIM��ʱ���ж϶��ڼ�����Ƿ���
 *
 */
void Class_LK_Motor::TIM_Alive_PeriodElapsedCallback()
{
    //�жϸ�ʱ������Ƿ���չ��������
    if (Flag == Pre_Flag)
    {
        //����Ͽ�����
        LK_Motor_Status = LK_Motor_Status_DISABLE;
    }
    else
    {
        //�����������
        LK_Motor_Status = LK_Motor_Status_ENABLE;
    }

    Pre_Flag = Flag;
}

/**
 * @brief TIM��ʱ���жϷ��ͳ�ȥ�Ļص�����
 *
 */
void Class_LK_Motor::TIM_Process_PeriodElapsedCallback()
{
    float Tmp_Now_Angle = Data.Now_Angle;
    while(Tmp_Now_Angle > 180.0f)
    {
        Tmp_Now_Angle -= 360.0f;
    }
    while (Tmp_Now_Angle < -180.0f)
    {
        Tmp_Now_Angle += 360.0f;
    }
    Transform_Angle = Tmp_Now_Angle;
    Cale_Now_Omage();

    switch (LK_Motor_Control_Method)
    {
        case (LK_Motor_Control_Method_TORQUE):
        {        
            Out = Target_Torque*Torque_Current/Current_Max*Current_Max_Cmd;
        }
        break;
        case (LK_Motor_Control_Method_OMEGA):
        {
            PID_Omega.Set_Target(Target_Omega_Angle);
            PID_Omega.Set_Now(Data.Now_Omega_Angle);
            PID_Omega.TIM_Adjust_PeriodElapsedCallback();

            Target_Current = PID_Omega.Get_Out();

            Out = Target_Current;
        }
        break;
        case (LK_Motor_Control_Method_ANGLE):
        {
            PID_Angle.Set_Target(Target_Angle);
            PID_Angle.Set_Now(Transform_Angle);
            PID_Angle.TIM_Adjust_PeriodElapsedCallback();

            Target_Omega_Angle = PID_Angle.Get_Out();

            PID_Omega.Set_Target(Target_Omega_Angle);
            PID_Omega.Set_Now(Data.Now_Omega_Angle);
            PID_Omega.TIM_Adjust_PeriodElapsedCallback();

            Target_Current = PID_Omega.Get_Out();

            Out = Target_Current;
        }
        break;
        default:
        {
            Out = 0.0f;
        }
        break;
    }    
    //��������
    Output();
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/

