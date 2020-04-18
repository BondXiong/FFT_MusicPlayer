#ifndef FREALIWINDOW_H
#define FREALIWINDOW_H

#include <QWidget>
#include <QTimer>
#include "fmod.hpp"
#include "fmod_dsp.h"
#include "fmod_errors.h"
#include "fmod_dsp_effects.h"
#include "curveplot.h"

class FreAliWindow : public QWidget
{
    Q_OBJECT
public:
    explicit FreAliWindow(QWidget *parent = nullptr);
    ~FreAliWindow();

    FMOD_DSP* sd;
    FMOD::System* mSystem = nullptr;
    FMOD::Channel* mChannel;
    FMOD::DSP* mDsp;
    FMOD::Sound* mSound;
    FMOD_RESULT mResult;
    FMOD::ChannelGroup *mChannelGroup;
    FMOD_DSP_PARAMETER_FFT *fftparameter;
    QTimer timer;

public:
    void setMusicPath(QString musicPath);
//    void setMusicName(QString musicName);
    uint getMusicDuration();
    uint getRealtimePos();
    void getMusicPath(QString &musicPath);
    CurvePlot* getCurvePlot();
    void playMusic(QString musicPath);
    void stopMusic();
    void togglePlay();
    void onTimerAction();
signals:
    void on_toolButton_clicked();
    void on_toolButton2_clicked();
    void on_horizontalSlider_sliderMoved(int position);
public slots:

private:
    QString musicPath;
//    QString musicName;
    CurvePlot* _curvePlot = nullptr;
    QVector<float>wav;
    uint musicDuration;
    uint realtimePos;
};

#endif // FREALIWINDOW_H
