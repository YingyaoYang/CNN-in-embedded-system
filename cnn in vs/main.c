/*
苏州大学高性能计算实验组
作者：杨迎尧
时间：2020-5-8
功能：LENET-5改版实现垃圾分类
*/
#include "lenet.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "read.h"

#define LENET_FILE 		"model.dat"  //保存或用以载入的模型名称

#define training_now 1//0表示不在训练
#define testing_now  1//0表示不在测试

#define batch_number 20

#define train_label_battery 0
#define train_label_bottle 1
#define train_label_cardboard 2
#define train_label_can 3

#define battery_SORT 1
#define bottle_SORT 2
#define cardboard_SORT 3
#define can_SORT 4

#define savethemodel 1

int test_number = 0;
int test_number_dog = 0;
int training_flag = 10;

int total_num = 0;                      
int stop = 0;
void training(LeNet5* lenet, image* train_data, uint8 train_label, int batch_size, int total_size)
{
	for (int i = 0, percent = 0; i <= total_size - batch_size; i += batch_size)
	{
		TrainBatch(lenet, train_data + i, train_label, batch_size);

		if (i * 100 / total_size > percent)
			printf("batchsize:%d\ttrain:%2d%%\n", batch_size, percent = i * 100 / total_size);
	}
}


int testing(LeNet5* lenet, image* test_data, uint8 test_label, int total_size)
{
	int right = 0, percent = 0;
	for (int i = 0; i < total_size; ++i)
	{
		uint8 l = test_label;
		int p = Predict(lenet, test_data[i], OUTPUT);
		right += l == p;
		if (i * 100 / total_size > percent)
			printf("test:%2d%%\n", percent = i * 100 / total_size);
	}
	return right;
}

int save(LeNet5* lenet, char filename[])
{

	FILE* fp = fopen(filename, "wb");
	if (!fp) return 1;
	fwrite(lenet, sizeof(LeNet5), 1, fp);
	fclose(fp);
	return 0;
}

int load(LeNet5* lenet, char filename[])
{
	FILE* fp = fopen(filename, "rb");
	if (!fp) return 1;
	fread(lenet, sizeof(LeNet5), 1, fp);
	fclose(fp);
	return 0;
}

void start()
{
	int temp_read = 0;
	int flag_douhao = 0;
	/*
	分配训练图集、标签集，测试图集、标签集空间
	*/

	train_data_battery_500 = (image60*)calloc(1000, sizeof(image60));
	train_data_bottle_500 = (image60*)calloc(1000, sizeof(image60));
	train_data_cardboard_500 = (image60*)calloc(1000, sizeof(image60));
	train_data_can_500 = (image60*)calloc(1000, sizeof(image60));
	test_data_battery_500 = (image60*)calloc(500, sizeof(image60));
	test_data_bottle_500 = (image60*)calloc(500, sizeof(image60));
	test_data_cardboard_500 = (image60*)calloc(500, sizeof(image60));
	test_data_can_500 = (image60*)calloc(500, sizeof(image60));

	train_data_battery_500_30 = (image30*)calloc(1000, sizeof(image30));
	train_data_bottle_500_30 = (image30*)calloc(1000, sizeof(image30));
	train_data_cardboard_500_30 = (image30*)calloc(1000, sizeof(image30));
	train_data_can_500_30 = (image30*)calloc(1000, sizeof(image30));
	test_data_battery_500_30 = (image30*)calloc(500, sizeof(image30));
	test_data_bottle_500_30 = (image30*)calloc(500, sizeof(image30));
	test_data_cardboard_500_30 = (image30*)calloc(500, sizeof(image30));
	test_data_can_500_30 = (image30*)calloc(500, sizeof(image30));

	forsobel = (image1*)calloc(500, sizeof(image1));
	int flag_space = 0;
	int count_read;
	/*
	判断是否读取到图集、标签集
	*/
	//read_test();
	//sobel_test();

	if (training_now == 1)
	{
		for (count_read = 1; count_read < 251; count_read++)
			read(count_read, battery_SORT, 1, 0);
		for (count_read = 1; count_read < 251; count_read++)
			read(count_read, bottle_SORT, 1, 0);	
		for (count_read = 1; count_read < 251; count_read++)
			read(count_read, cardboard_SORT, 1, 0);
		for (count_read = 1; count_read < 251; count_read++)
			read(count_read, can_SORT, 1, 0);
	}

	if (testing_now == 1)
	{
		for (count_read = 251; count_read < 301; count_read++)
			read(count_read, battery_SORT, 0, 1);
		for (count_read = 251; count_read < 301; count_read++)
			read(count_read, bottle_SORT, 0, 2);
		for (count_read = 251; count_read < 301; count_read++)
			read(count_read, cardboard_SORT, 0, 3);
		for (count_read = 251; count_read < 301; count_read++)
			read(count_read, can_SORT, 0, 4);
	}
	//expand();
	sobel_start();
	saveTo30();

	int number = 0;
	//for (int mm = 0; mm <200; mm++)
	//{
	//	//printf("%d\n", mm);
	//	for (int i = 0; i < 30; i++)
	//	{
	//		//printf("[");
	//		for (int j = 0; j < 30; j++)
	//		{
	//			//printf("%d", train_data_cardboard_500[mm][i][j]);
	//			if (test_data_cardboard_500_30[mm][i][j] == 0)
	//				number++;
	//		
	//			if (j == 30 - 1)
	//			{
	//				flag_douhao = 1;
	//			}
	//			//else
	//			//	printf(",");
	//		}
	//		//printf("]");
	//		if (flag_douhao == 1)
	//		{
	//		//	printf(",");
	//			flag_douhao = 0;
	//		}
	//		//printf("\n");
	//	}
	//	//printf("\n\n\n\n\n\n\n\n");
	//	printf("\n");
	//	if (number > 5)
	//	{
	//		printf("bottle %d....%d", mm, number);
	//	}
	//	number = 0;
	//}
	//


	//for (int mm = 0; mm < 10; mm++)
	//{
	//	//printf("%d\n", mm);
	//	for (int i = 0; i < 30; i++)
	//	{
	//		printf("[");
	//		for (int j = 0; j < 30; j++)
	//		{
	//			printf("%d", test_data_can_500_30[mm][i][j]);
	//					
	//			if (j == 30 - 1)
	//			{
	//				flag_douhao = 1;
	//			}
	//			else
	//				printf(",");
	//		}
	//		printf("]");
	//		if (flag_douhao == 1)
	//		{
	//				printf(",");
	//			flag_douhao = 0;
	//		}
	//		printf("\n");
	//	}
	//	printf("\n\n\n\n\n\n\n\n");

	//}


	LeNet5* lenet = (LeNet5*)malloc(sizeof(LeNet5));
	if (load(lenet, LENET_FILE))
		Initial(lenet);
	//for (int i = 0; i < OUTPUT; i++)
	//	printf("%f,", lenet->bias4_5[i]);
	//printf("\n\n");
	clock_t start = clock();
    //for (int batch = 0; batch < batch_number; batch++)
	while(stop == 0)
	{
		total_num = 0;
		if (training_now == 1)
		{
			int batches[] = { 32 };
				training(lenet, train_data_battery_500_30, train_label_battery, batches[0], 250);
				
				training(lenet, train_data_bottle_500_30, train_label_bottle, batches[0], 250);
				training(lenet, train_data_bottle_500_30, train_label_bottle, batches[0], 250);
				training(lenet, train_data_cardboard_500_30, train_label_cardboard, batches[0], 250);
				training(lenet, train_data_cardboard_500_30, train_label_cardboard, batches[0], 250);
				training(lenet, train_data_can_500_30, train_label_can, batches[0], 250);
				training(lenet, train_data_can_500_30, train_label_can, batches[0], 250);
		}
		for (int i = 0; i <4; i++)
			printf("%f,", lenet->bias8_9[i]);

		if (testing_now == 1)
		{
			int right = testing(lenet, test_data_battery_500_30, train_label_battery, 50);
			printf("the battery%d/%d\n", right, 50);
			total_num += right;
			right = testing(lenet, test_data_bottle_500_30, train_label_bottle, 50);
			printf("the bottle%d/%d\n", right, 50);
			total_num += right;
			printf("\n\n\n\n\n\n");
			right = testing(lenet, test_data_cardboard_500_30, train_label_cardboard, 50);
			printf("the cardboard%d/%d\n", right, 50);
			total_num += right;
			printf("\n\n\n\n\n\n");
			right = testing(lenet, test_data_can_500_30, train_label_can, 50);
			printf("the can%d/%d\n", right, 50);
			total_num += right;
			printf("the total is %d/%d ",total_num,200);
			if (total_num > 80)
				stop = 1;
		}
		printf("Time:%u\n", (unsigned)(clock() - start));
	}
	
	save(lenet, LENET_FILE);

	if (savethemodel == 1)
	{
		FILE* fp = fopen("model_int.c", "w");

		fprintf(fp, "#include \"model_int.h\"\n");
		fprintf(fp, "#ifdef GLOBLE_VAR              //GLOBLE_VAR在main.c文件中有宏定义\n");
		fprintf(fp, "   #define G_VAR_PREFIX         //前缀G_VAR_PREFIX定义为空\n");
		fprintf(fp, "#else\n");
		fprintf(fp, "  #define G_VAR_PREFIX extern\n");
		fprintf(fp, "#endif\n");


		fprintf(fp, "G_VAR_PREFIX LeNet5 lenet_1 ={\n");
		for (int i = 0; i < INPUT; i++)//input 1
		{
			fprintf(fp, "{\n");//打印1次
			fprintf(fp, "	{\n");//打印1次
			for (int j = 0; j < LAYER1; j++)//3
			{
				fprintf(fp, "		{\n");//打印3次
				for (int m = 0; m < LENGTH_KERNEL; m++)//LENGTH_KERNEL=11
				{
					fprintf(fp, "			{");//打印11次
					for (int n = 0; n < LENGTH_KERNEL; n++)//11次
					{
						if (n + 1 != LENGTH_KERNEL)//11次
							fprintf(fp, "%f,", lenet->weight0_1[i][j][m][n]);
						else
							fprintf(fp, "%f", lenet->weight0_1[i][j][m][n]);
					}//打印第一层
					fprintf(fp, "}");

					if (m + 1 != LENGTH_KERNEL && j + 1 != LENGTH_KERNEL && i + 1 != LENGTH_KERNEL)
						fprintf(fp, ",");
					fprintf(fp, "\n");
				}
				fprintf(fp, "		}");

				if (j + 1 != LAYER1 && i + 1 != LENGTH_KERNEL)
					fprintf(fp, ",");

				fprintf(fp, "\n");
			}
			fprintf(fp, "	}\n");
			fprintf(fp, "}");
			if (i + 1 != LENGTH_KERNEL)
				fprintf(fp, ",");
			fprintf(fp, "\n");
		}//ok

		//开始第二层
		fprintf(fp, "\n\n\n\n\n\n\n\n");
		fprintf(fp, "{\n");//打印1次 1-1

		for (int i = 0; i < LAYER2; i++)//3次
		{
			fprintf(fp, "	{\n");//2-1 2-2 2-3 
			for (int j = 0; j < LAYER3; j++)//8次
			{
				fprintf(fp, "		{\n");//3-1 - 3-8
				for (int m = 0; m < LENGTH_KERNEL; m++)//11次
				{
					fprintf(fp, "			{");
					for (int n = 0; n < LENGTH_KERNEL; n++)//11
					{
						if (n + 1 != LENGTH_KERNEL)
							fprintf(fp, "%f,", lenet->weight2_3[i][j][m][n]);
						else
							fprintf(fp, "%f", lenet->weight2_3[i][j][m][n]);
					}
					fprintf(fp, "}");

					//if (m + 1 != LENGTH_KERNEL && j +1 != LAYER3 && i + 1 != LAYER2)
					if (m + 1 != LENGTH_KERNEL) //&& j + 1 != LAYER3 )
						fprintf(fp, ",");//?
					fprintf(fp, "\n");
				}
				fprintf(fp, "		}");
				if (j + 1 != LAYER3)//?
					fprintf(fp, ",");
				fprintf(fp, "\n");
			}


			fprintf(fp, "	}");//打印3次 2-1 2-2 2-3
			if (i + 1 != LAYER2)
				fprintf(fp, ",");


			fprintf(fp, "\n");
		}

		fprintf(fp, "\n},");//1-1


		//第3层ok
		fprintf(fp, "\n\n\n\n\n\n\n\n");
		fprintf(fp, "{\n");//打印1次 1-1
		for (int i = 0; i < LAYER3 * LENGTH_FEATURE3 * LENGTH_FEATURE3; i++)//8
		{
			fprintf(fp, "	{");
			for (int j = 0; j < LAYER4; j++)
			{
				if (j + 1 != LAYER4)
					fprintf(fp, "%f,", lenet->weight4_5[i][j]);
				else
					fprintf(fp, "%f", lenet->weight4_5[i][j]);
			}

			fprintf(fp, "}");

			if (i + 1 != LAYER3 * LENGTH_FEATURE3 * LENGTH_FEATURE3)
				fprintf(fp, ",");
			fprintf(fp, "\n");

		}
		fprintf(fp, "\n},");//打印1次 1-1
		fprintf(fp, "\n\n\n\n\n\n\n\n\n");

		//第4层ok
		fprintf(fp, "\n\n\n\n\n\n\n\n");
		fprintf(fp, "{\n");//打印1次 1-1
		for (int i = 0; i < LAYER4; i++)//8
		{
			fprintf(fp, "	{");
			for (int j = 0; j < LAYER5; j++)
			{
				if (j + 1 != LAYER5)
					fprintf(fp, "%f,", lenet->weight6_7[i][j]);
				else
					fprintf(fp, "%f", lenet->weight6_7[i][j]);
			}

			fprintf(fp, "}");

			if (i + 1 != LAYER4)
				fprintf(fp, ",");
			fprintf(fp, "\n");

		}
		fprintf(fp, "\n},");//打印1次 1-1
		fprintf(fp, "\n\n\n\n\n\n\n\n\n");

		//第5层ok
		fprintf(fp, "\n\n\n\n\n\n\n\n");
		fprintf(fp, "{\n");//打印1次 1-1
		for (int i = 0; i < LAYER5; i++)//8
		{
			fprintf(fp, "	{");
			for (int j = 0; j < OUTPUT; j++)
			{
				if (j + 1 != OUTPUT)
					fprintf(fp, "%f,", lenet->weight8_9[i][j]);
				else
					fprintf(fp, "%f", lenet->weight8_9[i][j]);
			}

			fprintf(fp, "}");

			if (i + 1 != OUTPUT)
				fprintf(fp, ",");
			fprintf(fp, "\n");

		}
		fprintf(fp, "\n},");//打印1次 1-1
		fprintf(fp, "\n\n\n\n\n\n\n\n\n");

		//打印每层偏置项
		fprintf(fp, "{");
		for (int i = 0; i < LAYER1; i++)//3
		{
			if (i + 1 != LAYER1)

				fprintf(fp, "%f,", lenet->bias0_1[i]);

			else
				fprintf(fp, "%f", lenet->bias0_1[i]);
		}
		fprintf(fp, "},");

		fprintf(fp, "\n\n");

		fprintf(fp, "{");
		for (int i = 0; i < LAYER3; i++)//8
		{
			if (i + 1 != LAYER3)
				fprintf(fp, "%f,", lenet->bias2_3[i]);
			else
				fprintf(fp, "%f", lenet->bias2_3[i]);
		}
		fprintf(fp, "},");
		fprintf(fp, "\n\n");

		fprintf(fp, "{");
		for (int i = 0; i < LAYER4; i++)//8
		{
			if (i + 1 != LAYER4)
				fprintf(fp, "%f,", lenet->bias4_5[i]);
			else
				fprintf(fp, "%f", lenet->bias4_5[i]);
		}
		fprintf(fp, "},");

		fprintf(fp, "\n\n");

		fprintf(fp, "{");
		for (int i = 0; i < LAYER5; i++)//8
		{
			if (i + 1 != LAYER5)
				fprintf(fp, "%f,", lenet->bias6_7[i]);
			else
				fprintf(fp, "%f", lenet->bias6_7[i]);
		}
		fprintf(fp, "},");

		fprintf(fp, "\n\n");

		fprintf(fp, "{");
		for (int i = 0; i < OUTPUT; i++)//4
		{
			if (i + 1 != OUTPUT)
				fprintf(fp, "%f,", lenet->bias8_9[i]);
			else
				fprintf(fp, "%f", lenet->bias8_9[i]);
		}
		fprintf(fp, "}");
		fprintf(fp, "\n");
		fprintf(fp, "};\n");//打印1次

		//打印初始化函数
		fprintf(fp, "LeNet5* model_init()\n");
		fprintf(fp, "{\n");
		fprintf(fp, "return &lenet_1;\n");
		fprintf(fp, "}\n");

		fclose(fp);
	}

	//for (int i = 0; i < OUTPUT; i++)
	//	printf("%f,", lenet->bias4_5[i]);

	free(lenet);
	
	free(train_data_bottle_500);
	free(train_data_battery_500);
	free(train_data_cardboard_500);
	free(test_data_battery_500);
	free(test_data_bottle_500);
	free(test_data_cardboard_500);

	free(train_data_bottle_500_30);
	free(train_data_battery_500_30);
	free(train_data_cardboard_500_30);
	free(test_data_battery_500_30);
	free(test_data_bottle_500_30);
	free(test_data_cardboard_500_30);

	free(forsobel);
	system("pause");
}

int main()
{

	start();    //启动
	return 0;
}