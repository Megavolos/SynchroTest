#ifndef DIAGRAMSETTINGSWINDOW_H
#define DIAGRAMSETTINGSWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>

namespace Ui {
class DiagramSettingsWindow;
}

class DiagramSettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DiagramSettingsWindow(QWidget *parent = 0);
    ~DiagramSettingsWindow();
    quint16 getPiezo0Min ();
    quint16 getPiezo1Min ();


    QSpinBox *spin0, *spin1;
    qreal levelmin,levelmax;
    QDoubleSpinBox *tLimitMems, *tLimitPiezo, *tStepMems, *tStepPiezo,*measureMinP,*measureMaxP;

private slots:
    void on_piezo0MinSpinBox_valueChanged(int arg1);

    void on_piezo1MinSpinBox_valueChanged(int arg1);



    void on_measureMin_valueChanged(double arg1);

    void on_measureMax_valueChanged(double arg1);

private:
    Ui::DiagramSettingsWindow *ui;
    quint16 min0,min1;
};

#endif // DIAGRAMSETTINGSWINDOW_H
