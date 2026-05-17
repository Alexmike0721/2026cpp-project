#include "settingswindow.h"
#include "appconfig.h"
#include "startmenu.h"

#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QSlider>

SettingsWindow::SettingsWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Settings");
    resize(1280, 720);

    AppConfig config = loadConfig();

    QLabel *titleLabel = new QLabel("Settings", this);
    titleLabel->setGeometry(140, 105, 300, 50);
    titleLabel->setStyleSheet("font-size:34px; color:#333;");

    QLabel *masterLabel = new QLabel("Master Volume", this);
    masterLabel->setGeometry(220, 245, 220, 36);
    masterLabel->setStyleSheet("font-size:22px; color:#333;");

    masterSlider = new QSlider(Qt::Horizontal, this);
    masterSlider->setGeometry(450, 250, 480, 28);
    masterSlider->setRange(0, 100);
    masterSlider->setValue(config.masterVolume);

    masterValueLabel = new QLabel(QString::number(config.masterVolume), this);
    masterValueLabel->setGeometry(960, 245, 80, 36);
    masterValueLabel->setAlignment(Qt::AlignCenter);
    masterValueLabel->setStyleSheet("font-size:20px; color:#333;");

    QLabel *voiceLabel = new QLabel("Voice Volume", this);
    voiceLabel->setGeometry(220, 335, 220, 36);
    voiceLabel->setStyleSheet("font-size:22px; color:#333;");

    voiceSlider = new QSlider(Qt::Horizontal, this);
    voiceSlider->setGeometry(450, 340, 480, 28);
    voiceSlider->setRange(0, 100);
    voiceSlider->setValue(config.voiceVolume);

    voiceValueLabel = new QLabel(QString::number(config.voiceVolume), this);
    voiceValueLabel->setGeometry(960, 335, 80, 36);
    voiceValueLabel->setAlignment(Qt::AlignCenter);
    voiceValueLabel->setStyleSheet("font-size:20px; color:#333;");

    QPushButton *saveButton = new QPushButton("Save", this);
    saveButton->setGeometry(790, 520, 120, 40);

    QPushButton *backButton = new QPushButton("Back", this);
    backButton->setGeometry(940, 520, 120, 40);

    connect(masterSlider, &QSlider::valueChanged, this, [this](int value) {
        masterValueLabel->setText(QString::number(value));
    });
    connect(voiceSlider, &QSlider::valueChanged, this, [this](int value) {
        voiceValueLabel->setText(QString::number(value));
    });
    connect(saveButton, &QPushButton::clicked, this, &SettingsWindow::saveAndBack);
    connect(backButton, &QPushButton::clicked, this, &SettingsWindow::backToMenu);
}

void SettingsWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.fillRect(rect(), QColor(238, 238, 238));
    painter.setBrush(QColor(255, 255, 255, 210));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(120, 90, 1040, 530, 18, 18);
}

void SettingsWindow::saveAndBack()
{
    AppConfig config;
    config.masterVolume = masterSlider->value();
    config.voiceVolume = voiceSlider->value();
    saveConfig(config);
    backToMenu();
}

void SettingsWindow::backToMenu()
{
    StartMenu *menu = new StartMenu(false);
    menu->show();
    close();
}
