#ifndef CIMAGEBUTTON_H
#define CIMAGEBUTTON_H

#include <QPushButton>

class CImageButton : public QPushButton
{
    Q_OBJECT

public:
    CImageButton(QWidget* parent, int buttonType);
    ~CImageButton();
    void setPixmap(QPixmap* pNormal, QPixmap* pHover, QPixmap* pLighted);
    void setLighted(bool value);

    void paintEvent(QPaintEvent *pEvent) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void enterEvent(QEvent* e) override;
    void leaveEvent(QEvent* e) override;

protected:
    bool m_bLighted;
    bool m_bMouseIn;
    bool m_bLeftDown;
    QPixmap* m_pHover;
    QPixmap* m_pNormal;
    QPixmap* m_pLighted;

private:
    int m_titleButton;
};

#endif // CIMAGEBUTTON_H
