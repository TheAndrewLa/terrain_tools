#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->seedLE->setValidator(new QIntValidator(0, INT_MAX, this));
    scene_ = new QGraphicsScene();
    ui->graphicsView->setScene(scene_);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene_;
    delete item_;
}

void MainWindow::on_generatePB_clicked()
{
    if (!mtx_generator_.try_lock()) {
        return;
    }

    ui->generatePB->setEnabled(false);
    ui->imagePB->setEnabled(false);
    int width = ui->widthSB->value();
    int height = ui->heightSB->value();
    int x_center = ui->centerXSB->value();
    int y_center = ui->centerYSB->value();
    int scale = ui->scaleSB->value();
    double angle = ui->angleHS->value() * 0.01;
    double amplitude = ui->amplitudeHS->value() * 0.01;
    int octaves = ui->octavesSB->value();

    map_ = HeightMap {
        height, width, x_center, y_center,
        scale, angle, amplitude, octaves
    };


    std::thread th([this]{
        terrain_ = tg_.generate(map_);
        ui->outputL->setText("generated");

        ui->generatePB->setEnabled(true);
        ui->imagePB->setEnabled(true);

        mtx_generator_.unlock();
    });
    th.detach();

}

void MainWindow::on_randomPB_clicked()
{
    HeightMap map = tg_.generate_random_map();
    ui->seedLE->setText(QString::fromStdString(std::to_string(map.seed)));
    ui->widthSB->setValue(map.width);
    ui->heightSB->setValue(map.height);
    ui->centerXSB->setValue(map.x_offset);
    ui->centerYSB->setValue(map.y_offset);
    ui->scaleSB->setValue(map.scale);

    ui->angleLE->setText(QString::fromStdString(std::format("{:.2f}", map.angle)));
    ui->angleHS->setValue(map.angle * 100);

    ui->amplitudeLE->setText(QString::fromStdString(std::format("{:.2f}", map.amplitude)));
    ui->amplitudeHS->setValue(map.amplitude * 100);

    ui->octavesSB->setValue(map.octaves);
}

void MainWindow::on_imagePB_clicked()
{
    if (!mtx_generator_.try_lock()) {
        return;
    }

    ui->imagePB->setEnabled(false);
    ui->generatePB->setEnabled(false);

    std::thread th([this]{
        export_png(terrain_, file_image_);
        ui->outputL->setText("image");

        QPixmap p(file_image_);
        ui->graphicsView->scene()->clear();
        ui->graphicsView->scene()->addPixmap(p);

        ui->generatePB->setEnabled(true);
        ui->imagePB->setEnabled(true);

        mtx_generator_.unlock();
    });

    th.detach();
}


void MainWindow::on_amplitudeHS_valueChanged(int value)
{
    ui->amplitudeLE->setText(QString::fromStdString(std::format("{:.2f}", value * 0.01)));
}


void MainWindow::on_angleHS_valueChanged(int value)
{
    ui->angleLE->setText(QString::fromStdString(std::format("{:.2f}", value * 0.01)));
}

