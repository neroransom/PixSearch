#include "operate.h"
#include <QDebug>
#include <cmath>
#define pi 3.142857

QImage& turnGray(QImage& image,QImage* grayImage){
QRgb * line;
for(int y = 0; y<grayImage->height(); y++)
{
    line = (QRgb *)image.scanLine(y);
    for(int x = 0; x<grayImage->width(); x++)
    {
    int average = (qRed(line[x]) + qGreen(line[x]) + qBlue(line[x]))/3;
    grayImage->setPixel(x,y, qRgb(average, average, average));
    };

}
    return *grayImage;
}

int getGrayAVG(QImage* image)
{
    int sum = 0;
    for(int y = 0; y<image->height(); y++)
    {
        for(int x = 0; x<image->width(); x++)
        {
        sum+=image->pixelColor(y,x).red();
        }
    }
    return (sum/64);
}


int** dctTransform(QImage* image) //dct变换的实现
{
    const int m = 32, n = 32;
    int i, j, k, l;
    float dct[m][n];
    float ci, cj, dct1, sum;
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            if (i == 0)
                ci = 1 / sqrt(m);
            else
                ci = sqrt(2) / sqrt(m);
            if (j == 0)
                cj = 1 / sqrt(n);
            else
                cj = sqrt(2) / sqrt(n);
            sum = 0;
            for (k = 0; k < m; k++) {
                for (l = 0; l < n; l++) {
                    dct1 = image->pixelColor(k,l).red()*
                           cos((2 * k + 1) * i * pi / (2 * m)) *
                           cos((2 * l + 1) * j * pi / (2 * n));
                    sum = sum + dct1;
                }
            }
            dct[i][j] = ci * cj * sum;
        }
    }
                                   //获得的矩阵取其左上角8*8区域获得一个新矩阵，用于计算汉明距离
    int** cut = new int*[8];
    for(int i = 0;i<8;i++)
    {cut[i] = new int[8];}
    for(int y = 0;y<8;y++)
    {
       for(int x = 0;x<8;x++)
        {
            cut[y][x]=dct[y][x];
        }
    }
    return cut;
}

void getACode(bool* Code,QImage* image,int average)
{
    int i = 0;
    for(int y = 0; y<image->height(); y++)
    {
        for(int x = 0; x<image->width(); x++)
        {
           if(average>=image->pixelColor(y,x).red())
           {Code[i++]=1;}
           else
           {Code[i++]=0;}
        }
    }
}

void getDCode(bool *Code, QImage *image)
{
    int last = 0;
    int i = 0;
    for(int y = 0; y<image->height(); y++)
    {
        for(int x = 0; x<image->width(); x++)
        {
           if(image->pixelColor(y,x).red()>last)
           {    qDebug()<<image->pixelColor(y,x).red();
           Code[i++]=1;}
           else
           {Code[i++]=0;}
           last = image->pixelColor(y,x).red();
        }
    }
}

void getPCode(bool *Code, QImage *image)
{
    int** cutteddct = dctTransform(image);
    int sum = 0;
    for(int y = 0; y<8; y++)
      {
         for(int x = 0; x<8; x++)
         {
           sum+=cutteddct[y][x];
         }
      }
    int average = (sum/64);
    int i = 0;
    for(int y = 0; y<8; y++)
    {
        for(int x = 0; x<8; x++)
        {
           if(average>=cutteddct[y][x])
           {Code[i++]=1;}
           else
           {Code[i++]=0;}
        }
    }
    for(int i=0;i<8;i++)
    {
    delete[] cutteddct[i];
    }
}
