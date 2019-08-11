#include "gridvisualizer.h"

GridVisualizer::GridVisualizer(GLWidget277 *context, Sampler *s) : Drawable(context), sampler(s)
{}

void GridVisualizer::create()
{
    std::vector<glm::vec3> positions, colors;
    std::vector<GLuint> indices;
    int gridRes = (int) (std::sqrt((float) sampler->getNumSamples()) + 0.5f);
    int fineGridRes = 16*gridRes, idx = 0;
    int lineCount = 4 * (gridRes+1) * (fineGridRes+1);
    positions.resize(lineCount);
    colors.resize(lineCount);
    indices.resize(lineCount);
    float coarseScale = 1.f / gridRes, fineScale = 1.f / fineGridRes;
    for (int i=0; i<=gridRes; ++i)
    {
        for (int j=0; j<=fineGridRes; ++j)
        {
            glm::vec3 pt = Sampler::warpSample(glm::vec2(j * fineScale, i * coarseScale), warpMethod, sampler->thetaMax);
            positions[idx] = pt;
            colors[idx] = glm::vec3(0, 0.5f, 0.5f);
            indices[idx] = idx;
            idx++;

            pt = Sampler::warpSample(glm::vec2((j+1) * fineScale, i * coarseScale), warpMethod, sampler->thetaMax);
            positions[idx] = pt;
            colors[idx] = glm::vec3(0, 0.5f, 0.5f);
            indices[idx] = idx;
            idx++;

            pt = Sampler::warpSample(glm::vec2(i * coarseScale, j * fineScale), warpMethod, sampler->thetaMax);
            positions[idx] = pt;
            colors[idx] = glm::vec3(0, 0.5f, 0.5f);
            indices[idx] = idx;
            idx++;

            pt = Sampler::warpSample(glm::vec2(i * coarseScale, (j+1) * fineScale), warpMethod, sampler->thetaMax);
            positions[idx] = pt;
            colors[idx] = glm::vec3(0, 0.5f, 0.5f);
            indices[idx] = idx;
            idx++;
        }
    }
    if (warpMethod != NONE)
    {
        for (int i=0; i<lineCount; ++i)
        {
            positions[i] = positions[i] * 0.5f + glm::vec3(0.5f, 0.5f, 0.0f);
        }
    }

    count = indices.size();


    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices.data(), GL_DYNAMIC_DRAW);

    // Positions and colors
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(float), colors.data(), GL_DYNAMIC_DRAW);

}

GLenum GridVisualizer::drawMode()
{
    return GL_LINES;
}

void GridVisualizer::setWarpMethod(WarpMethod w)
{
    warpMethod = w;
}
