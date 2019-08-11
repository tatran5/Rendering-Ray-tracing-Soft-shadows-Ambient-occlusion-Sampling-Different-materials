#ifndef SAMPLERCONTROLS_H
#define SAMPLERCONTROLS_H

#include <QWidget>
#include "enums.h"

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

    //Control variables
    unsigned int numSamples;     // How many samples to generate when warping
    SampleMode squareSampleMode; // Which type of square sampling method to use when generating samples
    WarpMethod warpMethod;       // Which shape to warp the square samples onto

public slots:
    void slot_setSamples(int); // Connected to the slider bar and spinbox, and emits sig_setSamples to the main window.
    void slot_setSampleMode(int);
    void slot_setWarpMethod(int);
    void slot_updateNumSampleControls(int);
    void slot_setDisplayGrid();
    void slot_fixWarpMethodBox(int);
    void slot_fixSampleModeBox(int);

    void slot_computePDFs();
    void slot_updatePDFs(float disc, float sphere, float sphereCap, float hemiUnif, float hemiCos);
    void slot_updateThetaMax(int);


signals:
    void sig_setSamples(int);
    void sig_setSampleMode(SampleMode);
    void sig_setWarpMethod(WarpMethod);
    void sig_setDisplayGrid();
    void sig_computePDFs();
    void sig_updateThetaMax(int);
};

#endif // SAMPLERCONTROLS_H
