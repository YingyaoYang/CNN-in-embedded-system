
#pragma once
#include "stdio.h"
#include "stdlib.h"
#include "lenet.h"


long getWidth(FILE* fp);
long getHeight(FILE* fp);
unsigned short getBit(FILE* fp);
unsigned int getOffSet(FILE* fp);
void getData(FILE* fp, unsigned char* r, unsigned char* g, unsigned char* b, unsigned char data[28][28]);
void read(int number, int sort, int training_now, int testing_now);
image train_data_cat[10000];
image train_data_dog[10000];
image* train_data_battery_500;
//image train_data_cat_500[500];
image* train_data_bottle_500;
image* train_data_cardboard_500;
//image train_data_dog_500[500];
image* test_data_battery_500;
//image test_data_cat_500[500];
image* test_data_bottle_500;
//image test_data_dog_500[500];
image* test_data_cardboard_500;
