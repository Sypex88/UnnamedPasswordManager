//
// Created by trbec on 25.01.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_loginwindow.h" resolved

#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QPushButton>
#include <QLineEdit>
#include <mainwindow.h>


loginwindow::loginwindow(QWidget *parent) : QWidget(parent), ui(new Ui::loginwindow) {
    ui->setupUi(this);

}

loginwindow::~loginwindow() {
    delete ui;
}
void loginwindow::on_loginButton_clicked() {
    if (ui->EmailEdit->text() == "adm" && ui->PasswordEdit->text() == "adm") {
        mainwindow_ = new mainwindow;
        mainwindow_ -> show();
        this->close();
    }
}

