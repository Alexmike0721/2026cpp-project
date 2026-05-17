#ifndef APPCONFIG_H
#define APPCONFIG_H

struct AppConfig
{
    int masterVolume = 80;
    int voiceVolume = 80;
};

AppConfig loadConfig();
void saveConfig(const AppConfig &config);

#endif
