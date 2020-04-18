#include "cimagebutton.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtGui>

CImageButton::CImageButton(QWidget* parent, int buttonType)
    : QPushButton(parent)
    , m_bLighted(false)
    , m_bMouseIn(false)
    , m_bLeftDown(false)
    , m_titleButton(buttonType)
{

}

CImageButton::~CImageButton()
{

}

void CImageButton::setPixmap(QPixmap *pNormal, QPixmap *pHover, QPixmap *pLighted)
{
    m_pNormal = pNormal;
    m_pHover = pHover;
    m_pLighted = pLighted;
}

void CImageButton::setLighted(bool value)
{
    if(value != m_bLighted)
    {
        m_bLighted = value;
        repaint();
    }
}

void CImageButton::paintEvent(QPaintEvent *pEvent)
{
    QPainter painter;
    painter.begin(this);
//    QPixmap* pPixmap;
//    if(m_bLighted || m_bLeftDown)
//    {
//        pPixmap = m_pLighted;
//    }else{
//        if(m_bMouseIn)
//            pPixmap = m_pHover;
//        else
//            pPixmap = m_pNormal;
//    }
//    if(pPixmap)
//        painter.drawPixmap(rect(),*pPixmap);
//    if(text().count() > 0)
//        painter.drawText(rect(), Qt::AlignCenter, text());
    if(m_titleButton == 0)
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(QColor(255, 255, 255),2));
        painter.drawLine(QPoint(width()/4,height())/2, QPoint(width()/2, height()/2));
    }
    else if(m_titleButton == 1)
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(QColor(255, 255, 255),2));
        int xtemp = width()/4;
        int ytemp = height()*2/5;
        painter.drawRect(xtemp,ytemp,width()/3,height()/3);
        QPainterPath path;
        path.moveTo(xtemp + 3, ytemp);
        path.lineTo(xtemp + 3, ytemp - 3);
        path.lineTo(xtemp + 3 + width()/3, ytemp - 3);
        path.lineTo(xtemp + 3 + width()/3, ytemp - 3 + height()/3);
        path.lineTo(xtemp + width()/3, ytemp - 3 + height()/3);
        painter.drawPath(path);
    }
    else if(m_titleButton == 2)
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(QColor(255, 255, 255), 2));
        int xtemp = width()/4;
        int ytemp = height()*2/5;
        painter.drawRect(xtemp,ytemp,width()/3,height()/3);
    }
    else if(m_titleButton == 3)
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(QColor(255, 255, 255),2));
        painter.drawLine(QPoint(width()/2 - width()/5, height()/2 - height()/5), QPoint(width()/2 + width()/5, height()/2 + height()/5));
        painter.drawLine(QPoint(width()/2 - width()/5, height()/2 + height()/5), QPoint(width()/2 + width()/5, height()/2 - height()/5));
    }
    else
    {
        int b =1;
    }



    painter.end();
}

void CImageButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        m_bLeftDown = true;
        repaint();
    }
    QPushButton::mousePressEvent(e);
}

void CImageButton::mouseReleaseEvent(QMouseEvent *e)
{
   if(e->button() == Qt::LeftButton)
   {
       m_bLeftDown = false;
       repaint();
   }
   QPushButton::mouseReleaseEvent(e);
}

void CImageButton::enterEvent(QEvent *e)
{
    m_bMouseIn = true;
    repaint();
}

void CImageButton::leaveEvent(QEvent *e)
{
    m_bMouseIn = false;
    repaint();
}





