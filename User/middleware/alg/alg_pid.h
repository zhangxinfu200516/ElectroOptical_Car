/**
 * @file alg_pid.h
 * @author yssickjgd (1345578933@qq.com)
 * @brief PID�㷨
 * @version 0.1
 * @date 2023-08-29 0.1 23��������
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

#ifndef ALG_PID_H
#define ALG_PID_H

#ifdef __cplusplus
/* Includes ------------------------------------------------------------------*/

#include "drv_math.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief ΢������
 *
 */
enum Enum_PID_D_First
{
    PID_D_First_DISABLE = 0,
    PID_D_First_ENABLE,
};

/**
 * @brief Reusable, PID�㷨
 *
 */
class Class_PID
{
public:
    void Init(float __K_P, float __K_I, float __K_D, float __K_F = 0.0f, float __I_Out_Max = 0.0f, float __Out_Max = 0.0f,  float __I_Variable_Speed_A = 0.0f, float __I_Variable_Speed_B = 0.0f, float __I_Separate_Threshold = 0.0f, float __D_T = 0.001f, float __Dead_Zone = 0.0f,Enum_PID_D_First __D_First = PID_D_First_DISABLE);

    inline float Get_Integral_Error();
    inline float Get_Out();

    inline void Set_K_P(float __K_P);
    inline void Set_K_I(float __K_I);
    inline void Set_K_D(float __K_D);
    inline void Set_K_F(float __K_F);
    inline void Set_I_Out_Max(float __I_Out_Max);
    inline void Set_Out_Max(float __Out_Max);
    inline void Set_I_Variable_Speed_A(float __Variable_Speed_I_A);
    inline void Set_I_Variable_Speed_B(float __Variable_Speed_I_B);
    inline void Set_I_Separate_Threshold(float __I_Separate_Threshold);
    inline void Set_Target(float __Target);
    inline void Set_Now(float __Now);
    inline void Set_Integral_Error(float __Integral_Error);

    void TIM_Adjust_PeriodElapsedCallback();

protected:
    //��ʼ����س���

    // PID��ʱ������, s
    float D_T;
    //����, Error�������ֵ�ڲ����
    float Dead_Zone;
    //΢������
    Enum_PID_D_First D_First;

    //����

    //�ڲ�����

    //֮ǰ�ĵ�ǰֵ
    float Pre_Now = 0.0f;
    //֮ǰ��Ŀ��ֵ
    float Pre_Target = 0.0f;
    //֮ǰ�����ֵ
    float Pre_Out = 0.0f;
    //ǰ�����
    float Pre_Error = 0.0f;

    //������

    //���ֵ
    float Out = 0.0f;
		//
		float Iout = 0.0f;
    //д����

    // PID��P
    float K_P = 0.0f;
    // PID��I
    float K_I = 0.0f;
    // PID��D
    float K_D = 0.0f;
    //ǰ��
    float K_F = 0.0f;

    //�����޷�, 0Ϊ������
    float I_Out_Max = 0;
    //����޷�, 0Ϊ������
    float Out_Max = 0;

    //���ٻ��ֶ����ڶ���ֵ, 0Ϊ������
    float I_Variable_Speed_A = 0.0f;
    //���ٻ��ֱ�������, 0Ϊ������
    float I_Variable_Speed_B = 0.0f;
    //���ַ�����ֵ����Ϊ����, 0Ϊ������
    float I_Separate_Threshold = 0.0f;

    //Ŀ��ֵ
    float Target = 0.0f;
    //��ǰֵ
    float Now = 0.0f;

    //��д����

    //����ֵ
    float Integral_Error = 0.0f;

    //�ڲ�����
};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief ��ȡ���ֵ
 *
 * @return float ���ֵ
 */
float Class_PID::Get_Integral_Error()
{
    return (Integral_Error);
}

/**
 * @brief ��ȡ���ֵ
 *
 * @return float ���ֵ
 */
float Class_PID::Get_Out()
{
    return (Out);
}

/**
 * @brief �趨PID��P
 *
 * @param __K_P PID��P
 */
void Class_PID::Set_K_P(float __K_P)
{
    K_P = __K_P;
}

/**
 * @brief �趨PID��I
 *
 * @param __K_I PID��I
 */
void Class_PID::Set_K_I(float __K_I)
{
    K_I = __K_I;
}

/**
 * @brief �趨PID��D
 *
 * @param __K_D PID��D
 */
void Class_PID::Set_K_D(float __K_D)
{
    K_D = __K_D;
}

/**
 * @brief �趨ǰ��
 *
 * @param __K_D ǰ��
 */
void Class_PID::Set_K_F(float __K_F)
{
    K_F = __K_F;
}

/**
 * @brief �趨�����޷�, 0Ϊ������
 *
 * @param __I_Out_Max �����޷�, 0Ϊ������
 */
void Class_PID::Set_I_Out_Max(float __I_Out_Max)
{
    I_Out_Max = __I_Out_Max;
}

/**
 * @brief �趨����޷�, 0Ϊ������
 *
 * @param __Out_Max ����޷�, 0Ϊ������
 */
void Class_PID::Set_Out_Max(float __Out_Max)
{
    Out_Max = __Out_Max;
}

/**
 * @brief �趨�����ڶ���ֵ, 0Ϊ������
 *
 * @param __I_Variable_Speed_A �����ڶ���ֵ, 0Ϊ������
 */
void Class_PID::Set_I_Variable_Speed_A(float __I_Variable_Speed_A)
{
    I_Variable_Speed_A = __I_Variable_Speed_A;
}

/**
 * @brief �趨��������, 0Ϊ������
 *
 * @param __I_Variable_Speed_B ��������, 0Ϊ������
 */
void Class_PID::Set_I_Variable_Speed_B(float __I_Variable_Speed_B)
{
    I_Variable_Speed_B = __I_Variable_Speed_B;
}

/**
 * @brief �趨���ַ�����ֵ����Ϊ����, 0Ϊ������
 *
 * @param __I_Separate_Threshold ���ַ�����ֵ����Ϊ����, 0Ϊ������
 */
void Class_PID::Set_I_Separate_Threshold(float __I_Separate_Threshold)
{
    I_Separate_Threshold = __I_Separate_Threshold;
}

/**
 * @brief �趨Ŀ��ֵ
 *
 * @param __Target Ŀ��ֵ
 */
void Class_PID::Set_Target(float __Target)
{
    Target = __Target;
}

/**
 * @brief �趨��ǰֵ
 *
 * @param __Now ��ǰֵ
 */
void Class_PID::Set_Now(float __Now)
{
    Now = __Now;
}

/**
 * @brief �趨����, һ�����ڻ�������
 *
 * @param __Set_Integral_Error ����ֵ
 */
void Class_PID::Set_Integral_Error(float __Integral_Error)
{
    Integral_Error = __Integral_Error;
}

#endif
#endif
/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
