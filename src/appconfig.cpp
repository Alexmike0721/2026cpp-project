#include "appconfig.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>

static QString configFilePath()
{
    QDir dir(QCoreApplication::applicationDirPath() + "/../../../..");
    dir.mkpath("save");
    return dir.filePath("save/config.txt");
}

AppConfig loadConfig()
{
    AppConfig config;
    QFile file(configFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return config;
    }

    QTextStream stream(&file);
    stream >> config.masterVolume >> config.voiceVolume;

    if (config.masterVolume < 0 || config.masterVolume > 100) {
        config.masterVolume = 80;
    }
    if (config.voiceVolume < 0 || config.voiceVolume > 100) {
        config.voiceVolume = 80;
    }

    return config;
}

void saveConfig(const AppConfig &config)
{
    QDir dir(QCoreApplication::applicationDirPath() + "/../../../..");
    dir.mkpath("save");

    QFile file(configFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream stream(&file);
    stream << config.masterVolume << " " << config.voiceVolume << "\n";
}
