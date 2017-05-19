#ifndef DIFFGRAPHWINDOW_H
#define DIFFGRAPHWINDOW_H

#include <QDialog>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
namespace Ui {
class diffGraphWindow;
}

class diffGraphWindow : public QDialog
{
    Q_OBJECT

public:
    explicit diffGraphWindow(QWidget *parent = 0);
    ~diffGraphWindow();
    void setMemsAngleSample (qreal angle);
    void setPiezoAngleSample (qreal angle);


private:
    Ui::diffGraphWindow *ui;
    QwtPlotMagnifier *zoom_x,*zoom_y;
    QwtPlotPanner *d_panner;
    QwtPlotCurve *curveMems,*curvePiezo,*curveDiff;
    QwtPlotGrid *grid;
    QVector<qreal> mems,piezo,diff,t;
    quint16 samplingFreq;
    quint8 callCounter;

};

#endif // DIFFGRAPHWINDOW_H
