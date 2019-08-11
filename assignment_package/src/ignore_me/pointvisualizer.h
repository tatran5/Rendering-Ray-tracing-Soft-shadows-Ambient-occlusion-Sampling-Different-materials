#pragma once
#include "drawable.h"
#include <implement_me/sampler.h>

class PointVisualizer : public Drawable
{
public:
    PointVisualizer(GLWidget277 *context, Sampler* s);
    virtual void create();
    virtual GLenum drawMode();

private:
    Sampler* sampler;
};
