#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>

#include "pxlfile.h"


namespace Ui {
class ImportDialog;
}

class ImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportDialog(PxlFile& pxl_,QWidget *parent = nullptr);
    ~ImportDialog();

private slots:
    void on_buttonBox_accepted();
    void on_fileButton_clicked();
    void on_spinBox_ys_valueChanged(int);

private:
    Ui::ImportDialog *ui;
    PxlFile& pxl;
    QString import_filename;

signals:
    void signal_import_done(QString str);

};

#endif // IMPORTDIALOG_H
