#include "cggallery.h"
#include "appconfig.h"
#include "musicroom.h"
#include "settingswindow.h"
#include "startmenu.h"
#include "scene.h"

#include <QAudioOutput>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QTextStream>
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

static QString saveFilePath()
{
    QDir dir(QCoreApplication::applicationDirPath() + "/../../../..");
    dir.mkpath("save");
    return dir.filePath("save/save.txt");
}

static bool readSaveIndex(int &index)
{
    QFile file(saveFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream stream(&file);
    stream >> index;
    return true;
}

StartMenu::StartMenu(bool playTitleVoice, QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Galgame Demo");
    resize(1280, 720);

    QPushButton *startButton = createButton("Start", 82, 210);
    QPushButton *continueButton = createButton("Continue", 82, 264);
    QPushButton *cgButton = createButton("CG Gallery", 82, 318);
    QPushButton *musicButton = createButton("Music Room", 82, 372);
    QPushButton *configButton = createButton("Settings", 82, 426);
    QPushButton *loadButton = createButton("Load", 82, 480);
    QPushButton *quitButton = createButton("Quit", 82, 534);

    connect(startButton, &QPushButton::clicked, this, &StartMenu::startGame);
    connect(continueButton, &QPushButton::clicked, this, &StartMenu::continueGame);
    connect(cgButton, &QPushButton::clicked, this, &StartMenu::showCgGallery);
    connect(musicButton, &QPushButton::clicked, this, &StartMenu::showMusicRoom);
    connect(configButton, &QPushButton::clicked, this, &StartMenu::showConfig);
    connect(loadButton, &QPushButton::clicked, this, &StartMenu::showLoad);
    connect(quitButton, &QPushButton::clicked, this, &StartMenu::close);

    int saveIndex = 0;
    if (!readSaveIndex(saveIndex)) {
        continueButton->setEnabled(false);
        continueButton->setText("Continue - No Save");
    }

    if (playTitleVoice) {
        AppConfig config = loadConfig();
        titleAudio = new QAudioOutput(this);
        titleAudio->setVolume(config.masterVolume / 100.0);

        titlePlayer = new QMediaPlayer(this);
        titlePlayer->setAudioOutput(titleAudio);

        QString titleVoice = findAssetFile("assets/voice/VO01_SYS02.OGG");
        if (!titleVoice.isEmpty()) {
            titlePlayer->setSource(QUrl::fromLocalFile(titleVoice));
            titlePlayer->play();
        }
    }
}

QPushButton *StartMenu::createButton(const QString &text, int x, int y)
{
    QPushButton *button = new QPushButton(text, this);
    button->setGeometry(x, y, 230, 42);
    button->setStyleSheet(
        "QPushButton {"
        "background:rgba(255,255,255,205);"
        "border:1px solid rgba(255,255,255,230);"
        "border-radius:14px;"
        "color:#333;"
        "font-size:18px;"
        "font-weight:500;"
        "}"
        "QPushButton:hover {"
        "background:rgba(255,255,255,240);"
        "}"
        "QPushButton:pressed {"
        "background:rgba(235,235,235,230);"
        "}"
        "QPushButton:disabled {"
        "background:rgba(220,220,220,150);"
        "color:rgba(80,80,80,150);"
        "border:1px solid rgba(220,220,220,150);"
        "}"
    );
    return button;
}

void StartMenu::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QString titlePath = findAssetFile("assets/picture/TITLE_BASE01.png");
    if (!titlePath.isEmpty()) {
        painter.drawPixmap(rect(), QPixmap(titlePath));
    } else {
        painter.fillRect(rect(), QColor(245, 205, 215));
        painter.setPen(QColor(70, 35, 45));
        painter.drawText(70, 100, "Visual Novel Demo");
    }
}

void StartMenu::startGame()
{
    scene = new Scene(0);
    scene->show();
    hide();
}

void StartMenu::continueGame()
{
    int saveIndex = 0;
    if (!readSaveIndex(saveIndex)) {
        QMessageBox::information(this, "Continue", "还没有存档。");
        return;
    }

    scene = new Scene(saveIndex);
    scene->show();
    hide();
}

void StartMenu::showCgGallery()
{
    int saveIndex = 0;
    int unlockedCount = readSaveIndex(saveIndex) ? qMin(saveIndex + 2, 6) : 1;

    cgGallery = new CgGallery(unlockedCount);
    cgGallery->show();
    hide();
}

void StartMenu::showMusicRoom()
{
    musicRoom = new MusicRoom;
    musicRoom->show();
    hide();
}

void StartMenu::showConfig()
{
    settingsWindow = new SettingsWindow;
    settingsWindow->show();
    hide();
}

void StartMenu::showLoad()
{
    int saveIndex = 0;
    if (!readSaveIndex(saveIndex)) {
        QMessageBox::information(this, "Load", "还没有存档。");
        return;
    }

    QMessageBox::StandardButton button = QMessageBox::question(
        this,
        "Load",
        "读取存档位：" + QString::number(saveIndex + 1) + "？"
    );

    if (button == QMessageBox::Yes) {
        scene = new Scene(saveIndex);
        scene->show();
        hide();
    }
}
