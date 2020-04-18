#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include "basetitlebar.h"
#include "cimagebutton.h"
#include <QSlider>
#include "frealiwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

enum{
    songChangeButtonWidth = 60,
    songChangeButtonHeight = 60
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void openFile();
//    void playMusic(const QString &musicFile);
//    void togglePlay();
    void seekForw();
    void seekBackw();

private:
    Ui::Widget *ui;

    BaseTitleBar* m_titleBar;
    FreAliWindow* FA_window;
    QWidget* controlBar;

    QPushButton* lastSongButton;
    QPushButton* nextSongButton;
    QPushButton* start_stopButton;
    QSlider* progressSlider;
    QLabel* passedTime;
    QLabel* totalTime;

    QIcon* iconLeft;
    QIcon* iconRight;
    QIcon* iconPlaying;
    QIcon* iconPaused;

//    QMediaPlayer* mediaPlayer;
    QMediaPlaylist* playerList;
    QStringList filePath;
    QStringList fileNames;


    void initTitleBar();
    void initWindow();
private slots:
    void onTimer();
    void updateStatus(QMediaPlayer::State state);
//    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
//    void setPosition(int position);
//    void updateInfo();

    void onButtonMinClicked();
    void onButtonRestoreClicked();
    void onButtonMaxClicked();
    void onButtonCloseClicked();
};
#endif // WIDGET_H
