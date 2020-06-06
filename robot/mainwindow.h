#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <queue>
#include <stack>

using namespace std;

struct node
{
    int x,y,step;
    node(){}
    node(int xx,int yy,int s=0){
        x = xx;
        y = yy;
        step = s;
    }
};


namespace Ui {
class MainWindow;
}
const int MAX_SIZE = 100; /**迷宫的最大值 **/


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    int read_txt(string file);
    void find_m_n(int &m,int &n,string file);

    char maze[MAX_SIZE][MAX_SIZE]; /**迷宫**/
    int vis[MAX_SIZE][MAX_SIZE];  /**访问记录**/
    int m,n,sx,sy,ex,ey,ans=0; /** m、n是迷宫的长和宽，sx、sy是起点，ex,ey是终点，ans是最短路径长度 **/
    int dir[4][2] = {{1,0},{0,1},{-1,0},{0,-1}}; /**四个方向，上下左右**/
    node pre[MAX_SIZE][MAX_SIZE]; /**pre二维数组记录该位置的父节点**/
    string file;
    bool is_read = 0;
    bool suc_read = 0;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
