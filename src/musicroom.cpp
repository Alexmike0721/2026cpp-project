#include "musicroom.h"
#include "appconfig.h"
#include "startmenu.h"

#include <QAudioOutput>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLabel>
#include <QMediaPlayer>
#include <QPainter>
#include <QPushButton>
#include <QUrl>

static QString findAssetFile(const QString &name)
{
    QStringList roots;
    roots << QDir::currentPath();
    roots << QCoreApplication::applicationDirPath();
    roots << QCoreApplication::applicationDirPath() + "/../../../..";

    for (const QString &root : roots) {
        QString path = root + "/" + name;
        if (QFile::exists(path)) {
            return QFileInfo(path).absoluteFilePath();
        }
    }

    return "";
}

MusicRoom::MusicRoom(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Music Room");
    resize(1280, 720);

    int smallW = 257;
    int smallH = 46;
    int bigW = 257;
    int bigH = 132;

    for (int i = 1; i <= 12; ++i) {
        createMusicButton(i, 84, 110 + (i - 1) * 44, smallW, smallH);
    }

    for (int i = 13; i <= 21; ++i) {
        createMusicButton(i, 342, 110 + (i - 13) * 44, smallW, smallH);
    }

    for (int i = 22; i <= 25; ++i) {
        createMusicButton(i, 599, 110 + (i - 22) * 132, bigW, bigH);
    }

    QPushButton *stopButton = new QPushButton("Stop", this);
    stopButton->setGeometry(1010, 600, 100, 36);
    connect(stopButton, &QPushButton::clicked, this, &MusicRoom::stopMusic);

    QPushButton *backButton = new QPushButton("Back", this);
    backButton->setGeometry(1125, 600, 100, 36);
    connect(backButton, &QPushButton::clicked, this, &MusicRoom::backToMenu);

    infoLabel = new QLabel("Select a track", this);
    infoLabel->setGeometry(930, 545, 300, 34);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("background:rgba(255,255,255,200); color:#333;");

    AppConfig config = loadConfig();
    audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(config.masterVolume / 100.0);

    player = new QMediaPlayer(this);
    player->setAudioOutput(audioOutput);
}

QPushButton *MusicRoom::createMusicButton(int index, int x, int y, int w, int h)
{
    QPushButton *button = new QPushButton(this);
    button->setGeometry(x, y, w, h);

    QString number = QString("%1").arg(index, 2, 10, QChar('0'));
    QString path = findAssetFile("assets/picture/BGM_BTN" + number + ".png");
    if (!path.isEmpty()) {
        button->setIcon(QIcon(QPixmap(path)));
        button->setIconSize(button->size());
    } else {
        button->setText("BGM " + number);
    }

    button->setStyleSheet(
        "QPushButton { border:0px; background:transparent; }"
        "QPushButton:hover { border:2px solid white; }"
    );

    connect(button, &QPushButton::clicked, this, [this, index]() {
        playMusic(index);
    });
    return button;
}

QString MusicRoom::musicPathForIndex(int index)
{
    QString fileName;
    if (index >= 1 && index <= 21) {
        fileName = QString("BGM%1.OGG").arg(index, 2, 10, QChar('0'));
    } else if (index == 22) {
        fileName = "BGM00.OGG";
    } else if (index == 23) {
        fileName = "BGM_OP.OGG";
    } else if (index == 24) {
        fileName = "BGM_ED1.OGG";
    } else if (index == 25) {
        fileName = "BGM_ED2.OGG";
    }

    if (!fileName.isEmpty()) {
        return findAssetFile("assets/music/" + fileName);
    }

    return "";
}

void MusicRoom::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QString path = findAssetFile("assets/picture/BGM_BASE.png");
    if (!path.isEmpty()) {
        painter.drawPixmap(rect(), QPixmap(path));
    } else {
        painter.fillRect(rect(), QColor(240, 240, 240));
    }
}

void MusicRoom::playMusic(int index)
{
    QString path = musicPathForIndex(index);
    if (path.isEmpty()) {
        infoLabel->setText("Track " + QString::number(index) + " missing");
        return;
    }

    player->setSource(QUrl::fromLocalFile(path));
    player->play();
    infoLabel->setText("Playing Track " + QString::number(index));
}

void MusicRoom::stopMusic()
{
    player->stop();
    infoLabel->setText("Stopped");
}

void MusicRoom::backToMenu()
{
    player->stop();
    StartMenu *menu = new StartMenu(false);
    menu->show();
    close();
}
