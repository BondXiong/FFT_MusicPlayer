#include "curveplot.h"
#include <QDebug>

CurvePlot::CurvePlot(QWidget *parent) : QWidget(parent)
{
    margin = QMargin(0);
//    background = QBrush(QColor(Qt::black));
    pen = QPen(QBrush(QColor(Qt::red)), 5);
    pixmap = QPixmap(":/res/background.jpg");
    iAdjustVal = 0;
}

void CurvePlot::transformPoints(QVector<float> &data, int w, int h, QVector<QPointF> &points)
{
    float max = data[0], min = data[0];
    
    for (int i = 1; i < data.size(); i++) {
        if(max < data[i])
            max = data[i];
        if(min > data[i])
            min = data[i];
    }
    max += iAdjustVal;
    min -= iAdjustVal;
    float diffVal = max - min;
    for (int i = 0; i < data.size(); i++) {
        points.append(QPointF(i*w/data.size(), h - (data[i] - min)/diffVal*h));
//        qDebug()<<i <<":"<<data[i];

    }
//    qDebug()<<"333:"<<h - (data[3] - min)/diffVal*h;
}

void CurvePlot::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);

    painter.save();
    int w = width() - margin.mLeft - margin.mRight;
    int h = height() - margin.mTop - margin.mBottom;
    painter.drawPixmap(0, 0, w, h, pixmap);
    if(data.size()>0)
    {
      outPut(painter);
    }

//
}

//void CurvePlot::resizeEvent(QResizeEvent *event)
//{

//}

QSize CurvePlot::minSizeHint() const
{
    return QSize(margin.mLeft + margin.mRight + 40, margin.mTop + margin.mBottom + 40);
}

void CurvePlot::callPaintEvent()
{
//    int w = width() - margin.mLeft - margin.mRight;
//    int h = height() - margin.mTop - margin.mBottom;
//    pixmap.scaled(w, h, Qt::IgnoreAspectRatio);
//    painter.setPen(pen);
//    painter.drawLine(QPoint(0, height()/2), QPoint(this->width(), height()/2));
    update();
}
//-------------------------------绘图关键方法
void CurvePlot::outPut(QPainter &painter)
{

    QVector<float> dataTemp;
    int j = 0;
    float temp = 0;
    for(int i = 0; i < data.size(); i++){
        if(j <= 5){
            temp += data[i];
            j++;
            if(j == 5){
                dataTemp.append(temp);
                temp = 0;
                j = 0;
            }
        }
    }
    int w = width() - margin.mLeft - margin.mRight;
    int h = height() - margin.mTop - margin.mBottom;

    painter.setRenderHints(QPainter::SmoothPixmapTransform);
    QVector<QPointF> points;

    transformPoints(dataTemp, w, h, points);

    QPen pen1;
    double penWidth = w/dataTemp.size()*0.8;
    pen1.setWidth(penWidth);
    pen1.setColor(Qt::red);
    pen1.setCapStyle(Qt::RoundCap);
    painter.setPen(pen1);

    for (int i = 0; i < points.size() - 1; i++) {
        QPointF pointTemp = points.at(i);
        double x = pointTemp.x() + penWidth*0.9;
        painter.drawLine(QPointF(x, pointTemp.y()), QPointF(x, height()));
    }
    painter.restore();
}

void CurvePlot::setData(QVector<float> &data)
{
    this->data = data;
}



