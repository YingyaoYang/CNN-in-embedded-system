/*
 * camera.c
 *
 *  Created on: 2019年11月14日
 *      Author: YYY
 */
#include "camera.h"
uint8_t handle(LeNet5 *lenet);
int testing(LeNet5 *lenet,image test_data);

uint8_t image_camera[HEIGHT*WIDTH]={0};//图像数组
uint8_t image_handled[28][28] = {0};
static uint8_t order[10]=    //获取图像命令
{0x02,0x23,0x00,0x48,0xa8,  //QYF-BWSC03黑白摄像头模块使用串口通信，命令
0x00,0xa8,0x00,0x7f,0xff};  //格式见01_Doc/QYF-BWSC03黑白摄像头模块.zi

int cnt;
int sort_result = -1;
//====================================================================
//函数名称：camera_init
//函数返回：无
//参数说明：无
//功能概要：摄像头初始化
//====================================================================
//void camera_init()
//{
//    uart_init(UART_CAMERA, 115200); //初始化调试串口
//}
//====================================================================
//函数名称：camera_getimage
//函数返回：图像数组地址
//参数说明：无
//功能概要：从摄像头获得一副图像
//====================================================================
void CAMERA_USART_IRQHandler(void)
{

    /* If new data arrived. */
   // if ((kUSART_RxFifoNotEmptyFlag | kUSART_RxError) & USART_GetStatusFlags(UART_CAMERA))
    //{
        image_camera[cnt++] = USART_ReadByte(UART_CAMERA);

    //}
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

uint8_t* camera_getimage()
{



	//for( i=0;i<10;i++)
	USART_WriteBlocking(UART_CAMERA,order,10);
	cnt = 0;
	//for(i=0;i<HEIGHT*WIDTH;i++ )
	//{
	//	image[i] = USART_ReadByte(UART_CAMERA);
	//	USART_WriteByte(UART_CAMERA,image[i]);
	//}
	Delay_ms(5);
	return image_camera;
}
uint8_t handle(LeNet5 *lenet)
{
	uint8_t handled_width = 0;
	uint8_t handled_height = 0;
	uint8_t comp_width = 0;
	uint8_t comp_height = 0;
	uint8_t temp_min=255;


	for(handled_height = 0;handled_height<28;handled_height++)
	{
		comp_height = 3*handled_height+1;
		for(handled_width = 0;handled_width<28;handled_width++)
		{
			comp_width = 3*handled_width+1;
			for(int temp_height = comp_height-1,temp_comp = comp_height+2;temp_height<temp_comp;temp_height++)
			{
				for(int temp_width = comp_width-1,temp_comp_w = comp_width+2;temp_width<temp_comp_w;temp_width++)
				{
					if(image_camera[temp_height*84+temp_width]<temp_min)
					{
						temp_min = image_camera[temp_height*84+temp_width];
					}

				}
			}
			image_handled[handled_height][handled_width] = temp_min;
			temp_min = 255;
		}
	}

	sort_result = testing(lenet,image_handled);
	if(sort_result == 0)
		LCD_ShowString(150,300,RED,WHITE,"battery   ");
	else if(sort_result == 1)
		LCD_ShowString(150,300,RED,WHITE,"bottle    ");
	else if(sort_result == 2)
		LCD_ShowString(150,300,RED,WHITE,"cardboard ");
	else
		LCD_ShowString(150,300,RED,WHITE,"error     ");
	return sort_result;
}
int testing(LeNet5 *lenet,image test_data)
{
	int p = Predict(lenet,test_data,3);
	return p;
}
