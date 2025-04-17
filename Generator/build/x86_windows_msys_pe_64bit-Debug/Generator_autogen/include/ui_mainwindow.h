/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *randomPB;
    QLabel *seedL;
    QLabel *centerL;
    QLabel *scaleL;
    QLineEdit *seedLE;
    QPushButton *generatePB;
    QGraphicsView *graphicsView;
    QLabel *widthL;
    QLabel *heightL;
    QLabel *octavesL;
    QLabel *amplitudeL;
    QSpinBox *widthSB;
    QSpinBox *heightSB;
    QLabel *xCenterL;
    QLabel *yCenterL;
    QSpinBox *octavesSB;
    QPushButton *editOtavesPB;
    QSpinBox *scaleSB;
    QSpinBox *centerXSB;
    QSpinBox *centerYSB;
    QLabel *outputL;
    QPushButton *imagePB;
    QSlider *amplitudeHS;
    QLineEdit *amplitudeLE;
    QSlider *angleHS;
    QLabel *angleL;
    QLineEdit *angleLE;
    QLabel *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1039, 788);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        randomPB = new QPushButton(centralwidget);
        randomPB->setObjectName("randomPB");
        randomPB->setGeometry(QRect(260, 220, 93, 29));
        seedL = new QLabel(centralwidget);
        seedL->setObjectName("seedL");
        seedL->setGeometry(QRect(20, 220, 71, 31));
        centerL = new QLabel(centralwidget);
        centerL->setObjectName("centerL");
        centerL->setGeometry(QRect(20, 340, 71, 31));
        scaleL = new QLabel(centralwidget);
        scaleL->setObjectName("scaleL");
        scaleL->setGeometry(QRect(20, 420, 71, 31));
        seedLE = new QLineEdit(centralwidget);
        seedLE->setObjectName("seedLE");
        seedLE->setGeometry(QRect(100, 220, 141, 26));
        seedLE->setMaxLength(32769);
        generatePB = new QPushButton(centralwidget);
        generatePB->setObjectName("generatePB");
        generatePB->setGeometry(QRect(20, 600, 111, 41));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setGeometry(QRect(410, 140, 541, 541));
        widthL = new QLabel(centralwidget);
        widthL->setObjectName("widthL");
        widthL->setGeometry(QRect(20, 260, 63, 20));
        heightL = new QLabel(centralwidget);
        heightL->setObjectName("heightL");
        heightL->setGeometry(QRect(20, 300, 63, 20));
        octavesL = new QLabel(centralwidget);
        octavesL->setObjectName("octavesL");
        octavesL->setGeometry(QRect(20, 500, 101, 31));
        amplitudeL = new QLabel(centralwidget);
        amplitudeL->setObjectName("amplitudeL");
        amplitudeL->setGeometry(QRect(20, 540, 81, 20));
        widthSB = new QSpinBox(centralwidget);
        widthSB->setObjectName("widthSB");
        widthSB->setGeometry(QRect(100, 260, 141, 26));
        widthSB->setMinimum(1);
        widthSB->setMaximum(4097);
        widthSB->setValue(512);
        heightSB = new QSpinBox(centralwidget);
        heightSB->setObjectName("heightSB");
        heightSB->setGeometry(QRect(100, 300, 141, 26));
        heightSB->setMinimum(1);
        heightSB->setMaximum(4097);
        heightSB->setValue(512);
        xCenterL = new QLabel(centralwidget);
        xCenterL->setObjectName("xCenterL");
        xCenterL->setGeometry(QRect(100, 340, 21, 20));
        yCenterL = new QLabel(centralwidget);
        yCenterL->setObjectName("yCenterL");
        yCenterL->setGeometry(QRect(100, 380, 21, 20));
        octavesSB = new QSpinBox(centralwidget);
        octavesSB->setObjectName("octavesSB");
        octavesSB->setGeometry(QRect(100, 500, 141, 26));
        octavesSB->setMinimum(1);
        octavesSB->setMaximum(5);
        editOtavesPB = new QPushButton(centralwidget);
        editOtavesPB->setObjectName("editOtavesPB");
        editOtavesPB->setGeometry(QRect(260, 500, 93, 29));
        scaleSB = new QSpinBox(centralwidget);
        scaleSB->setObjectName("scaleSB");
        scaleSB->setGeometry(QRect(100, 420, 141, 26));
        scaleSB->setMinimum(1);
        scaleSB->setMaximum(8);
        scaleSB->setSingleStep(0);
        centerXSB = new QSpinBox(centralwidget);
        centerXSB->setObjectName("centerXSB");
        centerXSB->setGeometry(QRect(130, 340, 111, 26));
        centerXSB->setMinimum(0);
        centerXSB->setMaximum(65536);
        centerYSB = new QSpinBox(centralwidget);
        centerYSB->setObjectName("centerYSB");
        centerYSB->setGeometry(QRect(130, 380, 111, 26));
        centerYSB->setMinimum(0);
        centerYSB->setMaximum(65536);
        outputL = new QLabel(centralwidget);
        outputL->setObjectName("outputL");
        outputL->setGeometry(QRect(620, 60, 321, 81));
        outputL->setScaledContents(true);
        imagePB = new QPushButton(centralwidget);
        imagePB->setObjectName("imagePB");
        imagePB->setGeometry(QRect(160, 600, 111, 41));
        amplitudeHS = new QSlider(centralwidget);
        amplitudeHS->setObjectName("amplitudeHS");
        amplitudeHS->setGeometry(QRect(188, 540, 160, 22));
        amplitudeHS->setMaximum(500);
        amplitudeHS->setSingleStep(1);
        amplitudeHS->setValue(100);
        amplitudeHS->setOrientation(Qt::Orientation::Horizontal);
        amplitudeLE = new QLineEdit(centralwidget);
        amplitudeLE->setObjectName("amplitudeLE");
        amplitudeLE->setGeometry(QRect(100, 540, 81, 26));
        amplitudeLE->setMaxLength(32767);
        amplitudeLE->setReadOnly(true);
        angleHS = new QSlider(centralwidget);
        angleHS->setObjectName("angleHS");
        angleHS->setGeometry(QRect(188, 460, 160, 22));
        angleHS->setMaximum(36000);
        angleHS->setSingleStep(1);
        angleHS->setValue(0);
        angleHS->setOrientation(Qt::Orientation::Horizontal);
        angleL = new QLabel(centralwidget);
        angleL->setObjectName("angleL");
        angleL->setGeometry(QRect(20, 460, 81, 20));
        angleLE = new QLineEdit(centralwidget);
        angleLE->setObjectName("angleLE");
        angleLE->setGeometry(QRect(100, 460, 81, 26));
        angleLE->setMaxLength(32767);
        angleLE->setReadOnly(true);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(520, 90, 63, 20));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1039, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        randomPB->setText(QCoreApplication::translate("MainWindow", "Random", nullptr));
        seedL->setText(QCoreApplication::translate("MainWindow", "Seed:", nullptr));
        centerL->setText(QCoreApplication::translate("MainWindow", "Offset:", nullptr));
        scaleL->setText(QCoreApplication::translate("MainWindow", "Scale:", nullptr));
        generatePB->setText(QCoreApplication::translate("MainWindow", "Generate", nullptr));
        widthL->setText(QCoreApplication::translate("MainWindow", "Width:", nullptr));
        heightL->setText(QCoreApplication::translate("MainWindow", "Height:", nullptr));
        octavesL->setText(QCoreApplication::translate("MainWindow", "Octaves:", nullptr));
        amplitudeL->setText(QCoreApplication::translate("MainWindow", "Amplitude:", nullptr));
        xCenterL->setText(QCoreApplication::translate("MainWindow", "x:", nullptr));
        yCenterL->setText(QCoreApplication::translate("MainWindow", "y:", nullptr));
        editOtavesPB->setText(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        outputL->setText(QCoreApplication::translate("MainWindow", "no map", nullptr));
        imagePB->setText(QCoreApplication::translate("MainWindow", "Export png", nullptr));
        amplitudeLE->setInputMask(QString());
        amplitudeLE->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        angleL->setText(QCoreApplication::translate("MainWindow", "Angle:", nullptr));
        angleLE->setInputMask(QString());
        angleLE->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Status:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
