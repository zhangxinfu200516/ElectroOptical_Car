/**
 * @file drv_can.c
 * @author yssickjgd (1345578933@qq.com)
 * @brief ����SCUT-Robotlab��д��CANͨ�ų�ʼ������������
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

/* Includes ------------------------------------------------------------------*/

//#include "ita_chariot.h"
#include "drv_can.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

Struct_CAN_Manage_Object CAN1_Manage_Object = {0};
Struct_CAN_Manage_Object CAN2_Manage_Object = {0};

// CANͨ�ŷ��ͻ�����
uint8_t CAN1_0x1ff_Tx_Data[8];
uint8_t CAN1_0x200_Tx_Data[8];
uint8_t CAN1_0x2ff_Tx_Data[8];
uint8_t CAN1_0xxf1_Tx_Data[8];
uint8_t CAN1_0xxf2_Tx_Data[8];
uint8_t CAN1_0xxf3_Tx_Data[8];
uint8_t CAN1_0xxf4_Tx_Data[8];
uint8_t CAN1_0xxf5_Tx_Data[8];
uint8_t CAN1_0xxf6_Tx_Data[8];
uint8_t CAN1_0xxf7_Tx_Data[8];
uint8_t CAN1_0xxf8_Tx_Data[8];

uint8_t CAN2_0x1ff_Tx_Data[8];
uint8_t CAN2_0x200_Tx_Data[8];
uint8_t CAN2_0x2ff_Tx_Data[8];
uint8_t CAN2_0xxf1_Tx_Data[8];
uint8_t CAN2_0xxf2_Tx_Data[8];
uint8_t CAN2_0xxf3_Tx_Data[8];
uint8_t CAN2_0xxf4_Tx_Data[8];
uint8_t CAN2_0xxf5_Tx_Data[8];
uint8_t CAN2_0xxf6_Tx_Data[8];
uint8_t CAN2_0xxf7_Tx_Data[8];
uint8_t CAN2_0xxf8_Tx_Data[8];

uint8_t CAN_Supercap_Tx_Data[8];
uint8_t CAN_MiniPC_Tx_Data[8];

uint8_t CAN2_Gimbal_Tx_Chassis_Data[8];  //��̨�����̷��ͻ�����
uint8_t CAN2_Chassis_Tx_Gimbal_Data[8];   //���̸���̨���ͻ�����

//���̷ֱ���ĸ����ַ��ͽǶ����ٶ�����
uint8_t CAN1_0x1a_Tx_Streeing_Wheel_A_data[8];
uint8_t CAN1_0x1b_Tx_Streeing_Wheel_B_data[8];
uint8_t CAN1_0x1c_Tx_Streeing_Wheel_C_data[8];
uint8_t CAN1_0x1d_Tx_Streeing_Wheel_D_data[8];

//CAN_Massage_Unit Massage_queue[4] = 
//{
//    {&hcan2, 0x01a, CAN1_0x1a_Tx_Streeing_Wheel_A_data, 8},
//    {&hcan2, 0x01b, CAN1_0x1b_Tx_Streeing_Wheel_B_data, 8},
//    {&hcan2, 0x01c, CAN1_0x1c_Tx_Streeing_Wheel_C_data, 8},
//    {&hcan2, 0x01d, CAN1_0x1d_Tx_Streeing_Wheel_D_data, 8},
//};
static uint8_t CAN1_Tx_Index = 0;
//���̸���С�巢�͹�������
uint8_t CAN1_0x01E_Tx_Data[8];
//��̨c�巢��Image���CAN����
uint8_t CAN1_0x02E_TX_Data[8];
/*********LK��� ���ƻ�����***********/
uint8_t CAN1_0x141_Tx_Data[8];
uint8_t CAN1_0x142_Tx_Data[8];
uint8_t CAN1_0x143_Tx_Data[8];
uint8_t CAN1_0x144_Tx_Data[8];
uint8_t CAN1_0x145_Tx_Data[8];
uint8_t CAN1_0x146_Tx_Data[8];
uint8_t CAN1_0x147_Tx_Data[8];
uint8_t CAN1_0x148_Tx_Data[8];

uint8_t CAN2_0x141_Tx_Data[8];    
uint8_t CAN2_0x142_Tx_Data[8];
uint8_t CAN2_0x143_Tx_Data[8];
uint8_t CAN2_0x144_Tx_Data[8];
uint8_t CAN2_0x145_Tx_Data[8];    
uint8_t CAN2_0x146_Tx_Data[8];
uint8_t CAN2_0x147_Tx_Data[8];
uint8_t CAN2_0x148_Tx_Data[8];

/* Private function declarations ---------------------------------------------*/

/* function prototypes -------------------------------------------------------*/

/**
 * @brief ����CAN�Ĺ�����
 *
 * @param hcan CAN���
 * @param Object_Para ɸѡ�����0-27 | FIFOx | ID���� | ֡����
 * @param ID ��֤��
 * @param Mask_ID ������(0x3ff, 0x1fffffff)
 */
void can_filter_mask_config(CAN_HandleTypeDef *hcan, uint8_t Object_Para, uint32_t ID, uint32_t Mask_ID)
{
	
    //��⴫���Ƿ���ȷ
    assert_param(hcan != NULL);

	   //CAN��������ʼ���ṹ��
    CAN_FilterTypeDef can_filter_init_structure;
    //�˲������, 0-27, ��28���˲���
    can_filter_init_structure.FilterBank = Object_Para >> 3;
    //�˲���ģʽ������ID����ģʽ
    can_filter_init_structure.FilterMode = CAN_FILTERMODE_IDMASK;
    
	
    if ((Object_Para & 0x02))
    {   
        //29λ ��չ֡
			  // 32λ�˲�
        can_filter_init_structure.FilterScale = CAN_FILTERSCALE_32BIT;
        //��֤�� ��16bit
        can_filter_init_structure.FilterIdHigh = (ID << 3) >> 16;
        //��֤�� ��16bit
        can_filter_init_structure.FilterIdLow = ID << 3 | (Object_Para & 0x03) << 1;
        //������ ��16bit
        can_filter_init_structure.FilterMaskIdHigh = (Mask_ID << 3) >> 16;
        //������ ��16bit
        can_filter_init_structure.FilterMaskIdLow = Mask_ID << 3 | (0x03) << 1 ;
    }
    else
    {
        //11λ ��׼֡
			  // 32λ�˲�
        can_filter_init_structure.FilterScale = CAN_FILTERSCALE_16BIT;
        //��׼֡��֤�� ��16bit������
        can_filter_init_structure.FilterIdHigh = 0x0000 ; 
        //��֤�� ��16bit
		can_filter_init_structure.FilterIdLow =ID << 5 | (Object_Para & 0x02) << 4;  
        //��׼֡������ ��16bit������
        can_filter_init_structure.FilterMaskIdHigh =  0x0000 ;
        //������ ��16bit
        can_filter_init_structure.FilterMaskIdLow =(Mask_ID << 5) | 0x01 << 4 ; 
    }

    //�˲�����FIFO0��FIFO1
    can_filter_init_structure.FilterFIFOAssignment = (Object_Para >> 2) & 0x01;
    //�ӻ�ģʽѡ��ʼ��Ԫ , ǰ14����CAN1, ��14����CAN2
    can_filter_init_structure.SlaveStartFilterBank = 14;
    //ʹ���˲���
    can_filter_init_structure.FilterActivation = ENABLE;

    // ����������
    if(HAL_CAN_ConfigFilter(hcan, &can_filter_init_structure)!=HAL_OK)
    {
        //Error_Handler();
    }
	
}

/**
 * @brief ��ʼ��CAN����
 *
 * @param hcan CAN���
 * @param Callback_Function ����ص�����
 */
void CAN_Init(CAN_HandleTypeDef *hcan, CAN_Call_Back Callback_Function)
{
    if (hcan->Instance == CAN1)
    {
        CAN1_Manage_Object.CAN_Handler = hcan;
        CAN1_Manage_Object.Callback_Function = Callback_Function;					
//         can_filter_mask_config(hcan, CAN_FILTER(0) | CAN_FIFO_0 | CAN_STDID | CAN_DATA_TYPE, 0x200 ,0x7F8);  //ֻ����0x200-0x207
//         can_filter_mask_config(hcan, CAN_FILTER(1) | CAN_FIFO_1 | CAN_STDID | CAN_DATA_TYPE, 0x200, 0x7F8);
			can_filter_mask_config(hcan, CAN_FILTER(0) | CAN_FIFO_0 | CAN_STDID | CAN_DATA_TYPE, 0 ,0);
			can_filter_mask_config(hcan, CAN_FILTER(1) | CAN_FIFO_1 | CAN_STDID | CAN_DATA_TYPE, 0 ,0);
    }
    else if (hcan->Instance == CAN2)
    {
        CAN2_Manage_Object.CAN_Handler = hcan;
        CAN2_Manage_Object.Callback_Function = Callback_Function;
		can_filter_mask_config(hcan, CAN_FILTER(14) | CAN_FIFO_0 | CAN_STDID | CAN_DATA_TYPE, 0 ,0);  //ֻ����
			can_filter_mask_config(hcan, CAN_FILTER(14) | CAN_FIFO_0 | CAN_STDID | CAN_DATA_TYPE, 0 ,0);
//	    can_filter_mask_config(hcan, CAN_FILTER(15) | CAN_FIFO_1 | CAN_EXTID | CAN_DATA_TYPE, 0x200, 0x7F8);
    }
    /*�뿪��ʼģʽ*/
    HAL_CAN_Start(hcan);				
    
    /*���ж�*/
    HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);       //can ����fifo 0��Ϊ���ж�
	HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);       //can ����fifo 1��Ϊ���ж�
}

/**
 * @brief ��������֡
 *
 * @param hcan CAN���
 * @param ID ID
 * @param Data �����͵�����ָ��
 * @param Length ����
 * @return uint8_t ִ��״̬
 */
uint8_t CAN_Send_Data(CAN_HandleTypeDef *hcan, uint16_t ID, uint8_t *Data, uint16_t Length)
{
    CAN_TxHeaderTypeDef tx_header;
    uint32_t used_mailbox;

    //��⴫���Ƿ���ȷ
    assert_param(hcan != NULL);

    tx_header.StdId = ID;
    tx_header.ExtId = 0;
    tx_header.IDE = 0;
    tx_header.RTR = 0;
    tx_header.DLC = Length;

    return (HAL_CAN_AddTxMessage(hcan, &tx_header, Data, &used_mailbox));
}
/**
 * @brief ������չ֡
 *   
 * @param equipment_id �豸id
 * @param data2	������2����
 * @param cmd_id	����ָ��
 * @return ��չ֡id
 */
uint32_t EXT_ID_Set(uint8_t equipment_id,uint16_t data2,uint8_t cmd_id)
{
	uint32_t send_ext_id;
	send_ext_id	=	cmd_id<<24|data2<<8|equipment_id;
	return send_ext_id;
}

/**
 * @brief ������չ֡
 *
 * @param hcan CAN���
 * @param ID ID
 * @param Data �����͵�����ָ��
 * @param Length ����
 * @return uint8_t ִ��״̬
 */
uint8_t CAN_Send_EXT_Data(CAN_HandleTypeDef *hcan, uint32_t ID, uint8_t *Data, uint16_t Length)
{
    CAN_TxHeaderTypeDef tx_header;
    uint32_t used_mailbox;

    //��⴫���Ƿ���ȷ
    assert_param(hcan != NULL);

    tx_header.ExtId = ID;
    tx_header.StdId = 0;
    tx_header.IDE = 4;
    tx_header.RTR = 0;
    tx_header.DLC = Length;

    return (HAL_CAN_AddTxMessage(hcan, &tx_header, Data, &used_mailbox));
}
/**
 * @brief CAN��TIM��ʱ���жϷ��ͻص�����
 *
 */
uint8_t can_tx_status[5] = {1, 1, 1, 1, 1};
uint8_t can_tx = 1;
void TIM_CAN_PeriodElapsedCallback()
{
    static uint8_t mod2 = 0;
    mod2++;
    if(mod2 == 2)
    {
        CAN_Send_Data(&hcan1,0x142,CAN1_0x142_Tx_Data,8);
        mod2 = 0;
    }
}

/**
 * @brief HAL��CAN����FIFO0�ж�
 *
 * @param hcan CAN���
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    //ѡ��ص�����
    if (hcan->Instance == CAN1)
    {
        HAL_CAN_GetRxMessage(hcan, CAN_FILTER_FIFO0, &CAN1_Manage_Object.Rx_Buffer.Header, CAN1_Manage_Object.Rx_Buffer.Data);
        CAN1_Manage_Object.Callback_Function(&CAN1_Manage_Object.Rx_Buffer);
    }
    else if (hcan->Instance == CAN2)
    {
        HAL_CAN_GetRxMessage(hcan, CAN_FILTER_FIFO0, &CAN2_Manage_Object.Rx_Buffer.Header, CAN2_Manage_Object.Rx_Buffer.Data);
        CAN2_Manage_Object.Callback_Function(&CAN2_Manage_Object.Rx_Buffer);
    }
}

/**
 * @brief HAL��CAN����FIFO1�ж�
 *
 * @param hcan CAN���
 */
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    //ѡ��ص�����
    if (hcan->Instance == CAN1)
    {
        HAL_CAN_GetRxMessage(hcan, CAN_FILTER_FIFO1, &CAN1_Manage_Object.Rx_Buffer.Header, CAN1_Manage_Object.Rx_Buffer.Data);
        CAN1_Manage_Object.Callback_Function(&CAN1_Manage_Object.Rx_Buffer);
    }
    else if (hcan->Instance == CAN2)
    {
        HAL_CAN_GetRxMessage(hcan, CAN_FILTER_FIFO1, &CAN2_Manage_Object.Rx_Buffer.Header, CAN2_Manage_Object.Rx_Buffer.Data);
        CAN2_Manage_Object.Callback_Function(&CAN2_Manage_Object.Rx_Buffer);
    }
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
