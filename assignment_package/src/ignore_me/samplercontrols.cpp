#include "samplercontrols.h"
#include "ui_samplercontrols.h"

SamplerControls::SamplerControls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SamplerControls)
{
    ui->setupUi(this);
    setFixedSize(size());
}

SamplerControls::~SamplerControls()
{
    delete ui;
}

void SamplerControls::slot_setSamples(int i)
{
    this->numSamples = i;
    emit sig_setSamples(i);
}

void SamplerControls::slot_setSampleMode(int i)
{
    SampleMode mode = RANDOM;
    switch(i)
    {
    case 1:
        mode = GRID;
        break;
    case 2:
        mode = STRATIFIED;
        break;
    }
    emit sig_setSampleMode(mode);
}

void SamplerControls::slot_setWarpMethod(int i)
{
    WarpMethod method = NONE;
    ui->thetaMaxSpinBox->setEnabled(false);
    switch(i)
    {
    case 1:
        method = DISC_UNIFORM;
        break;
    case 2:
        method = DISK_CONCENTRIC;
        break;
    case 3:
        method = SPHERE;
        break;
    case 4:
        method = SPHERE_CAP;
        ui->thetaMaxSpinBox->setEnabled(true);
        break;
    case 5:
        method = HEMISPHERE_UNIFORM;
        break;
    case 6:
        method = HEMISPHERE_COSINE;
        break;
    }
    emit sig_setWarpMethod(method);
}

void SamplerControls::slot_updateNumSampleControls(int i)
{
//    ui->numSamplesSlider->setValue(i);
    ui->numSamplesSpinBox->setValue(i);
}

void SamplerControls::slot_setDisplayGrid()
{
    emit sig_setDisplayGrid();
}

void SamplerControls::slot_fixWarpMethodBox(int i)
{
    ui->comboBox_2->setCurrentIndex(i);
}

void SamplerControls::slot_fixSampleModeBox(int i)
{
    ui->comboBox->setCurrentIndex(i);
}

void SamplerControls::slot_computePDFs()
{
    emit sig_computePDFs();
}

void SamplerControls::slot_updatePDFs(float disc, float sphere, float sphereCap, float hemiUnif, float hemiCos)
{
    ui->discUnifPDFLabel->setText(QString::number(disc, 'f'));
    ui->discConcPDFLabel->setText(QString::number(disc, 'f'));
    ui->spherePDFLabel->setText(QString::number(sphere, 'f'));
    ui->sphereCapPDFLabel->setText(QString::number(sphereCap, 'f'));
    ui->hemiUnifPDFLabel->setText(QString::number(hemiUnif, 'f'));
    ui->hemiCosPDFLabel->setText(QString::number(hemiCos, 'f'));
}

void SamplerControls::slot_updateThetaMax(int i)
{
    emit sig_updateThetaMax(i);
}
