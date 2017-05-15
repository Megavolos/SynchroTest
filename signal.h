#ifndef SIGNAL_H
#define SIGNAL_H
#include <QVector>
#include <QPoint>
#include <qwt_plot.h>
#include <qmath.h>
#include <qwt_scale_div.h>
class Signal
{
public:
    Signal();
    void setStart (bool state);
    void setSamplingFrequencyAllChannels(quint32 freq);
    quint32 getSamplingFrequencyOneChannel();
    bool getStart();
    void setStartLevel (qint16 level);
    bool isSignalPresent ();
    bool isSignalEnd();
    void filter(QString text );
    void integrate();
    QVector<qreal>* getTime();
    QVector<qreal>* data();
    void setQwtPlotPointer (QwtPlot* _plot);
    void addSample(quint8 sample);
    qreal measure();


private:
    bool start;
    quint32 samplingFrequencyAllChannels;
    quint16 samplingFrequencyOneChannel;
    QwtPlot* plot;
    qint16 startLevel;
    QVector<qreal> _data, x;
};

#endif // SIGNAL_H
