#include "ending.h"
#include "appconfig.h"
#include "startmenu.h"

#include <QAbstractAnimation>
#include <QAudioOutput>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QMediaPlayer>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QStringList>
#include <QTimer>
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

static QString findFirstAssetFile(const QStringList &names)
{
    for (const QString &name : names) {
        QString path = findAssetFile(name);
        if (!path.isEmpty()) {
            return path;
        }
    }

    return "";
}

Ending::Ending(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Ending");
    resize(1280, 720);

    backgroundLabel = new QLabel(this);
    backgroundLabel->setGeometry(0, 0, width(), height());
    backgroundLabel->setStyleSheet("background:black;");
    backgroundLabel->setScaledContents(true);

    QString blackPath = findFirstAssetFile({
        "assets/picture/ed_black.png",
        "assets/picture/ETC_END.png"
    });
    if (!blackPath.isEmpty()) {
        backgroundLabel->setPixmap(QPixmap(blackPath));
    }

    nextBackgroundLabel = new QLabel(this);
    nextBackgroundLabel->setGeometry(0, 0, width(), height());
    nextBackgroundLabel->setStyleSheet("background:white;");
    nextBackgroundLabel->setScaledContents(true);

    QString whitePath = findFirstAssetFile({
        "assets/picture/ed_white.png",
        "assets/picture/ETC_END2.png"
    });
    if (!whitePath.isEmpty()) {
        nextBackgroundLabel->setPixmap(QPixmap(whitePath));
    }

    fadeEffect = new QGraphicsOpacityEffect(nextBackgroundLabel);
    nextBackgroundLabel->setGraphicsEffect(fadeEffect);
    fadeEffect->setOpacity(0.0);

    fadeAnimation = new QPropertyAnimation(fadeEffect, "opacity", this);

    textBoxLabel = new QLabel(this);
    textBoxLabel->setGeometry(0, 520, width(), 192);
    textBoxLabel->setScaledContents(true);
    QString textBoxPath = findAssetFile("assets/picture/MAIN_BASE00.png");
    if (!textBoxPath.isEmpty()) {
        textBoxLabel->setPixmap(QPixmap(textBoxPath));
    } else {
        textBoxLabel->setStyleSheet("background:white;");
    }
    textBoxLabel->hide();

    textLabel = new QLabel(this);
    textLabel->setGeometry(220, 568, 840, 70);
    textLabel->setWordWrap(true);
    textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    textLabel->setStyleSheet(
        "background:transparent;"
        "color:black;"
        "font-size:20px;"
        "font-weight:500;"
    );
    textLabel->hide();

    menuButton = new QPushButton("Menu", this);
    menuButton->setGeometry(1120, 640, 100, 36);
    menuButton->setStyleSheet(
        "QPushButton {"
        "background:rgba(255,255,255,210);"
        "border:1px solid rgba(255,255,255,230);"
        "border-radius:10px;"
        "font-size:16px;"
        "}"
        "QPushButton:hover { background:white; }"
    );
    menuButton->hide();
    connect(menuButton, &QPushButton::clicked, this, [this]() {
        returnToMenu();
    });

    QPropertyAnimation *openingFade = new QPropertyAnimation(fadeEffect, "opacity", this);
    openingFade->setDuration(2600);
    openingFade->setStartValue(0.0);
    openingFade->setEndValue(1.0);
    connect(openingFade, &QPropertyAnimation::finished, this, [this]() {
        nextBackgroundLabel->hide();
        fadeEffect->setOpacity(0.0);
    });
    openingFade->start(QAbstractAnimation::DeleteWhenStopped);

    AppConfig config = loadConfig();
    audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(config.masterVolume / 100.0);

    player = new QMediaPlayer(this);
    player->setAudioOutput(audioOutput);

    QString musicPath = findFirstAssetFile({
        "assets/music/BGM_ED2.OGG",
        "assets/music/ed.ogg",
        "assets/music/ed.mp3"
    });
    if (!musicPath.isEmpty()) {
        player->setSource(QUrl::fromLocalFile(musicPath));
        player->play();
    }

    QTimer::singleShot(4500, this, [this]() {
        fadeToImage("assets/picture/EV002A.png", 1600);
    });

    QStringList endingTexts;
    endingTexts << "Glorious Days。"
                << "歌唱美好的时光吧。"
                << "歌唱万里晴空，超越时间。"
                << "我们一定是这样做出的选择。"
                << "这里对我们来说就是最美好的世界。"
                << "Glorious Days 响彻胸膛。"
                << "久久不能平息。"
                << "那些熠熠生辉的景象被歌声唤醒，浮现在我疲倦的脑海里。"
                << "我们为了些十分渺小的事物，全力度过了那段岁月。"
                << "就算可笑，就算笨拙，也义无反顾。"
                << "我全力追逐着你。"
                << "所以回忆起来才觉得这般耀眼。"
                << "如果这段岁月也是Glorious Days。"
                << "那我们所能做的，只有一直全力以赴了吧。";

    int showTime = 8000;
    for (int i = 0; i < endingTexts.size(); ++i) {
        QTimer::singleShot(showTime, this, [this, endingTexts, i]() {
            showEndingText(endingTexts.at(i));
        });

        int textLength = endingTexts.at(i).size();
        int stayTime = 2600 + textLength * 120;
        stayTime = qBound(3200, stayTime, 7000);
        showTime += stayTime;
    }

    int textEndTime = showTime + 800;

    QTimer::singleShot(textEndTime, this, [this]() {
        hideEndingText();
    });

    QTimer::singleShot(textEndTime + 4000, this, [this]() {
        fadeToImage("assets/picture/EV122A.png", 1600);
    });

    QTimer::singleShot(textEndTime + 8000, this, [this]() {
        fadeToImage("assets/picture/ROLL_BACK_SYUSYOU01.png", 1800);
    });

    QTimer::singleShot(textEndTime + 9800, this, [this]() {
        menuButton->show();
        menuButton->raise();
        startRollImages();
    });

    QTimer::singleShot(155000, this, [this]() {
        changeRollBackground(2);
    });

    QTimer::singleShot(185000, this, [this]() {
        changeRollBackground(3);
    });

    QTimer::singleShot(215000, this, [this]() {
        changeRollBackground(4);
    });

    QTimer::singleShot(247000, this, [this]() {
        returnToMenu();
    });
}

void Ending::fadeToImage(const QString &fileName, int duration)
{
    QString path = findAssetFile(fileName);
    if (path.isEmpty()) {
        return;
    }

    fadeAnimation->stop();
    nextBackgroundLabel->setPixmap(QPixmap(path));
    nextBackgroundLabel->raise();
    nextBackgroundLabel->show();
    fadeEffect->setOpacity(0.0);

    fadeAnimation->disconnect();
    fadeAnimation->setDuration(duration);
    fadeAnimation->setStartValue(0.0);
    fadeAnimation->setEndValue(1.0);

    connect(fadeAnimation, &QPropertyAnimation::finished, this, [this, path]() {
        backgroundLabel->setPixmap(QPixmap(path));
        nextBackgroundLabel->hide();
        fadeEffect->setOpacity(0.0);

        for (QLabel *label : rollLabels) {
            label->raise();
        }
        if (textBoxLabel->isVisible()) {
            textBoxLabel->raise();
            textLabel->raise();
        }
        if (menuButton->isVisible()) {
            menuButton->raise();
        }
    });

    fadeAnimation->start();
}

void Ending::startRollImages()
{
    if (rollTimer != nullptr) {
        return;
    }

    int imageWidth = 300;
    int imageHeight = 172;
    int gap = 22;
    int startX = width() - imageWidth - 50;
    int oneLoopSteps = (imageHeight + gap) * 12;
    int stopSteps = oneLoopSteps * 5;

    for (int i = 1; i <= 12; ++i) {
        QString number = QString("%1").arg(i, 2, 10, QChar('0'));
        QString path = findAssetFile("assets/picture/ROLL_" + number + ".png");
        if (path.isEmpty()) {
            continue;
        }

        QLabel *label = new QLabel(this);
        label->setGeometry(startX, height() + (i - 1) * (imageHeight + gap), imageWidth, imageHeight);
        label->setPixmap(QPixmap(path));
        label->setScaledContents(true);
        label->show();
        label->raise();
        rollLabels.push_back(label);
    }

    rollTimer = new QTimer(this);
    connect(rollTimer, &QTimer::timeout, this, [this, imageHeight, gap, stopSteps]() {
        rollStepCount++;
        if (rollStepCount >= stopSteps) {
            rollTimer->stop();
            for (QLabel *label : rollLabels) {
                label->hide();
            }
            return;
        }

        int maxY = 0;
        for (QLabel *label : rollLabels) {
            if (label->y() > maxY) {
                maxY = label->y();
            }
        }

        for (QLabel *label : rollLabels) {
            label->move(label->x(), label->y() - 1);
            if (label->y() + imageHeight < 0) {
                label->move(label->x(), maxY + imageHeight + gap);
                maxY = label->y();
            }
        }
    });
    rollTimer->start(25);
}

void Ending::changeRollBackground(int number)
{
    QString fileName = QString("assets/picture/ROLL_BACK_SYUSYOU%1.png")
                           .arg(number, 2, 10, QChar('0'));
    fadeToImage(fileName, 1800);
}

void Ending::showEndingText(const QString &text)
{
    textLabel->setText(text);
    textBoxLabel->show();
    textLabel->show();
    textBoxLabel->raise();
    textLabel->raise();
}

void Ending::hideEndingText()
{
    textBoxLabel->hide();
    textLabel->hide();
}

void Ending::returnToMenu()
{
    if (rollTimer != nullptr) {
        rollTimer->stop();
    }
    if (player != nullptr) {
        player->stop();
    }

    StartMenu *menu = new StartMenu;
    menu->show();
    close();
}
