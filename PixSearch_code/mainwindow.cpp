#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //************************************** setting
    ui->setupUi(this);
    ui->thumbnail->setAlignment(Qt::AlignCenter);
    ui->thumbnail2->setAlignment(Qt::AlignCenter);
}

MainWindow::~MainWindow()
{
    int numofitem = ui->listWidget->count();
       for(int i=0;i<numofitem;i++)
       {
           QListWidgetItem *item = ui->listWidget->takeItem(0); //这里是0，不是i，因为每移除一个item都会导致每个item的row发生变化
           delete item;
       }
    delete ui;
}

void MainWindow::on_srcbutton_clicked() //of choose image
{
    QString src = QFileDialog::getOpenFileName(this, "choose source picture");
    if(src.isEmpty())
    {return;}
    ui->srcpath->setText(src);
    QPixmap imageshow(src);
    if(imageshow.isNull()){ui->thumbnail->setText("请传入可用格式的图片！");return;}
    imageshow = imageshow.scaled(ui->thumbnail->width()-4,ui->thumbnail->height()-4,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->thumbnail->setPixmap(imageshow);
    return;
}

void MainWindow::on_enterButton_clicked()//of
{
    ui->thumbnail2->clear();
    //***************************************
    int numofitem = ui->listWidget->count();
       for(int i=0;i<numofitem;i++)
       {
           QListWidgetItem *item = ui->listWidget->takeItem(0); //这里是0，不是i，因为每移除一个item都会导致每个item的row发生变化
           delete item;
       }
    //*************************************** clear two view for each enter
    QString src = ui->srcpath->text();
   //**************************************** show src picture(for special case that srcbutton haven't been pushed)
    if(src==NULL){ui->thumbnail->clear();return;}
    QPixmap imageshow(src);
    if(imageshow.isNull()){ui->thumbnail->setText("请传入可用格式的图片！");return;}
    imageshow = imageshow.scaled(ui->thumbnail->width()-4,ui->thumbnail->height()-4, Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->thumbnail->setPixmap(imageshow);
    //**************************************** start to search
    switch (ui->comboBox->currentIndex()) {
       case (0):aHash(src);break;
       case (1):dHash(src);break;
       case (2):pHash(src);break;
    }

}

void MainWindow::on_databutton_clicked()
{
     QString srcDirPath = QFileDialog::getExistingDirectory(this, "choose src Directory","/");
     if (srcDirPath.isEmpty())
        {
            return;
        }
        else
        {
         if(srcDirPath.back()!='/')
            {srcDirPath += "/";}
        }
     ui->datapath->setText(srcDirPath);
}

void MainWindow::aHash(QString src)         //aHash
{
     QImage image(src);
     image = image.scaled(8, 8);
   //************************************* turn gray
     QImage * grayImage = new QImage(8, 8, QImage::Format_Grayscale8);
     turnGray(image,grayImage);
 //************************************* gethashcode
     int average = getGrayAVG(grayImage);
     bool *HashCode = new bool[64];
     getACode(HashCode,grayImage,average);
 //************************************* iterate
     if(ui->datapath->text().isEmpty()){return;}
     QDirIterator it(ui->datapath->text(), QStringList() << "*.png"<<"*.jpeg"<<"*.jpg"<<"*.bmp",QDir::AllEntries | QDir::NoDotAndDotDot,QDirIterator::Subdirectories);
     while (it.hasNext()) {
         it.next();
         QString path = it.filePath();
         QImage image2(path);
         if(image2.isNull()){qDebug()<<it.filePath()<<"cannot be loaded";continue;} //picture cannot be loaded;
         image2 = image2.scaled(8, 8);
         QImage * grayImage2 = new QImage(8, 8, QImage::Format_Grayscale8);
         turnGray(image2,grayImage2);
         int average = getGrayAVG(grayImage2);
         bool *HashCode2 = new bool[64];
         getACode(HashCode2,grayImage2,average);
         int border = 64*(100-ui->SpinBox->value())/100;
         for(int i = 0, hamming = 0;i<64;i++)
         {
             if(HashCode[i] != HashCode2[i])
             {
                 ++hamming;
             }
             if(hamming>border)
             {
                 delete[] HashCode2;
                 break;
             }
             if(i==63)
             {
                 delete[] HashCode2;
                 QListWidgetItem *item = new QListWidgetItem;
                 item->setText(path);
                 ui->listWidget->addItem(item);
                 break;
             }
         }
     }
 //************************************* destructor
     if(HashCode!=nullptr)
     {delete[] HashCode;}
 //**********************************
 if(!ui->listWidget->count()){ui->listWidget->addItem("未能检索出相似图片");}
}

void MainWindow::dHash(QString src)
{
    QImage image(src);
    image = image.scaled(8, 8);
  //************************************* turn gray
    QImage * grayImage = new QImage(8, 8, QImage::Format_Grayscale8);
    turnGray(image,grayImage);
  //************************************* get code
    bool *HashCode = new bool[64];
    getDCode(HashCode,grayImage);
    //************************************* iterate
        if(ui->datapath->text().isEmpty()){return;}
        QDirIterator it(ui->datapath->text(), QStringList() << "*.png"<<"*.jpeg"<<"*.jpg"<<"*.bmp",QDir::AllEntries | QDir::NoDotAndDotDot,QDirIterator::Subdirectories);
        while (it.hasNext()) {
            it.next();
            QString path = it.filePath();
            QImage image2(path);
            if(image2.isNull()){qDebug()<<it.filePath()<<"cannot be loaded";continue;} //picture cannot be loaded;
            image2 = image2.scaled(8, 8);
            QImage * grayImage2 = new QImage(8, 8, QImage::Format_Grayscale8);
            turnGray(image2,grayImage2);
            bool *HashCode2 = new bool[64];
            getDCode(HashCode2,grayImage2);
            int border = 64*(100-ui->SpinBox->value())/100;
            for(int i = 0, hamming = 0;i<64;i++)
            {
                if(HashCode[i] != HashCode2[i])
                {
                    ++hamming;
                }
                if(hamming>border)
                {
                    delete[] HashCode2;
                    break;
                }
                if(i==63)
                {
                    delete[] HashCode2;
                    QListWidgetItem *item = new QListWidgetItem;
                    item->setText(path);
                    ui->listWidget->addItem(item);
                    break;
                }
            }
        }
    //************************************* destructor
        if(HashCode!=nullptr)
        {delete[] HashCode;}
    //************************************* nothing had been found
        if(!ui->listWidget->count()){ui->listWidget->addItem("未能检索出相似图片");}
}

void MainWindow::pHash(QString src)
{
    QImage image(src);
    image = image.scaled(32, 32);
  //************************************* turn gray
    QImage * grayImage = new QImage(32, 32, QImage::Format_Grayscale8);
    turnGray(image,grayImage);
  //************************************* get code
    bool *HashCode = new bool[64];
    getPCode(HashCode,grayImage);
    //************************************* iterate
    if(ui->datapath->text().isEmpty()){return;}
    QDirIterator it(ui->datapath->text(), QStringList() << "*.png"<<"*.jpeg"<<"*.jpg"<<"*.bmp",QDir::AllEntries | QDir::NoDotAndDotDot,QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        QString path = it.filePath();
        QImage image2(path);
        if(image2.isNull()){qDebug()<<it.filePath()<<"cannot be loaded";continue;} //picture cannot be loaded;
        image2 = image2.scaled(32, 32);
        QImage * grayImage2 = new QImage(32, 32, QImage::Format_Grayscale8);
        turnGray(image2,grayImage2);
        bool *HashCode2 = new bool[64];
        getPCode(HashCode2,grayImage2);
        int border = 64*(100-ui->SpinBox->value())/100;
        for(int i = 0, hamming = 0;i<64;i++)
        {
            if(HashCode[i] != HashCode2[i])
            {
                ++hamming;
            }
            if(hamming>border)
            {
                delete[] HashCode2;
                break;
            }
            if(i==63)
            {
                delete[] HashCode2;
                QListWidgetItem *item = new QListWidgetItem;
                item->setText(path);
                ui->listWidget->addItem(item);
                break;
            }
        }
    }
   //************************************* destructor
        if(HashCode!=nullptr)
        {delete[] HashCode;}
   //**********************************
    if(!ui->listWidget->count()){ui->listWidget->addItem("未能检索出相似图片");}
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString path = item->text();
    QPixmap showresult(path);
    showresult = showresult.scaled(ui->thumbnail2->width()-4,ui->thumbnail2->height()-4, Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->thumbnail2->setPixmap(showresult);
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{/*
    QString path =
    QUrl  url(path);
    url = url.fromLocalFile(path);
    QDesktopServices device;
    device.openUrl(url);
    */
      QString fileName = item->text();
     // if(fileName == "未能检索出相似图片"){return;}
      QProcess process;
      fileName.replace("/","\\");
      QString cmd = QString("explorer /select,\"%1\"").arg(fileName);
      qDebug()<<cmd;
      process.startDetached(cmd);

}
