#include "pointvisualizer.h"

PointVisualizer::PointVisualizer(GLWidget277 *context, Sampler *s) : Drawable(context), sampler(s)
{}

void PointVisualizer::create()
{
    //Indices
    count = this->sampler->getNumSamples();
    std::vector<GLuint> indices(count);
    for(GLuint i = 0; i < indices.size(); ++i)
    {
        indices[i] = i;
    }
    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices.data(), GL_DYNAMIC_DRAW);

    // Positions and colors
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(float), this->sampler->peekSamples().data(), GL_DYNAMIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(float), this->sampler->peekSampleColors().data(), GL_DYNAMIC_DRAW);

}

GLenum PointVisualizer::drawMode()
{
    return GL_POINTS;
}
