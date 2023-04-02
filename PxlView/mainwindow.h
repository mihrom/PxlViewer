#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include "importdialog.h"
#include "pxlfile.h"
#include "pxlplayback.h"
#include "pxlwriter.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;
    ImportDialog importDialog;

    PxlFile pxl;            // Ответственен за чтение PXL  файла
    PxlPlayback playback;   // Ответственен за воспроизведение
    PxlWriter writer;       // Ответственен за вывод картинки

    QTimer* timer;
    QString filename {""};

    void startPlayback(int msec);
    void stopPlayback();
    void writePxl();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void slot_import_done(QString filename);

private slots:
    void on_timer();

    void on_actionOpen_triggered();
    void on_actionimport_triggered();
    void on_actionSave_triggered();
    void on_actionAbout_triggered();

    void on_checkBox_black_stateChanged(int is_black);
    void on_horizontalSlider_size_valueChanged(int size);
    void on_horizontalSlider_timeline_actionTriggered(int);

    void on_playButton_clicked();
    void on_prevButton_clicked();
    void on_nextButton_clicked();

};
#endif // MAINWINDOW_H
