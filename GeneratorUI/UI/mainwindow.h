#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <thread>
#include <mutex>

#include <QMainWindow>
#include "../Generator/terraingenerator.h"
#include "../ImageGenerator/imagegenerator.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    TerrainGenerator* tg_;
    Terrain terrain_;
    HeightMap map_;
    std::mutex mtx_image_;
    std::atomic_bool is_exporting = false;
    std::atomic_bool is_generating = false;
    std::thread thread_image;
    std::thread thread_generator;
public:
    MainWindow(QWidget *parent = nullptr);
    MainWindow(TerrainGenerator* tg, QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_generatePB_clicked();

    void on_randomPB_clicked();

    void on_imagePB_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
