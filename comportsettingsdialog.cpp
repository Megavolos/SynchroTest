#include "comportsettingsdialog.h"
#include "ui_comportsettingsdialog.h"

ComPortSettingsDialog::ComPortSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComPortSettingsDialog)
{
    ui->setupUi(this);
    QSerialPortInfo serialPortInfo;
    QList<QSerialPortInfo> serialPorts;
    serialPorts=serialPortInfo.availablePorts();
    for (const QSerialPortInfo &info: serialPorts )
    {
        ui->comPortList->addItem(info.portName());
    }

}
ComPortSettingsDialog::~ComPortSettingsDialog()
{
    delete ui;

}



QString ComPortSettingsDialog::getSelectedPortName()
{
    return selectedPortName;
}

void ComPortSettingsDialog::on_comPortList_currentIndexChanged(const QString &arg1)
{
    selectedPortName=arg1;
}
