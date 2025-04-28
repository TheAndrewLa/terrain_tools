#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <thread>
#include "../FileConverter/fileconverter.h"
#include <iostream>
#include "../Types.h"
using namespace types;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->seedLE->setValidator(new QIntValidator(0, INT_MAX, this));
    scene_ = new QGraphicsScene();
    ui->graphicsView->setScene(scene_);
    get_current_map();
    ui->curveOutputL->setText(QString::fromStdString(static_cast<std::string>(curve_)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene_;
    delete item_;
}

void MainWindow::get_current_map() {
    uint32 width = ui->widthSB->value();
    uint32 height = ui->heightSB->value();
    uint32 x_center = ui->centerXSB->value();
    uint32 y_center = ui->centerYSB->value();
    double scale = ui->scaleSB->value();
    double angle = ui->angleSB->value();
    double amplitude = ui->amplitudeSB->value();
    uint32 octaves = ui->octavesSB->value();

    map_ = HeightMap {
        height, width, x_center, y_center,
        scale, angle, amplitude, octaves
    };
}

void MainWindow::on_generatePB_clicked()
{
    if (!mtx_generator_.try_lock()) {
        return;
    }

    ui->generatePB->setEnabled(false);
    ui->imagePB->setEnabled(false);

    get_current_map();

    std::thread th([this]{
        terrain_ = tg_.generate(map_, curve_);
        ui->outputL->setText("generated");

        ui->generatePB->setEnabled(true);
        ui->imagePB->setEnabled(true);

        mtx_generator_.unlock();
    });
    th.detach();
}

void MainWindow::generate_random() {
    tg_.generate_random_map(map_);
    ui->widthSB->setValue(map_.width);
    ui->heightSB->setValue(map_.height);
    ui->centerXSB->setValue(map_.x_offset);
    ui->centerYSB->setValue(map_.y_offset);
    ui->scaleSB->setValue(map_.scale);
    ui->angleSB->setValue(map_.angle);
    ui->amplitudeSB->setValue(map_.amplitude);
    ui->octavesSB->setValue(map_.octaves);
}

void MainWindow::on_randomPB_clicked()
{
    generate_random();
    ui->seedLE->setText(QString::fromStdString(std::to_string(map_.seed)));

}

void MainWindow::on_imagePB_clicked()
{
    if (terrain_.width() == 0 || terrain_.height() == 0) {
        return;
    }
    ui->imagePB->setEnabled(false);
    ui->generatePB->setEnabled(false);
    FileConverter::to_file(terrain_, "out");
    terrain_ = FileConverter::from_file("out");
    export_png(terrain_, file_image_);

    ui->outputL->setText("image");

    QPixmap p(file_image_);
    ui->graphicsView->scene()->clear();
    ui->graphicsView->scene()->addPixmap(p);

    ui->generatePB->setEnabled(true);
    ui->imagePB->setEnabled(true);
}


void MainWindow::on_seedLE_textChanged(const QString &arg1)
{
    map_.seed = arg1.toInt();
    generate_random();

}

void MainWindow::on_addPointPB_clicked()
{
    double x = ui->curveXSB->value();
    double y = ui->curveYSB->value();
    curve_.add_point(x, y);
    ui->curveOutputL->setText(QString::fromStdString(static_cast<std::string>(curve_)));
}


void MainWindow::on_clearCurvePB_clicked()
{
    curve_.clear();
}

void MainWindow::on_erosionCB_stateChanged(int arg1)
{
    tg_.appled_erosion = static_cast<bool>(arg1);
}

