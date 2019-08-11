#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "samplercontrols.h"
#include <iostream>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), controlWindow(new SamplerControls())
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    connect(controlWindow, SIGNAL(sig_setSamples(int)), ui->mygl, SLOT(slot_setSamples(int)));
    connect(controlWindow, SIGNAL(sig_setSampleMode(SampleMode)), ui->mygl, SLOT(slot_setSampleMode(SampleMode)));
    connect(controlWindow, SIGNAL(sig_setWarpMethod(WarpMethod)), ui->mygl, SLOT(slot_setWarpMethod(WarpMethod)));

    connect(this, SIGNAL(sig_updateNumSamples(int)), controlWindow, SLOT(slot_updateNumSampleControls(int)));
    connect(controlWindow, SIGNAL(sig_setDisplayGrid()), this, SLOT(slot_setDisplayGrid()));

    connect(ui->mygl, SIGNAL(sig_fixWarpMethodBox(int)), controlWindow, SLOT(slot_fixWarpMethodBox(int)));
    connect(ui->mygl, SIGNAL(sig_fixSampleModeBox(int)), controlWindow, SLOT(slot_fixSampleModeBox(int)));

    connect(controlWindow, SIGNAL(sig_computePDFs()), ui->mygl, SLOT(computePDFs()));

    connect(ui->mygl, SIGNAL(sig_sendPDFs(float,float,float,float,float)), controlWindow, SLOT(slot_updatePDFs(float,float,float,float,float)));

    connect(controlWindow, SIGNAL(sig_updateThetaMax(int)), ui->mygl, SLOT(slot_updateThetaMax(int)));

    controlWindow->show();
    controlWindow->move(QApplication::desktop()->screen()->rect().center() - this->rect().center() + QPoint(this->width(), 0));
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    controlWindow->close();
    ui->mygl->errorWindow.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionDisplay_Controls_triggered()
{
    controlWindow->show();
}

void MainWindow::slot_updateNumSamples(int i)
{
    emit sig_updateNumSamples(i);
}

void MainWindow::slot_setDisplayGrid()
{
    ui->mygl->displayGrid = !ui->mygl->displayGrid;
    ui->mygl->refreshVisuals();
}
