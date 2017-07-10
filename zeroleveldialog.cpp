#include "zeroleveldialog.h"
#include "ui_zeroleveldialog.h"

zeroLevelDialog::zeroLevelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::zeroLevelDialog)
{
    ui->setupUi(this);
    state=0;
}

zeroLevelDialog::~zeroLevelDialog()
{
    delete ui;
}

void zeroLevelDialog::setPiezo0Pointer(Sensor *piezo0)
{
    PIEZO0=piezo0;
}
void zeroLevelDialog::setPiezo1Pointer(Sensor *piezo1)
{
    PIEZO1=piezo1;
}
qreal zeroLevelDialog::getZeroLevelPiezo0()
{
    if (!PIEZO0->data()->isEmpty() && state) zeroLevelPiezo0=PIEZO0->data()->last();
    return zeroLevelPiezo0;
}
qreal zeroLevelDialog::getZeroLevelPiezo1()
{
    return zeroLevelPiezo1;
}

void zeroLevelDialog::on_zeroLevelCalcButton_clicked()
{
    if (!state)          //если надпись "Начать"
    {
        state=true;
        ui->zeroLevelCalcButton->setText("Остановить");
    }
    else
    {
        state=false;
        ui->zeroLevelCalcButton->setText("Начать");
    }

}
