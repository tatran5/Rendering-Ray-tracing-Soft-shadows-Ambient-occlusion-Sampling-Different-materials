#include "errorwindow.h"
#include "ui_errorwindow.h"

ErrorWindow::ErrorWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ErrorWindow)
{
    ui->setupUi(this);
    setFixedSize(size());
    ui->label->setWordWrap(true);
    setFocusPolicy(Qt::StrongFocus);
}

ErrorWindow::~ErrorWindow()
{
    delete ui;
}

void ErrorWindow::setMessage(QString s)
{
    ui->label->setText(s);
}

void ErrorWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Escape)
    {
        close();
    }
}
