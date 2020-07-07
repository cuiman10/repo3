#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCodec>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <time.h>

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openLenaJpg_triggered()
{
    srcImage = cv::imread("lena.jpg");
    if(!srcImage.data)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("默认的测试文件不存在，可以用以下两种方式的一种：1）复制一个文件到当前目录下，并命名为lena.jpg. 2)使用自定义方式打开一个自定义文件。"));
        msgBox.exec();
    }
    else
    {
        cvtColor(srcImage,srcImage,CV_BGR2RGB);
        img = QImage((const unsigned char*)(srcImage.data),srcImage.cols,srcImage.rows, srcImage.cols*srcImage.channels(), QImage::Format_RGB888);
        ui->label1->clear();
        img=  img.scaled(ui->label1->width(), ui->label1->height());
        ui->label1->setPixmap(QPixmap::fromImage(img));
    }
}

void MainWindow::on_exitSystem_triggered()
{
    exit(0);
}

void MainWindow::on_openCustomeFile_triggered()
{
    on_pushButton_clicked();
}

void MainWindow::on_restoreFile_triggered()
{
    srcImage.copyTo(dstImage);
    img = QImage((const unsigned char*)
                 (dstImage.data),dstImage.cols,dstImage.rows,dstImage.cols*dstImage.channels(),
                 QImage::Format_RGB888);
    img=img.scaled(ui->label1->size());
    ui->label2->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_copyright_triggered()
{
    QMessageBox::information(this,"版权",tr("本软件版权所有者为：天津职业技术师范大学。如果使用，请联系：lilizong@gmail.com"));
}

void MainWindow::on_about_triggered()
{
    QMessageBox::information(this,"关于",tr("本软件当前版本为1.0，由李立宗等人开发。如果有问题，欢迎联系：lilizong@gmail.com"));
    return;
}

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("打开待检索文件"),"",tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(filename).data();
    srcImage = cv::imread(name);
    if(!srcImage.data)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("未找到数据"));
        msgBox.exec();
    }
    else
    {
        cv::cvtColor(srcImage,srcImage,CV_BGR2RGB);
        img = QImage((const unsigned char*)(srcImage.data),srcImage.cols,srcImage.rows, srcImage.cols*srcImage.channels(), QImage::Format_RGB888);
        ui->label1->clear();
        img=  img.scaled(ui->label1->width(), ui->label1->height());
        ui->label1->setPixmap(QPixmap::fromImage(img));
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    Mat noImage = cv::imread("no.png");
    if(!noImage.data)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("默认的测试文件不存在，可以用以下两种方式的一种：1）复制一个文件到当前目录下，并命名为lena.jpg. 2)使用自定义方式打开一个自定义文件。"));
        msgBox.exec();
    }
    else
    {
        cv::cvtColor(noImage,noImage,CV_BGR2RGB);
        img = QImage((const unsigned char*)(noImage.data),noImage.cols,noImage.rows, noImage.cols*noImage.channels(), QImage::Format_RGB888);
        ui->label1->clear();
        img=  img.scaled(ui->label1->width(), ui->label1->height());
        ui->label1->setPixmap(QPixmap::fromImage(img));
    }


    noImage.copyTo(dstImage);
    img = QImage((const unsigned char*)
                 (dstImage.data),dstImage.cols,dstImage.rows,dstImage.cols*dstImage.channels(),
                 QImage::Format_RGB888);
    img=img.scaled(ui->label1->size());
    ui->label1->setPixmap(QPixmap::fromImage(img));
    showLabel(noImage,ui->label2);
    showLabel(noImage,ui->label3);
    showLabel(noImage,ui->label4);
    showLabel(noImage,ui->label5);
    showLabel(noImage,ui->label6);
    showLabel(noImage,ui->label7);
    showLabel(noImage,ui->label8);
    showLabel(noImage,ui->label9);

}

void MainWindow::on_pushButton_4_clicked()
{
    exit(0);
}
void MainWindow::showLabel(Mat m, QLabel *l)
{
    img = QImage((const unsigned char*)(m.data),m.cols,m.rows,m.cols*m.channels(), QImage::Format_RGB888);
    l->clear();
    img=img.scaled(l->width(),l->height());
    l->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::getFeature(cv::Mat m,int t[])
{
    //第1步：将图像灰度化并缩小尺寸，大小为mm*nn，这里处理为mm=8,nn=8
    //因此，缩小后的实际大小为8*8像素大小。
    int M=m.rows;
    int N=m.cols;
    cv::cvtColor(m,m,CV_BGR2GRAY);
    int i,j;
    //int FeatureResult[mm*nn];
    Mat Feature(mm,nn,CV_32SC1, Scalar::all(0));

    for(i=0;i<M;i++)
        for(j=0;j<N;j++)
        {
            Feature.at<int>(i/(M/mm),j/(N/nn))=Feature.at<int>(i/(M/mm),j/(N/nn))+m.at<uchar>(i,j);
            // Feature.at<uchar>(i/(M/8),j/(N/8))+=m.at<uchar>(i,j);
        }


    for(i=0;i<mm;i++)
        for(j=0;j<nn;j++)
            Feature.at<int>(i,j)=Feature.at<int>(i,j)/((M/mm)*(N/nn));


    //第2步：简化色彩。当前灰度级为8位，即256色，将其处理为cc色。
    //这里设置cc=64
    //int cc=64;
    for(i=0;i<mm;i++)
        for(j=0;j<nn;j++)
            Feature.at<int>(i,j)/=(256/cc);
    //第3步：计算图像的平均值。
    double sum,aver;
    sum=0;
    for(i=0;i<mm;i++)
        for(j=0;j<nn;j++)
            sum+=Feature.at<int>(i,j);
    aver=sum/(mm*nn);
    //  printf("sum=%lf,aver=%lf",sum,aver);
    //第4步：计算像素值与平均值大小关系。同时得到特征矩阵FeatureResult，并返回。
    int flag=0;
    for(i=0;i<mm;i++)
        for(j=0;j<nn;j++)
            if(Feature.at<int>(i,j)>aver)
                t[flag++]=1;
            else
                t[flag++]=0;
}

int* MainWindow::getFeature2(cv::Mat m)
{
    //第1步：将图像灰度化并缩小尺寸，大小为mm*nn，这里处理为mm=8,nn=8
    //因此，缩小后的实际大小为8*8像素大小。
    int M=m.rows;
    int N=m.cols;
    cv::cvtColor(m,m,CV_RGB2GRAY);
    int i,j;
    int FeatureResult[mm*nn];
    Mat Feature(mm,nn,CV_32SC1, Scalar::all(0));

    for(i=0;i<M;i++)
        for(j=0;j<N;j++)
        {
            Feature.at<int>(i/(M/mm),j/(N/nn))=Feature.at<int>(i/(M/mm),j/(N/nn))+m.at<uchar>(i,j);
        }

    for(i=0;i<mm;i++)
        for(j=0;j<nn;j++)
            Feature.at<int>(i,j)=Feature.at<int>(i,j)/((M/mm)*(N/nn));

    //第2步：简化色彩。当前灰度级为8位，即256色，将其处理为cc色。
    //这里设置cc=64

    for(i=0;i<mm;i++)
        for(j=0;j<nn;j++)
            Feature.at<int>(i,j)/=(256/cc);
    //第3步：计算图像的平均值。
    double sum,aver;
    sum=0;
    for(i=0;i<mm;i++)
        for(j=0;j<nn;j++)
            sum+=Feature.at<int>(i,j);
    aver=sum/(mm*nn);
    //第4步：计算像素值与平均值大小关系。同时得到特征矩阵FeatureResult，并返回。
    int flag=0;
    // int FeatureResult[mm*nn];
    for(i=0;i<mm;i++)
        for(j=0;j<nn;j++)
            if(Feature.at<int>(i,j)>aver)
                FeatureResult[flag++]=1;
            else
                FeatureResult[flag++]=0;

    int *t;
    t=FeatureResult;
    return t;


}
void MainWindow::on_pushButton_6_clicked()
{
    //提取特征！！！
    //读取文件到srcImage内，然后提取特征。
    int i,j;
    for(i=0;i<total;i++)
    {
        getImage(fileInfo,i);
        int t2[mm*nn];
        getFeature(srcImage,t2);
        for(j=0;j<featureNumber;j++)
        {
            FileFeature[i][j]=t2[j];
        }

    }
    QMessageBox::about(NULL, "提示", "特征提取完毕！！！");//, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

}
void MainWindow::showLabelGray(Mat m, QLabel *l)
{
    cv::Size dsize=Size(l->width(),l->height());
    cv::resize(m,m,dsize);
    img = QImage((const unsigned char*)(m.data),m.cols,m.rows,m.step,  QImage::Format_Indexed8);
    l->clear();
    l->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_pushButton_5_clicked()
{
    //设定路径，这里设置好要读取的路径，并且能够将路径下的图片随机地显示在下面。
    QString path = QFileDialog::getExistingDirectory();
    //显示当前选取的文件夹

    QDir *dir=new QDir(path);
    //过滤掉文件名”.",和文件名"..".
    dir->setFilter(QDir::Files | QDir::NoSymLinks);
    QStringList filter;
    fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
    total=fileInfo->count();
    //读取当前目录下文件的个数
    int i;
    //产生无重复的数组；
    srand(time(NULL));
    int output[100]={0};
    int m;
    for(i=0; i<total; i++)
    {
        while(output[m=rand()%total])
            ;
        output[m] = i;
    }
    //上述处理过程中，不会出现值为0的数组元素，下面随机将0处理一下，放到某个数组元素内。
    m=rand()%total;
    output[m]=0;
    getImage(fileInfo,output[0]);
    showLabel(srcImage,ui->label2);
    getImage(fileInfo,output[1]);
    showLabel(srcImage,ui->label3);
    getImage(fileInfo,output[2]);
    showLabel(srcImage,ui->label4);
    getImage(fileInfo,output[3]);
    showLabel(srcImage,ui->label5);
    getImage(fileInfo,output[4]);
    showLabel(srcImage,ui->label6);
    getImage(fileInfo,output[5]);
    showLabel(srcImage,ui->label7);
    getImage(fileInfo,output[6]);
    showLabel(srcImage,ui->label8);
    getImage(fileInfo,output[7]);
    showLabel(srcImage,ui->label9);


}
void MainWindow::getImage(QList<QFileInfo> *fileInfo,int i)
{
    QString s;//=fileInfo->at(0).fileName();
    QString dirS;  //包含一个目录的路径；
    s=fileInfo->at(i).fileName();
    dirS="image//";
    dirS=dirS.append(s);
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(dirS).data();
    srcImage = cv::imread(name);
    cv::cvtColor(srcImage,srcImage,CV_BGR2RGB);

}

void MainWindow::on_pushButton_2_clicked()
{
    //搜索按钮，主要完成检索图像的特征提取、特征比对、排序
    int t2[mm*nn];
    getFeature(srcImage,t2);
    //相同特征值的数量。
    int sameCount[fileNumber]={0};//忘记初始化，罪过！！！
    //和目录下图片挨个比较，并得出相同的位数的个数。
    for(int i=0;i<fileNumber;i++)
        for(int j=0;j<featureNumber;j++)
        {
            sameCount[i]+=FileFeature[i][j]*t2[j];
        }
    //将个数最多的fileNumber个找出来
    int maxi[8]={0};
    for(int i=0;i<8;i++)
    {
        int max=sameCount[0];
        for(int j=0;j<fileNumber;j++)
        {
            if(max<sameCount[j])
            {
                max=sameCount[j];
                maxi[i]=j;
            }
        }
        sameCount[maxi[i]]=0;//避免重复计算最大值，将上次最大值置零，再次求最大值。

    }
    getImage(fileInfo,maxi[0]);
    showLabel(srcImage,ui->label2);
    getImage(fileInfo,maxi[1]);
    showLabel(srcImage,ui->label3);
    getImage(fileInfo,maxi[2]);
    showLabel(srcImage,ui->label4);
    getImage(fileInfo,maxi[3]);
    showLabel(srcImage,ui->label5);
    getImage(fileInfo,maxi[4]);
    showLabel(srcImage,ui->label6);
    getImage(fileInfo,maxi[5]);
    showLabel(srcImage,ui->label7);
    getImage(fileInfo,maxi[6]);
    showLabel(srcImage,ui->label8);
    getImage(fileInfo,maxi[7]);
    showLabel(srcImage,ui->label9);
}

void MainWindow::on_action_triggered()
{
    on_pushButton_5_clicked();
}

void MainWindow::on_action_2_triggered()
{
    on_pushButton_2_clicked();
}

void MainWindow::on_action_3_triggered()
{
    on_pushButton_6_clicked();
}
