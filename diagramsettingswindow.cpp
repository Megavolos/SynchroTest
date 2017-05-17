#include "diagramsettingswindow.h"
#include "ui_diagramsettingswindow.h"

DiagramSettingsWindow::DiagramSettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiagramSettingsWindow)
{
    ui->setupUi(this);
    spin0=ui->piezo0MinSpinBox;
    spin1=ui->piezo1MinSpinBox;
    tLimitMems=ui->MEMS_tLimit;
    tLimitPiezo=ui->PIEZO_tLimit;
    tStepMems=ui->MEMS_tStep;
    tStepPiezo=ui->PIEZO_tStep;

}

DiagramSettingsWindow::~DiagramSettingsWindow()
{
    delete ui;
}

quint16 DiagramSettingsWindow::getPiezo0Min()
{
    return min0;
}

quint16 DiagramSettingsWindow::getPiezo1Min()
{
    return min1;
}

void DiagramSettingsWindow::on_piezo0MinSpinBox_valueChanged(int arg1)
{
    min0=arg1;

}

void DiagramSettingsWindow::on_piezo1MinSpinBox_valueChanged(int arg1)
{
    min1=arg1;


}
