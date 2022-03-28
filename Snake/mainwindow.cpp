#include "mainwindow.h"
#include "ui_mainwindow.h"

//Special credit to Alex Menilli who voiced Death sound
//Credit to Nik Pensyl for producing all sounds and Recording of Munch Sound
//Credit to: https://freemidi.org/download3-26214-all-of-me--john-legend for Midi files of music generated.
QSound music("../game_music.wav");
QSound munch("../Munch.wav");
QSound dead("../Death.wav");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setFixedSize(width(), height());
    start_width = width();
    start_height = height();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(painting()));
    food.push_back(6);
    food.push_back(7);
    getHighScore();
    scales.load("../snake_Background.png");
    head_up.load("../Snake_Head_Up.png");
    head_left.load("../Snake_Head_Left.png");
    head_right.load("../Snake_Head_Right.png");
    head_down.load("../Snake_Head_Down.png");
    head2_up.load("../Snake_Head2_Up.png");
    head2_left.load("../Snake_Head2_Left.png");
    head2_right.load("../Snake_Head2_Right.png");
    head2_down.load("../Snake_Head2_Down.png");
    music.setLoops(0);
    munch.setLoops(0);
    dead.setLoops(0);
}

void MainWindow::getHighScore() {
    fstream infile;
    infile.open("../snek_HighScore.txt", ios_base::in);
    string high;
    getline(infile, high);
    try {
        highscore = stoi(high);
    }  catch (exception &err) {
        highscore = 0;
    }
    infile.close();
}

void MainWindow::setHighScore() {
    fstream ofile;
    ofile.open("../snek_HighScore.txt", ios_base::out);
    ofile<<highscore;
    ofile.close();
}

void MainWindow::paintEvent(QPaintEvent *e) {
    QPainter p(this);

    if(start) {
        p.setBrush(Qt::black);
        p.drawRect(0, 0, width(), height());
        p.setPen(QPen(QColor(0, 70, 0), 5));
        p.drawLine(25, 100, 25, game_height-25);
        p.drawLine(25, game_height-25, game_width-25, game_height-25);
        p.drawLine(game_width-25, 100, game_width-25, game_height-25);
        p.drawLine(25, 100, game_width-25, 100);
        p.setPen(QPen(QColor(0, 70, 0), 2));
        p.setBrush(Qt::red);
        p.drawEllipse(food[0]*50+30, food[1]*50+105, 40, 40);
        if(begin==1) {
            _sleep(1000);
            begin++;
            music.play();
        } else {
            begin++;
        }
        if(music.isFinished()) {
            music.play();
        }
        snek1 = move(snek1, snek1_direction_current);
        snek1_direction_current = snek1_direction_new;
        QFont font("Courier", 35, QFont::DemiBold);
        p.setPen(Qt::white);
        QFontMetrics fm(font);
        p.setFont(font);
        QString Player_score = "Player 1: " + QString::fromStdString(to_string(snek1.size()));
        p.drawText(20, 50, Player_score);
        for(int i =0; i<snek1.size(); i++) {
            p.setBrush(Qt::green);
            if(i!=0) {
                p.drawImage(snek1[i][0]*50+25, snek1[i][1]*50+100, scales);
            } else {
                if(snek1_direction_current==0) {
                  p.drawImage(snek1[i][0]*50+25, snek1[i][1]*50+75, head_up);
                } else if(snek1_direction_current==1) {
                    p.drawImage(snek1[i][0]*50, snek1[i][1]*50+100, head_left);
                  } else if(snek1_direction_current==2) {
                    p.drawImage(snek1[i][0]*50+25, snek1[i][1]*50+100, head_down);
                  } else{
                    p.drawImage(snek1[i][0]*50+25, snek1[i][1]*50+100, head_right);
                  }

            }
        }
        if(players==2) {
            snek2 = move(snek2, snek2_direction_current);
            snek2_direction_current = snek2_direction_new;
            Player_score = "Player 2: " + QString::fromStdString(to_string(snek2.size()));
            p.setPen(Qt::white);
                    p.drawText(520, 50, Player_score);
            for(int i =0; i<snek2.size(); i++) {
                p.setBrush(Qt::blue);
                if(i!=0) {
                    p.drawImage(snek2[i][0]*50+25, snek2[i][1]*50+100, scales);
                } else {
                    if(snek2_direction_current==0) {
                      p.drawImage(snek2[i][0]*50+25, snek2[i][1]*50+75, head2_up);
                    } else if(snek2_direction_current==1) {
                        p.drawImage(snek2[i][0]*50, snek2[i][1]*50+100, head2_left);
                      } else if(snek2_direction_current==2) {
                        p.drawImage(snek2[i][0]*50+25, snek2[i][1]*50+100, head2_down);
                      } else{
                        p.drawImage(snek2[i][0]*50+25, snek2[i][1]*50+100, head2_right);
                      }

                }
            }

            check1PlayerWin();
        } else {
            check1PlayerWin();
        }


    } else {
        p.setBrush(QColor(0, 200, 235));
        p.drawRect(0, 0, width(), height());
    }

}

void MainWindow::check1PlayerWin() {
    if(snek1[0][0]==-1 || snek1[0][0]==15 || snek1[0][1]==-1 || snek1[0][1]==15) {
        game_over = true;
        start = false;
        if(players==2) {
            winner = 2;
            check2PlayerWin();
            if(winner==1) {
                winner = 0;
            }
        }
    } else if(players==2) {
        for(int j = 0; j<snek2.size(); j++) {
            if(snek1[0][1]==snek2[j][1] && snek1[0][0]==snek2[j][0]) {
                game_over = true;
                start = false;
                winner =2;
                check2PlayerWin();
                if(winner==1) {
                    winner = 0;
                }
            }
        }
    }
    if(players==2 && winner==-1){
        check2PlayerWin();
    }
    if(game_over) {
        GameOver();
    }
}

void MainWindow::check2PlayerWin() {
    if(snek2[0][0]==-1 || snek2[0][0]==15 || snek2[0][1]==-1 || snek2[0][1]==15) {
        game_over = true;
        start = false;
        winner = 1;
    } else {
        for(int j = 0; j<snek1.size(); j++) {
            if(snek2[0][1]==snek1[j][1] && snek2[0][0]==snek1[j][0]) {
                game_over = true;
                start = false;
                winner =1;
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::painting() {
    repaint();
    if(!game_over && start) {
        timer->start(speed);
    }
}

void MainWindow::newFood() {
    bool clear = false;
    srand(time(NULL));
    if(munch.isFinished()) {
        munch.play();
    } else {
        munch.play();
    }
    do {
        clear = true;
        food[0] = rand()%15;
        food[1] = rand()%15;
        for(int j = 0; j<snek1.size(); j++) {
            if(snek1[j][0]==food[0] && snek1[j][1]==food[1]) {
                clear = false;
                break;
            }
        }
        if(players==2 && clear) {
            for(int j = 0; j<snek2.size(); j++) {
                if(snek2[j][0]==food[0] && snek2[j][1]==food[1]) {
                    clear = false;
                    break;
                }
            }
        }
    } while(!clear);
}

void MainWindow::on_Beginbut_clicked() {
    if(players!=0 && diffictulty!=0) {
      start = true;
      ui->Easybut->hide();
      ui->Gamerbut->hide();
      ui->Beginbut->hide();
      ui->Instructbut->hide();
      ui->Mediumbut->hide();
      ui->Player1but->hide();
      ui->Player2but->hide();
      ui->ctrInv->hide();
      ui->sneklab->hide();
      ui->playerlab->hide();
      ui->difflab->hide();

      vector<int> coords;
      coords.push_back(5);
      coords.push_back(7);
      snek1_direction_current = 0;
      snek2_direction_current = 2;
      snek1_direction_new = 0;
      snek2_direction_new = 2;
      food[0] = 7;
      food[1] = 7;
      snek1.clear();
      snek2.clear();
      snek1.push_back(coords);
      coords[0] = 8;
      snek2.push_back(coords);
      setFixedSize(game_width, game_height);
      timer->start(speed);
    } else {
        QMessageBox msg;
        msg.setText("Make sure you have chosen an option from both sections.");
        msg.exec();
    }


}

vector<vector<int>> MainWindow::move(vector<vector<int>> snek, int direct) {
    for(int i =snek.size()-1; i>=0; i--) {

        if(i==0) {
            if(i+1==snek.size()) {
                switch(direct) {
                    case 0: {
                        if(snek[0][1]-1==food[1] && snek[0][0]==food[0]) {
                            snek.push_back(snek[i]);
                            newFood();
                        }
                        break;
                    }
                    case 1: {
                        if(snek[0][0]-1==food[0] && snek[0][1]==food[1]) {
                            snek.push_back(snek[i]);
                            newFood();
                        }
                        break;
                    }
                    case 2: {
                        if(snek[0][1]+1==food[1] && snek[0][0]==food[0]) {
                            snek.push_back(snek[i]);
                            newFood();
                        }
                        break;
                    }
                    case 3: {
                        if(snek[0][0]+1==food[0] && snek[0][1]==food[1]) {
                            snek.push_back(snek[i]);
                            newFood();
                            munch.play();
                        }
                        break;
                    }
                }
            }
            switch(direct) {
                case 0: {
                    snek[0][1] = snek[0][1]-1;
                    break;
                }
                case 1: {
                    snek[0][0] = snek[0][0]-1;
                    break;
                }
                case 2: {
                    snek[0][1] = snek[0][1]+1;
                    break;
                }
                case 3: {
                    snek[0][0] = snek[0][0]+1;
                    break;
                }
            }
        } else {
            if(i+1==snek.size()) {
                switch(direct) {
                    case 0: {
                        if(snek[0][1]-1==food[1] && snek[0][0]==food[0]) {
                            snek.push_back(snek[i]);
                            newFood();
                        }
                        break;
                    }
                    case 1: {
                        if(snek[0][0]-1==food[0] && snek[0][1]==food[1]) {
                            snek.push_back(snek[i]);
                            newFood();
                        }
                        break;
                    }
                    case 2: {
                        if(snek[0][1]+1==food[1] && snek[0][0]==food[0]) {
                            snek.push_back(snek[i]);
                            newFood();
                        }
                        break;
                    }
                    case 3: {
                        if(snek[0][0]+1==food[0] && snek[0][1]==food[1]) {
                            snek.push_back(snek[i]);
                            newFood();
                        }
                        break;
                    }
                }
            }
            snek[i]=snek[i-1];
            switch(direct) {
                case 0: {
                    if(snek[0][1]-1==snek[i][1] && snek[0][0]==snek[i][0]) {
                        start = false;
                        game_over = true;
                        GameOver();
                    }
                    break;
                }
                case 1: {
                    if(snek[0][0]-1==snek[i][0] && snek[0][1]==snek[i][1]) {
                        start = false;
                        game_over = true;
                         GameOver();
                    }
                    break;
                }
                case 2: {
                    if(snek[0][1]+1==snek[i][1] && snek[0][0]==snek[i][0]) {
                        start = false;
                        game_over = true;
                         GameOver();
                    }
                    break;
                }
                case 3: {
                    if(snek[0][0]+1==snek[i][0] && snek[0][1]==snek[i][1]) {
                        start = false;
                        game_over = true;
                         GameOver();
                    }
                    break;
                }
            }
        }
    }
    return snek;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    qDebug()<<"Test";
    if(!inverted) {
        switch(key) {
            case Qt::Key_Left: {
                if(snek1_direction_current==0 || snek1_direction_current==2) {
                    snek1_direction_new = 1;
                }
                break;
            }
            case Qt::Key_Right: {
                if(snek1_direction_current==0 || snek1_direction_current==2) {
                    snek1_direction_new = 3;
                }
                break;
            }
            case Qt::Key_Up: {
                if(snek1_direction_current==1 || snek1_direction_current==3) {
                    snek1_direction_new = 0;
                }
                break;
            }
            case Qt::Key_Down: {
                if(snek1_direction_current==1 || snek1_direction_current==3) {
                    snek1_direction_new = 2;
                }
                break;
            }
        }
        switch(key) {
            case Qt::Key_A: {
                if(snek2_direction_current==0 || snek2_direction_current==2) {
                    snek2_direction_new = 1;
                }
                break;
            }
            case Qt::Key_D: {
                if(snek2_direction_current==0 || snek2_direction_current==2) {
                    snek2_direction_new = 3;
                }
                break;
            }
            case Qt::Key_W: {
                if(snek2_direction_current==1 || snek2_direction_current==3) {
                    snek2_direction_new = 0;
                }
                break;
            }
            case Qt::Key_S: {
                if(snek2_direction_current==1 || snek2_direction_current==3) {
                    snek2_direction_new = 2;
                }
                break;
            }
        }
    } else {
        switch(key) {
            case Qt::Key_Right: {
                if(snek1_direction_current==0 || snek1_direction_current==2) {
                    snek1_direction_new = 1;
                }
                break;
            }
            case Qt::Key_Left: {
                if(snek1_direction_current==0 || snek1_direction_current==2) {
                    snek1_direction_new = 3;
                }
                break;
            }
            case Qt::Key_Down: {
                if(snek1_direction_current==1 || snek1_direction_current==3) {
                    snek1_direction_new = 0;
                }
                break;
            }
            case Qt::Key_Up: {
                if(snek1_direction_current==1 || snek1_direction_current==3) {
                    snek1_direction_new = 2;
                }
                break;
            }
        }
        switch(key) {
            case Qt::Key_D: {
                if(snek2_direction_current==0 || snek2_direction_current==2) {
                    snek2_direction_new = 1;
                }
                break;
            }
            case Qt::Key_A: {
                if(snek2_direction_current==0 || snek2_direction_current==2) {
                    snek2_direction_new = 3;
                }
                break;
            }
            case Qt::Key_S: {
                if(snek2_direction_current==1 || snek2_direction_current==3) {
                    snek2_direction_new = 0;
                }
                break;
            }
            case Qt::Key_W: {
                if(snek2_direction_current==1 || snek2_direction_current==3) {
                    snek2_direction_new = 2;
                }
                break;
            }
        }
    }

}

void MainWindow::on_Gamerbut_toggled(bool checked) {
    diffictulty = 3;
    speed = 70;
}

void MainWindow::on_Mediumbut_toggled(bool checked) {
    diffictulty = 2;
    speed = 90;
}

void MainWindow::on_Easybut_toggled(bool checked) {
    diffictulty = 1;
    speed = 100;
}

void MainWindow::on_Player1but_toggled(bool checked) {
    players = 1;
}

void MainWindow::on_Player2but_toggled(bool checked) {
    players = 2;
}

void MainWindow::GameOver() {
    if(game_over) {
    music.stop();
    if(dead.isFinished()) {
        dead.play();
    } else {
        dead.play();
    }
    QMessageBox msgBox;
    QString GameText;
    if(players==2) {
        if(winner==0) {

            if(snek1.size()==snek2.size()) {
               int size = snek1.size();
               GameText = "Thanks for playing. It was a tie. Both player's scores were : "+ QString::fromStdString(to_string(size)+". ") + "The high score is: " + QString::fromStdString(to_string(highscore) + ". ") + "Would you like to play agian?";
               if(size>highscore) {
                   GameText = "Thanks for playing. You guys beat the high score but tied each other. Both player's scores were : "+ QString::fromStdString(to_string(size)+". ") + "Would you like to play agian?";
                   highscore = size;
                   setHighScore();
               } else if(size==highscore) {
                   GameText = "Thanks for playing. You guys tied the high Score and with each other. Both player's scores were : "+ QString::fromStdString(to_string(size)+". ") + "Would you like to play agian?";
               }
            } else if(snek1.size()>snek2.size()) {
                int size = snek1.size();
                GameText = "Thanks for playing. Player 1 wins!!! Player's 1 score was: "+ QString::fromStdString(to_string(size)+". ") + "The high score is: " + QString::fromStdString(to_string(highscore) + ". ") + "Would you like to play agian?";
                if(size>highscore) {
                    GameText = "Thanks for playing. Player 1 wins and beat the high score!!! Player's 1 score was: "+ QString::fromStdString(to_string(size)+". ") + "Would you like to play agian?";
                    highscore = size;
                    setHighScore();
                } else if(size==highscore) {
                    GameText = "Thanks for playing. Player 1 wins and tied the high score!!! Player's 1 scores were : "+ QString::fromStdString(to_string(size)+". ") + "Would you like to play agian?";
                }
            } else {
                int size = snek2.size();
                GameText = "Thanks for playing. Player 2 wins!!! Player's 2 score was: "+ QString::fromStdString(to_string(size)+". ") + "The high score is: " + QString::fromStdString(to_string(highscore) + ". ") + "Would you like to play agian?";
                if(size>highscore) {
                    GameText = "Thanks for playing. Player 2 wins and beat the high score!!! Player's 2 score was: "+ QString::fromStdString(to_string(size)+". ") + "Would you like to play agian?";
                    highscore = size;
                    setHighScore();
                } else if(size==highscore) {
                    GameText = "Thanks for playing. Player 2 wins and tied the high score!!! Player's 2 scores were : "+ QString::fromStdString(to_string(size)+". ") + "Would you like to play agian?";
                }
            }

        } else if(winner==1) {
            int size = snek1.size();
            GameText = "Thanks for playing. Player 1 wins!!! Player's 1 score was: "+ QString::fromStdString(to_string(size)+". ") + "The high score is: " + QString::fromStdString(to_string(highscore) + ". ") + "Would you like to play agian?";
            if(size>highscore) {
                GameText = "Thanks for playing. Player 1 wins and beat the high score!!! Player's 1 score was: "+ QString::fromStdString(to_string(size)+". ") + "Would you like to play agian?";
                highscore = size;
                setHighScore();
            } else if(size==highscore) {
                GameText = "Thanks for playing. Player 1 wins and tied the high score!!! Player's 1 scores were : "+ QString::fromStdString(to_string(size)+". ") + "Would you like to play agian?";
            }
        } else {
            int size = snek2.size();
            GameText = "Thanks for playing. Player 2 wins!!! Player's 2 score was: "+ QString::fromStdString(to_string(size)+". ") + "The high score is: " + QString::fromStdString(to_string(highscore) + ". ") + "Would you like to play agian?";
            if(size>highscore) {
                GameText = "Thanks for playing. Player 2 wins and beat the high score!!! Player's 2 score was: "+ QString::fromStdString(to_string(size)+". ") + "Would you like to play agian?";
                highscore = size;
                setHighScore();
            } else if(size==highscore) {
                GameText = "Thanks for playing. Player 2 wins and tied the high score!!! Player's 2 scores were : "+ QString::fromStdString(to_string(size)+". ") + "Would you like to play agian?";
            }

        }
    } else {
        int size = snek1.size();
        GameText = "Thanks for playing. Your score was: "+ QString::fromStdString(to_string(size) +". ") + "The high score is: " + QString::fromStdString(to_string(highscore) + ". ") + "Would you like to play agian?";
        if(size>highscore) {
            GameText = "Thanks for playing. You beat the high score which is now: "+ QString::fromStdString(to_string(size)+". ") + "Would you like to play agian?";
            highscore = size;
            setHighScore();
        } else if(size==highscore) {
            GameText = "Thanks for playing. You tied the high score!!! Your score was: "+ QString::fromStdString(to_string(size)+". ") + "Would you like to play agian?";
        }
    }
    msgBox.setText(GameText);
    QAbstractButton *yes = msgBox.addButton(tr("Yes"), QMessageBox::YesRole);
    QAbstractButton *no = msgBox.addButton(tr("No"), QMessageBox::YesRole);
    msgBox.exec();
    if(msgBox.clickedButton()==yes) {
        start = false;
        setFixedSize(start_width, start_height);
        ui->Easybut->show();
        ui->Gamerbut->show();
        ui->Beginbut->show();
        ui->Mediumbut->show();
        ui->Player1but->show();
        ui->Player2but->show();
        ui->Instructbut->show();
        ui->sneklab->show();
        ui->playerlab->show();
        ui->ctrInv->show();
        ui->difflab->show();
        begin = 0;
        winner = -1;
    } else {
        qApp->exit(0);
    }
    game_over = false;
    }
}

void MainWindow::on_Instructbut_clicked() {
    QMessageBox msgBox;
    QString instructions = "Welcome to snek(our version of the snake game).\n\n The object of the game is to grow your snake as big as possible by eating the fruit(red circles). Music will start to play and your snake will start moving at a consistent speed, based on the difficulty chosen. \n\nPlayer 1 will always be the green snake and use the arrow keys to turn in the associated direction. \n\nIf two players is selected then a blue snake will appear as well as a green snake head. The green snake will be player 1 and use the arrow keys while player 2 will use the \"WASD\" keys to move around the grid in attempt to grow and survive.\n\n And most importantly ENJOY :).";
    msgBox.setText(instructions);
    msgBox.exec();
}

void MainWindow::on_ctrInv_toggled(bool checked) {
    if(!checked) {
        inverted = false;
    } else {
        inverted = true;
    }
}

