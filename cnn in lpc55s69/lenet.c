/*
 * lenet.c
 *
 *  Created on: 2019年12月5日
 *      Author: YYY
 */

#include "lenet.h"

#define DEMO_POWERQUAD POWERQUAD

#define GETLENGTH(array) (sizeof(array)/sizeof(*(array)))

#define GETCOUNT(array)  (sizeof(array)/sizeof(double))

/*
#define CONVOLUTE_VALID(input,output,weight)											\
{																						\
	uint32_t length      = (GETLENGTH(weight))*(GETLENGTH(*(weight)));					\
	double A[length];			                 										\
	double B[length];                             									    \
	double result[1];																	\
	uint32_t i=0;																		\
    uint32_t j=0;																		\
    PQ_SetFormat(POWERQUAD, kPQ_CP_MTX, kPQ_Double);									\
	FOREACH(o0,GETLENGTH(output))														\
		FOREACH(o1,GETLENGTH(*(output)))												\
		{																		     	\
			FOREACH(w0,GETLENGTH(weight))												\
				FOREACH(w1,GETLENGTH(*(weight)))										\
				{																		\
					A[i++] = (input)[o0 + w0][o1 + w1];									\
					B[j++] = (weight)[w0][w1];											\
				}																		\
			i=0;																		\
			j=0;																		\
			PQ_VectorDotProduct(DEMO_POWERQUAD, length, (void *)A, (void *)B, (void *)result);\
			PQ_WaitDone(DEMO_POWERQUAD);												\
			(output)[o0][o1] = result[0];												\
		}																																						\
}
*/
#define CONVOLUTE_VALID(input,output,weight)											\
{																						\
	for(int o0=0;o0<GETLENGTH(output);o0++)														\
		for(int o1=0;o1<GETLENGTH(*(output));o1++)												\
			for(int w0=0;w0<GETLENGTH(weight);w0++)												\
				for(int w1=0;w1<GETLENGTH(*(weight));w1++)										\
					(output)[o0][o1] += (input)[o0 + w0][o1 + w1] * (weight)[w0][w1];	\
}
#define CONVOLUTION_FORWARD(input,output,weight,bias,action)					\
{																				\
	for (int x = 0; x < GETLENGTH(weight); ++x)									\
		for (int y = 0; y < GETLENGTH(*weight); ++y)							\
			CONVOLUTE_VALID(input[x], output[y], weight[x][y]);					\
	for(int j=0;j< GETLENGTH(output);j++)												\
		for(int i=0;i< GETCOUNT(output[j]);i++)											\
		((double *)output[j])[i] = action(((double *)output[j])[i] + bias[j]);	\
}


#define SUBSAMP_MAX_FORWARD(input,output)														\
{																								\
	const int len0 = GETLENGTH(*(input)) / GETLENGTH(*(output));								\
	const int len1 = GETLENGTH(**(input)) / GETLENGTH(**(output));								\
	for(int i=0;i< GETLENGTH(output);i++)																\
	for(int o0=0;o0< GETLENGTH(*(output));o0++)															\
	for(int o1=0;o1< GETLENGTH(**(output));o1++)															\
	{																							\
		int x0 = 0, x1 = 0, ismax;																\
		for(int l0=0;l0< len0;l0++)																		\
			for(int l1=0;l1< len1;l1++)																	\
		{																						\
			ismax = input[i][o0*len0 + l0][o1*len1 + l1] > input[i][o0*len0 + x0][o1*len1 + x1];\
			x0 += ismax * (l0 - x0);															\
			x1 += ismax * (l1 - x1);															\
		}																						\
		output[i][o0][o1] = input[i][o0*len0 + x0][o1*len1 + x1];								\
	}																							\
}



#define DOT_PRODUCT_FORWARD(input,output,weight,bias,action)				\
{																			\
	for (int x = 0; x < GETLENGTH(weight); ++x)								\
		for (int y = 0; y < GETLENGTH(*weight); ++y)						\
			((double *)output)[y] += ((double *)input)[x] * weight[x][y];	\
	for(int j=0;j< GETLENGTH(bias);j++)												\
		((double *)output)[j] = action(((double *)output)[j] + bias[j]);	\
}

float PQ_MathInvSqrtFloatExample(float input)
{
    float result;
    PQ_InvSqrtF32(&input, &result);

    return result;
}

double exp(double x){
	int i,k,m,t;
	int xm = (int )x;
	double sum;
	double e;
	double ef;
	double z;
	double sub = x-xm;
	m=1;
	e=1.0;
	ef = 1.0;
	t = 10;
	z=1;
	sum = 1;
	if(xm<0){
		xm=(-xm);
		for(k=0;k<xm;k++)
		{
			ef*=2.718281;
		}
		e/=ef;
	}
	else{
		for(k=0;k<xm;k++)
		{
			e*=2.718281;
		}
	}
	for(i=1;i<t;i++){
		m*=i;
		z*=sub;
		sum+=z/m;
	}
	return sum*e;
}


double relu(double x)
{
	return x*(x > 0);
}

double relugrad(double y)
{
	return y > 0;
}

static void forward(LeNet5 *lenet, Feature *features, double(*action)(double))
{
	CONVOLUTION_FORWARD(features->input, features->layer1, lenet->weight0_1, lenet->bias0_1, action);
	SUBSAMP_MAX_FORWARD(features->layer1, features->layer2);
	CONVOLUTION_FORWARD(features->layer2, features->layer3, lenet->weight2_3, lenet->bias2_3, action);
	DOT_PRODUCT_FORWARD(features->layer3, features->output, lenet->weight4_5, lenet->bias4_5, action);
}
static inline void load_input(Feature *features, image input)
{
	double (*layer0)[LENGTH_FEATURE0][LENGTH_FEATURE0] = features->input;
	const long sz = sizeof(image) / sizeof(**input);
	double mean = 0, std = 0;
	for(int j=0;j< sizeof(image) / sizeof(*input);j++)
		for(int k=0; k<sizeof(*input) / sizeof(**input);k++)
	{
		mean += input[j][k];
		std += input[j][k] * input[j][k];
	}
	mean /= sz;
	std = sqrt(std / sz - mean*mean);
	for(int j=0; j<sizeof(image) / sizeof(*input);j++)
		for(int k=0;k< sizeof(*input) / sizeof(**input);k++)
	{
		layer0[0][j + PADDING][k + PADDING] = (input[j][k] - mean) / std;
	}
}

static inline void softmax(double input[OUTPUT], double loss[OUTPUT], int label, int count)
{
	double inner = 0;
	for (int i = 0; i < count; ++i)
	{
		double res = 0;
		for (int j = 0; j < count; ++j)
		{
			res += exp(input[j] - input[i]);
		}
		loss[i] = 1. / res;
		inner -= loss[i] * loss[i];
	}
	inner += loss[label];
	for (int i = 0; i < count; ++i)
	{
		loss[i] *= (i == label) - loss[i] - inner;
	}
}

static uint8 get_result(Feature *features, uint8 count)
{
	double *output = (double *)features->output;
	uint8 result = 0;
	double maxvalue = *output;
	for (uint8 i = 1; i < count; ++i)
	{
		if (output[i] > maxvalue)
		{
			maxvalue = output[i];
			result = i;
		}
	}
	return result;
}

uint8 Predict(LeNet5 *lenet, image input,uint8 count)
{
	Feature features = { 0 };
	load_input(&features, input);
	forward(lenet, &features, relu);
	return get_result(&features, count);
}
