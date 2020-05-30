#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ambientocclusion.h"
#include "camera.h"
#include "scene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // Terrain
    void testTerrain(QImage* p_image, float length, Camera *p_cam);


    //For debugging purpose
    void testCamera(QImage* p_image, float length, Camera *p_cam);
    void testSphereIntersection(QImage* p_image, float length, Camera* p_cam);
    void testSquarePlaneIntersection(QImage* p_image, float length, Camera *p_cam);
    void testSceneOneSphereOnePlane(QImage* p_image, float length, Camera* p_cam);
    void testSceneTwoSphereOnePlane(QImage* p_image, float length, Camera* p_cam);
    void testCornellBox(QImage *p_image, float length, Camera *p_cam);
    void testCornellBoxTwoLightsSimple(QImage *p_image, float length, Camera *p_cam);
    void testReflectionRefraction(QImage *p_image, float length, Camera *p_cam);
    void testSoftShadow(QImage *p_image, float length, Camera *p_cam);
    void testSceneAO(QImage *p_image, float length, Camera *p_cam);
    void testSceneAOSoft(QImage *p_image, float length, Camera *p_cam);

private slots:
    void on_createAndSave();
};

#endif // MAINWINDOW_H
