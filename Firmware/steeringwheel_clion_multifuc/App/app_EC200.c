#include "app_EC200.h"

uint8_t UPSpeedFlag = 1;
#define  SPEED_RATIO  4 	//�����ٱ�
#define  PI  3.14	       	//Բ����
#define  WHEEL_R  0.2286	   	//���ְ뾶
#define  NUM_OF_TEETH 20.0    //���̳���

uint8_t EC200_MQTTInit(){

    //ec200 MQTT��ʼ��
    if(EC200_Init()){
        printf("EC200 Ready");
    }
    else{
        printf("EC200 Failed");
        return 0;
    }

    //MQTT�ͻ��˳�ʼ��
    IRQ_JudgEnable = 1;
    Tx_Flag = 2;
    printf("AT+QMTOPEN=0,\"nas.lyracing.cn\",1883\r\n");
    Tx_Flag = 1;
    printf("\n Waiting for the MQTT client Open");
    while(!QMTOPEN_Flag)
    {
        printf(".");
        osDelay(200);
    }
    printf("\r\n");
    printf("\nQMTOPEN OK.\r\n");
    osDelay(500);

    Tx_Flag = 2;
    printf("AT+QMTCONN=0,\"hello\",\"Rain\",\"20031010\"\r\n");
    Tx_Flag = 1;
    printf("\n Waiting for the MQTT client Conncet");
    while(!QMTCONN_Flag)
    {
        printf(".");
        osDelay(200);
    }
    printf("\r\n");
    printf("MQTT Conncet OK.\r\n");

    //���Ͳ���
    Tx_Flag = 2;
    printf("AT+QMTPUBEX=0,0,0,0,\"mqtt\",%d\r\n",4);
    osDelay(10);
    printf("%s","{1,10,10,3,1000,1000,2,50,2,2,0.01,0.01,0.01,10.01,4,4}");
    //1,%d,%d,%d,%d,%d,%f,%d,%d,%d,%f,%f,%f,%f}
    osDelay(10);

    Tx_Flag =1;
    printf("\nMQTT Client ready\n");

    return 1;
}

/*//FrontSpeed,PedalTravel,batAlarm,MotorSpeed,batTemp,batLevel,gearMode,carMode,time_Count,batVol,carTravel,mcu1Temp,mcu2Temp,breakTravel,lmotorTemp,rmotorTemp,lmotorSpeed,rmotorSpeed,motorTemp
void carDataUpdate()//ģ�������ܶ�����
{
	//ID:0X196
	racingCarData.gearMode = 2; //0:�յ�  1:���� 2��ǰ����
	racingCarData.carMode = 2;//�ٶ�ģʽ
	racingCarData.batTemp = 40;//����¶� 40���϶�
	racingCarData.batLevel = 100;//������ص��� 100%
	racingCarData.batVol = 450;//������ص�ѹ450V
	racingCarData.batAlarm = 0;//�޸澯

	//ID:0X191
	if(UPSpeedFlag)
	{
		racingCarData.lmotorSpeed+=100;         //����ת��  2Bit offset -10000rpm �ֱ���:0.5
		if(racingCarData.lmotorSpeed == 5500)
			UPSpeedFlag = 0;
	}
	else
	{
		racingCarData.lmotorSpeed-=100;
		if(racingCarData.lmotorSpeed == 0)
			UPSpeedFlag = 1;
	}
	//ID:0X194
	racingCarData.rmotorSpeed = racingCarData.lmotorSpeed;

	//	//ID:0X193
//	uint8_t FrontSpeed;          //ǰ�ֳ��� ��������Ϊ�ο����� 1Byte
//	uint8_t PedalTravel;         //����̤�忪��    1Byte
//	uint8_t brakeTravel;         //ɲ��̤�忪��    1Byte
//	uint8_t carTravel;           //�����ܶ�����    1Byte
//	uint16_t l_motor_torque      //����Ŀ��ת��  2Byte
//  uint16_t r_motor_torque      //�ҵ��Ŀ��ת��  2Byte
	//ID:0X193
	racingCarData.FrontSpeed = (int)(racingCarData.lmotorSpeed/SPEED_RATIO*PI*2*WHEEL_R*3.6/60);  //��ת�ٻ���Ϊ����
	if(UPSpeedFlag)
	{
		racingCarData.l_motor_torque = 1000;
		racingCarData.r_motor_torque = racingCarData.l_motor_torque;
		racingCarData.PedalTravel = 100; //����̤�忪��Ϊ100 ����
		racingCarData.brakeTravel = 0;
	}

	else
	{
		racingCarData.l_motor_torque = 0;
		racingCarData.r_motor_torque = racingCarData.l_motor_torque;
		racingCarData.PedalTravel =0;
		racingCarData.brakeTravel = 40;
	}
	racingCarData.carTravel+=5;

	//ID:0X192
	racingCarData.lmotorTemp = 40; //�����¶�   1Byte 0~150���϶� offset:-50
	racingCarData.mcu1Temp = 40;   //���������1�¶� 1Byte 0~150���϶� offset:-50
	//ID:0X195
	racingCarData.rmotorTemp = 40; //�ҵ���¶�   1Byte 0~150���϶� offset:-50
	racingCarData.mcu2Temp = 40;   //���������2�¶� 1Byte 0~150���϶� offset:-50

}
*/
void MQTT_Pubdata(char *MQTTdata){
    Tx_Flag = 2;
    //��������
    printf("AT+QMTPUBEX=0,0,0,0,\"mqtt\",%d\r\n",strlen(MQTTdata));

    osDelay(10);

    //��������
    printf("%s",MQTTdata);
    //printf("%c",0x1A);
    //printf("0x%02x\n", 0x1A);
    //osDelay(10);

    //������ʾ
    //Tx_Flag = 1;
    //printf("\nPUBdata:\n");
    //printf("%s\n",MQTTdata);
    //memset(MQTTdata,0x00,sizeof(*MQTTdata));
}

void jsonPack(void)//json��� �ֶ� heap̫Сһ���Դ������
{
    static uint8_t changeFlag;
    //char json0[] = "{\"cSpeed\": %d,\"Pos\": %d,\"bAlarm\": %d,\"lmSpeed\": %d,\"rmSpeed\": %d,\"bTemp\": %d,\"bLevel\": %d,\"gMode\": %d,\"cMode\": %d,\"acc_x"\:%f,\"acc_y"\:%f,\"acc_z"\:%f}";
    //char json1[] = "{\"lmTorque\":%d,\"rmTorque\":%d,\"batVol\": %d,\"carDistce\": %d,\"mcu1Temp\": %d,\"mcu2Temp\": %d,\"brakeTravel\": %d,\"lmoTemp\": %d,\"rmoTemp\": %d,\"LbatAlr"\:&d,\"angle"\:&d}";
    char json0[] = "{1,%d,%d,%d,%d,%d,%f,%d,%d,%f,%f,%f,%f,%d,%d}";
    char json1[] = "{2,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%d,%d}";
    char t_json[300];
    if(!changeFlag)
    {
        sprintf(t_json, json0, racingCarData.FrontSpeed,\
		racingCarData.PedalTravel, \
		racingCarData.BatAlmLv, \
		racingCarData.l_motor_rpm, \
		racingCarData.r_motor_rpm, \
		racingCarData.BatCurrent, \
		racingCarData.gearMode, \
		racingCarData.gearMode,\
        racingCarData.acc_x,\
        racingCarData.acc_y,\
        racingCarData.acc_z,\
        racingCarData.roll,\
        racingCarData.lmcu_dccur,\
        racingCarData.rmcu_dccur);

        changeFlag = 1;
    }

    else if(changeFlag)
    {
        sprintf(t_json, json1,racingCarData.l_motor_torque, \
		racingCarData.r_motor_torque, \
		racingCarData.BatCurrent, \
		racingCarData.carTravel, \
		racingCarData.l_mcu_temp, \
		racingCarData.r_mcu_temp, \
		racingCarData.brakeTravel, \
		racingCarData.l_motor_temp, \
		racingCarData.r_motor_temp,\
        racingCarData.angle,\
        racingCarData.yaw,\
        racingCarData.pitch,\
        racingCarData.lmcu_accur,\
        racingCarData.rmcu_accur);

        changeFlag = 0;
    }
    MQTT_Pubdata(t_json);
    memset(t_json,0x00,sizeof(t_json)); //�������98
}

