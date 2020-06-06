#include "mainwindow.h"
#include "display.h"
#include "ui_mainwindow.h"
#include "cstring"
#include <string.h>
#include "iostream"
#include "QMenuBar"
#include "QAction"
#include "QMessageBox"
#include "QFileDialog"
#include "QDebug"
#include <windows.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("机器人布线");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_1_clicked()
{
    is_read = 0;
    suc_read = 0;
    QFileDialog *fd = new QFileDialog(this);
    fd->setWindowTitle(tr("选择文件"));
    fd->resize(240,320);
    fd->setNameFilter(tr("Images(*.txt)"));
    QStringList fileNames;

    if(fd->exec()){
        is_read = 1;
        fileNames = fd->selectedFiles();
        QString filename = fileNames[0];
        string infile = filename.toStdString();
        file = infile;
        find_m_n(m,n,infile);
        if(read_txt(infile)==0){
        ui->textBrowser->append( "文档内有非法数据，请检查是否有0,1，S，D之外的字符！");
        return;
        }
        suc_read = 1;
        ui->textBrowser->append( QString::fromStdString("成功读取！\n当前文档位置："+infile)+"\n可以开始进行展示");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(is_read&&suc_read){
        display *d = new display(file,m,n);
        d->show();
    }
    else if(is_read&&!suc_read){
         QMessageBox::critical(this, "警告", "请选择有效数据", "返回");
    }
    else if(!is_read){
         QMessageBox::warning(this, "提示", "请先选择文件", "返回");
    }
}

/**
读取迷宫的长和宽，即m和n
逐行读入一直到EOF，获得m
读入第一行直到'\n'，获得n
**/
void MainWindow::find_m_n(int &m,int &n,string file)
{
    ifstream in;
    in.open(file.data());
    assert(in.is_open());
    int tm = 0,tn = 0;
    string s;
    while(getline(in,s)){ /**逐行读入**/
        tm++;
    }
    m = tm;
    in.close();

    in.open(file.data());
    assert(in.is_open());
    char c;
    in>>noskipws;  /**输入**/
    while(!in.eof()) /**逐个字符读入，包括空格与回车符**/
    {
        in>>c;
        if(c=='0'||c=='1'||c=='S'||c=='D') tn++;
        else if(c=='\n') break;
    }
    n = tn;
    in.close();

}

/**
读取txt文档，获取迷宫
**/
int MainWindow::read_txt(string file)
{
    ifstream in;
    in.open(file.data()); /**将文件流与文件连接起来**/
    assert(in.is_open()); /**如果失败，则输出错误消息，并终止程序**/

    int i=0,j=0;
    char c;
    while(!in.eof()) /**逐个字符读入，忽略空格与回车符**/
    {
        in>>c;
        if(c!='0'&&c!='1'&&c!='S'&&c!='D'){
            in.close();
            return 0;
        }
        maze[i][j] = c;
        j++;
        if(j==n){
            i++;
            j=0;
        }

    }
    in.close();
    return 1;
}



