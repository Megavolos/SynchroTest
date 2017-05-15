#include "signal.h"

Signal::Signal()
{
    startLevel=0;
}

void Signal::setStart(bool state)
{
    start=state;
}
bool Signal::getStart()
{
    return start;
}
void Signal::setStartLevel(qint16 level)
{
    startLevel=level;
}

bool Signal::isSignalPresent()
{
    if (!getStart())
    {
        if (_data.last() > startLevel)
        {
            setStart(true);
            _data.erase(0,_data.end()-2);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return true;
    }
}

QVector<qreal>* Signal::data()
{
    return &_data;
}

void Signal::addSample(quint8 sample)
{
    _data.append(sample);
}

void Signal::integrate()
{
    QVector<qreal> out(_data.size());
    for (int i=1; i<_data.size(); i++)
    {
        if (_data.at(i)>startLevel)
        {
            out[i]=_data.at(i) + out.at(i-1);
        }
        else
        {
            out[i]=0;
        }
    }

    _data=out;
}
/*    Поиск конца сигнала. Применять после нахождения сигнала (isSignalPresent()).
 * После нахождения сигнала все семплы без сигнала удаляются из _data, и размер
 * _data становится равным 1. С этого момента стоит добавлять новые семплы в _data
 * при помощи функции addSample(qint8 sample).
 *    В функции isSignalEnd проверяется добавленно ли 40 семплов после начала сигнала
 * для предотвращения ложного срабатывания. Ложное срабатывание может получиться
 * из-за "провала" сигнала с пьезо на фронте до нуля. После ищется семпл со значением,
 * равным нулю, что означает окончание сигнала.
 *
 */
bool Signal::isSignalEnd()
{
   if (_data.size()>40)
   {
       if (_data.last()==0)
       {
           setStart(false); //сбрасываем флаг старта для ожидания последующего старта
           return true;
       }
       else
       {
           return false;
       }
   }
   else
   {
       return false;
   }
}
void Signal::setSamplingFrequencyAllChannels(quint32 freq)
{
    samplingFrequencyAllChannels=freq;
}
quint32 Signal::getSamplingFrequencyOneChannel()
{
    return samplingFrequencyAllChannels/4;
}

QVector<qreal>* Signal::getTime()
{
    qreal T=1/getSamplingFrequencyOneChannel();
    x.resize(_data.size());
    for (int i=1; i<_data.size()-1;i++)
    {
        x[i]=x.at(i-1)+T;
    }
    return &x;
}
void Signal::filter(QString text )              //in - вход фильтра, coeff - коэф.фильтра от 0 до 1
{
    bool ok;
    qreal coeff=text.toInt(&ok);
    QVector<qreal> out(_data.size());                                         //Выход фильтра
    for (int i=1; i<_data.size(); i++)
    {
        out[i]=coeff*_data.at(i) + (1.0-coeff)*out.at(i-1);                   //сам фильтр
    }
    _data=out;

}
void Signal::setQwtPlotPointer (QwtPlot* _plot)
{
    plot=_plot;
}
qreal Signal::measure()
{

    int min_x=0, max_x=0,skips=0,prev_y=0;
    QVector <qreal> result;
    qreal trans=0;
    qreal angle;
    QPoint point1,point2;
    QwtInterval interval;
    QwtScaleDiv scalediv;
    interval = plot->axisScaleDiv(QwtPlot::xBottom).interval();
    scalediv = plot->axisScaleDiv(QwtPlot::xBottom);
    max_x=(int)interval.maxValue();
    min_x=(int)interval.minValue();
    interval = plot->axisScaleDiv(QwtPlot::yRight).interval();


    for (int i = min_x; i<max_x;i++)
    {
        if (skips) skips++;
        if (_data.at(i)>800 && point1.isNull())
        {
            skips++;
            point1.setX(i);
            point1.setY(_data.at(i));
            trans=plot->canvasMap(QwtPlot::yRight).transform(point1.y());
            point1.setY(trans);
            trans=plot->canvasMap(QwtPlot::xBottom).transform(point1.x());
            point1.setX(trans);
        }
        if (_data.at(i)>2800 && point2.isNull())
        {
            if (skips>40)
            {
                skips=0;
                point2.setX(i);
                point2.setY(_data.at(i));
                trans=plot->canvasMap(QwtPlot::yRight).transform(point2.y());
                point2.setY(trans);
                trans=plot->canvasMap(QwtPlot::xBottom).transform(point2.x());
                point2.setX(trans);
                result.append((qreal)(point2.y()-point1.y())/(qreal)(point2.x()-point1.x()));
                angle=qRadiansToDegrees(- qAtan(result.at(result.size()-1)));
                return angle;
            }
            else
            {
                return -1;
            }

        }
        if (prev_y>2000 && _data.at(i) <10)
        {
            point1.setX(0);
            point1.setY(0);
            point2.setX(0);
            point2.setY(0);
            return -2;
        }
        prev_y=_data.at(i);

    }
    return -3;

}
