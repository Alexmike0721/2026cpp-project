#ifndef CGGALLERY_H
#define CGGALLERY_H

#include <QWidget>
#include <QStringList>

class QLabel;
class QPushButton;

class CgGallery : public QWidget
{
    Q_OBJECT

public:
    explicit CgGallery(int unlockedCount, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void showCg(int index);
    void closePreview();
    void backToMenu();

private:
    QPushButton *createThumbButton(int index, int x, int y);

    QStringList cgFiles;
    int unlockedCount = 0;
    QLabel *previewLabel = nullptr;
};

#endif
