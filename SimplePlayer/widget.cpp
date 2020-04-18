#include "widget.h"
#include "ui_widget.h"
#include "QFileDialog"
#include "QStandardPaths"
#include <QDesktopWidget>
#include "cimagebutton.h"
#include <QTime>
#include <QMainWindow>
#include <QMediaPlaylist>
#include <QAction>
#include <QMenu>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    this->setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
//    mediaPlayer = new QMediaPlayer;
    playerList = new QMediaPlaylist;
    ui->setupUi(this);
    this->resize(800,600);

    initTitleBar();
    initWindow();

    if(this->actions().size() > 0)
    {
        connect(this->actions().first(), SIGNAL(triggered(bool)), this, SLOT(openFile()));
    }


    connect(start_stopButton, &QPushButton::clicked, [=](){
        bool isPaused = true;
        FA_window->togglePlay();
        FA_window->mChannel->getPaused(&isPaused);
        if(filePath.isEmpty())
        {
            openFile();
        }
        if (isPaused) {
            start_stopButton->setIcon(*iconPaused);
        }else {
            start_stopButton->setIcon(*iconPlaying);
        }
    });
    connect(nextSongButton, &QPushButton::clicked, [=](){
        seekForw();
    });
    connect(lastSongButton, &QPushButton::clicked, [=](){
        seekBackw();
    });
    connect(&FA_window->timer, SIGNAL(timeout()), this, SLOT(onTimer()));
//    connect(mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(updateDuration(qint64)));
//    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(updatePosition(qint64)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initTitleBar()
{
    m_titleBar = new BaseTitleBar(this);
    m_titleBar->resize(this->width(), this->height()*0.1);
    m_titleBar->move(0, 0);

    connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
    connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
    connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
    connect(m_titleBar, SIGNAL(signalButtonClosedClicked()), this, SLOT(onButtonCloseClicked()));
}

void Widget::initWindow()
{

    FA_window = new FreAliWindow(this);

    controlBar = new QWidget(this);
    QHBoxLayout* myLayout = new QHBoxLayout(controlBar);

    QAction* searchMusic = new QAction(QStringLiteral("添加"), ui->songList);;
    this->addAction(searchMusic);
    setContextMenuPolicy(Qt::ActionsContextMenu);


    iconLeft = new QIcon(":res/leftButton.png");
    iconRight = new QIcon(":res/rightButton.png");
    iconPlaying = new QIcon(":res/playingButton.png");
    iconPaused = new QIcon(":res/pausedButton.png");

    start_stopButton = new QPushButton(this);
    lastSongButton = new QPushButton(this);
    nextSongButton = new QPushButton(this);

    lastSongButton->resize(songChangeButtonWidth,songChangeButtonHeight);
    lastSongButton->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 0);}");
    lastSongButton->raise();
    lastSongButton->setIcon(*iconLeft);
    lastSongButton->setIconSize(QSize(lastSongButton->width(), lastSongButton->height()));

    nextSongButton->resize(songChangeButtonWidth,songChangeButtonHeight);
    nextSongButton->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 0);}");
    nextSongButton->raise();
    nextSongButton->setIcon(*iconRight);
    nextSongButton->setIconSize(QSize(nextSongButton->width(), nextSongButton->height()));

    start_stopButton->resize(songChangeButtonWidth,songChangeButtonHeight);
    start_stopButton->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 0);}");
    start_stopButton->raise();
    start_stopButton->setIcon(*iconPaused);
    start_stopButton->setIconSize(QSize(start_stopButton->width(), start_stopButton->height()));

    progressSlider = new QSlider(this);
    progressSlider->setOrientation(Qt::Horizontal);
//    progressSlider->resize(this->width()*0.6, 50);
    progressSlider->setFixedWidth(this->width()*0.5);
    passedTime = new QLabel(this);
    passedTime->setText(QString("00:00"));
    passedTime->setStyleSheet("QLabel{color:white}");
    totalTime = new QLabel(this);
    totalTime->setText("00:00");
    totalTime->setStyleSheet("QLabel{color:white}");

//    QWidget* tempWid = new QWidget(this);
    myLayout->setSpacing(5);
    myLayout->addWidget(lastSongButton);
    myLayout->setSpacing(5);
    myLayout->addWidget(start_stopButton);
    myLayout->setSpacing(5);
    myLayout->addWidget(nextSongButton);
    myLayout->setSpacing(5);

    myLayout->addWidget(passedTime);
    myLayout->setSpacing(5);
    myLayout->addWidget(progressSlider);
    myLayout->setSpacing(5);
    myLayout->addWidget(totalTime);

    myLayout->setContentsMargins(5,0,0,0);
    myLayout->setSpacing(5);

    ui->songList->resize(this->width()*0.3, this->height()*0.8);
    ui->songList->setStyleSheet("QTableWidget{background-color: rgba(0,50,100,240);}");
    ui->songList->verticalHeader()->setVisible(false);
    ui->songList->horizontalHeader()->setVisible(false);
    ui->songList->setShowGrid(false);
    QFont songListFont("Helvetica");
    songListFont.setPointSize(12);
    ui->songList->setFont(songListFont);
    ui->songList->horizontalHeader()->resizeSection(0, ui->songList->width());
    ui->songList->move(0, m_titleBar->height());

    FA_window->getCurvePlot()->resize(m_titleBar->width() - ui->songList->width(),ui->songList->height());
    FA_window->move(ui->songList->width(),m_titleBar->height());

    ui->frame->setStyleSheet("QFrame{background:rgba(65,65,65, 250);}");
    ui->frame->resize(this->width(), this->height() - ui->songList->height() - m_titleBar->height());
    ui->frame->move(0, ui->songList->height() + m_titleBar->height());

//    controlBar->setStyleSheet("QWidget{background-color: white;}");
    controlBar->resize(ui->frame->width(), ui->frame->height());
    controlBar->move(0, this->height() - ui->frame->height()/2- controlBar->height()/2);
    controlBar->setLayout(myLayout);
}

void Widget::openFile()
{
    QStringList defaultMusicPath = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
    QStringList filePathTemp = QFileDialog::getOpenFileNames(this, QStringLiteral("选择音频文件"),
                                                    defaultMusicPath.isEmpty() ? QDir::homePath():defaultMusicPath.first(), "");
    filePath.append(filePathTemp);

    for (int i = 0; i < filePathTemp.size(); i++) {
        QString path = QDir::toNativeSeparators(filePathTemp.at(i));
        if(!path.isEmpty())
        {
//            playerList->addMedia(QUrl::fromLocalFile(path));
            fileNames.append(path.split("\\").last());
            int rowNum = ui->songList->rowCount();
            ui->songList->insertRow(rowNum);
            ui->songList->setColumnCount(1);
            QTableWidgetItem* musicName = new QTableWidgetItem(fileNames.at(rowNum));
            ui->songList->setItem(rowNum, 0, musicName);
            ui->songList->item(rowNum,0)->setTextColor(QColor(255,255,255));
            ui->songList->setColumnWidth(0, ui->songList->width());
        }
    }
    filePathTemp.clear();

    if(!filePath.isEmpty())
    {
        FA_window->stopMusic();
        start_stopButton->setEnabled(true);
        lastSongButton->setEnabled(true);
        nextSongButton->setEnabled(true);
        FA_window->playMusic(filePath.first());
        FA_window->setMusicPath(filePath.first());
//        FA_window->setMusicName(fileNames.first());
        start_stopButton->setIcon(*iconPlaying);
        uint duration = FA_window->getMusicDuration();

        progressSlider->setRange(0, duration);
        progressSlider->setEnabled(duration > 0);
        progressSlider->setPageStep(duration/100);
        QTime tempDuration(0, duration / 60000, qRound((duration % 60000) / 1000.0));
        totalTime->setText(tempDuration.toString(tr("mm:ss")));
    }
}

//void Widget::playMusic(const QString &musicFile)
//{
//    start_stopButton->setEnabled(true);
//    lastSongButton->setEnabled(true);
//    nextSongButton->setEnabled(true);
//    mediaPlayer->setMedia(QUrl::fromLocalFile(musicFile));
//    mediaPlayer->play();
//    start_stopButton->setIcon(*iconPlaying);
//}

//void Widget::togglePlay()
//{
//    if(mediaPlayer->mediaStatus() == QMediaPlayer::NoMedia){
//        openFile();
//    }else if (mediaPlayer->state() == QMediaPlayer::PlayingState) {
//        mediaPlayer->pause();

//        start_stopButton->setIcon(*iconPaused);
//    }else {
//        mediaPlayer->play();

//        start_stopButton->setIcon(*iconPlaying);
//    }
//}

void Widget::seekForw()
{
    FA_window->stopMusic();
    QString musicPath;
    FA_window->getMusicPath(musicPath);
    int index = filePath.indexOf(musicPath);
    qDebug()<<musicPath;
    qDebug()<<index;
    qDebug()<<filePath.size();
    if(index >= 0 & index < filePath.size() - 1)
    {
        FA_window->playMusic(filePath.at(index + 1));
        FA_window->setMusicPath(filePath.at(index + 1));
    }

}

void Widget::seekBackw()
{
    FA_window->stopMusic();
    QString musicPath;
    FA_window->getMusicPath(musicPath);
    int index = filePath.indexOf(musicPath);
    qDebug()<<musicPath;
    qDebug()<<index;
    qDebug()<<filePath.size();
    if(index > 0 & index < filePath.size())
    {
        FA_window->playMusic(filePath.at(index - 1));
        FA_window->setMusicPath(filePath.at(index - 1));
    }
}

void Widget::updateStatus(QMediaPlayer::State state)
{

}

//void Widget::updatePosition(qint64 position)
//{
//    progressSlider->setValue(position);
//    QTime duration(0, position / 60000, qRound((position % 60000) / 1000.0));
//    passedTime->setText(duration.toString(tr("mm:ss")));

//}

void Widget::updateDuration(qint64 duration)
{
    progressSlider->setRange(0, duration);
    progressSlider->setEnabled(duration > 0);
    progressSlider->setPageStep(duration/100);
    QTime tempDuration(0, duration / 60000, qRound((duration % 60000) / 1000.0));
    totalTime->setText(tempDuration.toString(tr("mm:ss")));
}

void Widget::onButtonMinClicked()
{
    qDebug()<<windowFlags();
    if(Qt::Tool == (windowFlags() & Qt::Tool))
    {
        hide();
    }
    else
    {
        showMinimized();
    }
}

void Widget::onButtonRestoreClicked()
{
//    QPoint windowPos;
//    QSize windowSize;
//    m_titleBar->getRestoreInfo(windowPos, windowSize);
//    this->setGeometry(QRect(windowPos, windowSize));

    showNormal();
    ui->songList->resize(this->width()*0.3, this->height()*0.8);
    ui->songList->move(0, m_titleBar->height());

    FA_window->getCurvePlot()->resize(this->width() - ui->songList->width(),ui->songList->height());
    FA_window->move(ui->songList->width(),m_titleBar->height());

    ui->frame->setStyleSheet("QFrame{background:rgba(65,65,65, 250);}");
    ui->frame->resize(this->width(), this->height() - ui->songList->height() - m_titleBar->height());
    ui->frame->move(0, ui->songList->height() + m_titleBar->height());

    controlBar->resize(ui->frame->width(), ui->frame->height());
    controlBar->move(0, this->height() - ui->frame->height()/2- controlBar->height()/2);
}

void Widget::onButtonMaxClicked()
{
//    QPoint pos = QPoint(geometry().x(),geometry().y());
//    QSize size = QSize(this->size());
//    m_titleBar->saveRestoreInfo(pos, size);
    showMaximized();
    ui->songList->resize(this->width()*0.3, this->height()*0.8);
    ui->songList->move(0, m_titleBar->height());

    FA_window->resize(this->width() - ui->songList->width(),ui->songList->height());
    FA_window->getCurvePlot()->resize(this->width() - ui->songList->width(),ui->songList->height());
    FA_window->move(ui->songList->width(),m_titleBar->height());

    ui->frame->setStyleSheet("QFrame{background:rgba(65,65,65, 250);}");
    ui->frame->resize(this->width(), this->height() - ui->songList->height() - m_titleBar->height());
    ui->frame->move(0, ui->songList->height() + m_titleBar->height());

    controlBar->resize(ui->frame->width(), ui->frame->height());
    controlBar->move(0, this->height() - ui->frame->height()/2- controlBar->height()/2);
}

void Widget::onButtonCloseClicked()
{
    close();
}

void Widget::onTimer()
{
    FA_window->onTimerAction();
    uint realtimePos = FA_window->getRealtimePos();
    progressSlider->setValue(realtimePos);
    QTime duration(0, realtimePos / 60000, qRound((realtimePos % 60000) / 1000.0));
    passedTime->setText(duration.toString(tr("mm:ss")));
}




