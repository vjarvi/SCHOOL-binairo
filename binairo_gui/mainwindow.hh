/*
 * This program is user interface for the game binairo. Game logic is implemented
 * in class GameBoard and UI in mainwindow. Objective of the game is to fill the
 * board with zeros and ones. Every row and column must have same number of zeros
 * and ones. Three or more of the same number is not allowed in a row.
 * Click a square to change its value between 0/1/empty. Click 'place' to save and check
 * for wrognly placed numbers. More detailed instructions in 'instructions.txt'.
 *
 *
 * program writer
 * Name: Valtti Järvi
 * Student number: 151326251
 * username: tfvaja
 * E-Mail: valtti.jarvi@tuni.fi
 *
 * */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <vector>
#include <QDebug>
#include <QTimer>
#include <iostream>
#include <sstream>

#include <gameboard.hh>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Activated when start is pressed. Hides widgets used in choosing starting
    // settings, and shows 'place' button. Error check for wrong seed/input.
    // calls draw_board() to generate board.
    void on_pushbutton_start_clicked();

    // changes squares value 0/1/empty.
    void handle_clicks();

    // check if changed square are correct. Adds corrects edited squares to
    // map_ in gameboard and shows wrong squares in red.
    // check if game is over.
    void on_pushbutton_place_number_clicked();

    // updates time for timer
    void update_time();

    // shows the widgets that start-button hides and vice versa. deletes board
    // that start-button generated.
    void on_pushbutton_reset_clicked();

    // option to fill the board randomly, only one of 'random' and 'input'
    // checkbox can be chosen at a time.
    void on_checkbox_random_toggled(bool checked);

    // same than previous but for input option.
    void on_checkbox_input_toggled(bool checked);

    // shows textbrowser for rules
    void on_pushbutton_help_clicked();

    // pauses timer and makes hides everything else.
    void on_pushbutton_pause_clicked();

private:
    Ui::MainWindow *ui;

    GameBoard gameboard;

    QTimer* timer;
    int minutes_ = 0;
    int seconds_ = 0;

    // true if user chooses random input
    bool random_input = true;

    // stores gameboard as buttons for ui
    std::vector<std::vector<QPushButton*>>  buttons_;

    // draws the board
    void draw_board();
};
#endif // MAINWINDOW_HH
