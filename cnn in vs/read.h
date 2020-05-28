#pragma once
#include "lenet.h"
int read(int number, int sort, int training_now, int testing_now);
void read_test();
void sobel_start();
void sobel_test();
//60*60存储用
image60* train_data_battery_500;
//image train_data_cat_500[500];
image60* train_data_bottle_500;
image60* train_data_cardboard_500;
image60* train_data_can_500;
//image train_data_dog_500[500];
image60* test_data_battery_500;
//image test_data_cat_500[500];
image60* test_data_bottle_500;
//image test_data_dog_500[500];
image60* test_data_cardboard_500;
image60* test_data_can_500;


//30*30存储用
image30* train_data_battery_500_30;
image30* train_data_bottle_500_30;
image30* train_data_cardboard_500_30;
image30* train_data_can_500_30;

image30* test_data_battery_500_30;
image30* test_data_bottle_500_30;
image30* test_data_cardboard_500_30;
image30* test_data_can_500_30;


//image1 为给测试用的140*140图片专用的格式
image1* forsobel;

void expand();
int temp_save_sobel[58 * 58];
void saveTo30();