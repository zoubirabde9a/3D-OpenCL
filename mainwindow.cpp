#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <qevent.h>
#include <qmessagebox.h>
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::keyPressEvent(QKeyEvent *event){
    std::cout << event->key() << std::endl;
}


void MainWindow::on_actionCPU_triggered()
{
    if (ui->actionCPU->isChecked())
    {
        ui->actionGPU->setChecked(false);
        ui->widget3D->setUpdateMethodGpu(false);
    }

}

void MainWindow::on_actionGPU_triggered()
{
    if (ui->actionGPU->isChecked())
    {
        ui->actionCPU->setChecked(false);
        ui->widget3D->setUpdateMethodGpu(true);
    }
}
