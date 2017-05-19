#include "signal.h"

double Signal::levelmax=1;
double Signal::levelmin=0;
Signal::Signal()
{
    startLevel=5;
    start=false;
    setSamplingFrequencyAllChannels(1000);
    LPFcoeff=1;
    peak=0;
    peakPrev=0;
    isMems=false;
    signalEnd=false;


}
void Signal::setLPFCoeff (qreal coeff)
{
    LPFcoeff=coeff;
}

void Signal::setStart(bool state)
{
    start=state;
}
bool Signal::getStart()
{
    return start;
}
bool Signal::getSignalEnd()
{
    return signalEnd;
}
void Signal::setSignalEnd(bool state)
{
    signalEnd=state;
}
void Signal::setStartLevel(qint16 level)
{
    startLevel=level;
}
void Signal::setIsMems(bool state)
{
    isMems=state;
    peak=255;
}

bool Signal::isSignalPresent()
{
    if (!isMems)
    {
        if (!getStart() && !_data.isEmpty())
        {
            if (_data.last() > startLevel)
            {
                setStart(true);
                setSignalEnd(false);
                if(_data.size()>1)
                {
                    _data.remove(0,_data.size()-2);
                }
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

}

QVector<qreal>* Signal::data()
{
    return &_data;
}

void Signal::addSample(quint8 sample)
{
    _data.append(sample);
}
void Signal::setFSLSlot(int arg1)
{
    falseSignalLevel=arg1;
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
       if (!isMems)
       {
           if (_data.last() <=falseSignalLevel )
           {
               setStart(false); //сбрасываем флаг старта для ожидания последующего старта
               setSignalEnd(true);
               return true;
           }
           else
           {
               return false;
           }
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
    quint32 freq=samplingFrequencyAllChannels/4;
    return freq;
}

QVector<qreal>* Signal::getTime()
{
    //максимальное значение по x = xMax
    //период дескритизации = T
    //количество семплов на экране = xMax/T
    qreal xMax;
    quint32 numPoints;
    qreal T=1/(qreal)getSamplingFrequencyOneChannel();
    xMax= plot->axisScaleDiv(QwtPlot::xBottom).interval().maxValue();
    //нужно меньше точек по Х сделать, а то массив забит лишними данными
    numPoints = _data.size();
    x.resize(numPoints);
    for (int i=1; i<numPoints;i++)
    {
        x[i]=x.at(i-1)+T;
    }
    return &x;
}
void Signal::filter()              //in - вход фильтра, coeff - коэф.фильтра от 0 до 1
{


    QVector<qreal> out(_data.size());                                         //Выход фильтра
    for (int i=1; i<_data.size(); i++)
    {
        out[i]=LPFcoeff*_data.at(i) + (1.0-LPFcoeff)*out.at(i-1);                   //сам фильтр
        if (out.at(i)>peak)
        {
            peakPrev=peak;
            peak=out.at(i);
        }

    }
    _data=out;

}
void Signal::setFalseSignalLevel(quint8 level)
{
    falseSignalLevel=level;
}

bool Signal::isNotFalseSignal()
{
    if (peak>falseSignalLevel)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void Signal::setLevelMin(qreal level)
{
    levelmin=level;
}
void Signal::setLevelMax(qreal level)
{
    levelmax=level;
}
void Signal::setQwtPlotPointer (QwtPlot* _plot)
{
    plot=_plot;
}
void Signal::clearNoSignalData()
{
    qreal time;
    time=_data.size()*1/(qreal)getSamplingFrequencyOneChannel();
    if (time>1)
    {
        clear();
    }
}

qreal Signal::measure()
{
    int skips=0;
    qreal min_x=0,max_x=0,prev_y=0;
    QVector <qreal> result;
    qreal trans=0;
    qreal angle;
    QPointF point1,point2;
    qreal level1=0,level2=0,level3=0;
    QwtInterval interval;
    QwtScaleDiv scalediv;
    qreal T=1/(qreal)getSamplingFrequencyOneChannel();
    qint16 numPoints;
    quint8 posMax=0;

    interval = plot->axisScaleDiv(QwtPlot::xBottom).interval();
    scalediv = plot->axisScaleDiv(QwtPlot::xBottom);
    max_x=_data.size();
    min_x=0;
    interval = plot->axisScaleDiv(QwtPlot::yRight).interval();
    numPoints = max_x;
    for (int i=0;i<_data.size();i++)
    {
        if (_data.at(max_x-i-1)>200)
        {
            posMax=max_x-i-1;
            break;
        }
    }
    level1=_data.at(posMax)*levelmin;
    level2=_data.at(posMax)*levelmax;
    level3=_data.at(posMax)*0.5;
    for (int i = 0; i<numPoints;i++)
    {
        if (skips) skips++;
        if (_data.at(i)>level1 && point1.isNull())
        {
            skips++;
            point1.setX(x.at(i));
            point1.setY(_data.at(i));
            trans=plot->canvasMap(QwtPlot::yLeft).transform(point1.y());
            point1.setY(trans);
            trans=plot->canvasMap(QwtPlot::xBottom).transform(point1.x());
            point1.setX(trans);
        }
        if (_data.at(i)>level2 && point2.isNull())
        {
            if (skips)
            {
                skips=0;
                point2.setX(x.at(i));
                point2.setY(_data.at(i));
                trans=plot->canvasMap(QwtPlot::yLeft).transform(point2.y());
                point2.setY(trans);
                trans=plot->canvasMap(QwtPlot::xBottom).transform(point2.x());
                point2.setX(trans);
                result.append((qreal)(point2.y()-point1.y())/(qreal)(point2.x()-point1.x()));
                angle=qRadiansToDegrees(- qAtan(result.at(result.size()-1)));
                return angle;
            }
            else
            {
                skips=0;
                point1.setX(0);
                point1.setY(0);
            }
        }
        if (prev_y>2000 && _data.at(i) <10)
        {
            point1.setX(0);
            point1.setY(0);
            point2.setX(0);
            point2.setY(0);
        }
        prev_y=_data.at(i);
    }
}
void Signal::clear()
{
    _data.clear();
    start=false;
    x.clear();
    peak=0;
    peakPrev=0;

}
