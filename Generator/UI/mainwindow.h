#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <mutex>

#include <QMainWindow>
#include <QGraphicsItem>
#include "../Generator/terraingenerator.h"
#include "../Generator/Curve.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>


using namespace terraingenerator;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    TerrainGenerator<uchar, double> tg_{};
    Terrain<uchar> terrain_;
    Terrain<uchar> moisture_;
    HeightMap<double> map_{};
    HeightMap<double> moisture_map_{};
    Curve<double> curve_{};
    QChartView* chart_view_ = nullptr;

    std::mutex mtx_image_;
    std::mutex mtx_generator_;

    const char* file_image_ = "out.png";
    QGraphicsScene *scene_ = nullptr;
    QGraphicsPixmapItem* item_ = nullptr;
    void get_current_map();
    void generate_random();
    QChartView* createCurveChartView(Curve<double>& curve);
    void updateChart();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_generatePB_clicked();

    void on_randomPB_clicked();

    void on_imagePB_clicked();

    void on_seedLE_textChanged(const QString &arg1);

    void on_addPointPB_clicked();

    void on_clearCurvePB_clicked();

    void on_erosionCB_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
