/*
 * lenet.h
 *
 *  Created on: 2019年12月5日
 *      Author: YYY
 */

#ifndef LENET_H_
#define LENET_H_
#include "fsl_powerquad.h"
#include "math.h"
/*

批量训练
void TrainBatch(LeNet5 *lenet, image *inputs, uint8 *labels, int batchSize);

训练
void Train(LeNet5 *lenet, image input, uint8 label);

预测
uint8 Predict(LeNet5 *lenet, image input, uint8 count);

初始化
void Initial(LeNet5 *lenet);
*/

#pragma once

#define LENGTH_KERNEL	11

#define LENGTH_FEATURE0	32
#define LENGTH_FEATURE1	22
#define LENGTH_FEATURE2	11
#define LENGTH_FEATURE3	1

#define INPUT			1
#define LAYER1			6
#define LAYER2			6
#define LAYER3			16
#define OUTPUT          3

#define ALPHA 0.01
#define PADDING 2

typedef unsigned char uint8;
typedef uint8 image[28][28];


typedef struct LeNet5
{
	double weight0_1[INPUT][LAYER1][LENGTH_KERNEL][LENGTH_KERNEL];
	double weight2_3[LAYER2][LAYER3][LENGTH_KERNEL][LENGTH_KERNEL];
	double weight4_5[LAYER3 * LENGTH_FEATURE3 * LENGTH_FEATURE3][OUTPUT];

	double bias0_1[LAYER1];
	double bias2_3[LAYER3];
	double bias4_5[OUTPUT];

}LeNet5;

typedef struct Feature
{
	double input[INPUT][LENGTH_FEATURE0][LENGTH_FEATURE0];
	double layer1[LAYER1][LENGTH_FEATURE1][LENGTH_FEATURE1];
	double layer2[LAYER2][LENGTH_FEATURE2][LENGTH_FEATURE2];
	double layer3[LAYER3][LENGTH_FEATURE3][LENGTH_FEATURE3];
	double output[OUTPUT];
}Feature;

extern uint8 Predict(LeNet5 *lenet, image input, uint8 count);

#endif /* LENET_H_ */
