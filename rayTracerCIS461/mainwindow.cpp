#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QBuffer>
#include <QFileDialog>
#include "sphere.h"
#include "squareplane.h"
#include "lambertmaterial.h"
#include "blinnphongmaterial.h"
#include "specularreflectionmaterial.h"
#include "speculartransmissionmaterial.h"
#include "light.h"
#include "pointlight.h"
#include "arealight.h"
#include "terrainnode.h"
#include "fractalterrain.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_createAndSave, SIGNAL(clicked(bool)), this, SLOT(on_createAndSave()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_createAndSave()
{
    Point3f eye = Point3f(0.f, 0.5f, 12.f);
    Point3f ref = Point3f(0.f, 0.f, 0.f);
    Vector3f worldUp = Point3f(0.f, 1.f, 0.f);
    float fov = 45;
    float length = 400; //pixel
    float nearClip = 0.1f;
    float farClip = 1000.f;
    Camera cam = Camera(eye, ref, worldUp, fov, nearClip, farClip,
                        length, length);
    QImage image(length, length, QImage::Format_RGB32);
    bool saveSuccess = false;

    //Test required render ------------
    image = QImage(length, length, QImage::Format_RGB32);
    testTerrain(&image, length, &cam);
    saveSuccess = image.save("new.png");
    if (saveSuccess) {
        ui->label->setPixmap((QPixmap::fromImage(image)));
        qDebug() << "saved new.png";
    }
}

void MainWindow::testTerrain(QImage *p_image, float length, Camera *p_cam) {
    Scene scene = Scene();
    scene.p_cam = p_cam;
    // Test terrain node
    TerrainNode terNode = TerrainNode();
    FractalTerrain terrain = FractalTerrain(2, terNode);

    for (int x = 0; x < 1; x++) {
        for (int y = 0; y < 1; y++) {
            Ray ray = scene.p_cam->rayCast(Point2f(x, y));
            Intersection intersection = Intersection();
            terrain.getIntersection(ray, &intersection);
            //
           // vec3 rgb = (intersection.m_normal + vec3(1.f, 1.f, 1.f)) * 0.5f;
            //p_image->setPixel(x, y, qRgb(rgb.x * 255.f, rgb.y * 255.f, rgb.z * 255.f));

        }
    }
}

void MainWindow::testSceneAOSoft(QImage *p_image, float length, Camera *p_cam) {
    Scene scene = Scene();
    scene.p_cam = p_cam;

    //create materials-------------
    LambertMaterial whiteDiffuse = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);
    LambertMaterial redDiffuse = LambertMaterial(Color3f(1.f, 0.f, 0.f), &scene);
    LambertMaterial greenDiffuse = LambertMaterial(Color3f(0.f, 1.f, 0.f), &scene);
    LambertMaterial yellowDiffuse = LambertMaterial(Color3f(1.f, 1.f, 0.25f), &scene);
    LambertMaterial purpleSpecular = LambertMaterial(Color3f(0.75f, 0.f, 0.75f), &scene);
    LambertMaterial mirroredSpecular = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);
    LambertMaterial mirroredSpecular2 = LambertMaterial(Color3f(0.25, 0.5, 1.f), &scene);
    LambertMaterial refractiveSpecular = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);
    LambertMaterial airBubbleMat = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);
    LambertMaterial refractiveSpecular2 = LambertMaterial(Color3f(0.15f, 1.f, 0.15f), &scene);
    LambertMaterial emissiveMaterial = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);

    //create walls-----------------
    //back wall
    std::unique_ptr<Primitive> primBackWall(new Primitive);
    std::unique_ptr<SquarePlane> up_backWall(new SquarePlane);
    primBackWall.get()->mup_shape = std::move(up_backWall);
    primBackWall.get()->m_name = "Back wall";
    Transform backWallTrans = Transform(vec3(0.f, 0.f, -5.f), vec3(0.f, 0.f, 0.f), vec3(10.f, 10.f, 1.f));
    primBackWall.get()->mup_shape.get()->m_transform = backWallTrans;
    primBackWall.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primBackWall));

    //right wall
    std::unique_ptr<Primitive> primRightWall(new Primitive);
    std::unique_ptr<SquarePlane> up_rightWall(new SquarePlane);
    primRightWall.get()->mup_shape = std::move(up_rightWall);
    primRightWall.get()->m_name = "Right wall";
    Transform rightWallTrans = Transform(vec3(5.f, 0.f, 0.f), vec3(0.f, -90.f, 0.f), vec3(10.f, 10.f, 1.f));
    primRightWall.get()->mup_shape.get()->m_transform = rightWallTrans;
    primRightWall.get()->mp_material = &greenDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primRightWall));

    //left wall
    std::unique_ptr<Primitive> primLeftWall(new Primitive);
    std::unique_ptr<SquarePlane> up_leftWall(new SquarePlane);
    primLeftWall.get()->mup_shape = std::move(up_leftWall);
    primLeftWall.get()->m_name = "Left wall";
    Transform leftWallTrans = Transform(vec3(-5.f, 0.f, 0.f), vec3(0.f, 90.f, 0.f), vec3(10.f, 10.f, 1.f));
    primLeftWall.get()->mup_shape.get()->m_transform = leftWallTrans;
    primLeftWall.get()->mp_material = &redDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primLeftWall));

    //ceiling
    std::unique_ptr<Primitive> primCeiling(new Primitive);
    std::unique_ptr<SquarePlane> up_ceiling(new SquarePlane);
    primCeiling.get()->mup_shape = std::move(up_ceiling);
    primCeiling.get()->m_name = "Ceiling";
    Transform ceilingTrans = Transform(vec3(0.f, 5.f, 0.f), vec3(90.f, 0.f, 0.f), vec3(10.f, 10.f, 1.f));
    primCeiling.get()->mup_shape.get()->m_transform = ceilingTrans;
    primCeiling.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primCeiling));

    //floor
    std::unique_ptr<Primitive> primFloor(new Primitive);
    std::unique_ptr<SquarePlane> up_floor(new SquarePlane);
    primFloor.get()->mup_shape = std::move(up_floor);
    primFloor.get()->m_name = "Floor";
    Transform floorTrans = Transform(vec3(0.f, -5.f, 0.f), vec3(-90.f, 0.f, 0.f), vec3(10.f, 10.f, 1.f));
    primFloor.get()->mup_shape.get()->m_transform = floorTrans;
    primFloor.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primFloor));

    //create spheres---------------
    //Shiny sphere
    std::unique_ptr<Primitive> primShinySphere(new Primitive);
    std::unique_ptr<Sphere> up_shinySphere(new Sphere);
    primShinySphere.get()->mup_shape = std::move(up_shinySphere);
    primShinySphere.get()->m_name = "Shiny sphere";
    Transform shinySphereTrans = Transform(vec3(-2.f, -5.f, -0.f), vec3(0.f, 0.f, 0.f), vec3(5.f, 5.f, 5.f));
    primShinySphere.get()->mup_shape.get()->m_transform = shinySphereTrans;
    primShinySphere.get()->mp_material = &purpleSpecular;
    scene.m_listUPointerPrim.push_back(std::move(primShinySphere));

    //transparent sphere 1
    std::unique_ptr<Primitive> primTransSphere1(new Primitive);
    std::unique_ptr<Sphere> up_transSphere1(new Sphere);
    primTransSphere1.get()->mup_shape = std::move(up_transSphere1);
    primTransSphere1.get()->m_name = "Transparent sphere 1";
    Transform transSphere1Trans = Transform(vec3(-3.f, 0.f, 2.f), vec3(0.f, 0.f, 0.f), vec3(4.f, 4.f, 4.f));
    primTransSphere1.get()->mup_shape.get()->m_transform = transSphere1Trans;
    primTransSphere1.get()->mp_material = &refractiveSpecular;
    scene.m_listUPointerPrim.push_back(std::move(primTransSphere1));

    //inside transparent sphere 1
    std::unique_ptr<Primitive> primInsideTransSphere1(new Primitive);
    std::unique_ptr<Sphere> up_insideTransSphere1(new Sphere);
    primInsideTransSphere1.get()->mup_shape = std::move(up_insideTransSphere1);
    primInsideTransSphere1.get()->m_name = "Inside transparent sphere 1";
    Transform insideTransSphere1Trans = Transform(vec3(-3.f, 0.f, 2.f), vec3(0.f, 0.f, 0.f), vec3(2.5f, 2.5f, 2.5f));
    primInsideTransSphere1.get()->mup_shape.get()->m_transform = insideTransSphere1Trans;
    primInsideTransSphere1.get()->mp_material = &airBubbleMat;
    scene.m_listUPointerPrim.push_back(std::move(primInsideTransSphere1));

    //transparent sphere green
    std::unique_ptr<Primitive> primInsideTransSphereGreen(new Primitive);
    std::unique_ptr<Sphere> up_insideTransSphereGreen(new Sphere);
    primInsideTransSphereGreen.get()->mup_shape = std::move(up_insideTransSphereGreen);
    primInsideTransSphereGreen.get()->m_name = "Inside transparent sphere green";
    Transform insideTransSphereGreenTrans = Transform(vec3(3.f, -2.f, -3.f), vec3(0.f, 0.f, 0.f), vec3(4.f, 4.f, 4.f));
    primInsideTransSphereGreen.get()->mup_shape.get()->m_transform = insideTransSphereGreenTrans;
    primInsideTransSphereGreen.get()->mp_material = &refractiveSpecular2;
    scene.m_listUPointerPrim.push_back(std::move(primInsideTransSphereGreen));

    //inside opaque 1
    std::unique_ptr<Primitive> primInsideOpaque1Sphere(new Primitive);
    std::unique_ptr<Sphere> up_insideOpaque1Sphere(new Sphere);
    primInsideOpaque1Sphere.get()->mup_shape = std::move(up_insideOpaque1Sphere);
    primInsideOpaque1Sphere.get()->m_name = "Inside opaque 1";
    Transform insideOpaque1Trans = Transform(vec3(3.f, -2.f, -3.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f));
    primInsideOpaque1Sphere.get()->mup_shape.get()->m_transform = insideOpaque1Trans;
    primInsideOpaque1Sphere.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primInsideOpaque1Sphere));

    //reflective sphere blue 1
    std::unique_ptr<Primitive> primReflectiveSphereBlue1(new Primitive);
    std::unique_ptr<Sphere> up_reflectiveSphereBlue1(new Sphere);
    primReflectiveSphereBlue1.get()->mup_shape = std::move(up_reflectiveSphereBlue1);
    primReflectiveSphereBlue1.get()->m_name = "Reflective sphere blue 1";
    Transform reflectiveSphereBlue1Trans = Transform(vec3(-3.f, 3.5f, -3.f), vec3(0.f, 0.f, 0.f), vec3(3.f, 3.f, 3.f));
    primReflectiveSphereBlue1.get()->mup_shape.get()->m_transform = reflectiveSphereBlue1Trans;
    primReflectiveSphereBlue1.get()->mp_material = &mirroredSpecular2;
    scene.m_listUPointerPrim.push_back(std::move(primReflectiveSphereBlue1));

    //transparent sphere 3
    std::unique_ptr<Primitive> primTransSphere3(new Primitive);
    std::unique_ptr<Sphere> up_transSphere3(new Sphere);
    primTransSphere3.get()->mup_shape = std::move(up_transSphere3);
    primTransSphere3.get()->m_name = "Transparent sphere 3";
    Transform transSphere3Trans = Transform(vec3(3.f, 3.5f, -3.f), vec3(0.f, 0.f, 0.f), vec3(3.f, 3.f, 3.f));
    primTransSphere3.get()->mup_shape.get()->m_transform = transSphere3Trans;
    primTransSphere3.get()->mp_material = &refractiveSpecular;
    scene.m_listUPointerPrim.push_back(std::move(primTransSphere3));

    //reflective sphere blue 2
    std::unique_ptr<Primitive> primReflectiveSphereBlue2(new Primitive);
    std::unique_ptr<Sphere> up_reflectiveSphereBlue2(new Sphere);
    primReflectiveSphereBlue2.get()->mup_shape = std::move(up_reflectiveSphereBlue2);
    primReflectiveSphereBlue2.get()->m_name = "Transparent sphere 3";
    Transform reflectiveSphereBlue2Trans = Transform(vec3(3.f, 3.5f, -3.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f));
    primReflectiveSphereBlue2.get()->mup_shape.get()->m_transform = reflectiveSphereBlue2Trans;
    primReflectiveSphereBlue2.get()->mp_material = &mirroredSpecular2;
    scene.m_listUPointerPrim.push_back(std::move(primReflectiveSphereBlue2));

    //yellow sphere
    std::unique_ptr<Primitive> primYellowSphere(new Primitive);
    std::unique_ptr<Sphere> up_yellowSphere(new Sphere);
    primYellowSphere.get()->mup_shape = std::move(up_yellowSphere);
    primYellowSphere.get()->m_name = "Yellow sphere";
    Transform yellowSphereTrans = Transform(vec3(0.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(2.f, 2.f, 2.f));
    primYellowSphere.get()->mup_shape.get()->m_transform = yellowSphereTrans;
    primYellowSphere.get()->mp_material = &yellowDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primYellowSphere));

    //square plane area light ---------------
    std::unique_ptr<Primitive> primAreaLight(new Primitive);
    std::unique_ptr<SquarePlane> up_squareLight(new SquarePlane);
    primAreaLight.get()->mup_shape = std::move(up_squareLight);
    primAreaLight.get()->m_name = "AreaLight";
    Transform areaLightTrans = Transform(vec3(-2.f, 4.9f, 0.f),
                                         vec3(45.f, 0.f, 0.f),
                                         vec3(0.5f, 0.5f, 0.5f));
    primAreaLight.get()->mup_shape.get()->m_transform = areaLightTrans;
    std::unique_ptr<Light>
            up_squareLightLight(new AreaLight(areaLightTrans,
                                              redDiffuse.m_color,
                                              &scene,
                                              primAreaLight.get()->mup_shape.get(), 100));
    primAreaLight.get()->mup_light = std::move(up_squareLightLight);
    primAreaLight.get()->mp_material = &redDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primAreaLight));


    //square plane area light 1 ---------------
    std::unique_ptr<Primitive> primAreaLight1(new Primitive);
    std::unique_ptr<SquarePlane> up_squareLight1(new SquarePlane);
    primAreaLight1.get()->mup_shape = std::move(up_squareLight1);
    primAreaLight1.get()->m_name = "AreaLigh1t";
    Transform areaLightTrans1 = Transform(vec3(2.f, 4.9f, 0.f),
                                         vec3(45.f, 0.f, 0.f),
                                         vec3(0.5f, 0.5f, 0.5f));
    primAreaLight1.get()->mup_shape.get()->m_transform = areaLightTrans1;
    std::unique_ptr<Light>
            up_squareLightLight1(new AreaLight(areaLightTrans1,
                                              greenDiffuse.m_color,
                                              &scene,
                                              primAreaLight1.get()->mup_shape.get(), 100));
    primAreaLight1.get()->mup_light = std::move(up_squareLightLight1);
    primAreaLight1.get()->mp_material = &greenDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primAreaLight1));

    AmbientOcclusion ao = AmbientOcclusion(256, &scene, 0.5f);

    //iterating
    for (int x = 0; x < length; x++) {
        for (int y = 0; y < length; y++) {
            Ray ray = scene.p_cam->rayCast(Point2f(x, y));
            Intersection intersection = Intersection();
            Color3f color = Color3f(0.f, 0.f, 0.f);
            if (scene.getIntersection(ray, &intersection)) {
                const Primitive* p_prim = intersection.mp_objectHit;
                if (p_prim->mp_material != nullptr && p_prim->mup_shape.get() != nullptr) {
                    Color3f aoFract = ao.getAOCol(&intersection);
                    color = p_prim->mp_material->getScatteredColor(&intersection, &ray, 0.f)
                            * aoFract;
                }
                p_image->setPixel(x, y, qRgb(color.x * 255.f, color.y * 255.f, color.z * 255.f));
            }
        }
    }
}

void MainWindow::testSceneAO(QImage *p_image, float length, Camera *p_cam) {
    Scene scene = Scene();
    scene.p_cam = p_cam;

    //create materials-------------
    LambertMaterial whiteDiffuse = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);
    LambertMaterial redDiffuse = LambertMaterial(Color3f(1.f, 0.f, 0.f), &scene);
    LambertMaterial greenDiffuse = LambertMaterial(Color3f(0.f, 1.f, 0.f), &scene);
    LambertMaterial yellowDiffuse = LambertMaterial(Color3f(1.f, 1.f, 0.25f), &scene);
    LambertMaterial purpleSpecular = LambertMaterial(Color3f(0.75f, 0.f, 0.75f), &scene);
    LambertMaterial mirroredSpecular = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);
    LambertMaterial mirroredSpecular2 = LambertMaterial(Color3f(0.25, 0.5, 1.f), &scene);
    LambertMaterial refractiveSpecular = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);
    LambertMaterial airBubbleMat = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);
    LambertMaterial refractiveSpecular2 = LambertMaterial(Color3f(0.15f, 1.f, 0.15f), &scene);
    LambertMaterial emissiveMaterial = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);

    //create walls-----------------
    //back wall
    std::unique_ptr<Primitive> primBackWall(new Primitive);
    std::unique_ptr<SquarePlane> up_backWall(new SquarePlane);
    primBackWall.get()->mup_shape = std::move(up_backWall);
    primBackWall.get()->m_name = "Back wall";
    Transform backWallTrans = Transform(vec3(0.f, 0.f, -5.f), vec3(0.f, 0.f, 0.f), vec3(10.f, 10.f, 1.f));
    primBackWall.get()->mup_shape.get()->m_transform = backWallTrans;
    primBackWall.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primBackWall));

    //right wall
    std::unique_ptr<Primitive> primRightWall(new Primitive);
    std::unique_ptr<SquarePlane> up_rightWall(new SquarePlane);
    primRightWall.get()->mup_shape = std::move(up_rightWall);
    primRightWall.get()->m_name = "Right wall";
    Transform rightWallTrans = Transform(vec3(5.f, 0.f, 0.f), vec3(0.f, -90.f, 0.f), vec3(10.f, 10.f, 1.f));
    primRightWall.get()->mup_shape.get()->m_transform = rightWallTrans;
    primRightWall.get()->mp_material = &greenDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primRightWall));

    //left wall
    std::unique_ptr<Primitive> primLeftWall(new Primitive);
    std::unique_ptr<SquarePlane> up_leftWall(new SquarePlane);
    primLeftWall.get()->mup_shape = std::move(up_leftWall);
    primLeftWall.get()->m_name = "Left wall";
    Transform leftWallTrans = Transform(vec3(-5.f, 0.f, 0.f), vec3(0.f, 90.f, 0.f), vec3(10.f, 10.f, 1.f));
    primLeftWall.get()->mup_shape.get()->m_transform = leftWallTrans;
    primLeftWall.get()->mp_material = &redDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primLeftWall));

    //ceiling
    std::unique_ptr<Primitive> primCeiling(new Primitive);
    std::unique_ptr<SquarePlane> up_ceiling(new SquarePlane);
    primCeiling.get()->mup_shape = std::move(up_ceiling);
    primCeiling.get()->m_name = "Ceiling";
    Transform ceilingTrans = Transform(vec3(0.f, 5.f, 0.f), vec3(90.f, 0.f, 0.f), vec3(10.f, 10.f, 1.f));
    primCeiling.get()->mup_shape.get()->m_transform = ceilingTrans;
    primCeiling.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primCeiling));

    //floor
    std::unique_ptr<Primitive> primFloor(new Primitive);
    std::unique_ptr<SquarePlane> up_floor(new SquarePlane);
    primFloor.get()->mup_shape = std::move(up_floor);
    primFloor.get()->m_name = "Floor";
    Transform floorTrans = Transform(vec3(0.f, -5.f, 0.f), vec3(-90.f, 0.f, 0.f), vec3(10.f, 10.f, 1.f));
    primFloor.get()->mup_shape.get()->m_transform = floorTrans;
    primFloor.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primFloor));

    //create spheres---------------
    //Shiny sphere
    std::unique_ptr<Primitive> primShinySphere(new Primitive);
    std::unique_ptr<Sphere> up_shinySphere(new Sphere);
    primShinySphere.get()->mup_shape = std::move(up_shinySphere);
    primShinySphere.get()->m_name = "Shiny sphere";
    Transform shinySphereTrans = Transform(vec3(-2.f, -5.f, -0.f), vec3(0.f, 0.f, 0.f), vec3(5.f, 5.f, 5.f));
    primShinySphere.get()->mup_shape.get()->m_transform = shinySphereTrans;
    primShinySphere.get()->mp_material = &purpleSpecular;
    scene.m_listUPointerPrim.push_back(std::move(primShinySphere));

    //transparent sphere 1
    std::unique_ptr<Primitive> primTransSphere1(new Primitive);
    std::unique_ptr<Sphere> up_transSphere1(new Sphere);
    primTransSphere1.get()->mup_shape = std::move(up_transSphere1);
    primTransSphere1.get()->m_name = "Transparent sphere 1";
    Transform transSphere1Trans = Transform(vec3(-3.f, 0.f, 2.f), vec3(0.f, 0.f, 0.f), vec3(4.f, 4.f, 4.f));
    primTransSphere1.get()->mup_shape.get()->m_transform = transSphere1Trans;
    primTransSphere1.get()->mp_material = &refractiveSpecular;
    scene.m_listUPointerPrim.push_back(std::move(primTransSphere1));

    //inside transparent sphere 1
    std::unique_ptr<Primitive> primInsideTransSphere1(new Primitive);
    std::unique_ptr<Sphere> up_insideTransSphere1(new Sphere);
    primInsideTransSphere1.get()->mup_shape = std::move(up_insideTransSphere1);
    primInsideTransSphere1.get()->m_name = "Inside transparent sphere 1";
    Transform insideTransSphere1Trans = Transform(vec3(-3.f, 0.f, 2.f), vec3(0.f, 0.f, 0.f), vec3(2.5f, 2.5f, 2.5f));
    primInsideTransSphere1.get()->mup_shape.get()->m_transform = insideTransSphere1Trans;
    primInsideTransSphere1.get()->mp_material = &airBubbleMat;
    scene.m_listUPointerPrim.push_back(std::move(primInsideTransSphere1));

    //transparent sphere green
    std::unique_ptr<Primitive> primInsideTransSphereGreen(new Primitive);
    std::unique_ptr<Sphere> up_insideTransSphereGreen(new Sphere);
    primInsideTransSphereGreen.get()->mup_shape = std::move(up_insideTransSphereGreen);
    primInsideTransSphereGreen.get()->m_name = "Inside transparent sphere green";
    Transform insideTransSphereGreenTrans = Transform(vec3(3.f, -2.f, -3.f), vec3(0.f, 0.f, 0.f), vec3(4.f, 4.f, 4.f));
    primInsideTransSphereGreen.get()->mup_shape.get()->m_transform = insideTransSphereGreenTrans;
    primInsideTransSphereGreen.get()->mp_material = &refractiveSpecular2;
    scene.m_listUPointerPrim.push_back(std::move(primInsideTransSphereGreen));

    //inside opaque 1
    std::unique_ptr<Primitive> primInsideOpaque1Sphere(new Primitive);
    std::unique_ptr<Sphere> up_insideOpaque1Sphere(new Sphere);
    primInsideOpaque1Sphere.get()->mup_shape = std::move(up_insideOpaque1Sphere);
    primInsideOpaque1Sphere.get()->m_name = "Inside opaque 1";
    Transform insideOpaque1Trans = Transform(vec3(3.f, -2.f, -3.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f));
    primInsideOpaque1Sphere.get()->mup_shape.get()->m_transform = insideOpaque1Trans;
    primInsideOpaque1Sphere.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primInsideOpaque1Sphere));

    //reflective sphere blue 1
    std::unique_ptr<Primitive> primReflectiveSphereBlue1(new Primitive);
    std::unique_ptr<Sphere> up_reflectiveSphereBlue1(new Sphere);
    primReflectiveSphereBlue1.get()->mup_shape = std::move(up_reflectiveSphereBlue1);
    primReflectiveSphereBlue1.get()->m_name = "Reflective sphere blue 1";
    Transform reflectiveSphereBlue1Trans = Transform(vec3(-3.f, 3.5f, -3.f), vec3(0.f, 0.f, 0.f), vec3(3.f, 3.f, 3.f));
    primReflectiveSphereBlue1.get()->mup_shape.get()->m_transform = reflectiveSphereBlue1Trans;
    primReflectiveSphereBlue1.get()->mp_material = &mirroredSpecular2;
    scene.m_listUPointerPrim.push_back(std::move(primReflectiveSphereBlue1));

    //transparent sphere 3
    std::unique_ptr<Primitive> primTransSphere3(new Primitive);
    std::unique_ptr<Sphere> up_transSphere3(new Sphere);
    primTransSphere3.get()->mup_shape = std::move(up_transSphere3);
    primTransSphere3.get()->m_name = "Transparent sphere 3";
    Transform transSphere3Trans = Transform(vec3(3.f, 3.5f, -3.f), vec3(0.f, 0.f, 0.f), vec3(3.f, 3.f, 3.f));
    primTransSphere3.get()->mup_shape.get()->m_transform = transSphere3Trans;
    primTransSphere3.get()->mp_material = &refractiveSpecular;
    scene.m_listUPointerPrim.push_back(std::move(primTransSphere3));

    //reflective sphere blue 2
    std::unique_ptr<Primitive> primReflectiveSphereBlue2(new Primitive);
    std::unique_ptr<Sphere> up_reflectiveSphereBlue2(new Sphere);
    primReflectiveSphereBlue2.get()->mup_shape = std::move(up_reflectiveSphereBlue2);
    primReflectiveSphereBlue2.get()->m_name = "Transparent sphere 3";
    Transform reflectiveSphereBlue2Trans = Transform(vec3(3.f, 3.5f, -3.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f));
    primReflectiveSphereBlue2.get()->mup_shape.get()->m_transform = reflectiveSphereBlue2Trans;
    primReflectiveSphereBlue2.get()->mp_material = &mirroredSpecular2;
    scene.m_listUPointerPrim.push_back(std::move(primReflectiveSphereBlue2));

    //yellow sphere
    std::unique_ptr<Primitive> primYellowSphere(new Primitive);
    std::unique_ptr<Sphere> up_yellowSphere(new Sphere);
    primYellowSphere.get()->mup_shape = std::move(up_yellowSphere);
    primYellowSphere.get()->m_name = "Yellow sphere";
    Transform yellowSphereTrans = Transform(vec3(0.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(2.f, 2.f, 2.f));
    primYellowSphere.get()->mup_shape.get()->m_transform = yellowSphereTrans;
    primYellowSphere.get()->mp_material = &yellowDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primYellowSphere));

    //lightSources ---------------
    std::unique_ptr<Primitive> primSquare(new Primitive);
    std::unique_ptr<SquarePlane> up_squareLight(new SquarePlane);
    primSquare.get()->mup_shape = std::move(up_squareLight);
    primSquare.get()->m_name = "Light Source";
    Transform squareTrans = Transform(vec3(0.f, 4.9f, 0.f), vec3(45.f, 0.f, 0.f), vec3(0.5f, 0.5f, 0.5f));
    primSquare.get()->mup_shape.get()->m_transform = squareTrans;
    std::unique_ptr<Light> up_squareLightLight(new PointLight(squareTrans, emissiveMaterial.m_color, &scene));
    primSquare.get()->mup_light = std::move(up_squareLightLight);
    primSquare.get()->mp_material = &emissiveMaterial;
    scene.m_listUPointerPrim.push_back(std::move(primSquare));

    AmbientOcclusion ao = AmbientOcclusion(256, &scene, 0.5f);
    //iterating
    for (int x = 0; x < length; x++) {
        for (int y = 0; y < length; y++) {
            Ray ray = scene.p_cam->rayCast(Point2f(x, y));
            Intersection intersection = Intersection();
            Color3f color = Color3f(0.f, 0.f, 0.f);
            if (scene.getIntersection(ray, &intersection)) {
                const Primitive* p_prim = intersection.mp_objectHit;
                if (p_prim->mp_material != nullptr && p_prim->mup_shape.get() != nullptr) {
                    color = ao.getAOCol(&intersection);
                }
                p_image->setPixel(x, y, qRgb(color.x * 255.f, color.y * 255.f, color.z * 255.f));
            }
        }
    }
}

void MainWindow::testSoftShadow(QImage *p_image, float length, Camera *p_cam)
{
    Scene scene = Scene();
    scene.p_cam = p_cam;

    LambertMaterial whiteDiffuse = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);


    //square plane floor ---------------
    std::unique_ptr<Primitive> primFloor(new Primitive);
    std::unique_ptr<SquarePlane> up_squareFloor(new SquarePlane);
    primFloor.get()->mup_shape = std::move(up_squareFloor);
    primFloor.get()->m_name = "Floor";
    Transform floorTrans = Transform(vec3(0.f, 0.f, 0.f),
                                     vec3(-90.f, 0.f, 0.f),
                                     vec3(10.f, 10.f, 1.f));
    primFloor.get()->mup_shape.get()->m_transform = floorTrans;
    primFloor.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primFloor));

    //sphere ---------------
    std::unique_ptr<Primitive> primSphere(new Primitive);
    std::unique_ptr<Sphere> up_sphere(new Sphere);
    primSphere.get()->mup_shape = std::move(up_sphere);
    primSphere.get()->m_name = "Sphere";
    Transform sphereTrans = Transform(vec3(0.f, 0.5f, 0.f),
                                      vec3(0.f, 0.f, 0.f),
                                      vec3(1.f, 1.f, 1.f));
    primSphere.get()->mup_shape.get()->m_transform = sphereTrans;
    primSphere.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primSphere));

    //square plane area light ---------------
    std::unique_ptr<Primitive> primAreaLight(new Primitive);
    std::unique_ptr<SquarePlane> up_squareLight(new SquarePlane);
    primAreaLight.get()->mup_shape = std::move(up_squareLight);
    primAreaLight.get()->m_name = "AreaLight";
    Transform areaLightTrans = Transform(vec3(0.f, 3.5f, 0.f),
                                         vec3(90.f, 0.f, 0.f),
                                         vec3(2.f, 2.f, 1.f));
    primAreaLight.get()->mup_shape.get()->m_transform = areaLightTrans;
    std::unique_ptr<Light>
            up_squareLightLight(new AreaLight(areaLightTrans,
                                              whiteDiffuse.m_color,
                                              &scene,
                                              primAreaLight.get()->mup_shape.get(), 256));
    primAreaLight.get()->mup_light = std::move(up_squareLightLight);
    primAreaLight.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primAreaLight));


    for (int x = 0; x < length; x++) {
        for (int y = 0; y < length; y++) {
            Ray ray = scene.p_cam->rayCast(Point2f(x, y));
            Intersection intersection = Intersection();
            Color3f color = Color3f(0.f, 0.f, 0.f);
            if (scene.getIntersection(ray, &intersection)) {
                const Primitive* p_prim = intersection.mp_objectHit;
                if (dynamic_cast<AreaLight*>(p_prim->mup_light.get()) != nullptr) {
                    color = p_prim->mup_light.get()->m_hueIntensity;
                } else if (p_prim->mp_material != nullptr &&
                           p_prim->mup_shape.get() != nullptr) {
                    color = p_prim->mp_material->
                            getScatteredColor(&intersection, &ray, 0.f);
                }
                p_image->setPixel(x, y, qRgb(color.x * 255.f,
                                             color.y * 255.f,
                                             color.z * 255.f));
            }
        }
    }
}

void MainWindow::testSceneOneSphereOnePlane(QImage *p_image, float length, Camera *p_cam)
{
    Scene scene = Scene();
    scene.p_cam = p_cam;

    std::unique_ptr<Primitive> primitiveSphere(new Primitive);
    primitiveSphere.get()->m_name = "Sphere";
    std::unique_ptr<Sphere>up_sphere(new Sphere());
    primitiveSphere.get()->mup_shape = std::move(up_sphere);
    Transform primitiveSphereTrans = Transform(vec3(-1.f, 1.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(2.f, 4.f, 2.f));
    primitiveSphere.get()->mup_shape.get()->m_transform = primitiveSphereTrans;

    std::unique_ptr<Primitive> primitiveSquarePlane(new Primitive);
    primitiveSquarePlane.get()->m_name = "SquarePlane";
    std::unique_ptr<SquarePlane> up_sPlane(new SquarePlane());
    primitiveSquarePlane.get()->mup_shape = std::move(up_sPlane);
    Transform primitiveSquarePlaneTrans = Transform(vec3(0.f, 0.f, -5.f), vec3(0.f, 30.f, 0.f), vec3(10.f, 10.f, 1.f));
    primitiveSquarePlane.get()->mup_shape.get()->m_transform = primitiveSquarePlaneTrans;

    scene.m_listUPointerPrim.push_back(std::move(primitiveSphere));
    scene.m_listUPointerPrim.push_back(std::move(primitiveSquarePlane));

    for (int x = 0; x < length; x++) {
        for (int y = 0; y < length; y++) {
            Ray ray = scene.p_cam->rayCast(Point2f(x, y));
            Intersection intersection = Intersection();
            if (scene.getIntersection(ray, &intersection)) {
                vec3 rgb = (intersection.m_normal + vec3(1.f, 1.f, 1.f)) * 0.5f;
                p_image->setPixel(x, y, qRgb(rgb.x * 255.f, rgb.y * 255.f, rgb.z * 255.f));
            }
        }
    }
}

void MainWindow::testCornellBox(QImage *p_image, float length, Camera *p_cam)
{
    Scene scene = Scene();
    scene.p_cam = p_cam;

    //create materials-------------
    LambertMaterial whiteDiffuse = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);
    LambertMaterial redDiffuse = LambertMaterial(Color3f(1.f, 0.f, 0.f), &scene);
    LambertMaterial greenDiffuse = LambertMaterial(Color3f(0.f, 1.f, 0.f), &scene);
    LambertMaterial yellowDiffuse = LambertMaterial(Color3f(1.f, 1.f, 0.25f), &scene);
    LambertMaterial purpleSpecular = LambertMaterial(Color3f(0.75f, 0.f, 0.75f), &scene);
    LambertMaterial mirroredSpecular = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);
    LambertMaterial mirroredSpecular2 = LambertMaterial(Color3f(0.25, 0.5, 1.f), &scene);
    LambertMaterial refractiveSpecular = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);
    LambertMaterial airBubbleMat = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);
    LambertMaterial refractiveSpecular2 = LambertMaterial(Color3f(0.15f, 1.f, 0.15f), &scene);
    LambertMaterial emissiveMaterial = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);

    //create walls-----------------
    //back wall
    std::unique_ptr<Primitive> primBackWall(new Primitive);
    std::unique_ptr<SquarePlane> up_backWall(new SquarePlane);
    primBackWall.get()->mup_shape = std::move(up_backWall);
    primBackWall.get()->m_name = "Back wall";
    Transform backWallTrans = Transform(vec3(0.f, 0.f, -5.f), vec3(0.f, 0.f, 0.f), vec3(10.f, 10.f, 1.f));
    primBackWall.get()->mup_shape.get()->m_transform = backWallTrans;
    primBackWall.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primBackWall));

    //right wall
    std::unique_ptr<Primitive> primRightWall(new Primitive);
    std::unique_ptr<SquarePlane> up_rightWall(new SquarePlane);
    primRightWall.get()->mup_shape = std::move(up_rightWall);
    primRightWall.get()->m_name = "Right wall";
    Transform rightWallTrans = Transform(vec3(5.f, 0.f, 0.f), vec3(0.f, -90.f, 0.f), vec3(10.f, 10.f, 1.f));
    primRightWall.get()->mup_shape.get()->m_transform = rightWallTrans;
    primRightWall.get()->mp_material = &greenDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primRightWall));

    //left wall
    std::unique_ptr<Primitive> primLeftWall(new Primitive);
    std::unique_ptr<SquarePlane> up_leftWall(new SquarePlane);
    primLeftWall.get()->mup_shape = std::move(up_leftWall);
    primLeftWall.get()->m_name = "Left wall";
    Transform leftWallTrans = Transform(vec3(-5.f, 0.f, 0.f), vec3(0.f, 90.f, 0.f), vec3(10.f, 10.f, 1.f));
    primLeftWall.get()->mup_shape.get()->m_transform = leftWallTrans;
    primLeftWall.get()->mp_material = &redDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primLeftWall));

    //ceiling
    std::unique_ptr<Primitive> primCeiling(new Primitive);
    std::unique_ptr<SquarePlane> up_ceiling(new SquarePlane);
    primCeiling.get()->mup_shape = std::move(up_ceiling);
    primCeiling.get()->m_name = "Ceiling";
    Transform ceilingTrans = Transform(vec3(0.f, 5.f, 0.f), vec3(90.f, 0.f, 0.f), vec3(10.f, 10.f, 1.f));
    primCeiling.get()->mup_shape.get()->m_transform = ceilingTrans;
    primCeiling.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primCeiling));

    //floor
    std::unique_ptr<Primitive> primFloor(new Primitive);
    std::unique_ptr<SquarePlane> up_floor(new SquarePlane);
    primFloor.get()->mup_shape = std::move(up_floor);
    primFloor.get()->m_name = "Floor";
    Transform floorTrans = Transform(vec3(0.f, -5.f, 0.f), vec3(-90.f, 0.f, 0.f), vec3(10.f, 10.f, 1.f));
    primFloor.get()->mup_shape.get()->m_transform = floorTrans;
    primFloor.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primFloor));

    //create spheres---------------
    //Shiny sphere
    std::unique_ptr<Primitive> primShinySphere(new Primitive);
    std::unique_ptr<Sphere> up_shinySphere(new Sphere);
    primShinySphere.get()->mup_shape = std::move(up_shinySphere);
    primShinySphere.get()->m_name = "Shiny sphere";
    Transform shinySphereTrans = Transform(vec3(-2.f, -5.f, -0.f), vec3(0.f, 0.f, 0.f), vec3(5.f, 5.f, 5.f));
    primShinySphere.get()->mup_shape.get()->m_transform = shinySphereTrans;
    primShinySphere.get()->mp_material = &purpleSpecular;
    scene.m_listUPointerPrim.push_back(std::move(primShinySphere));

    //transparent sphere 1
    std::unique_ptr<Primitive> primTransSphere1(new Primitive);
    std::unique_ptr<Sphere> up_transSphere1(new Sphere);
    primTransSphere1.get()->mup_shape = std::move(up_transSphere1);
    primTransSphere1.get()->m_name = "Transparent sphere 1";
    Transform transSphere1Trans = Transform(vec3(-3.f, 0.f, 2.f), vec3(0.f, 0.f, 0.f), vec3(4.f, 4.f, 4.f));
    primTransSphere1.get()->mup_shape.get()->m_transform = transSphere1Trans;
    primTransSphere1.get()->mp_material = &refractiveSpecular;
    scene.m_listUPointerPrim.push_back(std::move(primTransSphere1));

    //inside transparent sphere 1
    std::unique_ptr<Primitive> primInsideTransSphere1(new Primitive);
    std::unique_ptr<Sphere> up_insideTransSphere1(new Sphere);
    primInsideTransSphere1.get()->mup_shape = std::move(up_insideTransSphere1);
    primInsideTransSphere1.get()->m_name = "Inside transparent sphere 1";
    Transform insideTransSphere1Trans = Transform(vec3(-3.f, 0.f, 2.f), vec3(0.f, 0.f, 0.f), vec3(2.5f, 2.5f, 2.5f));
    primInsideTransSphere1.get()->mup_shape.get()->m_transform = insideTransSphere1Trans;
    primInsideTransSphere1.get()->mp_material = &airBubbleMat;
    scene.m_listUPointerPrim.push_back(std::move(primInsideTransSphere1));

    //transparent sphere green
    std::unique_ptr<Primitive> primInsideTransSphereGreen(new Primitive);
    std::unique_ptr<Sphere> up_insideTransSphereGreen(new Sphere);
    primInsideTransSphereGreen.get()->mup_shape = std::move(up_insideTransSphereGreen);
    primInsideTransSphereGreen.get()->m_name = "Inside transparent sphere green";
    Transform insideTransSphereGreenTrans = Transform(vec3(3.f, -2.f, -3.f), vec3(0.f, 0.f, 0.f), vec3(4.f, 4.f, 4.f));
    primInsideTransSphereGreen.get()->mup_shape.get()->m_transform = insideTransSphereGreenTrans;
    primInsideTransSphereGreen.get()->mp_material = &refractiveSpecular2;
    scene.m_listUPointerPrim.push_back(std::move(primInsideTransSphereGreen));

    //inside opaque 1
    std::unique_ptr<Primitive> primInsideOpaque1Sphere(new Primitive);
    std::unique_ptr<Sphere> up_insideOpaque1Sphere(new Sphere);
    primInsideOpaque1Sphere.get()->mup_shape = std::move(up_insideOpaque1Sphere);
    primInsideOpaque1Sphere.get()->m_name = "Inside opaque 1";
    Transform insideOpaque1Trans = Transform(vec3(3.f, -2.f, -3.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f));
    primInsideOpaque1Sphere.get()->mup_shape.get()->m_transform = insideOpaque1Trans;
    primInsideOpaque1Sphere.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primInsideOpaque1Sphere));

    //reflective sphere blue 1
    std::unique_ptr<Primitive> primReflectiveSphereBlue1(new Primitive);
    std::unique_ptr<Sphere> up_reflectiveSphereBlue1(new Sphere);
    primReflectiveSphereBlue1.get()->mup_shape = std::move(up_reflectiveSphereBlue1);
    primReflectiveSphereBlue1.get()->m_name = "Reflective sphere blue 1";
    Transform reflectiveSphereBlue1Trans = Transform(vec3(-3.f, 3.5f, -3.f), vec3(0.f, 0.f, 0.f), vec3(3.f, 3.f, 3.f));
    primReflectiveSphereBlue1.get()->mup_shape.get()->m_transform = reflectiveSphereBlue1Trans;
    primReflectiveSphereBlue1.get()->mp_material = &mirroredSpecular2;
    scene.m_listUPointerPrim.push_back(std::move(primReflectiveSphereBlue1));

    //transparent sphere 3
    std::unique_ptr<Primitive> primTransSphere3(new Primitive);
    std::unique_ptr<Sphere> up_transSphere3(new Sphere);
    primTransSphere3.get()->mup_shape = std::move(up_transSphere3);
    primTransSphere3.get()->m_name = "Transparent sphere 3";
    Transform transSphere3Trans = Transform(vec3(3.f, 3.5f, -3.f), vec3(0.f, 0.f, 0.f), vec3(3.f, 3.f, 3.f));
    primTransSphere3.get()->mup_shape.get()->m_transform = transSphere3Trans;
    primTransSphere3.get()->mp_material = &refractiveSpecular;
    scene.m_listUPointerPrim.push_back(std::move(primTransSphere3));

    //reflective sphere blue 2
    std::unique_ptr<Primitive> primReflectiveSphereBlue2(new Primitive);
    std::unique_ptr<Sphere> up_reflectiveSphereBlue2(new Sphere);
    primReflectiveSphereBlue2.get()->mup_shape = std::move(up_reflectiveSphereBlue2);
    primReflectiveSphereBlue2.get()->m_name = "Transparent sphere 3";
    Transform reflectiveSphereBlue2Trans = Transform(vec3(3.f, 3.5f, -3.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f));
    primReflectiveSphereBlue2.get()->mup_shape.get()->m_transform = reflectiveSphereBlue2Trans;
    primReflectiveSphereBlue2.get()->mp_material = &mirroredSpecular2;
    scene.m_listUPointerPrim.push_back(std::move(primReflectiveSphereBlue2));

    //yellow sphere
    std::unique_ptr<Primitive> primYellowSphere(new Primitive);
    std::unique_ptr<Sphere> up_yellowSphere(new Sphere);
    primYellowSphere.get()->mup_shape = std::move(up_yellowSphere);
    primYellowSphere.get()->m_name = "Yellow sphere";
    Transform yellowSphereTrans = Transform(vec3(0.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(2.f, 2.f, 2.f));
    primYellowSphere.get()->mup_shape.get()->m_transform = yellowSphereTrans;
    primYellowSphere.get()->mp_material = &yellowDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primYellowSphere));

    //lightSources ---------------
    std::unique_ptr<Primitive> primSquare(new Primitive);
    std::unique_ptr<SquarePlane> up_squareLight(new SquarePlane);
    primSquare.get()->mup_shape = std::move(up_squareLight);
    primSquare.get()->m_name = "Light Source";
    Transform squareTrans = Transform(vec3(0.f, 4.9f, 0.f), vec3(45.f, 0.f, 0.f), vec3(0.5f, 0.5f, 0.5f));
    primSquare.get()->mup_shape.get()->m_transform = squareTrans;
    std::unique_ptr<Light> up_squareLightLight(new PointLight(squareTrans, emissiveMaterial.m_color, &scene));
    primSquare.get()->mup_light = std::move(up_squareLightLight);
    primSquare.get()->mp_material = &emissiveMaterial;
    scene.m_listUPointerPrim.push_back(std::move(primSquare));

    //iterating
    for (int x = 0; x < length; x++) {
        for (int y = 0; y < length; y++) {
            Ray ray = scene.p_cam->rayCast(Point2f(x, y));
            Intersection intersection = Intersection();
            Color3f color = Color3f(0.f, 0.f, 0.f);
            if (scene.getIntersection(ray, &intersection)) {
                const Primitive* p_prim = intersection.mp_objectHit;
                if (p_prim->mp_material != nullptr && p_prim->mup_shape.get() != nullptr) {
                    color = p_prim->mp_material->getScatteredColor(&intersection, &ray, 0.f);
                }
                p_image->setPixel(x, y, qRgb(color.x * 255.f, color.y * 255.f, color.z * 255.f));
            }
        }
    }
}

void MainWindow::testCornellBoxTwoLightsSimple(QImage *p_image, float length, Camera *p_cam)
{
    Scene scene = Scene();
    scene.p_cam = p_cam;

    //create materials-------------
    LambertMaterial whiteDiffuse = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);
    LambertMaterial redDiffuse = LambertMaterial(Color3f(1.f, 0.f, 0.f), &scene);
    LambertMaterial greenDiffuse = LambertMaterial(Color3f(0.f, 1.f, 0.f), &scene);
    LambertMaterial yellowDiffuse = LambertMaterial(Color3f(1.f, 1.f, 0.25f), &scene);
    LambertMaterial purpleSpecular = LambertMaterial(Color3f(0.75f, 0.f, 0.75f), &scene);
    LambertMaterial mirroredSpecular = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);
    LambertMaterial mirroredSpecular2 = LambertMaterial(Color3f(0.25, 0.5, 1.f), &scene);
    LambertMaterial refractiveSpecular = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);
    LambertMaterial airBubbleMat = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);
    LambertMaterial refractiveSpecular2 = LambertMaterial(Color3f(0.15f, 1.f, 0.15f), &scene);
    LambertMaterial emissiveMaterial = LambertMaterial(Color3f(1.f, 0.5f, 0.5f), &scene);
    LambertMaterial emissiveMaterial2 = LambertMaterial(Color3f(0.5f, 1.f, 0.5f), &scene);

    //create walls-----------------
    //back wall
    std::unique_ptr<Primitive> primBackWall(new Primitive);
    std::unique_ptr<SquarePlane> up_backWall(new SquarePlane);
    primBackWall.get()->mup_shape = std::move(up_backWall);
    primBackWall.get()->m_name = "Back wall";
    Transform backWallTrans = Transform(vec3(0.f, 0.f, -5.f), vec3(0.f, 0.f, 0.f), vec3(10.f, 10.f, 1.f));
    primBackWall.get()->mup_shape.get()->m_transform = backWallTrans;
    primBackWall.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primBackWall));

    //right wall
    std::unique_ptr<Primitive> primRightWall(new Primitive);
    std::unique_ptr<SquarePlane> up_rightWall(new SquarePlane);
    primRightWall.get()->mup_shape = std::move(up_rightWall);
    primRightWall.get()->m_name = "Right wall";
    Transform rightWallTrans = Transform(vec3(5.f, 0.f, 0.f), vec3(0.f, -90.f, 0.f), vec3(10.f, 10.f, 1.f));
    primRightWall.get()->mup_shape.get()->m_transform = rightWallTrans;
    primRightWall.get()->mp_material = &greenDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primRightWall));

    //left wall
    std::unique_ptr<Primitive> primLeftWall(new Primitive);
    std::unique_ptr<SquarePlane> up_leftWall(new SquarePlane);
    primLeftWall.get()->mup_shape = std::move(up_leftWall);
    primLeftWall.get()->m_name = "Left wall";
    Transform leftWallTrans = Transform(vec3(-5.f, 0.f, 0.f), vec3(0.f, 90.f, 0.f), vec3(10.f, 10.f, 1.f));
    primLeftWall.get()->mup_shape.get()->m_transform = leftWallTrans;
    primLeftWall.get()->mp_material = &redDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primLeftWall));

    //ceiling
    std::unique_ptr<Primitive> primCeiling(new Primitive);
    std::unique_ptr<SquarePlane> up_ceiling(new SquarePlane);
    primCeiling.get()->mup_shape = std::move(up_ceiling);
    primCeiling.get()->m_name = "Ceiling";
    Transform ceilingTrans = Transform(vec3(0.f, 5.f, 0.f), vec3(90.f, 0.f, 0.f), vec3(10.f, 10.f, 1.f));
    primCeiling.get()->mup_shape.get()->m_transform = ceilingTrans;
    primCeiling.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primCeiling));

    //floor
    std::unique_ptr<Primitive> primFloor(new Primitive);
    std::unique_ptr<SquarePlane> up_floor(new SquarePlane);
    primFloor.get()->mup_shape = std::move(up_floor);
    primFloor.get()->m_name = "Floor";
    Transform floorTrans = Transform(vec3(0.f, -5.f, 0.f), vec3(-90.f, 0.f, 0.f), vec3(10.f, 10.f, 1.f));
    primFloor.get()->mup_shape.get()->m_transform = floorTrans;
    primFloor.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primFloor));

    //create spheres---------------
    //Shiny sphere
    std::unique_ptr<Primitive> primShinySphere(new Primitive);
    std::unique_ptr<Sphere> up_shinySphere(new Sphere);
    primShinySphere.get()->mup_shape = std::move(up_shinySphere);
    primShinySphere.get()->m_name = "Shiny sphere";
    Transform shinySphereTrans = Transform(vec3(-2.f, -5.f, -0.f), vec3(0.f, 0.f, 0.f), vec3(5.f, 5.f, 5.f));
    primShinySphere.get()->mup_shape.get()->m_transform = shinySphereTrans;
    primShinySphere.get()->mp_material = &purpleSpecular;
    scene.m_listUPointerPrim.push_back(std::move(primShinySphere));

    //transparent sphere 1
    std::unique_ptr<Primitive> primTransSphere1(new Primitive);
    std::unique_ptr<Sphere> up_transSphere1(new Sphere);
    primTransSphere1.get()->mup_shape = std::move(up_transSphere1);
    primTransSphere1.get()->m_name = "Transparent sphere 1";
    Transform transSphere1Trans = Transform(vec3(-3.f, 0.f, 2.f), vec3(0.f, 0.f, 0.f), vec3(4.f, 4.f, 4.f));
    primTransSphere1.get()->mup_shape.get()->m_transform = transSphere1Trans;
    primTransSphere1.get()->mp_material = &refractiveSpecular;
    scene.m_listUPointerPrim.push_back(std::move(primTransSphere1));

    //inside transparent sphere 1
    std::unique_ptr<Primitive> primInsideTransSphere1(new Primitive);
    std::unique_ptr<Sphere> up_insideTransSphere1(new Sphere);
    primInsideTransSphere1.get()->mup_shape = std::move(up_insideTransSphere1);
    primInsideTransSphere1.get()->m_name = "Inside transparent sphere 1";
    Transform insideTransSphere1Trans = Transform(vec3(-3.f, 0.f, 2.f), vec3(0.f, 0.f, 0.f), vec3(2.5f, 2.5f, 2.5f));
    primInsideTransSphere1.get()->mup_shape.get()->m_transform = insideTransSphere1Trans;
    primInsideTransSphere1.get()->mp_material = &airBubbleMat;
    scene.m_listUPointerPrim.push_back(std::move(primInsideTransSphere1));

    //transparent sphere green
    std::unique_ptr<Primitive> primInsideTransSphereGreen(new Primitive);
    std::unique_ptr<Sphere> up_insideTransSphereGreen(new Sphere);
    primInsideTransSphereGreen.get()->mup_shape = std::move(up_insideTransSphereGreen);
    primInsideTransSphereGreen.get()->m_name = "Inside transparent sphere green";
    Transform insideTransSphereGreenTrans = Transform(vec3(3.f, -2.f, -3.f), vec3(0.f, 0.f, 0.f), vec3(4.f, 4.f, 4.f));
    primInsideTransSphereGreen.get()->mup_shape.get()->m_transform = insideTransSphereGreenTrans;
    primInsideTransSphereGreen.get()->mp_material = &refractiveSpecular2;
    scene.m_listUPointerPrim.push_back(std::move(primInsideTransSphereGreen));

    //inside opaque 1
    std::unique_ptr<Primitive> primInsideOpaque1Sphere(new Primitive);
    std::unique_ptr<Sphere> up_insideOpaque1Sphere(new Sphere);
    primInsideOpaque1Sphere.get()->mup_shape = std::move(up_insideOpaque1Sphere);
    primInsideOpaque1Sphere.get()->m_name = "Inside opaque 1";
    Transform insideOpaque1Trans = Transform(vec3(3.f, -2.f, -3.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f));
    primInsideOpaque1Sphere.get()->mup_shape.get()->m_transform = insideOpaque1Trans;
    primInsideOpaque1Sphere.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primInsideOpaque1Sphere));

    //reflective sphere blue 1
    std::unique_ptr<Primitive> primReflectiveSphereBlue1(new Primitive);
    std::unique_ptr<Sphere> up_reflectiveSphereBlue1(new Sphere);
    primReflectiveSphereBlue1.get()->mup_shape = std::move(up_reflectiveSphereBlue1);
    primReflectiveSphereBlue1.get()->m_name = "Reflective sphere blue 1";
    Transform reflectiveSphereBlue1Trans = Transform(vec3(-3.f, 3.5f, -3.f), vec3(0.f, 0.f, 0.f), vec3(3.f, 3.f, 3.f));
    primReflectiveSphereBlue1.get()->mup_shape.get()->m_transform = reflectiveSphereBlue1Trans;
    primReflectiveSphereBlue1.get()->mp_material = &mirroredSpecular2;
    scene.m_listUPointerPrim.push_back(std::move(primReflectiveSphereBlue1));

    //transparent sphere 3
    std::unique_ptr<Primitive> primTransSphere3(new Primitive);
    std::unique_ptr<Sphere> up_transSphere3(new Sphere);
    primTransSphere3.get()->mup_shape = std::move(up_transSphere3);
    primTransSphere3.get()->m_name = "Transparent sphere 3";
    Transform transSphere3Trans = Transform(vec3(3.f, 3.5f, -3.f), vec3(0.f, 0.f, 0.f), vec3(3.f, 3.f, 3.f));
    primTransSphere3.get()->mup_shape.get()->m_transform = transSphere3Trans;
    primTransSphere3.get()->mp_material = &refractiveSpecular;
    scene.m_listUPointerPrim.push_back(std::move(primTransSphere3));

    //reflective sphere blue 2
    std::unique_ptr<Primitive> primReflectiveSphereBlue2(new Primitive);
    std::unique_ptr<Sphere> up_reflectiveSphereBlue2(new Sphere);
    primReflectiveSphereBlue2.get()->mup_shape = std::move(up_reflectiveSphereBlue2);
    primReflectiveSphereBlue2.get()->m_name = "Transparent sphere 3";
    Transform reflectiveSphereBlue2Trans = Transform(vec3(3.f, 3.5f, -3.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f));
    primReflectiveSphereBlue2.get()->mup_shape.get()->m_transform = reflectiveSphereBlue2Trans;
    primReflectiveSphereBlue2.get()->mp_material = &mirroredSpecular2;
    scene.m_listUPointerPrim.push_back(std::move(primReflectiveSphereBlue2));

    //yellow sphere
    std::unique_ptr<Primitive> primYellowSphere(new Primitive);
    std::unique_ptr<Sphere> up_yellowSphere(new Sphere);
    primYellowSphere.get()->mup_shape = std::move(up_yellowSphere);
    primYellowSphere.get()->m_name = "Yellow sphere";
    Transform yellowSphereTrans = Transform(vec3(0.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(2.f, 2.f, 2.f));
    primYellowSphere.get()->mup_shape.get()->m_transform = yellowSphereTrans;
    primYellowSphere.get()->mp_material = &yellowDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primYellowSphere));

    //lightSources ---------------
    //light 1
    std::unique_ptr<Primitive> primSquare1(new Primitive);
    std::unique_ptr<SquarePlane> up_squareLight1(new SquarePlane);
    primSquare1.get()->mup_shape = std::move(up_squareLight1);
    primSquare1.get()->m_name = "Light Source";
    Transform square1Trans = Transform(vec3(-2.f, 4.9f, 0.f), vec3(45.f, 0.f, 0.f), vec3(0.5f, 0.5f, 0.5f));
    primSquare1.get()->mup_shape.get()->m_transform = square1Trans;
    std::unique_ptr<Light> up_squareLight1Light(new PointLight(square1Trans, emissiveMaterial.m_color, &scene));
    primSquare1.get()->mup_light = std::move(up_squareLight1Light);
    primSquare1.get()->mp_material = &emissiveMaterial;
    scene.m_listUPointerPrim.push_back(std::move(primSquare1));

    //light 2
    std::unique_ptr<Primitive> primSquare2(new Primitive);
    std::unique_ptr<SquarePlane> up_squareLight2(new SquarePlane);
    primSquare2.get()->mup_shape = std::move(up_squareLight2);
    primSquare2.get()->m_name = "Light Source";
    Transform square2Trans = Transform(vec3(2.f, 4.9f, 0.f), vec3(45.f, 0.f, 0.f), vec3(0.5f, 0.5f, 0.5f));
    primSquare2.get()->mup_shape.get()->m_transform = square2Trans;
    std::unique_ptr<Light> up_squareLight2Light(new PointLight(square2Trans, emissiveMaterial2.m_color, &scene));
    primSquare2.get()->mup_light = std::move(up_squareLight2Light);
    primSquare2.get()->mp_material = &emissiveMaterial2;
    scene.m_listUPointerPrim.push_back(std::move(primSquare2));

    //iterating
    for (int x = 0; x < length; x++) {
        for (int y = 0; y < length; y++) {
            Ray ray = scene.p_cam->rayCast(Point2f(x, y));
            Intersection intersection = Intersection();
            Color3f color = Color3f(0.f, 0.f, 0.f);
            if (scene.getIntersection(ray, &intersection)) {
                const Primitive* p_prim = intersection.mp_objectHit;
                if (p_prim->mp_material != nullptr && p_prim->mup_shape.get() != nullptr) {
                    color = p_prim->mp_material->getScatteredColor(&intersection, &ray, 0.f);
                }
                p_image->setPixel(x, y, qRgb(color.x * 255.f, color.y * 255.f, color.z * 255.f));
            }
        }
    }
}

void MainWindow::testReflectionRefraction(QImage *p_image, float length, Camera *p_cam) {
    Scene scene = Scene();
    scene.p_cam = p_cam;
    int maxRecurDepth = 5;

    //create materials-------------
    LambertMaterial whiteDiffuse = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);
    LambertMaterial redDiffuse = LambertMaterial(Color3f(1.f, 0.f, 0.f), &scene);
    LambertMaterial greenDiffuse = LambertMaterial(Color3f(0.f, 1.f, 0.f), &scene);
    LambertMaterial yellowDiffuse = LambertMaterial(Color3f(1.f, 1.f, 0.25f), &scene);
    BlinnPhongMaterial purpleSpecular =
            BlinnPhongMaterial(Color3f(0.75f, 0.f, 0.75f), &scene,
                               10.f, Color3f(1.f, 1.f, 1.f));
    SpecularReflectionMaterial mirroredSpecular =
            SpecularReflectionMaterial(Color3f(1.f, 1.f, 1.f), &scene, maxRecurDepth);
    SpecularReflectionMaterial mirroredSpecular2 =
            SpecularReflectionMaterial(Color3f(0.25, 0.5, 1.f), &scene, maxRecurDepth);
    SpecularTransmissionMaterial refractiveSpecular =
            SpecularTransmissionMaterial(Color3f(1.f, 1.f, 1.f), &scene, maxRecurDepth,
                                         1.33f, 1.f);
    SpecularTransmissionMaterial airBubbleMat =
            SpecularTransmissionMaterial(Color3f(1.f, 1.f, 1.f), &scene, maxRecurDepth,
                                         1.f, 1.33f);
    SpecularTransmissionMaterial refractiveSpecular2 =
            SpecularTransmissionMaterial(Color3f(0.15f, 1.f, 0.15f), &scene, maxRecurDepth,
                                         1.33f, 1.f);
    LambertMaterial emissiveMaterial = LambertMaterial(Color3f(1.f, 1.f, 1.f), &scene);

    //create walls-----------------
    //back wall
    std::unique_ptr<Primitive> primBackWall(new Primitive);
    std::unique_ptr<SquarePlane> up_backWall(new SquarePlane);
    primBackWall.get()->mup_shape = std::move(up_backWall);
    primBackWall.get()->m_name = "Back wall";
    Transform backWallTrans = Transform(vec3(0.f, 0.f, -5.f), vec3(0.f, 0.f, 0.f), vec3(10.f, 10.f, 1.f));
    primBackWall.get()->mup_shape.get()->m_transform = backWallTrans;
    primBackWall.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primBackWall));

    //right wall
    std::unique_ptr<Primitive> primRightWall(new Primitive);
    std::unique_ptr<SquarePlane> up_rightWall(new SquarePlane);
    primRightWall.get()->mup_shape = std::move(up_rightWall);
    primRightWall.get()->m_name = "Right wall";
    Transform rightWallTrans = Transform(vec3(5.f, 0.f, 0.f), vec3(0.f, -90.f, 0.f), vec3(10.f, 10.f, 1.f));
    primRightWall.get()->mup_shape.get()->m_transform = rightWallTrans;
    primRightWall.get()->mp_material = &greenDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primRightWall));

    //left wall
    std::unique_ptr<Primitive> primLeftWall(new Primitive);
    std::unique_ptr<SquarePlane> up_leftWall(new SquarePlane);
    primLeftWall.get()->mup_shape = std::move(up_leftWall);
    primLeftWall.get()->m_name = "Left wall";
    Transform leftWallTrans = Transform(vec3(-5.f, 0.f, 0.f), vec3(0.f, 90.f, 0.f), vec3(10.f, 10.f, 1.f));
    primLeftWall.get()->mup_shape.get()->m_transform = leftWallTrans;
    primLeftWall.get()->mp_material = &redDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primLeftWall));

    //ceiling
    std::unique_ptr<Primitive> primCeiling(new Primitive);
    std::unique_ptr<SquarePlane> up_ceiling(new SquarePlane);
    primCeiling.get()->mup_shape = std::move(up_ceiling);
    primCeiling.get()->m_name = "Ceiling";
    Transform ceilingTrans = Transform(vec3(0.f, 5.f, 0.f), vec3(90.f, 0.f, 0.f), vec3(10.f, 10.f, 1.f));
    primCeiling.get()->mup_shape.get()->m_transform = ceilingTrans;
    primCeiling.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primCeiling));

    //floor
    std::unique_ptr<Primitive> primFloor(new Primitive);
    std::unique_ptr<SquarePlane> up_floor(new SquarePlane);
    primFloor.get()->mup_shape = std::move(up_floor);
    primFloor.get()->m_name = "Floor";
    Transform floorTrans = Transform(vec3(0.f, -5.f, 0.f), vec3(-90.f, 0.f, 0.f), vec3(10.f, 10.f, 1.f));
    primFloor.get()->mup_shape.get()->m_transform = floorTrans;
    primFloor.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primFloor));

    //create spheres---------------
    //Shiny sphere
    std::unique_ptr<Primitive> primShinySphere(new Primitive);
    std::unique_ptr<Sphere> up_shinySphere(new Sphere);
    primShinySphere.get()->mup_shape = std::move(up_shinySphere);
    primShinySphere.get()->m_name = "Shiny sphere";
    Transform shinySphereTrans = Transform(vec3(-2.f, -5.f, -0.f), vec3(0.f, 0.f, 0.f), vec3(5.f, 5.f, 5.f));
    primShinySphere.get()->mup_shape.get()->m_transform = shinySphereTrans;
    primShinySphere.get()->mp_material = &purpleSpecular;
    scene.m_listUPointerPrim.push_back(std::move(primShinySphere));

    //transparent sphere 1
    std::unique_ptr<Primitive> primTransSphere1(new Primitive);
    std::unique_ptr<Sphere> up_transSphere1(new Sphere);
    primTransSphere1.get()->mup_shape = std::move(up_transSphere1);
    primTransSphere1.get()->m_name = "Transparent sphere 1";
    Transform transSphere1Trans = Transform(vec3(-3.f, 0.f, 2.f), vec3(0.f, 0.f, 0.f), vec3(4.f, 4.f, 4.f));
    primTransSphere1.get()->mup_shape.get()->m_transform = transSphere1Trans;
    primTransSphere1.get()->mp_material = &refractiveSpecular;
    scene.m_listUPointerPrim.push_back(std::move(primTransSphere1));

    //inside transparent sphere 1
    std::unique_ptr<Primitive> primInsideTransSphere1(new Primitive);
    std::unique_ptr<Sphere> up_insideTransSphere1(new Sphere);
    primInsideTransSphere1.get()->mup_shape = std::move(up_insideTransSphere1);
    primInsideTransSphere1.get()->m_name = "Inside transparent sphere 1";
    Transform insideTransSphere1Trans = Transform(vec3(-3.f, 0.f, 2.f), vec3(0.f, 0.f, 0.f), vec3(2.5f, 2.5f, 2.5f));
    primInsideTransSphere1.get()->mup_shape.get()->m_transform = insideTransSphere1Trans;
    primInsideTransSphere1.get()->mp_material = &airBubbleMat;
    scene.m_listUPointerPrim.push_back(std::move(primInsideTransSphere1));

    //transparent sphere green
    std::unique_ptr<Primitive> primInsideTransSphereGreen(new Primitive);
    std::unique_ptr<Sphere> up_insideTransSphereGreen(new Sphere);
    primInsideTransSphereGreen.get()->mup_shape = std::move(up_insideTransSphereGreen);
    primInsideTransSphereGreen.get()->m_name = "Inside transparent sphere green";
    Transform insideTransSphereGreenTrans = Transform(vec3(3.f, -2.f, -3.f), vec3(0.f, 0.f, 0.f), vec3(4.f, 4.f, 4.f));
    primInsideTransSphereGreen.get()->mup_shape.get()->m_transform = insideTransSphereGreenTrans;
    primInsideTransSphereGreen.get()->mp_material = &refractiveSpecular2;
    scene.m_listUPointerPrim.push_back(std::move(primInsideTransSphereGreen));

    //inside opaque 1
    std::unique_ptr<Primitive> primInsideOpaque1Sphere(new Primitive);
    std::unique_ptr<Sphere> up_insideOpaque1Sphere(new Sphere);
    primInsideOpaque1Sphere.get()->mup_shape = std::move(up_insideOpaque1Sphere);
    primInsideOpaque1Sphere.get()->m_name = "Inside opaque 1";
    Transform insideOpaque1Trans = Transform(vec3(3.f, -2.f, -3.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f));
    primInsideOpaque1Sphere.get()->mup_shape.get()->m_transform = insideOpaque1Trans;
    primInsideOpaque1Sphere.get()->mp_material = &whiteDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primInsideOpaque1Sphere));

    //reflective sphere blue 1
    std::unique_ptr<Primitive> primReflectiveSphereBlue1(new Primitive);
    std::unique_ptr<Sphere> up_reflectiveSphereBlue1(new Sphere);
    primReflectiveSphereBlue1.get()->mup_shape = std::move(up_reflectiveSphereBlue1);
    primReflectiveSphereBlue1.get()->m_name = "Reflective sphere blue 1";
    Transform reflectiveSphereBlue1Trans = Transform(vec3(-3.f, 3.5f, -3.f), vec3(0.f, 0.f, 0.f), vec3(3.f, 3.f, 3.f));
    primReflectiveSphereBlue1.get()->mup_shape.get()->m_transform = reflectiveSphereBlue1Trans;
    primReflectiveSphereBlue1.get()->mp_material = &mirroredSpecular2;
    scene.m_listUPointerPrim.push_back(std::move(primReflectiveSphereBlue1));

    //transparent sphere 3
    std::unique_ptr<Primitive> primTransSphere3(new Primitive);
    std::unique_ptr<Sphere> up_transSphere3(new Sphere);
    primTransSphere3.get()->mup_shape = std::move(up_transSphere3);
    primTransSphere3.get()->m_name = "Transparent sphere 3";
    Transform transSphere3Trans = Transform(vec3(3.f, 3.5f, -3.f), vec3(0.f, 0.f, 0.f), vec3(3.f, 3.f, 3.f));
    primTransSphere3.get()->mup_shape.get()->m_transform = transSphere3Trans;
    primTransSphere3.get()->mp_material = &refractiveSpecular;
    scene.m_listUPointerPrim.push_back(std::move(primTransSphere3));

    //reflective sphere blue 2
    std::unique_ptr<Primitive> primReflectiveSphereBlue2(new Primitive);
    std::unique_ptr<Sphere> up_reflectiveSphereBlue2(new Sphere);
    primReflectiveSphereBlue2.get()->mup_shape = std::move(up_reflectiveSphereBlue2);
    primReflectiveSphereBlue2.get()->m_name = "Transparent sphere 3";
    Transform reflectiveSphereBlue2Trans = Transform(vec3(3.f, 3.5f, -3.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f));
    primReflectiveSphereBlue2.get()->mup_shape.get()->m_transform = reflectiveSphereBlue2Trans;
    primReflectiveSphereBlue2.get()->mp_material = &mirroredSpecular2;
    scene.m_listUPointerPrim.push_back(std::move(primReflectiveSphereBlue2));

    //yellow sphere
    std::unique_ptr<Primitive> primYellowSphere(new Primitive);
    std::unique_ptr<Sphere> up_yellowSphere(new Sphere);
    primYellowSphere.get()->mup_shape = std::move(up_yellowSphere);
    primYellowSphere.get()->m_name = "Yellow sphere";
    Transform yellowSphereTrans = Transform(vec3(0.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(2.f, 2.f, 2.f));
    primYellowSphere.get()->mup_shape.get()->m_transform = yellowSphereTrans;
    primYellowSphere.get()->mp_material = &yellowDiffuse;
    scene.m_listUPointerPrim.push_back(std::move(primYellowSphere));

    //lightSources ---------------
    std::unique_ptr<Primitive> primSphere(new Primitive);
    std::unique_ptr<Sphere> up_sphereLight(new Sphere);
    primSphere.get()->mup_shape = std::move(up_sphereLight);
    primSphere.get()->m_name = "Light Source";
    Transform sphereTrans = Transform(vec3(0.f, 4.0f, 5.0f), vec3(0.f, 0.f, 0.f), vec3(0.5f, 0.5f, 0.5f));
    primSphere.get()->mup_shape.get()->m_transform = sphereTrans;
    std::unique_ptr<Light> up_sphereLightLight(new PointLight(sphereTrans, emissiveMaterial.m_color, &scene));
    primSphere.get()->mup_light = std::move(up_sphereLightLight);
    primSphere.get()->mp_material = &emissiveMaterial;
    scene.m_listUPointerPrim.push_back(std::move(primSphere));

    //iterating
    for (int x = 0; x < length; x++) {
        for (int y = 0; y < length; y++) {
            Ray ray = scene.p_cam->rayCast(Point2f(x, y));
            Intersection intersection = Intersection();
            Color3f color = Color3f(0.f, 0.f, 0.f);
            if (scene.getIntersection(ray, &intersection)) {
                const Primitive* p_prim = intersection.mp_objectHit;
                if (p_prim->mp_material != nullptr && p_prim->mup_shape.get() != nullptr) {
                    color = p_prim->mp_material->getScatteredColor(&intersection, &ray, maxRecurDepth);
                    //                    qDebug() << color.x << color.y << color.z;
                }
                p_image->setPixel(x, y, qRgb(color.x * 255.f, color.y * 255.f, color.z * 255.f));
            }
        }
    }
}

void MainWindow::testCamera(QImage* p_image, float length, Camera* p_cam)
{
    for (int x = 0; x < length; x ++) {
        for (int y = 0; y < length; y ++) {
            Ray ray = p_cam->rayCast(Point2f(x, y));
            vec3 rgb = (ray.m_direction + vec3(1.f, 1.f, 1.f)) * 0.5f;
            p_image->setPixel(x, y, qRgb(rgb.x * 255.f, rgb.y * 255.f, rgb.z * 255.f));
        }
    }
}

void MainWindow::testSphereIntersection(QImage *p_image, float length, Camera* p_cam)
{
    Scene scene = Scene();
    std::unique_ptr<Primitive> primitive(new Primitive);
    primitive.get()->m_name = "Sphere";
    std::unique_ptr<Sphere> ptr(new Sphere());
    primitive.get()->mup_shape = std::move(ptr);

    scene.m_listUPointerPrim.push_back(std::move(primitive));

    Color3f colorAO = Color3f(0.f, 0.f, 0.f);
    for (int x = 0; x < length; x++) {
        for (int y = 0; y < length; y++) {
            Ray ray = p_cam->rayCast(Point2f(x, y));
            Intersection intersection = Intersection();
            if (scene.getIntersection(ray, &intersection)) {
                vec3 rgb = (intersection.m_normal + vec3(1.f, 1.f, 1.f)) * 0.5f;
                p_image->setPixel(x, y, qRgb(rgb.x * 255.f, rgb.y * 255.f, rgb.z * 255.f));
            }
        }
    }
}

void MainWindow::testSquarePlaneIntersection(QImage *p_image, float length, Camera* p_cam)
{
    Scene scene = Scene();
    std::unique_ptr<Primitive> primitive(new Primitive);
    primitive.get()->m_name = "SquarePlane";
    std::unique_ptr<SquarePlane> up_sPlane(new SquarePlane());
    primitive.get()->mup_shape = std::move(up_sPlane);

    scene.m_listUPointerPrim.push_back(std::move(primitive));
    for (int x = 0; x < length; x++) {
        for (int y = 0; y < length; y++) {
            Ray ray = p_cam->rayCast(Point2f(x, y));
            Intersection intersection = Intersection();
            if (scene.getIntersection(ray, &intersection)) {
                vec3 rgb = (intersection.m_normal + vec3(1.f, 1.f, 1.f)) * 0.5f;
                p_image->setPixel(x, y, qRgb(rgb.x * 255.f, rgb.y * 255.f, rgb.z * 255.f));
            }
        }
    }
}

void MainWindow::testSceneTwoSphereOnePlane(QImage *p_image, float length, Camera *p_cam)
{
    Scene scene = Scene();
    scene.p_cam = p_cam;

    std::unique_ptr<Primitive> primitiveSphere(new Primitive);
    primitiveSphere.get()->m_name = "Sphere0";
    std::unique_ptr<Sphere>up_sphere(new Sphere());
    primitiveSphere.get()->mup_shape = std::move(up_sphere);
    Transform primitiveSphereTrans = Transform(vec3(0.f, 0.f, 1.f), vec3(0.f, 0.f, 45.f), vec3(2.f, 1.f, 1.f));
    primitiveSphere.get()->mup_shape.get()->m_transform = primitiveSphereTrans;

    std::unique_ptr<Primitive> primitiveSphere1(new Primitive);
    primitiveSphere1.get()->m_name = "Sphere1";
    std::unique_ptr<Sphere>up_sphere1(new Sphere());
    primitiveSphere1.get()->mup_shape = std::move(up_sphere1);
    Transform primitiveSphereTrans1 = Transform(vec3(0.25f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(0.5f, 0.5f, 5.f));
    primitiveSphere1.get()->mup_shape.get()->m_transform = primitiveSphereTrans1;

    std::unique_ptr<Primitive> primitiveSquarePlane(new Primitive);
    primitiveSquarePlane.get()->m_name = "SquarePlane";
    std::unique_ptr<SquarePlane> up_sPlane(new SquarePlane());
    primitiveSquarePlane.get()->mup_shape = std::move(up_sPlane);
    Transform primitiveSquarePlaneTrans = Transform(vec3(0.f, -0.5f, 0.f), vec3(90.f, 0.f, 0.f), vec3(5.f, 5.f, 1.f));
    primitiveSquarePlane.get()->mup_shape.get()->m_transform = primitiveSquarePlaneTrans;


    scene.m_listUPointerPrim.push_back(std::move(primitiveSphere1));
    scene.m_listUPointerPrim.push_back(std::move(primitiveSquarePlane));
    scene.m_listUPointerPrim.push_back(std::move(primitiveSphere));

    for (int x = 0; x < length; x++) {
        for (int y = 0; y < length; y++) {
            Ray ray = scene.p_cam->rayCast(Point2f(x, y));
            Intersection intersection = Intersection();
            if (scene.getIntersection(ray, &intersection)) {
                vec3 rgb = (intersection.m_normal + vec3(1.f, 1.f, 1.f)) * 0.5f;
                p_image->setPixel(x, y, qRgb(rgb.x * 255.f, rgb.y * 255.f, rgb.z * 255.f));
            }
        }
    }
}
