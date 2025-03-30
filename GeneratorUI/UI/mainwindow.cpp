#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::MainWindow(TerrainGenerator* tg, QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow), tg_(tg)
{
    ui->setupUi(this);
    ui->seedLE->setValidator(new QIntValidator(0, INT_MAX, this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_generatePB_clicked()
{
    if (is_generating) {
        return;
    }
    int width = ui->widthSB->value();
    int height = ui->heightSB->value();
    int x_center = ui->centerXSB->value();
    int y_center = ui->centerYSB->value();
    int scale = ui->scaleSB->value();
    int amplitude = ui->amplitudeSB->value();
    int octaves = ui->octavesSB->value();
    map_ = HeightMap {
        height, width, x_center, y_center,
        scale, amplitude, octaves
    };
    std::thread th([this]{
        is_generating = true;
        terrain_ = tg_->generate(map_);
        ui->outputL->setText("generated");
        is_generating = false;
    });
    th.detach();

}

void MainWindow::on_randomPB_clicked()
{
    HeightMap map = tg_->generate_random_map();
    ui->seedLE->setText(QString::fromStdString(std::to_string(map.seed)));
    ui->widthSB->setValue(map.width);
    ui->heightSB->setValue(map.height);
    ui->centerXSB->setValue(map.x_offset);
    ui->centerYSB->setValue(map.y_offset);
    ui->scaleSB->setValue(map.scale);
    ui->amplitudeSB->setValue(map.amplitude);
    ui->octavesSB->setValue(map.octaves);
}

void MainWindow::on_imagePB_clicked()
{
    if (is_exporting) {
        return;
    }
    std::thread th([this]{
        is_exporting = true;
        ImageGenerator::export_png(terrain_, "out.png");
        ui->outputL->setText("image");
        is_exporting = false;
    });
    th.detach();
}

