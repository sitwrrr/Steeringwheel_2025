#include "APP_CAN.h"
//#include "ui.h"

void decode(uint32_t ID,uint8_t *canData){
    switch(ID){
        case 0x08C1EF21:  //VCUtoMCU1
            racingCarData.l_motor_target_rpm = (canData[0] + canData[1] * 256)/2 - 10000;
            racingCarData.l_motor_target_torque= (canData[2] + canData[3] * 256);
            racingCarData.l_target_controlmodeorder = canData[4];
            racingCarData.l_gearstage = canData[5] & 0x03;
            racingCarData.dccur = (canData[6] + canData[7] * 256 ) * 0.1;
            break;
        case 0x08B1EF21:  //VCUtoMCU2
            racingCarData.r_motor_target_rpm = (canData[0] + canData[1] * 256)/2 - 10000;
            racingCarData.r_motor_target_torque = (canData[2] + canData[3] * 256);
            racingCarData.r_target_controlmodeorder = canData[4];
            racingCarData.r_gearstage = canData[5] & 0x03;
            //racingCarData.dccur = (canData[6] + canData[7] * 256 ) * 0.1;
            break;
        case 0x0CFFC6EF:  //MCU1toVCU1
            racingCarData.l_motor_rpm = (canData[0] + canData[1] * 256)/2 - 10000;
            racingCarData.l_motor_torque = (canData[2] + canData[3] * 256);
            racingCarData.l_controlmodeorder = canData[4];
            racingCarData.l_mcu_ready = canData[5] & 0x01;
            racingCarData.l_mcu_precharge_state = (canData[5] >> 1) & 0x01;
            racingCarData.l_mcu_wrong_code = (canData[5] >> 2) | ((canData[6] & 0x03) << 6);
            racingCarData.l_mcu_selftest_state = (canData[6] >> 7) & 0x01;
            racingCarData.l_mcu_alert = canData[7] & 0x03;
            break;
        case 0x0CB221EF:  //MCU2toVCU1
            racingCarData.r_motor_rpm = (canData[0] + canData[1] * 256)/2 - 10000;
            racingCarData.r_motor_torque = (canData[2] + canData[3] * 256);
            racingCarData.r_controlmodeorder = canData[4];
            racingCarData.r_mcu_ready = canData[5] & 0x01;
            racingCarData.r_mcu_precharge_state = (canData[5] >> 1) & 0x01;
            racingCarData.r_mcu_wrong_code = (canData[5] >> 2) | ((canData[6] & 0x03) << 6);
            racingCarData.r_mcu_selftest_state = (canData[6] >> 7) & 0x01;
            racingCarData.r_mcu_alert = canData[7] & 0x03;
            break;
        case 0x0CFFC7EF:  //MCU1toVCU2
            racingCarData.l_mcu_temp = canData[0] - 50;
            racingCarData.l_motor_temp = canData[1] - 50;
            racingCarData.lmcu_dcvol = (canData[2] + canData[3] * 256) * 0.1;
            racingCarData.lmcu_dccur = (canData[4] + canData[5] * 256) * 0.1 - 1600;
            racingCarData.lmcu_accur = (canData[6] + canData[7] * 256) * 0.1 - 1600;
            break;
        case 0x0CB321EF:  //MCU2toVCU2
            racingCarData.r_mcu_temp = canData[0] - 50;
            racingCarData.r_motor_temp = canData[1] - 50;
            racingCarData.rmcu_dcvol = (canData[2] + canData[3] * 256) * 0.1;
            racingCarData.rmcu_dccur = (canData[4] + canData[5] * 256) * 0.1 - 1600;
            racingCarData.rmcu_accur = (canData[6] + canData[7] * 256) * 0.1 - 1600;
            break;
    }

}


void decode_power(uint32_t ID,uint8_t *candata){
    switch(ID){
        case 0x211:
            racingCarData.FrontSpeed = candata[0];
            racingCarData.PedalTravel = candata[1];
            racingCarData.brakeTravel = candata[2];
            racingCarData.carTravel = candata[3];
            racingCarData.IMD_Safe = candata[4] & 0x01;
            racingCarData.BMS_Safe = (candata[4]>>1) & 0x01;
            racingCarData.Emer_Stop = (candata[4]>>2) & 0x01;
            racingCarData.Inertia_Swich = (candata[4]>>3) & 0x01;
            racingCarData.Brake_Overtr_Switch = (candata[4]>>4) & 0x01;
            racingCarData.HVD = (candata[4]>>5) & 0x01;
            racingCarData.LMCU = (candata[4]>>6) & 0x01;
            racingCarData.RMCU = (candata[4]>>7) & 0x01;
            racingCarData.BSPD_Safe = candata[5] & 0x01;
            racingCarData.gearMode = (candata[5]>>5) & 0x03;
            racingCarData.angle = candata[6] - 90;
            break;

        case 0x50:
            racingCarData.sensor_diff = candata[1];

            if(racingCarData.sensor_diff == 0x51){
                racingCarData.acc_x = ((int16_t)(candata[2] | candata[3] << 8))  / 32768.0f * 16.0f;
                racingCarData.acc_y = ((int16_t)(candata[4] | candata[5] << 8))  / 32768.0f * 16.0f;
                racingCarData.acc_z = ((int16_t)(candata[6] | candata[7] << 8)) / 32768.0f * 16.0f;
            }
            else if(racingCarData.sensor_diff == 0x53){
                racingCarData.roll = ((int16_t)(candata[2] | candata[3] << 8)) / 32768.0 * 180.0;
                racingCarData.pitch = ((int16_t)(candata[4] | candata[5] << 8)) / 32768.0 * 180.0;
                racingCarData.yaw = ((int16_t)(candata[6] | candata[7] << 8)) / 32768.0 * 180.0;
            }
            break;

        case 0x186040F3:
            racingCarData.BatVoltage = (candata[1] + candata[0] * 256) * 0.1;
            racingCarData.BatCurrent = (candata[3] + candata[2] * 256) * 0.1 - 1000;
            racingCarData.BatSoc = candata[4];
            racingCarData.BatSoh = candata[5];
            racingCarData.BatState = candata[6] >> 4;   //调试的时候注意一下
            racingCarData.BatAlmLv = candata[6] & 0xF;
            racingCarData.Batlife = candata[7];
            break;
        case 0x186140F3:
            racingCarData.MaxCellVolt = candata[0] * 256 + candata[1];
            racingCarData.MinCellVolt = candata[0] * 256 + candata[1];
            break;
    }
}

//void test() {
//    if (racingCarData.carTravel != 123) {
//        racingCarData.l_motor_rpm += 100;
//        racingCarData.r_motor_rpm += 100;
//    }
//    else {
//        racingCarData.l_motor_rpm -= 100;
//        racingCarData.r_motor_rpm -= 100;
//    }
//
//    if(racingCarData.l_motor_rpm >= 8000)
//        racingCarData.carTravel = 123;
//    else if (racingCarData.l_motor_rpm <= 1000)
//        racingCarData.carTravel = 0;
//
//    lv_label_set_text_fmt(objects.lmot_rpm,"%02d",racingCarData.l_motor_rpm);
//    lv_label_set_text_fmt(objects.rmot_rpm,"%02d",racingCarData.r_motor_rpm);
//}

/*
void keyControlCanSend()
{
	KeyControlData[0] = 000;
	if(HAL_GPIO_ReadPin(INPUT2_GPIO_Port, INPUT2_Pin) == RESET) KeyControlData[0] |= 0x01 << 1;
	if(HAL_GPIO_ReadPin(INPUT3_GPIO_Port, INPUT3_Pin) == RESET) KeyControlData[0] |= 0x01 << 2;
	if(HAL_GPIO_ReadPin(INPUT4_GPIO_Port, INPUT4_Pin) == RESET) KeyControlData[0] |= 0x01 << 3;
	
	
	CAN2_Send(0X155, KeyControlData);
	
	
}
*/

