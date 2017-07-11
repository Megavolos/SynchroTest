#include "diffgraphwindow.h"
#include "ui_diffgraphwindow.h"

diffGraphWindow::diffGraphWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::diffGraphWindow)
{
    ui->setupUi(this);
    ui->qwtPlot->setCanvasBackground(Qt::black);
    zoom_x = new QwtPlotMagnifier( ui->qwtPlot->canvas());
    zoom_y = new QwtPlotMagnifier( ui->qwtPlot->canvas() );
    grid = new QwtPlotGrid;
    grid->enableXMin(false);
    grid->enableX(true);
    grid->setMajorPen(QPen(Qt::white,1,Qt::DotLine));
    grid->setMinorPen(QPen(Qt::gray,1,Qt::DotLine));
    grid->attach(ui->qwtPlot);
    zoom_x->setMouseButton(Qt::MidButton);
    zoom_x->setWheelModifiers(Qt::ShiftModifier);
    zoom_x->setAxisEnabled(QwtPlot::xBottom, true);
    zoom_x->setAxisEnabled(QwtPlot::yRight ,false);
    zoom_x->setAxisEnabled(QwtPlot::yLeft ,false);
    zoom_y->setMouseButton(Qt::MidButton);
    zoom_y->setWheelModifiers(Qt::ControlModifier);
    zoom_y->setAxisEnabled(QwtPlot::xBottom,false);
    zoom_y->setAxisEnabled(QwtPlot::yRight,true);
    zoom_y->setAxisEnabled(QwtPlot::yLeft,true);
    QwtPlotPanner *d_panner = new QwtPlotPanner(ui->qwtPlot->canvas() );
    d_panner->setMouseButton( Qt::RightButton );
    curveDiff = new QwtPlotCurve("Diff");
    curveMems = new QwtPlotCurve("Mems");
    curvePiezo = new QwtPlotCurve("Piezo");

    curveDiff->setPen(QPen(Qt::blue,2,Qt::SolidLine));
    curveMems->setPen(QPen(Qt::red,2,Qt::SolidLine));
    curvePiezo->setPen(QPen(Qt::green,2,Qt::SolidLine));

    curveDiff->attach(ui->qwtPlot);
    curveMems->attach(ui->qwtPlot);
    curvePiezo->attach(ui->qwtPlot);
    ui->qwtPlot->setAxisAutoScale(QwtPlot::xBottom,true);
    ui->qwtPlot->setAxisAutoScale(QwtPlot::yLeft,true);
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,-10,10,1 );
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0,30,5 );

    samplingFreq=500;
    callCounter=0;
    endXreached=false;
    prevMaxX=0;
    angle1=0,angle0=0;
    out=0;
    diffs.resize(2);

}




void diffGraphWindow::setAngleSample(quint8 channel, qreal angle)
{ 

    qreal maxX = ui->qwtPlot->axisScaleDiv(QwtPlot::xBottom).interval().maxValue();
    maxX=quint16(maxX);
    //общее кол-во пикселей = poin2-point1
    //период дескритизации = T, значит , допустим для 30 сек = 10/T семплов, допустим 7500 семплов
    //

    if (maxX!=prevMaxX )
    {
        endXreached=false;
        callCounter=0;
        callCounterMems=0;
       // ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0,maxX,2 );
        t.clear();
        tMems.clear();
        diffs[0].clear();
        diffs[1].clear();
    }

    prevMaxX=maxX;
    if (qIsNaN(angle))
    {
        if (channel == 1 || channel == 3)
        {
            if (diffs[1].isEmpty())
            {
                angle=0;
            }
            else
            {
                angle=diffs[1].last();
            }
        }
        else
        {
            if (diffs[0].isEmpty())
            {
                angle=0;
            }
            else
            {
                return;
            }
        }

    }
    // Пьезо
    if (channel==1)
    {
       angle1=angle;
       if (angle3==0)
       {
           return;
       }
       diffs[1].append(angle3-angle1);
       angle1=0;
       if (!endXreached)
       {
           callCounter++;
           t.append(callCounter);
       }
    }

    if (channel==3)
    {
       angle3=angle;

       if (angle1==0)
       {
           return;
       }
       diffs[1].append(angle3-angle1);
       angle3=0;
       if (!endXreached)
       {
           callCounter++;
           t.append(callCounter);
       }
    }
    //MEMS
    if (channel==0)
    {
       angle0=angle;
       if (angle2==0)
       {
           return;
       }
       diffs[0].append(angle2-angle0);
       angle0=0;
       if (!endXreached)
       {
           callCounterMems++;
           tMems.append(callCounterMems);
       }
    }

    if (channel==2)
    {
       angle2=angle;

       if (angle0==0)
       {
           return;
       }
       diffs[0].append(angle2-angle0);
       angle2=0;
    }

    if (channel == 1 || channel == 3)
    {

        if (callCounter==maxX) endXreached=true;
        if (endXreached) diffs[1].pop_front();
        filter(&diffs[1]);
        curvePiezo->setSamples(t,diffs.at(1));

    }
    if (channel == 0 || channel == 2)
    {

        if (callCounter==maxX) endXreached=true;
        if (endXreached) diffs[0].pop_front();
        filter(&diffs[0]);
        curveMems->setSamples(t,diffs.at(0));
    }

    ui->qwtPlot->replot();

}
void diffGraphWindow::filter(QVector<qreal>* diffsP)              //in - вход фильтра, coeff - коэф.фильтра от 0 до 1
{

    if (diffsP->size()>1)
    {
        out=ui->LPFcoeff->value()*diffsP->last() + (1.0-ui->LPFcoeff->value())*out;                   //сам фильтр
        (*diffsP)[(diffsP->size()-1)]=out;
    }

}
diffGraphWindow::~diffGraphWindow()
{
    delete ui;
}
