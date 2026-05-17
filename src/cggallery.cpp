#include "cggallery.h"
#include "startmenu.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>

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

CgGallery::CgGallery(int unlockedCount, QWidget *parent)
    : QWidget(parent), unlockedCount(unlockedCount)
{
    setWindowTitle("CG Gallery");
    resize(1280, 720);

    cgFiles << "EV002A.png"
            << "EV007A.png"
            << "EV119B.png"
            << "EV120A.png"
            << "EV120B.png"
            << "EV122A.png";

    int startX = 180;
    int startY = 150;
    int thumbW = 280;
    int thumbH = 158;
    int gapX = 70;
    int gapY = 80;

    for (int i = 0; i < cgFiles.size(); ++i) {
        int row = i / 3;
        int col = i % 3;
        createThumbButton(i, startX + col * (thumbW + gapX), startY + row * (thumbH + gapY));
    }

    QPushButton *backButton = new QPushButton("Back", this);
    backButton->setGeometry(40, 34, 110, 36);
    connect(backButton, &QPushButton::clicked, this, &CgGallery::backToMenu);

    previewLabel = new QLabel(this);
    previewLabel->setGeometry(0, 0, width(), height());
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setStyleSheet("background:rgba(0,0,0,220);");
    previewLabel->hide();
}

QPushButton *CgGallery::createThumbButton(int index, int x, int y)
{
    QPushButton *button = new QPushButton(this);
    button->setGeometry(x, y, 280, 158);

    QString imageName = index < unlockedCount ? cgFiles.at(index) : "CG_SECRET.png";
    QString path = findAssetFile("assets/picture/" + imageName);
    if (!path.isEmpty()) {
        QPixmap pix(path);
        button->setIcon(QIcon(pix));
        button->setIconSize(button->size());
    }

    button->setStyleSheet(
        "QPushButton { border:2px solid rgba(255,255,255,210); background:white; }"
        "QPushButton:hover { border:3px solid white; }"
    );

    connect(button, &QPushButton::clicked, this, [this, index]() {
        showCg(index);
    });
    return button;
}

void CgGallery::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QString path = findAssetFile("assets/picture/CG_BASE.png");
    if (!path.isEmpty()) {
        painter.drawPixmap(rect(), QPixmap(path));
    } else {
        painter.fillRect(rect(), QColor(240, 240, 240));
    }
}

void CgGallery::showCg(int index)
{
    if (index >= unlockedCount) {
        return;
    }

    QString path = findAssetFile("assets/picture/" + cgFiles.at(index));
    if (path.isEmpty()) {
        return;
    }

    previewLabel->setPixmap(QPixmap(path).scaled(previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    previewLabel->show();
    previewLabel->raise();
}

void CgGallery::closePreview()
{
    previewLabel->hide();
}

void CgGallery::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    if (previewLabel->isVisible()) {
        closePreview();
    }
}

void CgGallery::backToMenu()
{
    StartMenu *menu = new StartMenu(false);
    menu->show();
    close();
}
