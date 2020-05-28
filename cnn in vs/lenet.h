
#pragma once

#define LENGTH_KERNEL	11

#define LENGTH_FEATURE0	32//32*32   30*30
#define LENGTH_FEATURE1	22
#define LENGTH_FEATURE2	11
#define LENGTH_FEATURE3	1
//#define	LENGTH_FEATURE4	(LENGTH_FEATURE3 >> 1)
//#define LENGTH_FEATURE5	(LENGTH_FEATURE4 - LENGTH_KERNEL + 1)

#define INPUT			1
#define LAYER1			5
#define LAYER2			5
#define LAYER3			10
//#define LAYER4			16
//#define LAYER5			120
#define LAYER4          8
#define LAYER5          8

#define OUTPUT          4

#define ALPHA 0.01
#define PADDING 1

typedef unsigned char uint8;
typedef uint8 image60[60][60];
typedef uint8 image30[30][30];
typedef uint8 image[30][30];
//定义一个140*140的存储，给测试图片用
typedef uint8 image1[140][140];


typedef struct LeNet5
{
	double weight0_1[INPUT][LAYER1][LENGTH_KERNEL][LENGTH_KERNEL];
	double weight2_3[LAYER2][LAYER3][LENGTH_KERNEL][LENGTH_KERNEL];
	//double weight4_5[LAYER4][LAYER5][LENGTH_KERNEL][LENGTH_KERNEL];
	double weight4_5[LAYER3 * LENGTH_FEATURE3 * LENGTH_FEATURE3][LAYER4];
	double weight6_7[LAYER4][LAYER5];
	double weight8_9[LAYER5][OUTPUT];


	double bias0_1[LAYER1];
	double bias2_3[LAYER3];
	//double bias4_5[LAYER5];
	double bias4_5[LAYER4];
	double bias6_7[LAYER5];
	double bias8_9[OUTPUT];

}LeNet5;

typedef struct Feature
{
	double input[INPUT][LENGTH_FEATURE0][LENGTH_FEATURE0];
	double layer1[LAYER1][LENGTH_FEATURE1][LENGTH_FEATURE1];
	double layer2[LAYER2][LENGTH_FEATURE2][LENGTH_FEATURE2];
	double layer3[LAYER3][LENGTH_FEATURE3][LENGTH_FEATURE3];
	double layer4[LAYER4];
	double layer5[LAYER5];
	double output[OUTPUT];
}Feature;

void TrainBatch(LeNet5 *lenet, image *inputs, uint8 labels, int batchSize);

void Train(LeNet5 *lenet, image input, uint8 label);

uint8 Predict(LeNet5 *lenet, image input, uint8 count);

void Initial(LeNet5 *lenet);
