#ifndef SAMPLERCONTROLS_H
#define SAMPLERCONTROLS_H

#include <QWidget>

namespace Ui {
class SamplerControls;
}

class SamplerControls : public QWidget
{
    Q_OBJECT

public:
    explicit SamplerControls(QWidget *parent = 0);
    ~SamplerControls();

private:
    Ui::SamplerControls *ui;
};

#endif // SAMPLERCONTROLS_H
