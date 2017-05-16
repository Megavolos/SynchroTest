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
    void filter();
    void integrate();
    QVector<qreal>* getTime();
    QVector<qreal>* data();
    void setQwtPlotPointer (QwtPlot* _plot);
    void addSample(quint8 sample);
    qreal measure();
    void clear();
    void setLPFCoeff (qreal coeff);
    void clearNoSignalData();
    bool isNotFalseSignal();
    void setFalseSignalLevel(quint8 level);


private:
    bool start;
    quint32 samplingFrequencyAllChannels;
    quint16 samplingFrequencyOneChannel;
    QwtPlot* plot;
    qint16 startLevel;
    QVector<qreal> _data, x;
    qreal LPFcoeff;
    qreal peak, peakPrev;
    quint8 falseSignalLevel;
};

#endif // SIGNAL_H
