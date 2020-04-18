#include "frealiwindow.h"
#include <QDebug>

FreAliWindow::FreAliWindow(QWidget *parent) : QWidget(parent)
{
    wav.resize(512);
    mResult = FMOD::System_Create(&mSystem);
    if(mResult != FMOD_OK)
    {
        exit(-14);
    }
    mResult = mSystem->init(32, FMOD_INIT_NORMAL, 0);
    if(mResult != FMOD_OK)
    {
        exit(-15);
    }
    _curvePlot = new CurvePlot(this);
    _curvePlot->setAdjustVal(0.5);
//    _curvePlot->outPut();
}

FreAliWindow::~FreAliWindow()
{

}

void FreAliWindow::setMusicPath(QString musicPath)
{
    this->musicPath = musicPath;
}

void FreAliWindow::playMusic(QString musicPath)
{
    mResult = mSystem->createSound(musicPath.toLocal8Bit(), FMOD_LOOP_NORMAL|FMOD_2D, 0, &mSound);
    if(mResult != FMOD_OK)
    {
        exit(-12);
    }
    mResult = mSystem->playSound(mSound, 0, false, &mChannel);
    if(mResult != FMOD_OK)
    {
        exit(-11);
    }
    mSound->setMode(FMOD_LOOP_OFF);
    mResult = mSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &mDsp);
    mResult = mChannel->addDSP(0, mDsp);
    mSound->getLength(&musicDuration, FMOD_TIMEUNIT_MS);
    mSystem->update();

    timer.start(100);
}

void FreAliWindow::stopMusic()
{
    bool isPaused = true;
    mChannel->getPaused(&isPaused);
    mChannel->setPaused(!isPaused);
    timer.stop();
}

void FreAliWindow::togglePlay()
{
    bool isPaused = true;
//  将参数设定到channel,同时播放或暂停音乐
//  mChannel->setPaused(!isPaused);
//  将channel读取到参数变量-------------------------------关键代码
    mChannel->getPaused(&isPaused);
    mChannel->setPaused(!isPaused);
    bool isPlaying;
    mChannel->isPlaying(&isPlaying);
    if(!isPlaying)
    {
        timer.stop();
    }else{
        timer.start(100);
    }
}
//------------------------------------绘图关键方法
void FreAliWindow::onTimerAction()
{
    mDsp->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&fftparameter, 0, 0, 0);
    for (int j = 0;j<(fftparameter->length/4);j++) {
        wav[j] = fftparameter->spectrum[0][j]*30;
    }
    _curvePlot->setData(wav);
    _curvePlot->callPaintEvent();

    mChannel->getPosition(&realtimePos, FMOD_TIMEUNIT_MS);
    bool isPlaying;
    mChannel->isPlaying(&isPlaying);
    if(!isPlaying)
    {
        timer.stop();
    }
}

uint FreAliWindow::getMusicDuration()
{
    return this->musicDuration;
}

uint FreAliWindow::getRealtimePos()
{
    return  this->realtimePos;
}

CurvePlot* FreAliWindow::getCurvePlot()
{
    return this->_curvePlot;
}

void FreAliWindow::getMusicPath(QString &musicPath)
{
    musicPath = this->musicPath;
}






