#define _USE_MATH_DEFINES
#include "warpfunctions.h"
#include <math.h>
//#include "../globalincludes.h"
#include "utils.h"

glm::vec3 WarpFunctions::squareToDiskUniform(const glm::vec2 &sample)
{
    float r = std::sqrt(sample.x); //get the radius
    float theta = 2 * M_PI * sample.y;
    return glm::vec3(r * std::cos(theta), r * std::sin(theta), 0.f);
}

glm::vec3 WarpFunctions::squareToDiskConcentric(const glm::vec2 &sample)
{
    //Map uniform random numver to [-1, 1]^2
    glm::vec2 sampleOffset = 2.f * sample - glm::vec2(1.f, 1.f);

    //Handle distort at the origin
    if (sampleOffset.x == 0 && sampleOffset.y == 0) {
        return glm::vec3(0.f, 0.f, 0.f);
    }

    //Concentric mapping to point
    float theta , r;
    if (glm::abs(sampleOffset.x) > std::abs(sampleOffset.y)) {
        r = sampleOffset.x;
        theta = M_PI / 4.f * (sampleOffset.y / sampleOffset.x);
    } else {
        r = sampleOffset.y;
        theta = M_PI / 2.f  - M_PI / 4.f * (sampleOffset.x / sampleOffset.y);
    }
    return r * glm::vec3(glm::cos(theta), glm::sin(theta), 0.f);
}

float WarpFunctions::squareToDiskPDF(const glm::vec3 &sample)
{
    return 1.f / M_PI;
}

glm::vec3 WarpFunctions::squareToSphereUniform(const glm::vec2 &sample)
{
    float z = 1.f -  2 * sample.x;;
    float r = std::sqrt(std::max(0.f, 1.f - z * z));
    float phi = 2 * M_PI * sample.y;
    return glm::vec3(r * std::cos(phi), r * std::sin(phi), z);
}

float WarpFunctions::squareToSphereUniformPDF(const glm::vec3 &sample)
{
    return 1.f / (4.f * M_PI);
}

glm::vec3 WarpFunctions::squareToSphereCapUniform(const glm::vec2 &sample, float thetaMin)
{
    float fraction = thetaMin / 180.f;
    float z = 1.f -  2 * glm::mix(fraction, 1.f, sample.x);
    float r = std::sqrt(std::max(0.f, 1.f - z * z));
    float phi = 2 * M_PI * sample.y;
    return glm::vec3(r * std::cos(phi), r * std::sin(phi), z);
}

float WarpFunctions::squareToSphereCapUniformPDF(const glm::vec3 &sample, float thetaMin)
{
    return 1.f / (2.f * M_PI * (1 - glm::cos(glm::radians(180 - thetaMin)))) ;
}

glm::vec3 WarpFunctions::squareToHemisphereUniform(const glm::vec2 &sample)
{
    float z = sample.x;
    float r = std::sqrt(std::max(0.f, 1.f - z * z));
    float phi = 2 * M_PI * sample.y;
    return glm::vec3(r * std::cos(phi), r * std::sin(phi), z);
}

float WarpFunctions::squareToHemisphereUniformPDF(const glm::vec3 &sample)
{
    return 1.f / (M_PI * 2.f);
}

glm::vec3 WarpFunctions::squareToHemisphereCosine(const glm::vec2 &sample)
{
    glm::vec3 d = squareToDiskConcentric(sample);
    float z = std::sqrt(std::max(0.f, 1 - d.x * d.x - d.y * d.y));
    return glm::vec3(d.x, d.y, z);
}

float WarpFunctions::squareToHemisphereCosinePDF(const glm::vec3 &sample)
{
    //TODO
    float cosTheta = glm::dot(sample, glm::vec3(0.f, 0.f, 1.f));
    return cosTheta / M_PI;
}
