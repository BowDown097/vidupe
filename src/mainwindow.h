#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDragEnterEvent>
#include <QMimeData>
#include "ui_mainwindow.h"
#include "video.h"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow() { deleteTemporaryFiles(); delete ui; }

private:
    Ui::MainWindow *ui;

    QVector<Video *> _videoList;
    QHash<QString, Video *> _everyVideo;
    QStringList _rejectedVideos;
    QStringList _extensionList;

    Prefs _prefs;
    bool _userPressedStop = false;
    QString _previousRunFolders;
    int _previousRunThumbnails = -1;

    void deleteTemporaryFiles() const;
    bool detectffmpeg() const;

    void loadExtensions();
    void loadLocations();

    void calculateThreshold(const int &value);

    void findVideos(QDir &dir);
    void processVideos();
    void videoSummary();

    void closeEvent(QCloseEvent *event) { Q_UNUSED (event) _userPressedStop = true; }
    void dragEnterEvent(QDragEnterEvent *event) { if(event->mimeData()->hasUrls()) event->acceptProposedAction(); }
    void dropEvent(QDropEvent *event);

public slots:
    void addStatusMessage(const QString &message) const;
    void addVideo(Video *addMe);
    void removeVideo(Video *deleteMe, const QString &reason);
    void setComparisonMode(const int &mode) { if(mode == _prefs._PHASH) ui->selectPhash->click(); else ui->selectSSIM->click(); ui->directoryBox->setFocus(); }
    void on_thresholdSlider_valueChanged(const int &value) { ui->thresholdSlider->setValue(value); calculateThreshold(value); ui->directoryBox->setFocus(); }
    void on_thresholdSliderMax_valueChanged(const int &value) { ui->thresholdSlider->setMaximum(value); }

private slots:
    void on_selectThumbnails_activated(const int &index) { ui->directoryBox->setFocus(); _prefs._thumbnails = index;
                                                           if(_prefs._thumbnails == cutEnds) ui->differentDurationCombo->setCurrentIndex(0); }
    void on_selectPhash_clicked(const bool &checked) { if(checked) _prefs._comparisonMode = _prefs._PHASH; ui->directoryBox->setFocus(); }
    void on_selectSSIM_clicked(const bool &checked) { if(checked) _prefs._comparisonMode = _prefs._SSIM; ui->directoryBox->setFocus(); }
    void on_blocksizeCombo_activated(const int &index) { _prefs._ssimBlockSize = static_cast<int>(pow(2, index+1)); ui->directoryBox->setFocus(); }
    void on_differentDurationCombo_activated(const int &index) { _prefs._differentDurationModifier = index; ui->directoryBox->setFocus(); }
    void on_sameDurationCombo_activated(const int &index) { _prefs._sameDurationModifier = index; ui->directoryBox->setFocus(); }

    void on_browseFolders_clicked() const;
    void on_directoryBox_returnPressed() { on_findDuplicates_clicked(); }
    void on_findDuplicates_clicked();
};

#endif // MAINWINDOW_H
