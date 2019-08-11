#pragma once
#include "drawable.h"
#include <implement_me/sampler.h>

class GridVisualizer : public Drawable
{
public:
    GridVisualizer(GLWidget277 *context, Sampler* s);
    virtual void create();
    virtual GLenum drawMode();

    void setWarpMethod(WarpMethod w);

private:
    Sampler* sampler;
    WarpMethod warpMethod;
};
