#ifndef COMPORTSETTINGSDIALOG_H
#define COMPORTSETTINGSDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPortInfo>
namespace Ui {
class ComPortSettingsDialog;
}

class ComPortSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ComPortSettingsDialog(QWidget *parent = 0);
    ~ComPortSettingsDialog();

    QString getSelectedPortName();


private slots:


    void on_comPortList_currentIndexChanged(const QString &arg1);

private:

    Ui::ComPortSettingsDialog *ui;
    QString selectedPortName;

};

#endif // COMPORTSETTINGSDIALOG_H
