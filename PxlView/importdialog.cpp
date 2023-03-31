#include <QFileDialog>

#include "importdialog.h"
#include "ui_importdialog.h"


ImportDialog::ImportDialog(PxlFile &pxl_, QWidget *parent):
    QDialog(parent),
    ui(new Ui::ImportDialog),
    pxl(pxl_)
{
    ui->setupUi(this);

    QStringList lstrip = {"LINE","ZIGZAG"};
    QStringList lcolor = {"RGB","RGBA","GRBA"};

    ui->comboBox_strip->addItems(lstrip);
    ui->comboBox_color->addItems(lcolor);

    ui->comboBox_strip->setCurrentIndex(1);
    ui->comboBox_color->setCurrentIndex(1);

    ui->buttonBox->buttons().at(0)->setDisabled(true);
}


ImportDialog::~ImportDialog()
{
    delete ui;
}


void ImportDialog::on_buttonBox_accepted()
{
    if(import_filename == "")
        return;

     QImage import(import_filename);

     format_strip_t fstrip = STRIP_LINE;
     if (ui->comboBox_strip->currentText() == "LINE") {
         fstrip = STRIP_LINE;
     } else if (ui->comboBox_strip->currentText() == "ZIGZAG") {
         fstrip = STRIP_ZIGZAG;
     }

     format_color_t fcolor = COLOR_RGBA;
     auto fc = ui->comboBox_color->currentText();
     if (fc == "RGB") {
         fcolor = COLOR_RGB;
     } else if (fc == "RGBA") {
         fcolor = COLOR_RGBA;
     } else if (fc == "GRBA") {
         fcolor = COLOR_GRBA;
     }

     pxl.setStrip(fstrip, fcolor);
     pxl.setFramesData(ui->spinBox_nframes->value(), ui->spinBox_repeats->value());
     pxl.setSize(ui->spinBox_xs->value(), ui->spinBox_ys->value());

     pxl.importFramesFromImage(import, ui->spinBox_timeout->value());

    emit signal_import_done(import_filename);
}


void ImportDialog::on_fileButton_clicked()
{

    QString selfilter = tr("All files (*.*)");
    import_filename = QFileDialog::getOpenFileName(
            this,
            nullptr,
            nullptr,
            tr("All files (*.*);;JPEG (*.jpg *.jpeg);;PNG (*.png) "),
            &selfilter
    );

    ui->label_file->setText(import_filename);

    QImage import(import_filename);

    auto size = import.size();
    bool bad_image = false;


    if(size.width() <= ui->spinBox_xs->value())
        bad_image = true;

    if(size.height() <= ui->spinBox_ys->value())
        bad_image = true;

    if(!bad_image){
        ui->spinBox_xs->setMaximum(size.width());
        ui->spinBox_ys->setMaximum(size.height());
        ui->spinBox_nframes->setMaximum(size.height()/ui->spinBox_ys->value());
    }

    ui->buttonBox->buttons().at(0)->setDisabled(bad_image);
}


void ImportDialog::on_spinBox_ys_valueChanged(int)
{
    QImage import(import_filename);
    auto size = import.size();

    if(size.height() == 0)
        return;

    if(size.height() <= ui->spinBox_ys->value())
        ui->spinBox_ys->setMaximum(size.height());

    if(size.height() >= ui->spinBox_ys->value()){
        int new_repeats;

        new_repeats = size.height() / ui->spinBox_ys->value();
        ui->spinBox_nframes->setMaximum(new_repeats);
    }
}
