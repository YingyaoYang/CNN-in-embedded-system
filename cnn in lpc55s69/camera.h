/*
 * camera.h
 *
 *  Created on: 2019年11月14日
 *      Author: YYY
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "fsl_usart.h"      //用到uart构件
#include "lcd.h"
#include "lenet.h"
#include "fsl_debug_console.h"
//数据传输串口
#define      UART_CAMERA      USART0
#define CAMERA_USART_IRQHandler FLEXCOMM0_IRQHandler
#define CAMERA_USART_IRQn FLEXCOMM0_IRQn

//图像大小
#define 	WIDTH 		84    //图像宽度
#define 	HEIGHT		84   //图像高度
extern int sort_result;
//====================================================================
//函数名称：camera_init
//函数返回：无
//参数说明：无
//功能概要：摄像头初始化
//====================================================================
extern void camera_init();
extern void CAMERA_USART_IRQHandler(void);
//====================================================================
//函数名称：camera_getimage
//函数返回：图像数组地址
//参数说明：无
//功能概要：从摄像头获得一副图像
//====================================================================
extern uint8_t* camera_getimage();
extern uint8_t handle(LeNet5 *lenet);
#endif /* CAMERA_H_ */
