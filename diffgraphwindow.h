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

    void setAngleSample (quint8 channel, qreal angle);
    void setDiffSample(qreal diff);
    void filter(QVector<qreal>* diffsP);

private:
    Ui::diffGraphWindow *ui;
    QwtPlotMagnifier *zoom_x,*zoom_y;
    QwtPlotPanner *d_panner;
    QwtPlotCurve *curveMems,*curvePiezo,*curveDiff;
    QwtPlotGrid *grid;
    QVector<qreal> mems,piezo,diff,t;
    QVector<QVector<qreal>> diffs;
    quint16 samplingFreq;
    quint16 callCounter;
    qreal angle3,angle2,angle1,angle0,prevMaxX;
    bool endXreached;
    qreal out;

};

#endif // DIFFGRAPHWINDOW_H
