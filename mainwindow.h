#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QDebug>

#include <QLabel>
#include <QActionGroup>

#include <QVector3D>
#include <QImage>
#include <QPixmap>

#include <QFileDialog>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QPainter>
#include <QMessageBox>

#include "sliceplane.h"
#include "volume3d.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void build();               // Build a volume
    void reset();               // reset the xyz and rpy of the plane
    void updateLabel();         // update the label (containg the image of the slice)

    void saveImage();           // For the MenuBar
    void saveImage_as();
    void printoutImage();
    void loadImage();


private:
    Ui::MainWindow *ui;

    Volume3D vol;               // Handled in volume3d.h
    SlicePlane pln;             // Handled in sliceplane.h
    QPixmap pixmap;             // Handled in reslicer.h

    QString filename;           // to save files
    int mode;                   // To select different geometries
    float roll, pitch, yaw;
    int W,H,D;                  // A reference to the volume of vol object

    void updateSlice();         // Updates the reslicer when the plane features change

};
#endif // MAINWINDOW_H
