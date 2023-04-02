#include <QFileDialog>
#include <QTimer>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;


void MainWindow::startPlayback(int msec)
{
    timer->start(msec);
    playback.run(msec);

    ui->playButton->setText("stop");
}


void MainWindow::stopPlayback()
{
    timer->stop();
    playback.stop();

    ui->playButton->setText("play");
}


void MainWindow::writePxl()
{
    if(pxl.isEmpty())
        return;

    QImage img = writer.draw(pxl.frameToImage(playback.getCurrentFrameNumber()));

    ui->label_canvas->resize(img.size());
    ui->label_canvas->setPixmap(QPixmap::fromImage(img));    
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , importDialog(pxl)
    , playback()
    , timer(new QTimer(this))
{
    ui->setupUi(this);

    connect(&importDialog, &ImportDialog::signal_import_done, this, &MainWindow::slot_import_done);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timer()));

    writer.setPixelSize(ui->horizontalSlider_size->value());
    writer.setBlackBackground(ui->checkBox_black->checkState());
}


MainWindow::~MainWindow()
{
    delete timer;
    delete ui;
}


////////////////////////////////////////////////////////////////////////////


void MainWindow::slot_import_done(QString filename)
{
    playback.setFramesTimeouts(pxl.getFramesTimeouts());
    playback.setRepeats(pxl.getRepeats());

    ui->label_filename->setText(filename);

    ui->horizontalSlider_timeline->setValue(playback.getCurrentFrameNumber());
    ui->horizontalSlider_timeline->setMaximum(pxl.getFramesCount()-1);

    ui->playButton->setDisabled(pxl.getRepeats()==0);
    ui->playButton->setToolTip(("repeats = " + to_string(pxl.getRepeats())).c_str());

    writer.resize(pxl.getSize());
    writePxl();
}


////////////////////////////////////////////////////////////////////////////


void MainWindow::on_timer()
{
    if(playback.isRunning())
    {
        if(playback.timerStep())
            writePxl();

        ui->horizontalSlider_timeline->setValue(playback.getCurrentFrameNumber());
    }

    if(!playback.isRunning())
        stopPlayback();

}


void MainWindow::on_actionOpen_triggered()
{
    stopPlayback();

    QString selfilter = tr("PXL (*.pxl)");
    filename = QFileDialog::getOpenFileName(
            this,
            nullptr,
            nullptr,
            tr("PXL (*.pxl);;All files (*.* *) "),
            &selfilter
    );

    ui->label_filename->setText(filename);

    if(filename != ""){
        pxl.loadPxlFile(filename.toStdString());

        if(pxl.isEmpty())
            return;

        playback.setFramesTimeouts(pxl.getFramesTimeouts());
        playback.setRepeats(pxl.getRepeats());

        writer.resize(pxl.getSize());
        writePxl();

        ui->horizontalSlider_timeline->setValue(playback.getCurrentFrameNumber());
        ui->horizontalSlider_timeline->setMaximum(pxl.getFramesCount()-1);

        ui->playButton->setDisabled(pxl.getRepeats()==0);
        ui->playButton->setToolTip(("repeats = " + to_string(pxl.getRepeats())).c_str());
    }
}


void MainWindow::on_actionimport_triggered()
{
    stopPlayback();

    importDialog.setModal(true);
    importDialog.exec();
}


void MainWindow::on_actionSave_triggered()
{
    QString saveFilename = QFileDialog::getSaveFileName(
            this,
            nullptr,
            nullptr,
            tr("PXL (*.pxl) "));

    QFileInfo fi(saveFilename);
    if(fi.completeSuffix() == "")
        saveFilename += ".pxl";

    if(pxl.isEmpty()){
        QMessageBox::information(this,"Information","Image is not initialized.");
        return;
    }

    pxl.savePxlFile(saveFilename.toStdString());
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this,
        "About",
        "PXL Viewer\n"
        "Version 1.0.0\n"
        "\n"
        "Created by\n"
        "Mihrom / development\n"
        "Dragon_Knight / file format.\n");
}


////////////////////////////////////////////////////////////////////////////


void MainWindow::on_checkBox_black_stateChanged(int is_black)
{
    writer.setBlackBackground(is_black);
    writePxl();
}


void MainWindow::on_horizontalSlider_size_valueChanged(int size)
{
    writer.setPixelSize(size);
    writePxl();
}


void MainWindow::on_horizontalSlider_timeline_actionTriggered(int)
{   
    playback.jumpToFrame(ui->horizontalSlider_timeline->value());
    writePxl();
}


////////////////////////////////////////////////////////////////////////////


void MainWindow::on_playButton_clicked()
{
    if(pxl.getRepeats()==0) {
        QMessageBox::information(this,"",("Number of repeats = " + to_string(pxl.getRepeats())).c_str());
        return;
    }

    if(playback.isRunning())
        stopPlayback();
    else
        startPlayback(10);

    writePxl();
}


void MainWindow::on_prevButton_clicked()
{
    playback.prev();

    stopPlayback();
    writePxl();

    ui->horizontalSlider_timeline->setValue(playback.getCurrentFrameNumber());
}


void MainWindow::on_nextButton_clicked()
{
    playback.next();

    stopPlayback();
    writePxl();

    ui->horizontalSlider_timeline->setValue(playback.getCurrentFrameNumber());
}
