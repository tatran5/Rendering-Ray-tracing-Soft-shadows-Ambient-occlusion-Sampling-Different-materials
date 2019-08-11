#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "samplercontrols.h"


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *e);

private slots:
    void on_actionQuit_triggered();

    void on_actionDisplay_Controls_triggered();

    void slot_updateNumSamples(int);
    void slot_setDisplayGrid();

private:
    Ui::MainWindow *ui;
    SamplerControls* controlWindow;

signals:
    void sig_updateNumSamples(int);
};


#endif // MAINWINDOW_H
