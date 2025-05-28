#ifndef _N100_H_
#define _N100_H_
#include "drv_math.h"
#include "drv_can.h"
// 单位说明：

// 角速度：弧度/秒 (rad/s)

// 加速度：米/秒² (m/s²)，含重力分量

// 磁力计：毫高斯 (mG)

// 气压：帕斯卡 (Pa)

// 温度：摄氏度 (°C)

// 时间戳：微秒 (µs)
struct Struct_IMUData_Packet_t
{
    float gyroscope_x;          // unit: rad/s
    float gyroscope_y;          // unit: rad/s
    float gyroscope_z;          // unit: rad/s
    float accelerometer_x;      // m/s^2
    float accelerometer_y;      // m/s^2
    float accelerometer_z;      // m/s^2
    float magnetometer_x;       // mG
    float magnetometer_y;       // mG
    float magnetometer_z;       // mG
    float imu_temperature;      // C
    float Pressure;             // Pa
    float pressure_temperature; // C
    uint32_t Timestamp;         // us
} __attribute__((packed));

struct Struct_AHRSData_Packet_t
{
    float RollSpeed;   //unit: rad/s
	float PitchSpeed;  //unit: rad/s
	float HeadingSpeed;//unit: rad/s
	float Roll;        //unit: rad
	float Pitch;       //unit: rad
	float Heading;     //unit: rad
	float Qw;//w          //Quaternion
	float Qx;//x
	float Qy;//y
	float Qz;//z
	long long Timestamp; //unit: us
}__attribute__((packed));

struct Struct_Frame_Format_t
{
    uint8_t Frame_header;
    uint8_t Command_Type;
    uint8_t Data_Length;
    uint8_t Serial_Number;
    uint8_t Frame_header_CRC8;
    uint16_t Data_CRC16;
    uint8_t Data[48];
    uint8_t Frame_Tail;
}__attribute__((packed));
class Class_N100
{
public:
    Struct_Frame_Format_t  Frame_Format;
    void Init(CAN_HandleTypeDef *hcan);
    void CAN_RxCpltCallback(uint8_t *Rx_Data);
    void TIM_Process_PeriodElapsedCallback();
    void Data_Process();
protected:
    Struct_CAN_Manage_Object *CAN_Manage_Object;

    uint32_t Flag = 0;
    uint32_t Pre_Flag = 0;

    Struct_AHRSData_Packet_t AHRSData;
};

#endif /* _N100_H_ */