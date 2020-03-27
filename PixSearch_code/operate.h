#ifndef OPERATE_H
#define OPERATE_H
#include "QImage.h"

QImage& turnGray(QImage& image,QImage* grayImage);
int getGrayAVG(QImage* image);
void getACode(bool* Code, QImage* image,int average);
void getDCode(bool* Code, QImage* image);
void getPCode(bool* Code, QImage* image);
#endif // OPERATE_H
