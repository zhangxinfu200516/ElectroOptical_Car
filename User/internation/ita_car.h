#ifndef ITA_CAR_H
#define ITA_CAR_H
/* USER CODE BEGIN Includes */
#include "drv_can.h"
#include "drv_uart.h"
#include "crt_chassis.h"
#include "crt_gimbal.h"
#include "dvc_N100.h"
/* USER CODE BEGIN  */
class Class_EoCar;
// extern Class_EoCar EoCar;

class Class_EoCar
{
public:
    Class_Tricycle_Chassis Chassis;
    Class_Gimbal Gimbal;
    Class_N100 N100;
    void Init();
    void TIM_Calculate_PeriodElapsedCallback();
protected:
    void TIM_Unline_Protect_PeriodElapsedCallback();
    void TIM1msMod50_Alive_PeriodElapsedCallback();
};

#endif 
