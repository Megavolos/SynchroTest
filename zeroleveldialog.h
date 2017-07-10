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
    ~zeroLevelDialog();
    qreal getZeroLevelPiezo0();
    qreal getZeroLevelPiezo1();
    void setPiezo0Pointer(Sensor* piezo0);
    void setPiezo1Pointer(Sensor *piezo1);
private slots:
    void on_zeroLevelCalcButton_clicked();

private:
    Ui::zeroLevelDialog *ui;
    bool state;
    qreal zeroLevelPiezo0, zeroLevelPiezo1;
    Sensor* PIEZO0;
    Sensor* PIEZO1;
};

#endif // ZEROLEVELDIALOG_H
