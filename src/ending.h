#ifndef ENDING_H
#define ENDING_H

#include <QWidget>
#include <QVector>

class QLabel;
class QMediaPlayer;
class QAudioOutput;
class QTimer;
class QGraphicsOpacityEffect;
class QPropertyAnimation;
class QPushButton;

class Ending : public QWidget
{
    Q_OBJECT

public:
    explicit Ending(QWidget *parent = nullptr);

private:
    void fadeToImage(const QString &fileName, int duration = 1500);
    void startRollImages();
    void changeRollBackground(int number);
    void returnToMenu();
    void showEndingText(const QString &text);
    void hideEndingText();

    QLabel *backgroundLabel = nullptr;
    QLabel *nextBackgroundLabel = nullptr;
    QLabel *textBoxLabel = nullptr;
    QLabel *textLabel = nullptr;
    QPushButton *menuButton = nullptr;
    QGraphicsOpacityEffect *fadeEffect = nullptr;
    QPropertyAnimation *fadeAnimation = nullptr;
    QVector<QLabel *> rollLabels;
    QTimer *rollTimer = nullptr;
    int rollStepCount = 0;
    QMediaPlayer *player = nullptr;
    QAudioOutput *audioOutput = nullptr;
};

#endif
