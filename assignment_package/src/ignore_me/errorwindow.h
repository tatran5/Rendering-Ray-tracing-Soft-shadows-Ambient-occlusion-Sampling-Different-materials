#pragma once
#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class ErrorWindow;
}

class ErrorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ErrorWindow(QWidget *parent = 0);
    ~ErrorWindow();

protected:
    void keyPressEvent(QKeyEvent *e);


private:
    Ui::ErrorWindow *ui;

public:

    void setMessage(QString s);
};
