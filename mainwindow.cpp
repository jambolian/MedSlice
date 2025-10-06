#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ui_HelpDialog.h"
#include "reslicer.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), roll(0.0f), pitch(0.0f)
    , yaw(0.0f), mode(0)
{
        ui->setupUi(this);

    // Connect the Ui Btn to the Function
        connect(ui->btnReset, &QPushButton::clicked, this, &MainWindow::reset);

    // Connect the Sliders to the Slots
        connect(ui->sldRoll, &QSlider::valueChanged, this, &MainWindow::updateLabel);
        connect(ui->sldPitch, &QSlider::valueChanged, this, &MainWindow::updateLabel);
        connect(ui->sldYaw, &QSlider::valueChanged, this, &MainWindow::updateLabel);
        connect(ui->sldX, &QSlider::valueChanged, this, &MainWindow::updateLabel);
        connect(ui->sldY, &QSlider::valueChanged, this, &MainWindow::updateLabel);
        connect(ui->sldZ, &QSlider::valueChanged, this, &MainWindow::updateLabel);

    // Gets the dimensions of the Label which displays the slice
        const QSize s = ui->lblShowImage->contentsRect().size();
        this->W = std::max(1, s.width());
        this->H = std::max(1, s.height());
        this->D = std::min(W, H);

    // Set Min and Max for the Sliders

        // Rotation Sliders
        ui->sldRoll->setMinimum(-180);
        ui->sldRoll->setMaximum(180);
        ui->sldPitch->setMinimum(-180);
        ui->sldPitch->setMaximum(180);
        ui->sldYaw->setMinimum(-180);
        ui->sldYaw->setMaximum(180);
        // Point Sliders
        ui->sldX->setMinimum(0);
        ui->sldY->setMinimum(0);
        ui->sldZ->setMinimum(0);
        ui->sldX->setMaximum(W-1);
        ui->sldY->setMaximum(H-1);
        ui->sldZ->setMaximum(D-1);

    // Configure the MenuBar Items

        // In the File Menu
        connect(ui->actionLoad,&QAction::triggered, this, &MainWindow::loadImage);
        connect(ui->actionSave,&QAction::triggered, this, &MainWindow::saveImage);
        connect(ui->actionSave_as,&QAction::triggered, this, &MainWindow::saveImage_as);
        connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::printoutImage);
        connect(ui->actionInstruction, &QAction::triggered, this, [this]
        {
            QDialog dlg(this);
            Ui::HelpDialog helpUi;
            helpUi.setupUi(&dlg);
            connect(helpUi.btnOK, &QPushButton::clicked, &dlg, &QDialog::accept);
            dlg.exec();
        });
        ui->menubar->addAction("Exit",this,&qApp->quit);
        connect(ui->actionClose, &QAction::triggered, this, [](){qApp->quit();});

        // Make an exclusive group and add the geometrical items
        QActionGroup* sliceGroup = new QActionGroup(this);
        sliceGroup->setExclusive(true);
        sliceGroup->addAction(ui->actionR);
        sliceGroup->addAction(ui->actionX);
        sliceGroup->addAction(ui->actionY);
        sliceGroup->addAction(ui->actionZ);
        ui->actionR->setChecked(true);

        // React to Gradiant Selection and rebuild the Volume Object
        connect(sliceGroup, &QActionGroup::triggered, this,
                                [this](QAction* sender)
                                {
                                if (sender == ui->actionR)
                                    mode = 0;
                                else if (sender == ui->actionX)
                                    mode = 1;
                                else if (sender == ui->actionY)
                                    mode = 2;
                                else if (sender == ui->actionZ)
                                    mode = 3;

            ui->statusbar->showMessage("Mode: " + QString::number(mode), 2000);
            build();
        });

    // Clicks the Reset Btn to
        build();
}


MainWindow::~MainWindow()
{
    delete ui;
}

// **************************************************************************************************

void MainWindow::build()
{
    // Dimensions of the Sphere inside the Cube
        float r1 = 0.3;
        float r2 = 0.8;

    // Initialize the Volume with Dimensions W, H, D from the Ui Label
        vol = Volume3D(W, H, D);
        vol.makeSpherePhantom(mode, r1, r2);

    // Initialize the Plane
        pln.setPoint( {(W-1)*0.5f, (H-1)*0.5f, (D-1)*0.5f} );
        pln.setRotationAngles(0,0,0);
        pln.setGrid(W, H, 1.0f, 1.0f);

        reset();

    // Create Image and Update the Ui Label
        updateSlice();
}


void MainWindow::reset()
{
    // Set Values for the Sliders
        ui->sldRoll->setValue(0);
        ui->sldPitch->setValue(0);
        ui->sldYaw->setValue(0);
        ui->sldX->setValue((W-1)*0.5f);
        ui->sldY->setValue((H-1)*0.5f);
        ui->sldZ->setValue((D-1)*0.5f);

    // Set Label Values
        ui->lblRoll->setNum(0);
        ui->lblPitch->setNum(0);
        ui->lblYaw->setNum(0);
        ui->lblX->setNum(0);
        ui->lblY->setNum(0);
        ui->lblZ->setNum(0);
}

// ============================================= MenuBar =============================================

void MainWindow::loadImage()
{
    QString filenameLoad = QFileDialog::getOpenFileName(this,
                                                        "Load the Image,",
                                                        ".",
                                                        "All Files (*.*);;JGEP (*.jpg);;PNG (*.png)");

    if (filenameLoad.isEmpty()) return;

    if (!pixmap.load(filenameLoad))
        return;

    QPixmap file = pixmap.scaled(W,H,Qt::AspectRatioMode::KeepAspectRatio
                                 , Qt::TransformationMode::SmoothTransformation);
    ui->lblShowImage->setPixmap(file);
    ui->statusbar->showMessage("Image loaded from " + filenameLoad + " successfully");
}

void MainWindow::saveImage()
{
    if (filename.isEmpty())
        saveImage_as();
    else
    {
        if(pixmap.save(filename,"PNG"))
            ui->statusbar->showMessage("Image saved to " + filename + " successfully");
        else
            ui->statusbar->showMessage("Image was NOT saved");
    }
}

void MainWindow::saveImage_as()
{
        filename = QFileDialog::getSaveFileName(this,
                                                "Save the Image,",
                                                ".",
                                                "JGEP (*.jpg);; PNG (*.png)");
        if (filename.isEmpty())
            ui->statusbar->showMessage("Image was NOT saved");
        else
            saveImage();
}

void MainWindow::printoutImage()
{
    // Setting up the printer
        QPrinter printer;
        QPrintDialog printDialog(&printer, this);
        if (printDialog.exec() != QDialog::Accepted)
        {
            QMessageBox::critical(this, "Print", "Dialog cancled!");
            return;
        }

        qDebug() << "Accepted: " << printer.printerName();

        QPixmap pm = pixmap;

        if (pm.isNull())
        {
            QMessageBox::warning(this, "Print", "No image to print.");
            return;
        }

    // Setting up the Painter
        QPainter painter;
        if (!painter.begin(&printer)) {
            QMessageBox::warning(this, "Print", "Could not start printer painter.");
            return;
        }

        //Get the dimensions of the page from the printerlayout
        QRect page = printer.pageLayout().paintRectPixels(printer.resolution());

        pm.size().scale(ui->lblShowImage->size(), Qt::KeepAspectRatio);

        painter.drawPixmap(page.x(),page.y(), pm);
        painter.end();
}

void MainWindow::updateLabel()
{
    // Update the rpy and xyz from the Sliders
        int roll = ui->sldRoll->value();
        int pitch = ui->sldPitch->value();
        int yaw = ui->sldYaw->value();
        int x = ui->sldX->value();
        int y = ui->sldY->value();
        int z = ui->sldZ->value();

    // Set the values from Sliders to the Lables
        ui->lblRoll->setText(QString::number(roll));
        ui->lblPitch->setText(QString::number(pitch));
        ui->lblYaw->setText(QString::number(yaw));
        ui->lblX->setNum(x - int(((vol.xVoxelCount()-1)*0.5)));
        ui->lblY->setNum(y - int(((vol.yVoxelCount()-1)*0.5)));
        ui->lblZ->setNum(z - int(((vol.yVoxelCount()-1)*0.5)));

    // Update the plane
        pln.setRotationAngles(float(roll), float(pitch), float(yaw));
        pln.setPoint({float(x),float(y),float(z)});

    // Update the displayed image
        updateSlice();
}

void MainWindow::updateSlice()
{
    // Takes the Volume and the Plane, and draws an Image using reslicer.h
        QImage img = Reslicer::resliceToImage(vol, pln);
        this->pixmap = QPixmap::fromImage(img);
        ui->lblShowImage->setPixmap(pixmap);
}

