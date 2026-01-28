//
// Created by trbec on 20.01.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved


#include "ui_mainwindow.h"
#include <mainwindow.h>



mainwindow::mainwindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::mainwindow) {
    ui->setupUi(this);

    QListWidgetItem *item = new QListWidgetItem("Test");
    ui->EntrylistWidget->addItem(item);
}

mainwindow::~mainwindow() {
    delete ui;
}
