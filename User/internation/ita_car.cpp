#include "ita_car.h"

#define encoder_resolution 400
void Class_EoCar::Init()
{
    //底盘初始化
    Chassis.Init();
    Gimbal.Init();
    N100.Init(&hcan1);
}

void Class_EoCar::TIM_Calculate_PeriodElapsedCallback()
{
    Chassis.TIM_Calculate_PeriodElapsedCallback();
    Gimbal.TIM_Calculate_PeriodElapsedCallback();
}