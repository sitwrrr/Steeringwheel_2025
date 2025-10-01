#include "bsp_EC200.h"

uint8_t ATCommand_upload(char *uploadstr,char *targetstr){
    uint8_t i = 0;

    osDelay(RESPONSEWAITING);
    Tx_Flag = 2;
    printf("%s\r\n",uploadstr);
    osDelay(RESPONSEWAITING);
    while(strstr((const char *)Rx_string,(const char *)targetstr ) == NULL){
        printf("%s\r\n",uploadstr);
        osDelay(RESPONSEWAITING);
        i++;
        if(i>=5){
            Tx_Flag = 1;
            printf("/n%s Fail\n",uploadstr);
            return 0;
        }
    }
    Tx_Flag = 1;
    printf("\n%s OK\n",uploadstr);
    memset(Rx_string,0x00,sizeof(Rx_string)); //�������
    Rx_Count = 0;
    return 1;
}

uint8_t EC200_Init(void)
{
    uint16_t err_Count = 0;

    HAL_GPIO_WritePin(RST_4G_GPIO_Port,PWR_4G_Pin,GPIO_PIN_SET);					//�����������rst����
    HAL_GPIO_WritePin(RST_4G_GPIO_Port,RST_4G_Pin,GPIO_PIN_SET);
    osDelay(3000);																	//
    HAL_GPIO_WritePin(PWR_4G_GPIO_Port,PWR_4G_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PWR_4G_GPIO_Port,RST_4G_Pin,GPIO_PIN_RESET);				//4Gģ�鿪��
    HAL_UART_Receive_IT(&huart3,(uint8_t *)&Rx_buff,1);
    Tx_Flag = 1;
    IRQ_JudgEnable = 1;
    printf("\nec200 start\n");
    printf("...ready...\n");
    while(!EC200_RdyFlag){
        osDelay(1000);
        printf(".");
        err_Count++;
        if(err_Count>20){
            printf("restart fault");			//ʧ�ܺ󣬿���ֻ�ܿ������ϵ�

            return 0;
        }
    }
    printf("\nrestart success\n");

    //�������
    IRQ_JudgEnable = 0; 	//�����ж��ж�
    Tx_Flag = 2;
    printf("ATE0\r\n"); //�رջ���

    //CIMI,��鿨��
    //CSQ,����ź�
    //CGATT,������總����� 										/*��Ҫ*/
    return ATCommand_upload("AT+CIMI","460") \
		&  ATCommand_upload("AT+CSQ","CSQ") \
		&  ATCommand_upload("AT+CGATT?","+CGATT: 1");
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart->Instance == USART3){
        if(Rx_Count >= 255){
            Rx_Count = 0;
            memset(Rx_string,0x00,sizeof(Rx_string));
            HAL_UART_Transmit(&huart1,(uint8_t *)"overflow",10,0xffff);
        }
        else{
            Rx_string[Rx_Count++] = Rx_buff;
            if((Rx_string[Rx_Count-2] == 0x0D) && (Rx_string[Rx_Count-1] == 0x0A) && (Rx_Count != 2) && (IRQ_JudgEnable)){        //�жϽ�����

                HAL_UART_Transmit(&huart1, (uint8_t *)&Rx_string, Rx_Count-1,0xFFFF);
                while(HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX);

                //������Ч�ַ���
                if(strcmp((const char*)Rx_string,(const char*)" ") == 0 || strlen(Rx_string) == 0){
                    memset(Rx_string,0x00,sizeof(Rx_string));
                    HAL_UART_Receive_IT(&huart3, (uint8_t *)&Rx_string, 1);
                }

                //4Gģ�����
                if(strstr((const char*)Rx_string,(const char*)"RDY")){
                    EC200_RdyFlag = 1;
                    memset(Rx_string,0x00,sizeof(Rx_string));
                    HAL_UART_Receive_IT(&huart3, (uint8_t *)&Rx_string, 1);
                }

                //����IP
                if(strstr((const char *)Rx_string,(const char *)"OPEN")){
                    QMTOPEN_Flag = 1;
                    memset(Rx_string,0x00,sizeof(Rx_string));
                    HAL_UART_Receive_IT(&huart3, (uint8_t *)&Rx_string, 1);
                }

                //���ӿͻ���
                if(strstr((const char *)Rx_string,(const char *)"CONN")){
                    QMTCONN_Flag = 1;
                    memset(Rx_string,0x00,sizeof(Rx_string));
                    HAL_UART_Receive_IT(&huart3, (uint8_t *)&Rx_string, 1);
                }

                //MQTT����״̬�ж�
                if(strstr((const char *)Rx_string,(const char *)"PUBEX")){
                    PUBOK_Flag = 1;
                    memset(Rx_string,0x00,sizeof(Rx_string));
                    HAL_UART_Receive_IT(&huart3, (uint8_t *)&Rx_string, 1);

                }

                //ʧЧ�ж�
                if(strstr((const char *)Rx_string,(const char *)"ERROR")){
                    EC200_RdyFlag = 0;
                    QMTOPEN_Flag = 0;
                    QMTCONN_Flag = 0;
                    PUBOK_Flag = 0;
                    memset(Rx_string,0x00,sizeof(Rx_string));
                    HAL_UART_Receive_IT(&huart3, (uint8_t *)&Rx_string, 1);
                }

                Rx_Count = 0;
            }
        }
        HAL_UART_Receive_IT(&huart3,(uint8_t *)&Rx_buff,1);
    }

}
