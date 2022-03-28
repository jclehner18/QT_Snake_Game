#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QKeyEvent>
#include <QPainter>
#include <QMessageBox>
#include <QTimer>
#include <random>
#include <iostream>
#include <string>
#include <fstream>
#include <QSound>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void GameOver();
    vector<vector<int>> move(vector<vector<int>> snek, int direct);
    void check1PlayerWin();
    void check2PlayerWin();
    void newFood();
    void getHighScore();
    void setHighScore();

protected:
    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *e);

private slots:
    void on_Beginbut_clicked();
    void on_Instructbut_clicked();
    void on_ctrInv_toggled(bool checked);
    void on_Gamerbut_toggled(bool checked);
    void on_Mediumbut_toggled(bool checked);
    void on_Easybut_toggled(bool checked);
    void on_Player1but_toggled(bool checked);
    void on_Player2but_toggled(bool checked);
    void painting();


private:
    bool start = false;
    bool inverted = false;
    int begin = 0;
    bool game_over = false;
    int winner = -1;
    int players = 0;
    int diffictulty = 0;
    int highscore = 0;
    int speed;
    Ui::MainWindow *ui;
    vector<vector<int>> snek1;
    vector<vector<int>> snek2;
    vector<int> food;
    QTimer *timer;
    /*or directions:
     *       0 is up
     *       1 is left
     *       2 is down
     *       3 if right
     */
    int snek1_direction_current = 0;
    int snek2_direction_current = 2;
    int snek1_direction_new = 0;
    int snek2_direction_new = 2;
    int start_width;
    int start_height;
    int game_width = 800;
    int game_height = 875;
    QImage scales;
    QImage head_up;
    QImage head_down;
    QImage head_right;
    QImage head_left;
    QImage head2_up;
    QImage head2_down;
    QImage head2_right;
    QImage head2_left;


};
#endif // MAINWINDOW_H
