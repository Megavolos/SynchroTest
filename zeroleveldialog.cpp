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

void zeroLevelDialog::setPiezoPointer(Sensor *piezo)
{
    PIEZO=piezo;
}


void zeroLevelDialog::on_zeroLevelCalcButton_clicked()
{
    if (!state)          //если надпись "Начать"
    {
        state=true;
        ui->zeroLevelCalcButton->setText("Остановить");
        PIEZO->setZeroLevelMeasuring(state);
    }
    else
    {
        state=false;
        ui->zeroLevelCalcButton->setText("Начать");
        PIEZO->setZeroLevelMeasuring(state);
    }

}
