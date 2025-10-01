/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lvgl.h"
#include "screens.h"
#include "SLM_App.h"
#include "app_can.h"
#include "app_indev.h"
#include "iwdg.h"
#include "string.h"
#include "app_EC200.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for lvgl_meter */
osThreadId_t lvgl_meterHandle;
const osThreadAttr_t lvgl_meter_attributes = {
  .name = "lvgl_meter",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for lvgl */
osThreadId_t lvglHandle;
const osThreadAttr_t lvgl_attributes = {
  .name = "lvgl",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ws2812b */
osThreadId_t ws2812bHandle;
const osThreadAttr_t ws2812b_attributes = {
  .name = "ws2812b",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for iot */
osThreadId_t iotHandle;
const osThreadAttr_t iot_attributes = {
  .name = "iot",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for iot_upload */
osThreadId_t iot_uploadHandle;
const osThreadAttr_t iot_upload_attributes = {
  .name = "iot_upload",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for lvgl_mutex */
osMutexId_t lvgl_mutexHandle;
const osMutexAttr_t lvgl_mutex_attributes = {
  .name = "lvgl_mutex"
};
/* Definitions for Upld_data_event */
osEventFlagsId_t Upld_data_eventHandle;
const osEventFlagsAttr_t Upld_data_event_attributes = {
  .name = "Upld_data_event"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void entry_lvgl_meter(void *argument);
void entry_lvgl(void *argument);
void entry_ws2812b(void *argument);
void entry_iot(void *argument);
void entry_iot_upload(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of lvgl_mutex */
  lvgl_mutexHandle = osMutexNew(&lvgl_mutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of lvgl_meter */
  lvgl_meterHandle = osThreadNew(entry_lvgl_meter, NULL, &lvgl_meter_attributes);

  /* creation of lvgl */
  lvglHandle = osThreadNew(entry_lvgl, NULL, &lvgl_attributes);

  /* creation of ws2812b */
  ws2812bHandle = osThreadNew(entry_ws2812b, NULL, &ws2812b_attributes);

  /* creation of iot */
  iotHandle = osThreadNew(entry_iot, NULL, &iot_attributes);

  /* creation of iot_upload */
  //iot_uploadHandle = osThreadNew(entry_iot_upload, NULL, &iot_upload_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* creation of Upld_data_event */
  Upld_data_eventHandle = osEventFlagsNew(&Upld_data_event_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_entry_lvgl_meter */
/**
  * @brief  Function implementing the lvgl_meter thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_entry_lvgl_meter */
void entry_lvgl_meter(void *argument)
{
  /* USER CODE BEGIN entry_lvgl_meter */
//  uint32_t r_event;
  static uint8_t saf_cir_ofl_blink = 0;
  racingCarData.BatVoltage = 480.4;
  /* Infinite loop */
  for(;;)
  {
//      r_event = osEventFlagsWait(Upld_data_eventHandle,GUI_UPDATE_EVENT,osFlagsWaitAll,osWaitForever);
//      if(r_event) {
          Key_Num_Click = key_scan_click();
          Key_Num_Press = key_scan_press();

          if(racingCarData.RMCU==0)
              safety_circuit_offline = 1;
          else
              safety_circuit_offline = 0;

          if(racingCarData.BSPD_Safe == 0)
              lv_label_set_text(objects.history_log_fail,"BSPD_Safe");
          else if(racingCarData.IMD_Safe == 0)
              lv_label_set_text(objects.history_log_fail,"IMD_Safe");
          else if(racingCarData.BMS_Safe == 0)
              lv_label_set_text(objects.history_log_fail,"BMS_Safe");
          else if(racingCarData.Emer_Stop == 0)
              lv_label_set_text(objects.history_log_fail,"Emer_Stop");
          else if(racingCarData.Inertia_Swich == 0)
              lv_label_set_text(objects.history_log_fail,"Inertia_Swich");                                                                                                             
          else if(racingCarData.Brake_Overtr_Switch == 0)
              lv_label_set_text(objects.history_log_fail,"Brake_Overtr_Switch");
          else if(racingCarData.HVD == 0)
              lv_label_set_text(objects.history_log_fail,"HVD");
          else if(racingCarData.LMCU == 0)
              lv_label_set_text(objects.history_log_fail,"LMCU");
          else if(racingCarData.RMCU == 0)
              lv_label_set_text(objects.history_log_fail,"RMCU");

          //安全回路检测
          if(safety_circuit_offline == 1) {
              if (saf_cir_ofl_blink<=10) {
                  lv_obj_clear_flag(objects.saf_cir_ofl_label, LV_OBJ_FLAG_HIDDEN);
                  saf_cir_ofl_blink++;
              }
              else if(saf_cir_ofl_blink<=15 && saf_cir_ofl_blink >10) {
                  lv_obj_add_flag(objects.saf_cir_ofl_label, LV_OBJ_FLAG_HIDDEN);
                  saf_cir_ofl_blink++;
              }
              else
                  saf_cir_ofl_blink = 0;
          }
          else
              lv_obj_add_flag(objects.saf_cir_ofl_label,LV_OBJ_FLAG_HIDDEN);

          //重要消息显示
          lv_obj_add_flag(objects.history_log_fail,LV_OBJ_FLAG_HIDDEN);
          if(racingCarData.l_mcu_ready == 0 || Key_Num_Press == 4) {
              history_log_appear_flag = 1;
              lv_label_set_text_fmt(objects.history_log_lready,"%1d",racingCarData.l_mcu_ready);
              lv_label_set_text_fmt(objects.history_log_lprecharge,"%1d", racingCarData.l_mcu_precharge_state);
              lv_label_set_text_fmt(objects.history_log_lwrong_code,"%3d",racingCarData.l_mcu_wrong_code);
          }
          if(racingCarData.r_mcu_ready == 0 || Key_Num_Press == 4) {
              history_log_appear_flag = 1;
              lv_label_set_text_fmt(objects.history_log_rready,"%1d",racingCarData.r_mcu_ready);
              lv_label_set_text_fmt(objects.history_log_rprecharge,"%1d", racingCarData.r_mcu_precharge_state);
              lv_label_set_text_fmt(objects.history_log_rwrong_code,"%3d",racingCarData.r_mcu_wrong_code);
          }
          if(racingCarData.l_mcu_selftest_state == 0 || Key_Num_Press == 4) {
              history_log_appear_flag = 1;
              lv_label_set_text_fmt(objects.history_log_rselftest,"%1d",racingCarData.r_mcu_selftest_state);

          }
          if(racingCarData.l_mcu_alert != 0 || Key_Num_Press == 4) {
              history_log_appear_flag = 1;
              lv_label_set_text_fmt(objects.history_log_ralert,"%1d",racingCarData.r_mcu_alert);
          }
          if(racingCarData.r_mcu_selftest_state == 0 || Key_Num_Press == 4) {
              history_log_appear_flag = 1;
              lv_label_set_text_fmt(objects.history_log_rselftest,"%1d",racingCarData.r_mcu_selftest_state);
          }
          if(racingCarData.r_mcu_alert != 0 || Key_Num_Press == 4) {
              history_log_appear_flag = 1;
              lv_label_set_text_fmt(objects.history_log_ralert,"%1d",racingCarData.r_mcu_alert);
          }

          lv_label_set_text_fmt(objects.history_log_batstate,"%1d",racingCarData.BatState);

          if(racingCarData.BatAlmLv != 0 || Key_Num_Press == 4) {
              history_log_appear_flag = 1;
              lv_label_set_text_fmt(objects.history_log_batalmlv,"%1d",racingCarData.BatAlmLv);
          }

          if(racingCarData.l_mcu_ready == 1 && racingCarData.r_mcu_ready == 1 && racingCarData.l_mcu_selftest_state == 1 && racingCarData.r_mcu_selftest_state == 1 && racingCarData.l_mcu_alert == 0 && racingCarData.r_mcu_alert == 0 && racingCarData.BatAlmLv == 0) {
              history_log_appear_flag = 0;
          }

          lv_label_set_text_fmt(objects.history_log_rpm_difference,"%4d",racingCarData.l_motor_rpm-racingCarData.r_motor_rpm);

          if(history_log_appear_flag == 1) lv_obj_clear_flag(objects.history_log_panel,LV_OBJ_FLAG_HIDDEN);
          else lv_obj_add_flag(objects.history_log_panel,LV_OBJ_FLAG_HIDDEN);

          if(Key_Num_Press == 4) lv_obj_clear_flag(objects.history_log_panel,LV_OBJ_FLAG_HIDDEN);
          else lv_obj_add_flag(objects.history_log_panel,LV_OBJ_FLAG_HIDDEN);


          lv_label_set_text_fmt(objects.speed,"%02d",racingCarData.FrontSpeed);
          lv_label_set_text_fmt(objects.lmot_rpm,"%02d",racingCarData.l_motor_rpm);
          lv_label_set_text_fmt(objects.rmot_rpm,"%02d",racingCarData.r_motor_rpm);
          lv_label_set_text_fmt(objects.mcu1_temp,"%02d",racingCarData.l_mcu_temp);
          lv_label_set_text_fmt(objects.mcu2_temp,"%02d",racingCarData.r_mcu_temp);
          lv_label_set_text_fmt(objects.motor2_temp,"%02d",racingCarData.l_motor_temp);
          lv_label_set_text_fmt(objects.motor1_temp,"%02d",racingCarData.r_motor_temp);
          lv_label_set_text_fmt(objects.mincellvolt,"%04d",racingCarData.MinCellVolt);

          //测试系统bootloader IAP
          lv_label_set_text_fmt(objects.lap,"%d",key_scan_press());

          lv_label_set_text_fmt(objects.bat_volt,"%03d",racingCarData.BatVoltage);

          lv_bar_set_value(objects.soc,racingCarData.BatSoc,LV_ANIM_OFF);

          if(racingCarData.gearMode == 0){
              lv_label_set_text(objects.gear, "N");
          }
          else if(racingCarData.gearMode == 1){
              lv_label_set_text(objects.gear, "AC");
          }
          else if(racingCarData.gearMode == 2){
              lv_label_set_text(objects.gear, "SK");
          }
          else if(racingCarData.gearMode == 3){
              lv_label_set_text(objects.gear,"AU");
          }
          else if(racingCarData.gearMode == 4){
              lv_label_set_text(objects.gear,"EF");
          }
      //CAN1_Send(0x210,data);
//      }
//      osMutexRelease(lvgl_mutexHandle);
      osDelay(50);
  }
  /* USER CODE END entry_lvgl_meter */
}

/* USER CODE BEGIN Header_entry_lvgl */
/**
* @brief Function implementing the lvgl thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_entry_lvgl */
void entry_lvgl(void *argument)
{
  /* USER CODE BEGIN entry_lvgl */
  static uint8_t boot_steady = 0;
  /* Infinite loop */
  for(;;)
  {
      osMutexWait(lvgl_mutexHandle, osWaitForever);
      uint32_t time_till_next = lv_timer_handler();
      if(time_till_next == LV_NO_TIMER_READY) time_till_next = LV_DEF_REFR_PERIOD; /*try again soon because the other thread can make the timer ready*/
      HAL_IWDG_Refresh(&hiwdg1);

      if(key_scan_press() == 3){   //可以添加延时进入
          boot_steady++;
          lv_obj_clear_flag(objects.boot_steady,LV_OBJ_FLAG_HIDDEN);
          if(boot_steady == 17) {
              uint32_t i = 0;
              void (*SysMemBootJump)(void);        /* 声明一个函数指针 */
              __IO uint32_t BootAddr = 0x1FF09800; /* STM32H7的系统BootLoader地址 */

              /* 关闭全局中断 */
              __disable_irq(); // 关闭全局中断

              /* 关闭滴答定时器，复位到默认值 */
              SysTick->CTRL = 0;
              SysTick->LOAD = 0;
              SysTick->VAL = 0;

              /* 设置所有时钟到默认状态，使用HSI时钟 */
              HAL_RCC_DeInit();

              /* 关闭所有中断，清除所有中断挂起标志 */
              for (i = 0; i < 8; i++) {
                  NVIC->ICER[i] = 0xFFFFFFFF;
                  NVIC->ICPR[i] = 0xFFFFFFFF;
              }

              /* 使能全局中断 */
              __enable_irq(); // 开启全局中断
//              HAL_FLASH_Program();

              /* 跳转到系统BootLoader，首地址是MSP，地址+4是复位中断服务程序地址 */
              SysMemBootJump = (void (*)(void)) (*((uint32_t *) (BootAddr + 4)));

              /* 设置主堆栈指针 */
              __set_MSP(*(uint32_t *) BootAddr);

              /* 在RTOS工程，这条语句很重要，设置为特权级模式，使用MSP指针 */
              __set_CONTROL(0);

              /* 跳转到系统BootLoader */
              SysMemBootJump();

              /* 跳转成功的话，不会执行到这里，用户可以在这里添加代码 */
              while (1) {

              }
          }
      }
      else{
          lv_obj_add_flag(objects.boot_steady,LV_OBJ_FLAG_HIDDEN);
          boot_steady = 0;
      }

      osMutexRelease(lvgl_mutexHandle);
      osDelay(time_till_next); /* delay to avoid unnecessary polling */
  }
  /* USER CODE END entry_lvgl */
}

/* USER CODE BEGIN Header_entry_ws2812b */
/**
* @brief Function implementing the ws2812b thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_entry_ws2812b */
void entry_ws2812b(void *argument)
{
  /* USER CODE BEGIN entry_ws2812b */
    CanFilter_init();
    uint32_t r_event;
  /* Infinite loop */
  for(;;)
  {
      r_event = osEventFlagsWait(Upld_data_eventHandle,GUI_UPDATE_EVENT,osFlagsWaitAll,osWaitForever);
      if(r_event) {
          RPM_LED_Shine();
      }
      osDelay(50);
  }
  /* USER CODE END entry_ws2812b */
}

/* USER CODE BEGIN Header_entry_iot */
/**
* @brief Function implementing the iot thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_entry_iot */
void entry_iot(void *argument)
{
  /* USER CODE BEGIN entry_iot */
  /* Infinite loop */
  for(;;)
  {
      // event_bit = osEventFlagsWait(Upld_data_eventHandle,  //�ȴ��������ݣ��ղ����Ͳ�������һ����
      //							GUI_UPDATE_EVENT,
      //							osFlagsWaitAll,
      //							osWaitForever);
      MQTTClient_RdyFlag = EC200_MQTTInit();

      if(MQTTClient_RdyFlag)
          iot_uploadHandle = osThreadNew(entry_iot_upload, NULL, &iot_upload_attributes);
      //lv_event_send(ui_Iot,MQTT_INIT_OK,NULL);
      osDelay(500);
      if(PUBOK_Flag == 1){
          osThreadSuspend(iotHandle);
      }
  }
  /* USER CODE END entry_iot */
}

/* USER CODE BEGIN Header_entry_iot_upload */
/**
* @brief Function implementing the iot_upload thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_entry_iot_upload */
void entry_iot_upload(void *argument)
{
  /* USER CODE BEGIN entry_iot_upload */
    uint32_t event_bit;
  /* Infinite loop */
  for(;;)
  {
      event_bit = osEventFlagsWait(Upld_data_eventHandle,
                                   GUI_UPDATE_EVENT,
                                   osFlagsWaitAll,
                                   osWaitForever);
      if(event_bit){
          if(eTaskGetState(iotHandle) == eSuspended){
              if(PUBOK_Flag == 1 )
              {
                  //	  	 if(uploadFlag)     //�ϳ���ǵý⿪
                  //		  {
                  Tx_Flag = 1;
                  printf("\nuploadcardata\n");
                  jsonPack();
                  //		  }
                  //		  uploadFlag = 0;
              }
              else
                  osThreadResume(iotHandle);
          }
      }
    osDelay(50);
  }
  /* USER CODE END entry_iot_upload */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

