#ifndef STARTMENU_H
#define STARTMENU_H

#include <QMainWindow>

class QPushButton;
class Scene;
class CgGallery;
class MusicRoom;
class QMediaPlayer;
class QAudioOutput;
class SettingsWindow;

class StartMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartMenu(bool playTitleVoice = true, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void startGame();
    void continueGame();
    void showCgGallery();
    void showMusicRoom();
    void showConfig();
    void showLoad();

private:
    QPushButton *createButton(const QString &text, int x, int y);

    Scene *scene = nullptr;
    CgGallery *cgGallery = nullptr;
    MusicRoom *musicRoom = nullptr;
    SettingsWindow *settingsWindow = nullptr;
    QMediaPlayer *titlePlayer = nullptr;
    QAudioOutput *titleAudio = nullptr;
};

#endif
