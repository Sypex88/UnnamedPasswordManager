//
// Created by trbec on 20.01.2026.
//

#ifndef UNTITLED4_MAINWINDOW_H
#define UNTITLED4_MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE

namespace Ui {
    class mainwindow;
}

QT_END_NAMESPACE

class mainwindow : public QMainWindow {
    Q_OBJECT

public:
    explicit mainwindow(QWidget *parent = nullptr);

    ~mainwindow() override;

private:
    Ui::mainwindow *ui;
};


#endif //UNTITLED4_MAINWINDOW_H