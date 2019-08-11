#include "samplercontrols.h"
#include "ui_samplercontrols.h"

SamplerControls::SamplerControls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SamplerControls)
{
    ui->setupUi(this);
}

SamplerControls::~SamplerControls()
{
    delete ui;
}
