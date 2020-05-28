
#include<malloc.h>
#include "stdio.h"
#include "string.h"
#include "ReadBmp.h"
#include "lenet.h"
#define BM 19778		// 位图的标志

uint8 image_handled[28][28] = { 0.0 };
int count_test = 0;
//获得图片的宽度,在18-21字节 
long getWidth(FILE* fp)
{
	long width;
	fseek(fp, 18, SEEK_SET);
	fread(&width, 1, 4, fp);
	return width;
} //获得图片的高度 ，在22-25字节
long getHeight(FILE* fp)
{
	long height;
	fseek(fp, 22, SEEK_SET);
	fread(&height, 1, 4, fp);
	return height;
}
//获得每个像素的位数,在28-29字节 
unsigned short getBit(FILE* fp)
{
	unsigned short bit;
	fseek(fp, 28, SEEK_SET);
	fread(&bit, 1, 2, fp);
	return bit;
}
//获得数据的起始位置
unsigned int getOffSet(FILE* fp)
{
	unsigned int OffSet;
	fseek(fp, 10L, SEEK_SET);
	fread(&OffSet, 1, 4, fp);
	return OffSet;
}
//获得图像的数据
void getData(FILE* fp, unsigned char* r, unsigned char* g, unsigned char* b, int number, int sort, int training_now, int testing_now)
{
	int i, j = 0;
	int stride = 252;

	uint8 black_weight = 0.6;
	uint8 white_weight = 0.4;
	uint8 sum_comp;//用来比较缩小的值与灰度中值
	int sum_cal = 0;//存放9格之和
	int temp_position_height = 0;//比较位置的高度
	int temp_position_width = 0;//比较位置的宽度

	unsigned char* pix = NULL;
	long height, width;
	//uint8 image_tempsave[84][84] = { 0 };
	height = getHeight(fp);
	width = getWidth(fp);
	//height = ;
	//width = 84;
	//fseek(fp, getOffSet(fp), SEEK_SET);	//找到位图的数据区
	fseek(fp, 54, SEEK_SET);	//找到位图的数据区
	stride = (24 * width + 31) / 8;	//对齐,计算一个width有多少个8位 
	stride = stride / 4 * 4;		//取四的倍数 r,g,b,alph 
							//写入数组 
	pix = (unsigned char*)calloc(252, sizeof(unsigned char));
	for (j = 0; j < height; j++)
	{
		fread(pix, 1, stride, fp);
		for (i = 0; i < width; i++)
		{
			*(r + (height - 1 - j) + i) = pix[i * 3 + 2];
			//*(g+(height-1-j)+i)=pix[i*3+1];	
			//*(b+(height-1-j)+i)=pix[i*3];
		}
	}
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width - 1; j++)
		{
			if (training_now != 0)
			{
				if (sort == 1)
					*(train_data_battery_500 + count_test)[i][j] = *(r + i + j);
				else if (sort == 2)
					*(train_data_bottle_500 + count_test)[i][j] = *(r + i + j);
				else if (sort == 3)
					*(train_data_cardboard_500 + count_test)[i][j] = *(r + i + j);

					printf("%3d ", *(train_data_bottle_500 + count_test)[i][j]);
			}
			else if (testing_now != 0)
			{
				if (sort == 1)
					*(test_data_battery_500 + count_test)[i][j] = *(r + i + j);
				else if (sort == 2)
					*(test_data_bottle_500 + count_test)[i][j] = *(r + i + j);
				else if (sort == 3)
					*(test_data_cardboard_500 + count_test)[i][j] = *(r + i + j);
			}
		}
		if (training_now != 0)
		{
			if (sort == 1)
				*(train_data_battery_500 + count_test)[i][j] = *(r + i + j);
			else if (sort == 2)
				*(train_data_bottle_500 + count_test)[i][j] = *(r + i + j);
			else if (sort == 3)
				*(train_data_cardboard_500 + count_test)[i][j] = *(r + i + j);
			printf("%3d ", *(train_data_bottle_500 + count_test)[i][j]);
			printf("\n");
		}
		else if (testing_now != 0)
		{
			if (sort == 1)
				*(test_data_battery_500 + count_test)[i][j] = *(r + i + j);
			else if (sort == 2)
				*(test_data_bottle_500 + count_test)[i][j] = *(r + i + j);
			else if (sort == 3)
				*(test_data_cardboard_500 + count_test)[i][j] = *(r + i + j);
		}
	}

	/*
	for (i = 0; i < (height / 3); i++)
	{
		temp_position_height = i * 3 + 1;
		for (j = 0; j < (width/3); j++)
		{
			temp_position_width = j * 3 + 1;
			for (int temp_height = temp_position_height - 1, temp_comp = temp_position_height + 2; temp_height < temp_comp; temp_height++)
			{
				for (int temp_width = temp_position_width - 1, temp_comp_w = temp_position_width + 2; temp_width < temp_comp_w; temp_width++)
				{
					sum_comp = image_tempsave[temp_height][temp_width];

					///if (sum_comp > 170 ||sum_comp<80)
					//	sum_cal += black_weight * sum_comp;
					//else
					//	sum_cal += white_weight * sum_comp;
					sum_cal += sum_comp;

					//sum_cal /= 9;
					//printf("%d\t", sum_cal);
				}
			}
			if (sort == 1)
			{
				train_data_cat[number][i][j] = sum_cal;
				//printf("%d\t", sum_cal);
				if (training_now == 1)
				{
				//	train_data_cat_500[count_test][i][j] = (float)sum_cal/9;
					*(train_data_cat_500 + count_test)[i][j] = sum_cal / 9;
				//	printf("%u\t", *(train_data_cat_500+count_test)[i][j]);

				}
				if (testing_now == 1)
					//test_data_cat_500[count_test][i][j] = sum_cal / 9 ;
					*(test_data_cat_500 + count_test)[i][j] = sum_cal / 9;
			}
			else if (sort == 2)
			{
				//train_data_dog[number][i][j] = sum_cal / 9 / 255;
				if(training_now == 2)
					*(train_data_dog_500+count_test)[i][j] = sum_cal / 9 ;
				if (training_now == 1)
					*(train_data_dog_500 + count_test)[i][j] = sum_cal / 9;
				if (testing_now == 2)
					//test_data_dog_500[count_test][i][j] = sum_cal / 9 / 255;
					*(test_data_dog_500 + count_test)[i][j] = sum_cal / 9;
			}

			sum_cal = 0;
		}
	}
	*/
	printf("%d\n", count_test);
	count_test++;
	if (count_test == 165)
		count_test = 0;
	free(pix);
}


void read(int number, int sort, int training_now, int testing_now)
{
	char path_head_train[110] = "C:\\Users\\YYY\\Desktop\\garbage-dataset-512-resized\\garbage-dataset-512-resized\\train_bmp\\battery";
	char path_head_train_1[110] = "C:\\Users\\YYY\\Desktop\\garbage-dataset-512-resized\\garbage-dataset-512-resized\\train_bmp\\bottle";
	char path_head_train_2[110] = "C:\\Users\\YYY\\Desktop\\garbage-dataset-512-resized\\garbage-dataset-512-resized\\train_bmp\\cardboard";
	char path_head_test[100] = "C:\\Users\\YYY\\Desktop\\garbage-dataset-512-resized\\garbage-dataset-512-resized\\test_bmp\\battery";
	char path_head_test_1[100] = "C:\\Users\\YYY\\Desktop\\garbage-dataset-512-resized\\garbage-dataset-512-resized\\test_bmp\\bottle";
	char path_head_test_2[100] = "C:\\Users\\YYY\\Desktop\\garbage-dataset-512-resized\\garbage-dataset-512-resized\\test_bmp\\cardboard";
	char path_tail[5] = ".bmp";
	int temp = 0;
	char char_temp[6];
	char temp_path[300] = { 0 };
	unsigned char* r, * g, * b;
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
	}
	if (testing_now != 0)
	{
		if (sort == 1)
			strcpy(temp_path, path_head_test);
		else if (sort == 2)
			strcpy(temp_path, path_head_test_1);
		else if (sort == 3)
			strcpy(temp_path, path_head_test_2);
	}
	strcat(temp_path, char_temp);
	strcat(temp_path, path_tail);
	FILE* fp = fopen(temp_path, "r");
	r = (unsigned char*)malloc(4000);
	b = (unsigned char*)malloc(4000);
	g = (unsigned char*)malloc(4000);
	getData(fp, r, g, b, number, sort, training_now, testing_now);

}