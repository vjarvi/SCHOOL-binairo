/*
 * program writer
 * Name: Valtti Järvi
 * Student number: 151326251
 * username: tfvaja
 * E-Mail: valtti.jarvi@tuni.fi
 *
 * */
#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: #D3D3D3;");
    ui->pushbutton_place_number->hide();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
          this, SLOT(update_time()));
    ui->lcdnumber_minutes->setStyleSheet("QLCDNumber { color: red; background-color: Tomato; }");
    ui->lcdnumber_seconds->setStyleSheet("QLCDNumber {color: red; background-color: Tomato; }");
    ui->lineedit_seed_or_input->setStyleSheet("background-color: white;");
    ui->pushbutton_place_number->setStyleSheet("background-color: white;");
    ui->pushbutton_reset->setStyleSheet("background-color: white;");
    ui->pushbutton_start->setStyleSheet("background-color: white;");
    ui->textbrowser_rules->setText("Objective of the game is to fill the board with"
                                   " zeros and ones. Every row and column must have"
                                   " same number of zeros and ones. Three or more"
                                   " of the same number is not allowed in a row."
                                   " Click a square to change its value. Click "
                                   "'place' to save and check for wrognly placed "
                                   "numbers");
    ui->textbrowser_rules->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}








void MainWindow::on_pushbutton_start_clicked()
{

    if ( ui->checkbox_random->isChecked() == ui->checkbox_input->isChecked()){
        ui->label_general->setText("Choose random or input!");
        return;
    }
    // boolean to check for wrong input
    bool input_check;

    // for random seed
    if ( random_input ){

        int seed = ui->lineedit_seed_or_input->text().toULong(&input_check);
        // error check
        if ( !input_check ){
            ui->label_general->setText("Enter number for seed!");
            return;
        }
        // calls fill_randomly(), false if error
        if ( not gameboard.fill_randomly(seed) ){
            ui->label_general->setText("Bad seed!");
            return;
        }
    // for user input
    } else{
        std::string input = "'" + ui->lineedit_seed_or_input->text().toStdString() + "'";

        if ( not gameboard.fill_from_input(input)){
            ui->label_general->setText("Bad input!");
            return;
        }
    }
    // hide widgets used in start, start timer etc.
    ui->checkbox_random->hide();
    ui->checkbox_input->hide();
    ui->lineedit_seed_or_input->hide();
    ui->pushbutton_place_number->show();
    ui->label_random_or_input->hide();
    ui->label_enter_seed->hide();
    ui->label_general->setText("Play!");
    ui->pushbutton_start->hide();
    timer->start(1000);

    MainWindow::draw_board();
}


void MainWindow::handle_clicks()
{
    // find sender button and change its text and color back to black if needed.
    for ( auto& row : buttons_ ){
        for ( auto& button : row ){

            if ( button == sender() ){
                if ( button->text() == " "){
                    button->setText("0");
                } else if ( button->text() == "0"){
                    button->setText("1");
                } else if ( button->text() == "1"){
                    button->setText(" ");
                }
                button->setStyleSheet("color: black;");
                ui->label_general->setText("Play!");
                return;
            }
        }
    }
}


void MainWindow::on_pushbutton_place_number_clicked()
{

     for (size_t y = 0; y < buttons_.size(); ++y) {
        const auto& row = buttons_[y];
        for (size_t x = 0; x < row.size(); ++x) {
            QPushButton* button = row[x];
            // saves all edited squares to gameboard class
            if ( button->text() != " " and button->isEnabled() ){

                char symbol = button->text().toStdString()[0];
                bool can_add = gameboard.add_symbol(x,y,symbol);

                if ( can_add){
                    button->setEnabled(false);
                    gameboard.print();

                } else{
                    // wrong number in square.
                    button->setStyleSheet("color: red;");
                    ui->label_general->setText("Breaking rules!");
                }
            }
        }
    }
    // check if game is over, stop timer, change background color hide unnencessary buttons.
    if ( gameboard.is_game_over() ){
        timer->stop();

        std::stringstream s;
        std::stringstream m;
        s << seconds_;
        m << minutes_;
        QString ss = QString::fromStdString(s.str());
        QString mm = QString::fromStdString(m.str());

        ui->label_general->setText("You won in time " + mm + " : " + ss );
        ui->pushbutton_help->hide();
        ui->pushbutton_pause->hide();
        ui->pushbutton_place_number->hide();
        this->setStyleSheet("background-color: #FFD700;");
    }
}


void MainWindow::update_time()
{
    seconds_ += 1;
    if (seconds_ % 60 == 0)
    {
        minutes_ += 1;
        seconds_ = 0;
    }
    ui->lcdnumber_seconds->display(seconds_);
    ui->lcdnumber_minutes->display(minutes_);
}


void MainWindow::on_pushbutton_reset_clicked()
{
    //in summary, reverses what 'on_pushbutton_start_clicked()' did.
    ui->checkbox_random->setChecked(false);
    ui->checkbox_random->show();
    ui->checkbox_input->setChecked(false);
    ui->checkbox_input->show();
    ui->label_random_or_input->show();
    ui->label_general->setText("");
    ui->label_enter_seed->show();
    ui->label_enter_seed->setText("Enter seed or input:");
    ui->lineedit_seed_or_input->show();
    ui->lineedit_seed_or_input->setText("");
    ui->pushbutton_place_number->hide();
    ui->pushbutton_start->show();

    this->setStyleSheet("background-color: #D3D3D3;");
    ui->pushbutton_help->show();
    ui->pushbutton_pause->show();

    for (auto &row : buttons_) {
        for (auto &button : row) {
            disconnect(button, &QPushButton::clicked, this, &MainWindow::handle_clicks);
            delete button;
        }
        row.clear();
    }
    buttons_.clear();

    timer->stop();
    seconds_ = 0;
    minutes_ = 0;
    ui->lcdnumber_minutes->display(0);
    ui->lcdnumber_seconds->display(0);
}


void MainWindow::on_checkbox_random_toggled(bool checked)
{
    // if input is chosen, removes it so only one option is chosen
    if ( checked ){
        ui->checkbox_input->setChecked(false);
    }
    random_input = true;
    ui->label_enter_seed->setText("Enter random seed:");
}


void MainWindow::on_checkbox_input_toggled(bool checked)
{
    // if random is chosen, removes it so only one option is chosen
    if ( checked ){
        ui->checkbox_random->setChecked(false);
    }
    random_input = false;
    ui->label_enter_seed->setText("Enter input:");
}


void MainWindow::on_pushbutton_help_clicked()
{
    if ( ui->textbrowser_rules->isVisible() ){
        ui->textbrowser_rules->hide();
    } else{
        ui->textbrowser_rules->show();
    }
}


void MainWindow::on_pushbutton_pause_clicked()
{
    // hides everything if conditions are met.
    if ( not timer->isActive() and not ui->label_random_or_input->isVisible() ){
        timer->start();
        ui->pushbutton_reset->show();
        ui->pushbutton_place_number->show();
        ui->label_general->setText("Play!");
        ui->pushbutton_pause->setText("Pause");

        for ( auto& row : buttons_ ){
            for ( auto& button : row ){
                button->show();
            }
        }
    // if game was paused, this happens.
    } else if ( not ui->label_random_or_input->isVisible() ){
        timer->stop();
        ui->pushbutton_reset->hide();
        ui->pushbutton_place_number->hide();
        ui->label_general->setText("Game paused");
        ui->pushbutton_pause->setText("Paused");

        for ( auto& row : buttons_ ){
            for ( auto& button : row ){
                button->hide();
            }
        }
    }
}


void MainWindow::draw_board()
{
    // parameters for board position.
    const int MARGIN = 61;
    const int SIZE = 30;

    // copies gameboard for drawing.
    std::vector<std::vector<std::string>> board = gameboard.return_board();

    // makes map of button-objects.
    for ( unsigned int i = 0; i<board.size(); ++i ){
        buttons_.push_back(std::vector<QPushButton*>());

        for ( unsigned int j = 0; j<board[i].size(); ++j){

            QPushButton* button = new QPushButton(this);
            button->setGeometry(MARGIN + (j * SIZE+MARGIN),
                                MARGIN + (i * SIZE+MARGIN),
                                SIZE, SIZE);

            QString buttonvalue = QString::fromStdString(board[i][j]);
            button->setText(buttonvalue);
            if ( buttonvalue == "1" or buttonvalue == "0" ){
                button->setEnabled(false);
            }
            button->show();

            connect(button, &QPushButton::clicked,
                    this, &MainWindow::handle_clicks);

            buttons_[i].push_back(button);
        }
    }
}

