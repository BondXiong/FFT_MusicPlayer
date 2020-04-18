#ifndef CURVEPLOT_H
#define CURVEPLOT_H

#include <QWidget>
#include  <QPainter>
#include <QPaintEvent>

class QMargin
{
public:
    QMargin(int left, int top, int right, int bottom)
        :mLeft(left)
        ,mTop(top)
        ,mRight(right)
        ,mBottom(bottom)
    {

    }
    QMargin(int h, int v)
        :mLeft(h)
        ,mTop(v)
        ,mRight(h)
        ,mBottom(v)
    {

    }
    QMargin(int d = 0)
        :mLeft(d)
        ,mTop(d)
        ,mRight(d)
        ,mBottom(d)
    {

    }

    int mLeft;
    int mTop;
    int mRight;
    int mBottom;
};

class CurvePlot : public QWidget
{
    Q_OBJECT

public:
    QMargin margin;
    QPen pen;
    QBrush background;
public:
    explicit CurvePlot(QWidget *parent = nullptr);
    void callPaintEvent();
    void outPut(QPainter &painter);
    void setAdjustVal(float val)
    {
        iAdjustVal = val;
    }
    void setData(QVector<float> &data);

    QSize minSizeHint() const;

protected:
    void paintEvent(QPaintEvent *event) override;
//    void resizeEvent(QResizeEvent *event) override;
private:
    QVector<float> data;
    QPixmap pixmap;
    float iAdjustVal;
private:
    void transformPoints(QVector<float> &data, int w, int h, QVector<QPointF> &points);

signals:

};

#endif // CURVEPLOT_H
