#include <stdio.h>
#include <stdlib.h>
#include "read.h"
int count_test = 0;
#pragma pack(2)
/*定义WORD为两个字节的类型*/
typedef unsigned short WORD;
/*定义DWORD为e四个字节的类型*/
typedef unsigned long DWORD;
/*位图文件头*/
typedef struct BMP_FILE_HEADER
{
	WORD bType; /* 文件标识符 */
	DWORD bSize; /* 文件的大小 */
	WORD bReserved1; /* 保留值,必须设置为0 */
	WORD bReserved2; /* 保留值,必须设置为0 */
	DWORD bOffset; /* 文件头的最后到图像数据位开始的偏移量 */
} BMPFILEHEADER;
/*位图信息头*/
typedef struct BMP_INFO
{
	DWORD bInfoSize; /* 信息头的大小 */
	DWORD bWidth; /* 图像的宽度 */
	DWORD bHeight; /* 图像的高度 */
	WORD bPlanes; /* 图像的位面数 */
	WORD bBitCount; /* 每个像素的位数 */
	DWORD bCompression; /* 压缩类型 */
	DWORD bmpImageSize; /* 图像的大小,以字节为单位 */
	DWORD bXPelsPerMeter; /* 水平分辨率 */
	DWORD bYPelsPerMeter; /* 垂直分辨率 */
	DWORD bClrUsed; /* 使用的色彩数 */
	DWORD bClrImportant; /* 重要的颜色数 */
} BMPINF;
/*彩色表*/
typedef struct RGB_QUAD
{
	WORD rgbBlue; /* 蓝色强度 */
	WORD rgbGreen; /* 绿色强度 */
	WORD rgbRed; /* 红色强度 */
	WORD rgbReversed; /* 保留值 */
} RGBQUAD;
int read(int number, int sort, int training_now, int testing_now)
{
	char path_head_train[110] = "C:\\Users\\yyy\\Desktop\\garbage-dataset-512-resized-bmp\\battery\\battery";
	char path_head_train_1[110] = "C:\\Users\\yyy\\Desktop\\garbage-dataset-512-resized-bmp\\bottle\\bottle";
	char path_head_train_2[110] = "C:\\Users\\yyy\\Desktop\\garbage-dataset-512-resized-bmp\\cardboard\\cardboard";
	char path_head_train_3[110] = "C:\\Users\\yyy\\Desktop\\garbage-dataset-512-resized-bmp\\cans\\cans";
	char path_head_test[110] = "C:\\Users\\yyy\\Desktop\\garbage-dataset-512-resized-bmp\\battery\\battery";
	char path_head_test_1[110] = "C:\\Users\\yyy\\Desktop\\garbage-dataset-512-resized-bmp\\bottle\\bottle";
	char path_head_test_2[110] = "C:\\Users\\yyy\\Desktop\\garbage-dataset-512-resized-bmp\\cardboard\\cardboard";
	char path_head_test_3[110] = "C:\\Users\\yyy\\Desktop\\garbage-dataset-512-resized-bmp\\cans\\cans";
	char path_tail[5] = ".bmp";
	int temp = 0;
	char char_temp[6];
	char temp_path[300] = { 0 };
	int count_flag;
	temp = number;
	itoa(temp, char_temp, 10);
	if (training_now != 0)
	{
		if (sort == 1)
			strcpy(temp_path, path_head_train);
		else if (sort == 2)
			strcpy(temp_path, path_head_train_1);
		else if (sort == 3)
			strcpy(temp_path, path_head_train_2);
		else if (sort == 4)
			strcpy(temp_path, path_head_train_3);

	}
	if (testing_now != 0)
	{
		if (sort == 1)
			strcpy(temp_path, path_head_test);
		else if (sort == 2)
			strcpy(temp_path, path_head_test_1);
		else if (sort == 3)
			strcpy(temp_path, path_head_test_2);
		else if (sort == 4)
			strcpy(temp_path, path_head_test_3);
	}
	strcat(temp_path, char_temp);
	strcat(temp_path, path_tail);

	int comp_w = 0;
	int comp_h = 59;
	int print_enter = 0;
	int temp_save[60][60];
	FILE* fp;
	BMPFILEHEADER fileHeader;
	BMPINF infoHeader;
	long offset, bmpImageSize, width, height, bytesPerPixel, size, bitCount;
	int i, j;
	// unsigned char **p;
	WORD c;
	if ((fp = fopen(temp_path, "r")) == NULL)
	{
		printf("Cann't open the file!\n");
		exit(0);
	}
	fseek(fp, 0, 0);
	fread(&fileHeader, sizeof(fileHeader), 1, fp);
	fread(&infoHeader, sizeof(infoHeader), 1, fp);
	//计算并输出位图数据的偏移量，图像的大小，宽度和高度，每个像素点所占的字节
	size = fileHeader.bSize;
	offset = fileHeader.bOffset;
	bmpImageSize = infoHeader.bmpImageSize;
	width = infoHeader.bWidth;
	height = infoHeader.bHeight;
	bitCount = infoHeader.bBitCount;
	bytesPerPixel = infoHeader.bBitCount / 8;
	//printf("%d %d %d %d %d %d\n", size, offset, bmpImageSize, width, height, bitCount, bytesPerPixel);
	//输出每个像素点所占字节中的内容
	fseek(fp, offset, 0);
	//c = fgetc(fp);
	while (!feof(fp))
	{
		c = fgetc(fp);
		temp_save[comp_h][comp_w++] = c;
		if (comp_w == 60)
		{
			comp_w = 0;
			comp_h--;
		}
	}
	//comp = 0;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width ; j++)
		{
			if (training_now != 0)
			{
				if (sort == 1)
					train_data_battery_500 [count_test][i][j] = temp_save[i][j];
				else if (sort == 2)
					train_data_bottle_500[count_test][i][j] = temp_save[i][j];
				else if (sort == 3)
				{
					train_data_cardboard_500[count_test][i][j] = temp_save[i][j];
				//	printf("%4d ", train_data_cardboard_500[count_test][i][j]);
				}	
				else if (sort == 4)
				{
					train_data_can_500[count_test][i][j] = temp_save[i][j];
				}
			}
			else if (testing_now != 0)
			{
				if (sort == 1)
				{
					test_data_battery_500[count_test][i][j] = temp_save[i][j];
					//		printf("%4d ", test_data_battery_500[count_test][i][j]);
				}
				else if (sort == 2)
				{
					count_flag = count_test - 50;
					test_data_bottle_500[count_flag][i][j] = temp_save[i][j];
				}
				else if (sort == 3)
				{
					count_flag = count_test - 100;
					test_data_cardboard_500[count_flag][i][j] = temp_save[i][j];
				}
				else if (sort == 4)
				{
					count_flag = count_test - 150;
					test_data_can_500[count_flag][i][j] = temp_save[i][j];
				}
			}
			
		}
	//	printf("\n\n");
	}
	
	printf("%d\n", count_test);
	count_test++;
	if (count_test == 250)
		count_test = 0;
	fclose(fp);
}

void sobel_start()//图像边缘提取
{
	int X, O, Y;
	int i, j;
	int i00, i01, i02;
	int i10, i11, i12;
	int i20, i21, i22;
	int w = 60;
	
	//itrate over entire image as a single ,continue raster line
	//read the data,28 is the width,26 is the height
	for (int m = 0; m < 250; m++)
	{
		/*
		///////////////////////////////
		train_battery
		//////////////////////////////
		*/
		int count = 0;
		for (i = 0; i < 60 - 2; i++)
		{
			for (j = 0; j < 60 - 2; j++)
				//read in the required 3*3 region from the input
			{
				i00 = train_data_battery_500[m][i][j];
				i01 = train_data_battery_500[m][i][j + 1];
				i02 = train_data_battery_500[m][i][j + 2];

				i10 = train_data_battery_500[m][i + 1][j];
				i11 = train_data_battery_500[m][i + 1][j + 1];
				i12 = train_data_battery_500[m][i + 1][j + 2];

				i20 = train_data_battery_500[m][i + 2][j];
				i21 = train_data_battery_500[m][i + 2][j + 1];
				i22 = train_data_battery_500[m][i + 2][j + 2];
				/*
					apply horizontal and vertical filter maskes,the final filter
					output is the sum of the absolute values of these filters
				*/
				X = -i00 + i20
					- 2 * i01 + 2 * i21
					- i02 + i22;
				Y = i00 + 2 * i10 + i20
					- i02 - 2 * i12 - i22;
				O = abs(X) + abs(Y);
				/*
				clamp to 8-bit range,the output is always positive to the absolute
				value,so we only need to check for overflow
				*/
				if (O > 255)
					O = 255;
				temp_save_sobel[count++] = O;
			}
		}
		count = 0;
		for (i = 1; i < 60 - 1; i++)
		{
			for (j = 1; j < 60 - 1; j++)
				//read in the required 3*3 region from the input
			{
				train_data_battery_500[m][i][j] = temp_save_sobel[count++];
			}
		}

		/*
		///////////////////////////////
		train_bottle
		//////////////////////////////
		*/
		count = 0;
		for (i = 0; i < 60 - 2; i++)
		{
			for (j = 0; j < 60 - 2; j++)
				//read in the required 3*3 region from the input
			{
				i00 = train_data_bottle_500[m][i][j];
				i01 = train_data_bottle_500[m][i][j + 1];
				i02 = train_data_bottle_500[m][i][j + 2];

				i10 = train_data_bottle_500[m][i + 1][j];
				i11 = train_data_bottle_500[m][i + 1][j + 1];
				i12 = train_data_bottle_500[m][i + 1][j + 2];

				i20 = train_data_bottle_500[m][i + 2][j];
				i21 = train_data_bottle_500[m][i + 2][j + 1];
				i22 = train_data_bottle_500[m][i + 2][j + 2];
				/*
					apply horizontal and vertical filter maskes,the final filter
					output is the sum of the absolute values of these filters
				*/
				X = -i00 + i20
					- 2 * i01 + 2 * i21
					- i02 + i22;
				Y = i00 + 2 * i10 + i20
					- i02 - 2 * i12 - i22;
				O = abs(X) + abs(Y);
				/*
				clamp to 8-bit range,the output is always positive to the absolute
				value,so we only need to check for overflow
				*/
				if (O > 255)
					O = 255;
				temp_save_sobel[count++] = O;
			}
		}
		count = 0;
		for (i = 1; i < 60 - 1; i++)
		{
			for (j = 1; j < 60 - 1; j++)
				//read in the required 3*3 region from the input
			{
				train_data_bottle_500[m][i][j] = temp_save_sobel[count++];
			}
		}

		/*
		///////////////////////////////
		train_cardboard
		//////////////////////////////
		*/
		count = 0;
		for (i = 0; i < 60 - 2; i++)
		{
			for (j = 0; j < 60 - 2; j++)
				//read in the required 3*3 region from the input
			{
				i00 = train_data_cardboard_500[m][i][j];
				i01 = train_data_cardboard_500[m][i][j + 1];
				i02 = train_data_cardboard_500[m][i][j + 2];

				i10 = train_data_cardboard_500[m][i + 1][j];
				i11 = train_data_cardboard_500[m][i + 1][j + 1];
				i12 = train_data_cardboard_500[m][i + 1][j + 2];

				i20 = train_data_cardboard_500[m][i + 2][j];
				i21 = train_data_cardboard_500[m][i + 2][j + 1];
				i22 = train_data_cardboard_500[m][i + 2][j + 2];
				/*
					apply horizontal and vertical filter maskes,the final filter
					output is the sum of the absolute values of these filters
				*/
				X = -i00 + i20
					- 2 * i01 + 2 * i21
					- i02 + i22;
				Y = i00 + 2 * i10 + i20
					- i02 - 2 * i12 - i22;
				O = abs(X) + abs(Y);
				/*
				clamp to 8-bit range,the output is always positive to the absolute
				value,so we only need to check for overflow
				*/
				if (O > 255)
					O = 255;
				temp_save_sobel[count++] = O;
			}
		}
		count = 0;
		for (i = 1; i < 60 - 1; i++)
		{
			for (j = 1; j < 60 - 1; j++)
				//read in the required 3*3 region from the input
			{
				train_data_cardboard_500[m][i][j] = temp_save_sobel[count++];
			}
		}
		/*
		///////////////////////////////
		train_can
		//////////////////////////////
		*/
		count = 0;
		for (i = 0; i < 60 - 2; i++)
		{
			for (j = 0; j < 60 - 2; j++)
				//read in the required 3*3 region from the input
			{
				i00 = train_data_can_500[m][i][j];
				i01 = train_data_can_500[m][i][j + 1];
				i02 = train_data_can_500[m][i][j + 2];

				i10 = train_data_can_500[m][i + 1][j];
				i11 = train_data_can_500[m][i + 1][j + 1];
				i12 = train_data_can_500[m][i + 1][j + 2];

				i20 = train_data_can_500[m][i + 2][j];
				i21 = train_data_can_500[m][i + 2][j + 1];
				i22 = train_data_can_500[m][i + 2][j + 2];
				/*
					apply horizontal and vertical filter maskes,the final filter
					output is the sum of the absolute values of these filters
				*/
				X = -i00 + i20
					- 2 * i01 + 2 * i21
					- i02 + i22;
				Y = i00 + 2 * i10 + i20
					- i02 - 2 * i12 - i22;
				O = abs(X) + abs(Y);
				/*
				clamp to 8-bit range,the output is always positive to the absolute
				value,so we only need to check for overflow
				*/
				if (O > 255)
					O = 255;
				temp_save_sobel[count++] = O;
			}
		}
		count = 0;
		for (i = 1; i < 60 - 1; i++)
		{
			for (j = 1; j < 60 - 1; j++)
				//read in the required 3*3 region from the input
			{
				train_data_can_500[m][i][j] = temp_save_sobel[count++];
			}
		}
	}
	for (int m = 0; m < 50; m++)
	{
		/*
		///////////////////////////////
		test_battery
		//////////////////////////////
		*/
		int count = 0;
		for (i = 0; i < 60 - 2; i++)
		{
			for (j = 0; j < 60 - 2; j++)
				//read in the required 3*3 region from the input
			{
				i00 = test_data_battery_500[m][i][j];
				i01 = test_data_battery_500[m][i][j + 1];
				i02 = test_data_battery_500[m][i][j + 2];

				i10 = test_data_battery_500[m][i + 1][j];
				i11 = test_data_battery_500[m][i + 1][j + 1];
				i12 = test_data_battery_500[m][i + 1][j + 2];

				i20 = test_data_battery_500[m][i + 2][j];
				i21 = test_data_battery_500[m][i + 2][j + 1];
				i22 = test_data_battery_500[m][i + 2][j + 2];
				/*
					apply horizontal and vertical filter maskes,the final filter
					output is the sum of the absolute values of these filters
				*/
				X = -i00 + i20
					- 2 * i01 + 2 * i21
					- i02 + i22;
				Y = i00 + 2 * i10 + i20
					- i02 - 2 * i12 - i22;
				O = abs(X) + abs(Y);
				/*
				clamp to 8-bit range,the output is always positive to the absolute
				value,so we only need to check for overflow
				*/
				if (O > 255)
					O = 255;
				temp_save_sobel[count++] = O;
			}
		}
		count = 0;
		for (i = 1; i < 60 - 1; i++)
		{
			for (j = 1; j < 60 - 1; j++)
				//read in the required 3*3 region from the input
			{
				test_data_battery_500[m][i][j] = temp_save_sobel[count++];
			}
		}
		/*
		///////////////////////////////
		test_cardboard
		//////////////////////////////
		*/
		count = 0;
		for (i = 0; i < 60 - 2; i++)
		{
			for (j = 0; j < 60 - 2; j++)
				//read in the required 3*3 region from the input
			{
				i00 = test_data_cardboard_500[m][i][j];
				i01 = test_data_cardboard_500[m][i][j + 1];
				i02 = test_data_cardboard_500[m][i][j + 2];

				i10 = test_data_cardboard_500[m][i + 1][j];
				i11 = test_data_cardboard_500[m][i + 1][j + 1];
				i12 = test_data_cardboard_500[m][i + 1][j + 2];

				i20 = test_data_cardboard_500[m][i + 2][j];
				i21 = test_data_cardboard_500[m][i + 2][j + 1];
				i22 = test_data_cardboard_500[m][i + 2][j + 2];
				/*
					apply horizontal and vertical filter maskes,the final filter
					output is the sum of the absolute values of these filters
				*/
				X = -i00 + i20
					- 2 * i01 + 2 * i21
					- i02 + i22;
				Y = i00 + 2 * i10 + i20
					- i02 - 2 * i12 - i22;
				O = abs(X) + abs(Y);
				/*
				clamp to 8-bit range,the output is always positive to the absolute
				value,so we only need to check for overflow
				*/
				if (O > 255)
					O = 255;
				temp_save_sobel[count++] = O;
			}
		}
		count = 0;
		for (i = 1; i < 60 - 1; i++)
		{
			for (j = 1; j <60 - 1; j++)
				//read in the required 3*3 region from the input
			{
				test_data_cardboard_500[m][i][j] = temp_save_sobel[count++];
			}
		}
	}
	for (int m = 0; m < 50; m++)
	{
		/*
		///////////////////////////////
		test_bottle
		//////////////////////////////
		*/
		int count = 0;
		for (i = 0; i < 60 - 2; i++)
		{
			for (j = 0; j < 60 - 2; j++)
				//read in the required 3*3 region from the input
			{
				i00 = test_data_bottle_500[m][i][j];
				i01 = test_data_bottle_500[m][i][j + 1];
				i02 = test_data_bottle_500[m][i][j + 2];

				i10 = test_data_bottle_500[m][i + 1][j];
				i11 = test_data_bottle_500[m][i + 1][j + 1];
				i12 = test_data_bottle_500[m][i + 1][j + 2];

				i20 = test_data_bottle_500[m][i + 2][j];
				i21 = test_data_bottle_500[m][i + 2][j + 1];
				i22 = test_data_bottle_500[m][i + 2][j + 2];
				/*
					apply horizontal and vertical filter maskes,the final filter
					output is the sum of the absolute values of these filters
				*/
				X = -i00 + i20
					- 2 * i01 + 2 * i21
					- i02 + i22;
				Y = i00 + 2 * i10 + i20
					- i02 - 2 * i12 - i22;
				O = abs(X) + abs(Y);
				/*
				clamp to 8-bit range,the output is always positive to the absolute
				value,so we only need to check for overflow
				*/
				if (O > 255)
					O = 255;
				temp_save_sobel[count++] = O;
			}
		}
		count = 0;
		for (i = 1; i < 60 - 1; i++)
		{
			for (j = 1; j < 60 - 1; j++)
				//read in the required 3*3 region from the input
			{
				test_data_bottle_500[m][i][j] = temp_save_sobel[count++];
			}
		}
		/*
		///////////////////////////////
		test_can
		//////////////////////////////
		*/
		count = 0;
		for (i = 0; i < 60 - 2; i++)
		{
			for (j = 0; j < 60 - 2; j++)
				//read in the required 3*3 region from the input
			{
				i00 = test_data_can_500[m][i][j];
				i01 = test_data_can_500[m][i][j + 1];
				i02 = test_data_can_500[m][i][j + 2];

				i10 = test_data_can_500[m][i + 1][j];
				i11 = test_data_can_500[m][i + 1][j + 1];
				i12 = test_data_can_500[m][i + 1][j + 2];

				i20 = test_data_can_500[m][i + 2][j];
				i21 = test_data_can_500[m][i + 2][j + 1];
				i22 = test_data_can_500[m][i + 2][j + 2];
				/*
					apply horizontal and vertical filter maskes,the final filter
					output is the sum of the absolute values of these filters
				*/
				X = -i00 + i20
					- 2 * i01 + 2 * i21
					- i02 + i22;
				Y = i00 + 2 * i10 + i20
					- i02 - 2 * i12 - i22;
				O = abs(X) + abs(Y);
				/*
				clamp to 8-bit range,the output is always positive to the absolute
				value,so we only need to check for overflow
				*/
				if (O > 255)
					O = 255;
				temp_save_sobel[count++] = O;
			}
		}
		count = 0;
		for (i = 1; i < 60 - 1; i++)
		{
			for (j = 1; j < 60 - 1; j++)
				//read in the required 3*3 region from the input
			{
				test_data_can_500[m][i][j] = temp_save_sobel[count++];
			}
		}
	}
	//for (int i = 0; i < 60; i++)
	//{
	//	printf("[");
	//	for (int j = 0; j < 60; j++)
	//	{
	//		printf("%d,", train_data_bottle_500[0][i][j]);
	//	}
	//	printf("]\n");
	//}
}

void saveTo30()
{
	for (int m = 0; m < 250; m++)
	{
		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				train_data_battery_500_30[m][i][j] = (train_data_battery_500[m][i * 2][j * 2] + train_data_battery_500[m][i * 2][j * 2 + 1] + train_data_battery_500[m][i * 2 + 1][j * 2] + train_data_battery_500[m][i * 2 + 1][j * 2 + 1]) / 4;
				train_data_bottle_500_30[m][i][j] = (train_data_bottle_500[m][i * 2][j * 2] + train_data_bottle_500[m][i * 2][j * 2 + 1] + train_data_bottle_500[m][i * 2 + 1][j * 2] + train_data_bottle_500[m][i * 2 + 1][j * 2 + 1]) / 4;
				train_data_can_500_30[m][i][j] = (train_data_can_500[m][i * 2][j * 2] + train_data_can_500[m][i * 2][j * 2 + 1] + train_data_can_500[m][i * 2 + 1][j * 2] + train_data_can_500[m][i * 2 + 1][j * 2 + 1]) / 4;
				train_data_cardboard_500_30[m][i][j] = (train_data_cardboard_500[m][i * 2][j * 2] + train_data_cardboard_500[m][i * 2][j * 2 + 1] + train_data_cardboard_500[m][i * 2 + 1][j * 2] + train_data_cardboard_500[m][i * 2 + 1][j * 2 + 1]) / 4;
			}
		}
	}
	for (int m = 0; m < 50; m++)
	{
		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				test_data_battery_500_30[m][i][j] = (test_data_battery_500[m][i * 2][j * 2] + test_data_battery_500[m][i * 2][j * 2 + 1] + test_data_battery_500[m][i * 2 + 1][j * 2] + test_data_battery_500[m][i * 2 + 1][j * 2 + 1]) / 4;
				test_data_bottle_500_30[m][i][j] = (test_data_bottle_500[m][i * 2][j * 2] + test_data_bottle_500[m][i * 2][j * 2 + 1] + test_data_bottle_500[m][i * 2 + 1][j * 2] + test_data_bottle_500[m][i * 2 + 1][j * 2 + 1]) / 4;
				test_data_can_500_30[m][i][j] = (test_data_can_500[m][i * 2][j * 2] + test_data_can_500[m][i * 2][j * 2 + 1] + test_data_can_500[m][i * 2 + 1][j * 2] + test_data_can_500[m][i * 2 + 1][j * 2 + 1]) / 4;
				test_data_cardboard_500_30[m][i][j] = (test_data_cardboard_500[m][i * 2][j * 2] + test_data_cardboard_500[m][i * 2][j * 2 + 1] + test_data_cardboard_500[m][i * 2 + 1][j * 2] + test_data_cardboard_500[m][i * 2 + 1][j * 2 + 1]) / 4;
			}
		}
	}
}

//测试用读取bmp的函数
//只读一个bmp文件进来测试
//由于内存问题，目前最大为140*140.
//由于bmp的格式问题，长度、宽度必须为4的倍数
void read_test()
{
	int temp = 0;
	int count_flag;
	int flag_douhao = 0;
	int comp_w = 0;
	int comp_h = 139;
	int print_enter = 0;
	int temp_save[140][140];
	FILE* fp;
	BMPFILEHEADER fileHeader;
	BMPINF infoHeader;
	long offset, bmpImageSize, width, height, bytesPerPixel, size, bitCount;
	int i, j;
	// unsigned char **p;
	WORD c;

	//在此处更改使用的图片
	if ((fp = fopen("C:\\Users\\yyy\\Desktop\\2222.bmp", "r")) == NULL)
	{
		printf("Cann't open the file!\n");
		exit(0);
	}
	fseek(fp, 0, 0);
	fread(&fileHeader, sizeof(fileHeader), 1, fp);
	fread(&infoHeader, sizeof(infoHeader), 1, fp);
	//计算并输出位图数据的偏移量，图像的大小，宽度和高度，每个像素点所占的字节
	size = fileHeader.bSize;
	offset = fileHeader.bOffset;
	bmpImageSize = infoHeader.bmpImageSize;
	width = infoHeader.bWidth;
	height = infoHeader.bHeight;
	bitCount = infoHeader.bBitCount;
	bytesPerPixel = infoHeader.bBitCount / 8;
	//printf("%d %d %d %d %d %d\n", size, offset, bmpImageSize, width, height, bitCount, bytesPerPixel);
	//输出每个像素点所占字节中的内容
	fseek(fp, offset, 0);
	//c = fgetc(fp);
	while (!feof(fp))
	{
		//printf("%4d ", c);
		c = fgetc(fp);
		temp_save[comp_h][comp_w++] = c;
		if (comp_w == 140)
		{
			comp_w = 0;
			comp_h--;
		}
	}
	//comp = 0;
	//图片数据载入内存
	for (i = 0; i < height; i++)
	{
		//printf("[");
		for (j = 0; j < width; j++)
		{	
			forsobel[0][i][j] = temp_save[i][j];
		}
	}
	//输出图片数据进行测试
	for (i = 0; i < 140; i++)
	{
		printf("[");
		for (j = 0; j < 140; j++)
		{
			printf("%d", forsobel[0][i][j]);
			if (j == 140 - 1)
			{
				flag_douhao = 1;

			}
			else
				printf(",");
		}
		printf("]");
		if (flag_douhao == 1)
		{
			printf(",");
			flag_douhao = 0;
		}
		printf("\n");
	}

	fclose(fp);
}
//函数名： sobel_test
//功能：测试用sobel函数
void sobel_test()
{
	int X, O, Y;
	int i00, i01, i02;
	int i10, i11, i12;
	int i20, i21, i22;
	int w = 140;
	int count = 0;
	int i, j;
	int flag_douhao1 = 0;
	int flag_douhao = 0;

	//sobel算子算法
	for (i = 0; i < 140 - 2; i++)
	{
		for (j = 0; j < 140 - 2; j++)
			//read in the required 3*3 region from the input
		{
			i00 = forsobel[0][i][j];
			i01 = forsobel[0][i][j + 1];
			i02 = forsobel[0][i][j + 2];

			i10 = forsobel[0][i + 1][j];
			i11 = forsobel[0][i + 1][j + 1];
			i12 = forsobel[0][i + 1][j + 2];

			i20 = forsobel[0][i + 2][j];
			i21 = forsobel[0][i + 2][j + 1];
			i22 = forsobel[0][i + 2][j + 2];
			/*
				apply horizontal and vertical filter maskes,the final filter
				output is the sum of the absolute values of these filters
			*/
			//X为对图片数据进行X轴上的sobel算子
			X = -i00 + i20
				- 2 * i01 + 2 * i21
				- i02 + i22;
			//Y为对图片数据进行Y轴上的sobel算子
			Y = i00 + 2 * i10 + i20
				- i02 - 2 * i12 - i22;
			//O为X和Y相加，若要单独输出X或Y，则只取其一
			O = abs(X) + abs(Y);
			/*
			clamp to 8-bit range,the output is always positive to the absolute
			value,so we only need to check for overflow
			*/
			if (O > 255)
				O = 255;
			temp_save_sobel[count++] = O;
		}
	}
	count = 0;
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	//打印sobel算子处理后的图片
	for (i = 1; i < 140 - 1; i++)
	{
		for (j = 1; j < 140 - 1; j++)
			//read in the required 3*3 region from the input
		{
			forsobel[0][i][j] = temp_save_sobel[count++];
		}
	}
	for (i = 0; i < 28; i++)
	{
		printf("[");
		for (j = 0; j < 28; j++)
		{
			printf("%d", forsobel[0][i][j]);
			if (j == 140 - 1)
			{
				flag_douhao1 = 1;
			}
			else
				printf(",");
		}
		printf("]");
		if (flag_douhao1 == 1)
		{
			printf(",");
			flag_douhao1 = 0;
		}
		printf("\n");
	}

}