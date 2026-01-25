//
// Created by trbec on 25.01.2026.
//

#ifndef UNTITLED4_LOGINWINDOW_H
#define UNTITLED4_LOGINWINDOW_H

#include <QWidget>
#include <ui_mainwindow.h>

#include "mainwindow.h"


QT_BEGIN_NAMESPACE

namespace Ui {
    class loginwindow;
}

QT_END_NAMESPACE

class loginwindow : public QWidget {
    Q_OBJECT

public:
    explicit loginwindow(QWidget *parent = nullptr);

    ~loginwindow() override;
private slots:
    void on_loginButton_clicked();

private:
    Ui::loginwindow *ui;
    mainwindow *mainwindow_ = nullptr;
};


#endif //UNTITLED4_LOGINWINDOW_H