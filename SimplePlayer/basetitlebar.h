#ifndef BASETITLEBAR_H
#define BASETITLEBAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include "cimagebutton.h"

enum ButtonType
{
    MIN_BUTTON = 0,
    MIN_MAX_BUTTON,
    MAX_BUTTON,
    ONLY_CLOSE_BUTTON
};

class BaseTitleBar : public QWidget
{
    Q_OBJECT
public:
    BaseTitleBar(QWidget *parent = nullptr);
    ~BaseTitleBar();

    void setBackgroundColor(int r, int g, int b);
    void setTitleIcon(QString filePath);
    void setTitleText(QString titleText);
    void setTitleWidth(int width);
    void setButtonType(ButtonType buttonType);
    void setTitleRoll();

    void saveRestoreInfo(const QPoint point, const QSize size);
    void getRestoreInfo(QPoint &point, QSize &size);

private:
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void initControl();
    void initConnection();
    void loadStyleSheet(const QString &sheetName);

    QLabel* m_pIcon;
    QLabel* m_pTitleText;
    CImageButton* m_pButtonMin;
    CImageButton* m_pButtonRestore;
    CImageButton* m_pButtonMax;
    CImageButton* m_pButtonClose;

    int m_ColorR;
    int m_ColorG;
    int m_ColorB;

    QPoint m_restorePos;
    QSize m_restoreSize;

    bool m_isPressed;
    QPoint m_startMovePos;

    QTimer m_titleRollTimer;
    QString m_titleText;
    ButtonType m_buttonType;

signals:
    void signalButtonMinClicked();
    void signalButtonRestoreClicked();
    void signalButtonMaxClicked();
    void signalButtonClosedClicked();

private slots:
    void onButtonMinClicked();
    void onButtonRestoreClicked();
    void onButtonMaxClicked();
    void onButtonCloseClicked();
    void onRollTitle();
};

#endif // BASETITLEBAR_H
