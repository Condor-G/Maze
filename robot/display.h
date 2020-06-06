#ifndef DISPLAY_H
#define DISPLAY_H

#include <QWidget>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <queue>
#include <stack>
#include <mainwindow.h>

using namespace std;



namespace Ui {
class display;
}



class display : public QWidget
{
    Q_OBJECT

public:
    int read_txt(string file);
    void find_m_n(int &m,int &n,string file);
    void write_txt(string file);
    void show_maze(char [][MAX_SIZE]);
    void search_start(int &sx,int &sy);
    void search_end(int &ex,int &ey);
    int bfs();
    void mark_way();
    void show_way();

    int way[MAX_SIZE*MAX_SIZE/2][2];
    bool can_reach = 0;
    bool button_showway = 0;

    char maze[MAX_SIZE][MAX_SIZE]; /**迷宫**/
    char origin[MAX_SIZE][MAX_SIZE]; /**原始迷宫**/
    int vis[MAX_SIZE][MAX_SIZE];  /**访问记录**/
    int m,n,sx,sy,ex,ey,ans=0; /** m、n是迷宫的长和宽，sx、sy是起点，ex,ey是终点，ans是最短路径长度 **/
    int dir[4][2] = {{1,0},{0,1},{-1,0},{0,-1}}; /**四个方向，上下左右**/
    node pre[MAX_SIZE][MAX_SIZE]; /**pre二维数组记录该位置的父节点**/

    QWidget *Qmaze[MAX_SIZE][MAX_SIZE];

    explicit display(string,int,int,QWidget *parent = nullptr);
    ~display();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::display *ui;
};

#endif // DISPLAY_H
