#ifndef COMPARISON_H
#define COMPARISON_H

#include <QDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QLabel>
#include "video.h"

namespace Ui { class Comparison; }

class Comparison : public QDialog
{
    Q_OBJECT

public:
    Comparison(const QVector<Video *> &videosParam, const Prefs &prefsParam);
    ~Comparison();

    void reportMatchingVideos();

private:
    Ui::Comparison *ui;

    QVector<Video *> _videos;
    Prefs _prefs;
    int _leftVideo = 0;
    int _rightVideo = 0;
    int _videosDeleted = 0;
    int64_t _spaceSaved = 0;
    bool _seekForwards = true;

    int _durationModifier = 0;
    int _phashSimilarity = 0;
    double _ssimSimilarity = 0.0;

    int _zoomLevel = 0;
    QPixmap _leftZoomed;
    int _leftW = 0;
    int _leftH = 0;
    QPixmap _rightZoomed;
    int _rightW = 0;
    int _rightH = 0;

    void confirmToExit();
    bool bothVideosMatch(const Video *left, const Video *right);
    int phashSimilarity(const Video *left, const Video *right, const int &leftHash, const int &rightHash);

    void showVideo(const QString &side) const;

    QString readableDuration(const int64_t &milliseconds) const;
    QString readableFileSize(const int64_t &filesize) const;
    QString readableBitRate(const double &kbps) const;

    int comparisonsSoFar() const;
    void highlightBetterProperties() const;
    void updateUI();

    void openFileManager(const QString &filename) const;

    void deleteVideo(const int &side);

    void moveVideo(const QString &from, const QString &to);

    double sigma(const cv::Mat &m, const int &i, const int &j, const int &block_size) const;
    double covariance(const cv::Mat &m0, const cv::Mat &m1, const int &i, const int &j, const int &block_size) const;
    double ssim(const cv::Mat &m0, const cv::Mat &m1, const int &block_size) const;

    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);

private slots:
    void on_prevVideo_clicked();
    void on_nextVideo_clicked();

    void on_selectPhash_clicked ( const bool &checked) { if(checked) _prefs._comparisonMode = _prefs._PHASH;
                                                         emit switchComparisonMode(_prefs._comparisonMode); }
    void on_selectSSIM_clicked ( const bool &checked) { if(checked) _prefs._comparisonMode = _prefs._SSIM;
                                                        emit switchComparisonMode(_prefs._comparisonMode); }

    void on_leftImage_clicked() { QDesktopServices::openUrl(QUrl::fromLocalFile(_videos[_leftVideo]->filename)); }
    void on_rightImage_clicked() { QDesktopServices::openUrl(QUrl::fromLocalFile(_videos[_rightVideo]->filename)); }

    void on_leftFileName_clicked() { openFileManager(_videos[_leftVideo]->filename); }
    void on_rightFileName_clicked() { openFileManager(_videos[_rightVideo]->filename); }

    void on_leftDelete_clicked() { deleteVideo(_leftVideo); }
    void on_rightDelete_clicked() { deleteVideo(_rightVideo); }

    void on_leftMove_clicked() { moveVideo(_videos[_leftVideo]->filename, _videos[_rightVideo]->filename); }
    void on_rightMove_clicked() { moveVideo(_videos[_rightVideo]->filename, _videos[_leftVideo]->filename); }
    void on_swapFilenames_clicked() const;
    void on_swapFolders_clicked() const;
    void on_swapFilesToFolders_clicked() const;


    void on_thresholdSlider_valueChanged(const int &value);
    void on_thresholdSliderMax_valueChanged(const int &value);

signals:
    void sendStatusMessage(const QString &message);
    void switchComparisonMode(const int &mode);
    void adjustThresholdSlider(const int &value);
    void adjustThresholdSliderMax(const int &value);

};


class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget *parent) { Q_UNUSED (parent) }
protected:
    void mousePressEvent(QMouseEvent *event) { Q_UNUSED (event) emit clicked(); }
signals:
    void clicked();
};

#endif // COMPARISON_H
