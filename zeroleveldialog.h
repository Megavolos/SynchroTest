#ifndef ZEROLEVELDIALOG_H
#define ZEROLEVELDIALOG_H

#include <QDialog>
#include <sensor.h>
namespace Ui {
class zeroLevelDialog;
}

class zeroLevelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit zeroLevelDialog(QWidget *parent = 0);
    void setPiezoPointer(Sensor *piezo);
    ~zeroLevelDialog();

private slots:
    void on_zeroLevelCalcButton_clicked();

private:
    Ui::zeroLevelDialog *ui;
    bool state;
    qreal zeroLevelPiezo;
    Sensor* PIEZO;

};

#endif // ZEROLEVELDIALOG_H
