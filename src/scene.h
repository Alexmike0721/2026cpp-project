#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QStringList>
#include <QVector>
#include <QDebug>

class QLabel;
class QPushButton;
class QMediaPlayer;
class QAudioOutput;
class QGraphicsOpacityEffect;
class QPropertyAnimation;
class QTimer;

struct StoryItem
{
    QString name;
    QString text;
    QString background;
    QString voice;
};

QDebug operator<<(QDebug debug, const StoryItem &item);

class Scene : public QWidget
{
    Q_OBJECT

public:
    explicit Scene(int startIndex = 0, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void skipText();
    void showBacklog();
    void saveGame();
    void backToMenu();

private:
    void advanceText();
    void showCurrentText();
    void changeBackground(const QString &fileName);

    QLabel *nameLabel = nullptr;
    QLabel *backgroundLabel = nullptr;
    QLabel *nextBackgroundLabel = nullptr;
    QLabel *textBoxLabel = nullptr;
    QLabel *textLabel = nullptr;
    QPushButton *saveButton = nullptr;
    QPushButton *skipButton = nullptr;
    QPushButton *backlogButton = nullptr;
    QPushButton *menuButton = nullptr;
    QMediaPlayer *voicePlayer = nullptr;
    QAudioOutput *voiceOutput = nullptr;
    QGraphicsOpacityEffect *backgroundEffect = nullptr;
    QPropertyAnimation *backgroundAnimation = nullptr;
    QTimer *skipTimer = nullptr;

    QVector<StoryItem> story;
    QStringList backlog;
    QString currentBackground;

    int textIndex = 0;
    int savedIndex = 0;
    bool skipMode = false;
};

#endif
