#include "mygl.h"
#include "la.h"

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <implement_me/warpfunctions.h>


MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent),
      prog_flat(this),
      pointVisualizer(this, &sampler),
      gridVisualizer(this, &sampler),
      numSamples(1024), sampleMode(RANDOM), warpMethod(NONE),
      displayGrid(false),
      nope(":/forms/nope.wav"), incorrect(":/forms/incorrect.wav"),
      doh(":/forms/doh.wav"),
      errorWindow(), counter(0)
{
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();

    glDeleteVertexArrays(1, &vao);
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(3);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0, 0, 0, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    // Create and set up the diffuse shader
    // Create and set up the flat lighting shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");
    prog_flat.setModelMatrix(glm::mat4(1.f));

    sampler.generateSamples(numSamples, sampleMode, warpMethod);
    pointVisualizer.create();

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    //    vao.bind();
    glBindVertexArray(vao);
}

void MyGL::resizeGL(int w, int h)
{

    camera.setAspect(w, h);
    glm::mat4 viewproj = camera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    prog_flat.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function updateGL is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    prog_flat.setViewProjMatrix(camera.getViewProj());

    //TODO: Read the warping method from the GUI, and if it's NONE translate the samples
    if(warpMethod == NONE)
    {
        prog_flat.setModelMatrix(glm::translate(glm::mat4(), glm::vec3(-0.5, -0.5, 0)));
    }
    else
    {
        float s = 0.5f;
        prog_flat.setModelMatrix(glm::scale(glm::mat4(), glm::vec3(s, s, s)));
    }

    prog_flat.draw(pointVisualizer);

    if(displayGrid)
    {
        prog_flat.setModelMatrix(glm::translate(glm::mat4(), glm::vec3(-0.5f, -0.5f, 0)));
        prog_flat.draw(gridVisualizer);
    }
}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    if (e->key() == Qt::Key_Escape)
    {
        QApplication::quit();
    }
    else if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
    {
        errorWindow.hide();
    }
}

void MyGL::mousePressEvent(QMouseEvent *e)
{
    mX = e->globalX();
    mY = e->globalY();
}

void MyGL::mouseMoveEvent(QMouseEvent *e)
{
    camera.rotatePhi(mY - e->globalY());
    camera.rotateTheta(e->globalX() - mX);
    mX = e->globalX();
    mY = e->globalY();
    update();
}

float roundToOne(float f)
{
    if(std::fabs(1.f - f) < 0.00001f)
    {
        return 1;
    }
    return f;
}

int MyGL::computePDFs()
{
    try{
    Sampler newSampler;
    int samples = numSamples;

    //Disc
    newSampler.generateSamples(samples, sampleMode, DISC_UNIFORM);
    float discUnifPDF = 0;
    for(int i = 0; i < newSampler.getNumSamples() * 3; i += 3)
    {
        discUnifPDF += WarpFunctions::squareToDiskPDF(glm::vec3(newSampler.peekSamples()[i], newSampler.peekSamples()[i+1], newSampler.peekSamples()[i+2]));
    }
    discUnifPDF /= (newSampler.getNumSamples());
    discUnifPDF *= M_PI;

    //Sphere
    newSampler.generateSamples(samples, sampleMode, SPHERE);
    float spherePDF = 0;
    for(int i = 0; i < newSampler.getNumSamples() * 3; i += 3)
    {
        spherePDF += WarpFunctions::squareToSphereUniformPDF(glm::vec3(newSampler.peekSamples()[i], newSampler.peekSamples()[i+1], newSampler.peekSamples()[i+2]));
    }
    spherePDF /= (newSampler.getNumSamples());
    spherePDF *= 4 * M_PI;

    // Sphere cap
    newSampler.generateSamples(samples, sampleMode, SPHERE_CAP);
    float sphereCapPDF = 0;
    for(int i = 0; i < newSampler.getNumSamples() * 3; i += 3)
    {
        sphereCapPDF += WarpFunctions::squareToSphereCapUniformPDF(glm::vec3(newSampler.peekSamples()[i], newSampler.peekSamples()[i+1], newSampler.peekSamples()[i+2]), sampler.thetaMax);
    }
    sphereCapPDF /= (newSampler.getNumSamples());
    sphereCapPDF *= (2 * M_PI * (1 - glm::cos(glm::radians(180 - sampler.thetaMax))));

    // Hemisphere (unif)
    newSampler.generateSamples(samples, sampleMode, HEMISPHERE_UNIFORM);
    float hemisphereUnifPDF = 0;
    for(int i = 0; i < newSampler.getNumSamples() * 3; i += 3)
    {
        hemisphereUnifPDF += WarpFunctions::squareToHemisphereUniformPDF(glm::vec3(newSampler.peekSamples()[i], newSampler.peekSamples()[i+1], newSampler.peekSamples()[i+2]));
    }
    hemisphereUnifPDF /= (newSampler.getNumSamples());
    hemisphereUnifPDF *= 2 * M_PI;

    // Hemisphere (cosine)
    newSampler.generateSamples(samples, sampleMode, HEMISPHERE_COSINE);
    float hemisphereCosPDF = 0;
    for(int i = 0; i < newSampler.getNumSamples() * 3; i += 3)
    {
        glm::vec3 vec(newSampler.peekSamples()[i], newSampler.peekSamples()[i+1], newSampler.peekSamples()[i+2]);
        float cosTheta = glm::dot(vec, glm::vec3(0,0,1));
        hemisphereCosPDF += WarpFunctions::squareToHemisphereCosinePDF(vec) * M_PI / cosTheta;
    }
    hemisphereCosPDF /= (newSampler.getNumSamples());

    emit sig_sendPDFs(roundToOne(discUnifPDF), roundToOne(spherePDF), roundToOne(sphereCapPDF), roundToOne(hemisphereUnifPDF), roundToOne(hemisphereCosPDF));

    return 0;
    }
    catch(const std::runtime_error& e)
    {
        switch(counter)
        {
        case(0):
            nope.play();
            break;
        case(1):
            incorrect.play();
            break;
        case(2):
            doh.play();
            break;
        }
        counter = (counter + 1) % 3;
        errorWindow.setMessage(QString("You must implement all warping functions before you can test your PDFs!"));
        errorWindow.show();
        errorWindow.setFocus();
    }
}

void MyGL::refreshVisuals()
{
    sampler.generateSamples(numSamples, sampleMode, warpMethod);

    pointVisualizer.destroy(); pointVisualizer.create();

    if(displayGrid)
    {
        gridVisualizer.destroy(); gridVisualizer.create();
    }

    update();
}


void MyGL::slot_setSamples(int i)
{
    //DELETEME
    //TODO: Make it update both color and position VBOs here, but only position VBOs in the other two slots
    numSamples = std::pow(2.f, i);//i;
    emit sig_updateNumSamples(numSamples);
    refreshVisuals();
}

void MyGL::slot_setSampleMode(SampleMode m)
{
    SampleMode prev = sampleMode;
    try
    {
        sampleMode = m;
        refreshVisuals();
    }
    catch(const std::exception& e)
    {
        switch(counter)
        {
        case(0):
            nope.play();
            break;
        case(1):
            incorrect.play();
            break;
        case(2):
            doh.play();
            break;
        }
        counter = (counter + 1) % 3;
        errorWindow.setMessage(QString(e.what()));
        errorWindow.show();
        errorWindow.setFocus();

        int index = 0;
        switch(prev)
        {
        case(RANDOM):
            index = 0;
            break;
        case(GRID):
            index = 1;
            break;
        case(STRATIFIED):
            index = 2;
            break;
        }
        emit sig_fixSampleModeBox(index);
    }
}

void MyGL::slot_setWarpMethod(WarpMethod m)
{
    WarpMethod prev = warpMethod;
    try
    {
        warpMethod = m;
        gridVisualizer.setWarpMethod(m);
        refreshVisuals();
    }
    catch(const std::exception& e)
    {
        switch(counter)
        {
        case(0):
            nope.play();
            break;
        case(1):
            incorrect.play();
            break;
        case(2):
            doh.play();
            break;
        }
        counter = (counter + 1) % 3;
        errorWindow.setMessage(QString(e.what()));
        errorWindow.show();
        errorWindow.setFocus();

        int index = 0;
        switch(prev)
        {
        case(NONE):
            index = 0;
            break;
        case(DISC_UNIFORM):
            index = 1;
            break;
        case(DISK_CONCENTRIC):
            index = 2;
            break;
        case(SPHERE):
            index = 3;
            break;
        case(SPHERE_CAP):
            index = 4;
            break;
        case(HEMISPHERE_UNIFORM):
            index = 5;
            break;
        case(HEMISPHERE_COSINE):
            index = 6;
            break;
        }
        emit sig_fixWarpMethodBox(index);
    }
}

void MyGL::slot_updateThetaMax(int i)
{
    sampler.thetaMax = i;
    refreshVisuals();
}
