/**
 * @file alg_pid.cpp
 * @author yssickjgd 1345578933@qq.com
 * @brief PID�㷨
 * @version 0.1
 * @date 2023-08-29 0.1 23��������
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "alg_pid.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief PID��ʼ��
 *
 * @param __K_P Pֵ
 * @param __K_I Iֵ
 * @param __K_D Dֵ
 * @param __K_F ǰ��
 * @param __I_Out_Max �����޷�
 * @param __Out_Max ����޷�
 * @param __D_T ʱ��Ƭ����
 */
void Class_PID::Init(float __K_P, float __K_I, float __K_D, float __K_F, float __I_Out_Max, float __Out_Max, float __I_Variable_Speed_A, float __I_Variable_Speed_B, float __I_Separate_Threshold,float __D_T, float __Dead_Zone,  Enum_PID_D_First __D_First)
{
    K_P = __K_P;
    K_I = __K_I;
    K_D = __K_D;
    K_F = __K_F;
    I_Out_Max = __I_Out_Max;
    Out_Max = __Out_Max;
    D_T = __D_T;
    Dead_Zone = __Dead_Zone;
    I_Variable_Speed_A = __I_Variable_Speed_A;
    I_Variable_Speed_B = __I_Variable_Speed_B;
    I_Separate_Threshold = __I_Separate_Threshold;
    D_First = __D_First;
}

/**
 * @brief PID����ֵ
 *
 * @return float ���ֵ
 */
void Class_PID::TIM_Adjust_PeriodElapsedCallback()
{
    // P���
    float p_out = 0.0f;
    // I���
    float i_out = 0.0f;
    // D���
    float d_out = 0.0f;
    // F���
    float f_out = 0.0f;
    //���
    float error;
    //����ֵ���
    float abs_error;
    //���Ա��ٻ���
    float speed_ratio;

    error = Target - Now;
    abs_error = Math_Abs(error);

    //�ж�����
    if (abs_error < Dead_Zone)
    {
        Target = Now;
        error = 0.0f;
        abs_error = 0.0f;
    }

    //����p��

    p_out = K_P * error;

    //����i��

    if (I_Variable_Speed_A == 0.0f && I_Variable_Speed_B == 0.0f)
    {
        //�Ǳ��ٻ���
        speed_ratio = 1.0f;
    }
    else
    {
        //���ٻ���
        if (abs_error <= I_Variable_Speed_A)
        {
            speed_ratio = 1.0f;
        }
        else if (I_Variable_Speed_A < abs_error && abs_error < I_Variable_Speed_B)
        {
            speed_ratio = (I_Variable_Speed_B - abs_error) / (I_Variable_Speed_B - I_Variable_Speed_A);
        }
        if (abs_error >= I_Variable_Speed_B)
        {
            speed_ratio = 0.0f;
        }
    }
    //�����޷�
    if (I_Out_Max != 0.0f)
    {
        Math_Constrain(&Integral_Error, -I_Out_Max / K_I, I_Out_Max / K_I);
    }
    if (I_Separate_Threshold == 0.0f)
    {
        //û�л��ַ���
        Integral_Error += speed_ratio * D_T * error;
        i_out = K_I * Integral_Error;
    }
    else
    {
        //���ַ���ʹ��
        if (abs_error < I_Separate_Threshold)
        {
            Integral_Error += speed_ratio * D_T * error;
            i_out = K_I * Integral_Error;
        }
        else
        {
            Integral_Error = 0.0f;
            i_out = 0.0f;
        }
    }
		Iout = i_out;
    //����d��

    if (D_First == PID_D_First_DISABLE)
    {
        //û��΢������
        d_out = K_D * (error - Pre_Error) / D_T;
    }
    else
    {
        //΢������ʹ��
        d_out = K_D * (Out - Pre_Out) / D_T;
    }

    //����ǰ��

    f_out = (Target - Pre_Target) * K_F;

    //�����ܹ������

    Out = p_out + i_out + d_out + f_out;
    //����޷�
    if (Out_Max != 0.0f)
    {
        Math_Constrain(&Out, -Out_Max, Out_Max);
    }

    //�ƺ���
    Pre_Now = Now;
    Pre_Target = Target;
    Pre_Out = Out;
    Pre_Error = error;
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
