#include "basetitlebar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include "cimagebutton.h"

#define BUTTON_HEIGHT 30
#define BUTTON_WIDTH 30
#define TITLE_HEIGHT 30

BaseTitleBar::BaseTitleBar(QWidget *parent)
    : QWidget(parent)
    , m_ColorR(153)
    , m_ColorG(153)
    , m_ColorB(153)
    , m_isPressed(false)
    , m_buttonType(MIN_MAX_BUTTON)
{
    initControl();
    m_pButtonRestore->setVisible(false);
    initConnection();
    loadStyleSheet("MyTitle");
}

BaseTitleBar::~BaseTitleBar()
{

}

void BaseTitleBar::initControl()
{
    m_pIcon = new QLabel;
    m_pTitleText = new QLabel;

    m_pButtonMin = new CImageButton(this, MIN_BUTTON);
    m_pButtonRestore = new CImageButton(this, MIN_MAX_BUTTON);
    m_pButtonMax = new CImageButton(this, MAX_BUTTON);
    m_pButtonClose = new CImageButton(this, ONLY_CLOSE_BUTTON);

    m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

    m_pTitleText->setObjectName("TitleContent");
    m_pButtonMin->setObjectName("ButtonMin");
    m_pButtonRestore->setObjectName("ButtonRestore");
    m_pButtonMax->setObjectName("ButtonMax");
    m_pButtonClose->setObjectName("ButtonClose");

//    QPixmap* buttonMinPix = new QPixmap();
//    QPainter painter(buttonMinPix);
//    painter.setRenderHint(QPainter::Antialiasing, true);
//    painter.setPen(QPen(QColor(0, 160, 230),3));
//    painter.drawLine(QPointF(0, height()), QPointF(width() / 2, height() / 2));


    QHBoxLayout* myLayout = new QHBoxLayout(this);
    myLayout->addWidget(m_pIcon);
    myLayout->addWidget(m_pTitleText);
    myLayout->addWidget(m_pButtonMin);
    myLayout->addWidget(m_pButtonRestore);
    myLayout->addWidget(m_pButtonMax);
    myLayout->addWidget(m_pButtonClose);

    myLayout->setContentsMargins(5,0,0,0);
    myLayout->setSpacing(0);

    m_pTitleText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setFixedHeight(TITLE_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

void BaseTitleBar::initConnection()
{
    connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
    connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
    connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
    connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));
}

void BaseTitleBar::loadStyleSheet(const QString &sheetName)
{

}

void BaseTitleBar::setBackgroundColor(int r, int g, int b)
{
    m_ColorR = r;
    m_ColorG = g;
    m_ColorB = b;

    update();
}

void BaseTitleBar::setTitleIcon(QString filePath)
{
    QPixmap titleIcon(filePath);
    m_pIcon->setPixmap(titleIcon.scaled(25, 25));
}

void BaseTitleBar::setTitleText(QString titleText)
{
    m_pTitleText->setText(titleText);
    m_titleText = titleText;
}

void BaseTitleBar::setTitleWidth(int width)
{
    this->setFixedWidth(width);
}

void BaseTitleBar::setButtonType(ButtonType buttonType)
{
    m_buttonType = buttonType;
    switch (m_buttonType) {
    case MIN_BUTTON:
        {
            m_pButtonRestore->setVisible(false);
            m_pButtonMax->setVisible(false);
        }
        break;
    case MIN_MAX_BUTTON:
        {
            m_pButtonRestore->setVisible(false);
        }
        break;
    case MAX_BUTTON:
        {
            m_pButtonMax->setVisible(false);
            break;
        }
    case ONLY_CLOSE_BUTTON:
        {
            m_pButtonMin->setVisible(false);
            m_pButtonRestore->setVisible(false);
            m_pButtonMax->setVisible(false);
        }
        break;
    default:
        break;
    }

}

void BaseTitleBar::setTitleRoll()
{
    connect(&m_titleRollTimer, SIGNAL(timeout()), this, SLOT(onRollTitle()));
    m_titleRollTimer.start(200);
}

void BaseTitleBar::saveRestoreInfo(const QPoint point, const QSize size)
{
    m_restorePos = point;
    m_restoreSize = size;
}

void BaseTitleBar::getRestoreInfo(QPoint &point, QSize &size)
{
    point = this->m_restorePos;
    size = this->m_restoreSize;
}

void BaseTitleBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPainterPath pathBack;
    pathBack.setFillRule(Qt::WindingFill);
    pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.fillPath(pathBack, QBrush(QColor(m_ColorR, m_ColorG, m_ColorB)));

    if(this->width() != this->parentWidget()->width())
    {
        this->setFixedWidth(this->parentWidget()->width());
    }
    QWidget::paintEvent(event);
}

void BaseTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(m_buttonType == MIN_MAX_BUTTON)
    {
        if(m_pButtonMax->isVisible())
        {
            onButtonMaxClicked();
        }else
        {
            onButtonRestoreClicked();
        }
    }

    return QWidget::mouseDoubleClickEvent(event);
}

void BaseTitleBar::mousePressEvent(QMouseEvent *event)
{
    if(m_buttonType == MIN_MAX_BUTTON)
    {
        if(m_pButtonMax->isVisible())
        {
            m_isPressed = true;
            m_startMovePos = event->globalPos();
        }
    }
    else
    {
        m_isPressed = true;
        m_startMovePos = event->globalPos();
    }

    return QWidget::mousePressEvent(event);
}

void BaseTitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isPressed)
    {
        QPoint movePoint = event->globalPos() - m_startMovePos;
        QPoint widgetPos = this->parentWidget()->pos();
        m_startMovePos = event->globalPos();
        this->parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
    }
    return QWidget::mouseMoveEvent(event);
}

void BaseTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    return QWidget::mouseReleaseEvent(event);
}

void BaseTitleBar::onButtonMinClicked()
{
    emit signalButtonMinClicked();
}

void BaseTitleBar::onButtonRestoreClicked()
{
    m_pButtonRestore->setVisible(false);
    m_pButtonMax->setVisible(true);
    emit signalButtonRestoreClicked();
}

void BaseTitleBar::onButtonMaxClicked()
{
    m_pButtonMax->setVisible(false);
    m_pButtonRestore->setVisible(true);
    emit signalButtonMaxClicked();
}

void BaseTitleBar::onButtonCloseClicked()
{
    emit signalButtonClosedClicked();
}

void BaseTitleBar::onRollTitle()
{
    static int nPos = 0;
    QString titleContent = m_titleText;
    if(nPos > titleContent.length())
    {
        nPos = 0;
    }
    m_pTitleText->setText(titleContent.mid(nPos));
    nPos++;
}




