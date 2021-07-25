#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

 protected:
   virtual void  keyPressEvent(QKeyEvent *event);
private slots:
    void on_actionCPU_triggered();
    void on_actionGPU_triggered();
};

#endif // MAINWINDOW_H
