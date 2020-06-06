#include "display.h"
#include "ui_display.h"
#include "cstring"
#include "iostream"
#include "QMenuBar"
#include "QAction"
#include "queue"
#include "ostream"
#include "stack"
#include "QMessageBox"
#include "QFileDialog"
#include "QDebug"
#include <windows.h>
#include <QWidget>
#include <QtWidgets/QMainWindow>
#include <QAction>

display::display(string file,int mm,int nn,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::display)
{
    ui->setupUi(this);
    m = mm; n = nn;
    setWindowTitle("显示布局");
    int length = n*30-5;
    if(length<=200) length = 200;
    resize(length,m*30-8+35);
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            Qmaze[i][j] = new QWidget(this);
            Qmaze[i][j]->setGeometry(30*j,30*i,30,30);
        }
    }
    ui->pushButton_3->setGeometry(length-65+5,m*30+2,50,30);
    ui->pushButton_2->setGeometry(length-50-70-10,m*30+2,70,30);
    ui->pushButton->setGeometry(length-60-120-10,m*30+2,60,30);

    if(read_txt(file)==0){
    return;
    }
    show_maze(origin);
    if(bfs()) {            //如果可以从起点到达终点
         can_reach = 1;
         mark_way();
    }


}

display::~display()
{
    delete ui;
}

void display::on_pushButton_clicked()
{
    show_maze(origin);
}

void display::on_pushButton_2_clicked()
{
    button_showway = 1;
    if(can_reach){
        show_way();
        QMessageBox::information(this, "成功", "最短路径长度为"+QString::number(ans), "确定");
    }
    else{
        QMessageBox::warning(this, "警告", "起点无法到达终点，不能布线！", "返回");
    }
}

void display::on_pushButton_3_clicked()
{
    if(can_reach&&button_showway){ //可以到达终点，并且点过“展示布局”按钮，保存文档
        QString fileName = QFileDialog::getSaveFileName (this,tr("保存最短路径布局"),"",tr("文本文件 (*.txt)"));
        if(!fileName.isEmpty ())
                    {
                        QFile file(fileName);
                        if(!file.open (QIODevice::WriteOnly))
                        {
                            QMessageBox msgBox;
                            msgBox.setText("保存文件失败！");
                            msgBox.exec();
                        }
                        else
                        {
                            write_txt(fileName.toStdString());
                            file.close();
                        }
                    }
    }

    else if(!button_showway){ //没有点“展示布局”按钮，需要提醒先去点按钮
        QMessageBox::information(this, "提示", "请先显示路径", "返回");
    }
    else if(!can_reach&&button_showway){ //点过“展示布局”按钮，起点无法到达终点，弹出警告对话框
        QMessageBox::warning(this, "警告", "起点终点不能相连！\n无法保存", "返回");
    }
}
/**
读取迷宫的长和宽，即m和n
逐行读入一直到EOF，获得m
读入第一行直到'\n'，获得n
**/
void display::find_m_n(int &m,int &n,string file)
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
int display::read_txt(string file)
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
        origin[i][j] = c;
        j++;
        if(j==n){
            i++;
            j=0;
        }

    }
    in.close();
    return 1;
}

/**
写出txt文档，将最短路径标出
**/
void display::write_txt(string file)
{
    ofstream out;
    out.open(file.data()); /**将文件流与文件连接起来**/
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            out<<maze[i][j]<<" ";
        }
        out<<endl;
    }
    out<<"\n最短路径长度为:"<<ans<<endl;
    out.close();
}

/**
显示存储在二维数组内的迷宫
**/

void display::show_maze(char maze[][MAX_SIZE])
{
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(maze[i][j]=='1') Qmaze[i][j]->setStyleSheet("border-image:url(:/img/1.jpg);");
            else if(maze[i][j]=='0') Qmaze[i][j]->setStyleSheet("border-image:url(:/img/0.jpg);");
            else if(maze[i][j]=='S') Qmaze[i][j]->setStyleSheet("border-image:url(:/img/S.jpg);");
            else if(maze[i][j]=='D') Qmaze[i][j]->setStyleSheet("border-image:url(:/img/D.jpg);");
            else if(maze[i][j]=='*') Qmaze[i][j]->setStyleSheet("border-image:url(:/img/x.jpg);");
        }

    }
}

/**
搜索起点'S'，并保存将坐标在sx和sy中
**/
void display::search_start(int &sx,int &sy)
{
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(maze[i][j]=='S'){
                sx = i;
                sy = j;
            }
        }
    }
}

/**
搜索终点'D'，并保存将坐标在ex和ey中
**/
void display::search_end(int &ex,int &ey)
{
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(maze[i][j]=='D'){
                ex = i;
                ey = j;
            }
        }
    }
}


/**
广度优先搜索
如果可以到达终点，返回1；如果不能，返回0
**/
int display::bfs()
{
    queue<node> Q;
    search_start(sx,sy);
    search_end(ex,ey);
    memset(vis,0,sizeof(vis));
    memset(pre,0,sizeof(pre));
    node p(sx,sy);
    vis[sx][sy] = 1;
    Q.push(p);
    while(!Q.empty()){
        p = Q.front(); Q.pop();
        if(p.x==ex&&p.y==ey){
            ans = p.step;
            can_reach = 1; //标记为可以到达
            return 1;
        }
        for(int k=0;k<4;k++){
            int tx = p.x + dir[k][0];
            int ty = p.y + dir[k][1];
            int ts = p.step + 1;
            if(tx<0||ty<0||tx>=m||ty>=n) continue; //越界
            if(vis[tx][ty]||maze[tx][ty]=='1') continue; //访问过或者是障碍
            node q(tx,ty,ts);
            pre[tx][ty] = p; //记录该点的父节点
            vis[tx][ty] = 1;
            Q.push(q);
        }
    }
    return 0; //如果无法到达终点，则不能布线，返回0
}


/**
利用栈和pre数组显示路径
**/
void display::mark_way()
{
    stack<node> S;
    node p(ex,ey);
    S.push(p);
    while(1)
    {
        p = S.top();
        if(p.x==sx&&p.y==sy) break;
        S.push(pre[p.x][p.y]);
    }
    int i=0;
    while(!S.empty()){
        p = S.top(); S.pop();
        if(maze[p.x][p.y]=='0'){
            way[i][0] = p.x;
            way[i][1] = p.y;
            i++;
        }


    }
}

void display::show_way()
{
    for(int i=0;i<m;i++)  {
        for(int j=0;j<n;j++){
            maze[i][j] = origin[i][j];
        }
    }
    for(int i=0;i<ans-1;i++)
    {
        maze[way[i][0]][way[i][1]] = '*';
        //ui->textBrowser->setText("");
        show_maze(maze);
    }
}

