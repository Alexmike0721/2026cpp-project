#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>

class QLabel;
class QSlider;

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void saveAndBack();
    void backToMenu();

private:
    QLabel *masterValueLabel = nullptr;
    QLabel *voiceValueLabel = nullptr;
    QSlider *masterSlider = nullptr;
    QSlider *voiceSlider = nullptr;
};

#endif
