#include "ending.h"
#include "appconfig.h"
#include "scene.h"
#include "startmenu.h"

#include <QAudioOutput>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QTextStream>
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

static QString saveFilePath()
{
    QDir dir(QCoreApplication::applicationDirPath() + "/../../../..");
    dir.mkpath("save");
    return dir.filePath("save/save.txt");
}

QDebug operator<<(QDebug debug, const StoryItem &item)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "StoryItem("
                    << "name=" << item.name
                    << ", text=" << item.text
                    << ", background=" << item.background
                    << ", voice=" << item.voice
                    << ")";
    return debug;
}

Scene::Scene(int startIndex, QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Galgame Demo - Scene");
    resize(1280, 720);

    story = {
        {"", "年初的时候，我的第三本小说应该是出版了。", "ETC_BK017D.png", ""},
        {"", "也不知道反响如何。", "", ""},
        {"", "藤原先生几乎没联系我。", "", ""},
        {"", "她读到我写的书了吗？", "", ""},
        {"", "成功把什么传递给她了吗？", "", ""},
        {"", "能传达到就好了。", "", ""},
        {"", "她说一个人独处很痛苦。", "", ""},
        {"", "如果我的话语能传达给她，那她是不是就不会孤单一人了。", "", ""},
        {"", "······", "", ""},
        {"", "我现在······", "", ""},
        {"", "好想给她写信。", "", ""},
        {"国见洸太郎", "「我们······我和你······」", "", "VO14_5754.OGG"},
        {"国见洸太郎", "「没准还挺像的」", "EV007A.png", "VO14_5755.OGG"},
        {"国见洸太郎", "「牺牲了一切追寻着你，最后等待我的或许只有寂寞」", "", "VO14_5756.OGG"},
        {"国见洸太郎", "「全力追求着你的我，最后同样变得一无所有」", "", "VO14_5757.OGG"},
        {"国见洸太郎", "「但是回头一想，那个季节是多么耀眼动人」", "", "VO14_5758.OGG"},
        {"国见洸太郎", "「真是无可替代的至宝」", "", "VO14_5759.OGG"},
        {"国见洸太郎", "「你没有给我回信，一言不发地离开我。到了现在，我感觉自己能理解你为什么这么做了」", "", "VO14_5760.OGG"},
        {"国见洸太郎", "「你只是尽了自己的全力」", "", "VO14_5761.OGG"},
        {"国见洸太郎", "「一心为着那些你应当付出全力的事物」", "", "VO14_5762.OGG"},
        {"国见洸太郎", "「正因为这就是你，所以你最后的最后写在信里的话语，肯定满含着无比的决心吧」", "", "VO14_5763.OGG"},
        {"国见洸太郎", "「你发誓不会再来见我了」", "", "VO14_5764.OGG"},
        {"国见洸太郎", "「如果这是你的决意，那我会郑重接受」", "", "VO14_5765.OGG"},
        {"国见洸太郎", "「但是即便如此，我还是想再尽一些全力说出来」", "", "VO14_5766.OGG"},
        {"国见洸太郎", "「我想见你」", "", "VO14_5767.OGG"},
        {"国见洸太郎", "「我喜欢你」", "", "VO14_5768.OGG"},
        {"", "在我的幻想中，幻想出来的她收下了幻想出来的信。", "", ""},
        {"", "接着，她······", "EV119B.png", ""},
        {"", "也不知道为什么。", "", ""},
        {"", "我至今都没有想过。", "", ""},
        {"", "星奏究竟是抱着什么样的想法读那封信的呢。", "", ""},
        {"", "我把注意力都放在了有没有回信的问题上。", "", ""},
        {"", "完全没有想象她收到信的瞬间。", "", ""},
        {"", "虽然信没能足以打动她的内心。", "", ""},
        {"", "但就算不是我希望的那样，话语还是传达给了她。", "", ""},
        {"", "如果这多少给了她短暂的慰藉，那所有的一切就不是徒劳。", "", ""},
        {"", "······", "", ""},
        {"", "里面除了原本的曲子外，还加了别的曲子······", "", ""},
        {"", "这不是三年前我们开始生活后创作的曲子吗。", "", ""},
        {"", "我有这种感觉。", "", ""}
    };

    textIndex = qBound(0, startIndex, story.size() - 1);

    backgroundLabel = new QLabel(this);
    backgroundLabel->setGeometry(0, 0, width(), height());
    backgroundLabel->setScaledContents(true);
    backgroundLabel->lower();

    nextBackgroundLabel = new QLabel(this);
    nextBackgroundLabel->setGeometry(0, 0, width(), height());
    nextBackgroundLabel->setScaledContents(true);
    nextBackgroundLabel->hide();

    backgroundEffect = new QGraphicsOpacityEffect(nextBackgroundLabel);
    nextBackgroundLabel->setGraphicsEffect(backgroundEffect);
    backgroundEffect->setOpacity(0.0);

    backgroundAnimation = new QPropertyAnimation(backgroundEffect, "opacity", this);

    textBoxLabel = new QLabel(this);
    textBoxLabel->setGeometry(0, 520, width(), 192);
    textBoxLabel->setScaledContents(true);
    QString textBoxPath = findAssetFile("assets/picture/MAIN_BASE00.png");
    if (!textBoxPath.isEmpty()) {
        textBoxLabel->setPixmap(QPixmap(textBoxPath));
    } else {
        textBoxLabel->setStyleSheet("background:white;");
    }

    nameLabel = new QLabel(this);
    nameLabel->setGeometry(145, 560, 170, 30);
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setStyleSheet("background:transparent; color:#333; font-size:18px; font-weight:bold;");

    textLabel = new QLabel(this);
    textLabel->setGeometry(190, 584, 900, 76);
    textLabel->setWordWrap(true);
    textLabel->setStyleSheet("background:transparent; color:black; font-size:20px;");

    saveButton = new QPushButton("Save", this);
    saveButton->setGeometry(90, 650, 80, 30);

    skipButton = new QPushButton("Skip", this);
    skipButton->setGeometry(180, 650, 80, 30);

    backlogButton = new QPushButton("Backlog", this);
    backlogButton->setGeometry(270, 650, 90, 30);

    menuButton = new QPushButton("Menu", this);
    menuButton->setGeometry(1110, 650, 80, 30);

    connect(saveButton, &QPushButton::clicked, this, &Scene::saveGame);
    connect(skipButton, &QPushButton::clicked, this, &Scene::skipText);
    connect(backlogButton, &QPushButton::clicked, this, &Scene::showBacklog);
    connect(menuButton, &QPushButton::clicked, this, &Scene::backToMenu);

    AppConfig config = loadConfig();
    voiceOutput = new QAudioOutput(this);
    voiceOutput->setVolume(config.voiceVolume / 100.0);
    voicePlayer = new QMediaPlayer(this);
    voicePlayer->setAudioOutput(voiceOutput);

    skipTimer = new QTimer(this);
    connect(skipTimer, &QTimer::timeout, this, [this]() {
        advanceText();
    });

    showCurrentText();
}

void Scene::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    if (currentBackground.isEmpty()) {
        painter.fillRect(rect(), QColor(190, 215, 235));
    }
}

void Scene::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }

    advanceText();
}

void Scene::advanceText()
{
    textIndex++;
    if (textIndex >= story.size()) {
        if (skipTimer->isActive()) {
            skipTimer->stop();
        }
        voicePlayer->stop();
        Ending *ending = new Ending;
        ending->show();
        close();
        return;
    }

    showCurrentText();
}

void Scene::showCurrentText()
{
    const StoryItem &item = story.at(textIndex);
    qDebug() << "Current story:" << item;

    if (!item.background.isEmpty()) {
        changeBackground(item.background);
    }

    nameLabel->setText(item.name.isEmpty() ? " " : item.name);
    textLabel->setText(item.text);

    QString oneLine = item.text;
    if (backlog.isEmpty() || backlog.last() != oneLine) {
        backlog << oneLine;
    }

    voicePlayer->stop();
    if (!skipMode && !item.voice.isEmpty()) {
        QString voicePath = findAssetFile("assets/voice/" + item.voice);
        if (!voicePath.isEmpty()) {
            voicePlayer->setSource(QUrl::fromLocalFile(voicePath));
            voicePlayer->play();
        }
    }
}

void Scene::changeBackground(const QString &fileName)
{
    if (fileName == currentBackground) {
        return;
    }

    QString path = findAssetFile("assets/picture/" + fileName);
    if (path.isEmpty()) {
        return;
    }

    if (currentBackground.isEmpty()) {
        currentBackground = fileName;
        backgroundLabel->setPixmap(QPixmap(path));
        return;
    }

    backgroundAnimation->stop();
    nextBackgroundLabel->setPixmap(QPixmap(path));
    nextBackgroundLabel->show();
    nextBackgroundLabel->raise();
    backgroundEffect->setOpacity(0.0);

    textBoxLabel->raise();
    nameLabel->raise();
    textLabel->raise();
    saveButton->raise();
    skipButton->raise();
    backlogButton->raise();
    menuButton->raise();

    backgroundAnimation->disconnect();
    backgroundAnimation->setDuration(1200);
    backgroundAnimation->setStartValue(0.0);
    backgroundAnimation->setEndValue(1.0);

    connect(backgroundAnimation, &QPropertyAnimation::finished, this, [this, fileName, path]() {
        currentBackground = fileName;
        backgroundLabel->setPixmap(QPixmap(path));
        nextBackgroundLabel->hide();
        backgroundEffect->setOpacity(0.0);

        textBoxLabel->raise();
        nameLabel->raise();
        textLabel->raise();
        saveButton->raise();
        skipButton->raise();
        backlogButton->raise();
        menuButton->raise();
    });

    backgroundAnimation->start();
}

void Scene::skipText()
{
    skipMode = !skipMode;
    skipButton->setText(skipMode ? "Skip On" : "Skip");

    if (skipMode) {
        voicePlayer->stop();
        skipTimer->start(350);
    } else {
        skipTimer->stop();
    }
}

void Scene::showBacklog()
{
    if (skipMode) {
        skipMode = false;
        skipTimer->stop();
        skipButton->setText("Skip");
    }

    QMessageBox::information(this, "Backlog", backlog.join("\n\n"));
}

void Scene::saveGame()
{
    if (skipMode) {
        skipMode = false;
        skipTimer->stop();
        skipButton->setText("Skip");
    }

    savedIndex = textIndex;

    QFile file(saveFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Save", "存档失败：无法写入存档文件。");
        return;
    }

    QTextStream stream(&file);
    stream << savedIndex << "\n";
    file.close();

    QMessageBox::information(this, "Save", "已保存到存档位：" + QString::number(savedIndex + 1));
}

void Scene::backToMenu()
{
    skipTimer->stop();
    voicePlayer->stop();

    StartMenu *menu = new StartMenu(false);
    menu->show();
    close();
}
