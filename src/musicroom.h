#ifndef MUSICROOM_H
#define MUSICROOM_H

#include <QWidget>

class QLabel;
class QPushButton;
class QMediaPlayer;
class QAudioOutput;

class MusicRoom : public QWidget
{
    Q_OBJECT

public:
    explicit MusicRoom(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void playMusic(int index);
    void stopMusic();
    void backToMenu();

private:
    QPushButton *createMusicButton(int index, int x, int y, int w, int h);
    QString musicPathForIndex(int index);

    QLabel *infoLabel = nullptr;
    QMediaPlayer *player = nullptr;
    QAudioOutput *audioOutput = nullptr;
};

#endif
