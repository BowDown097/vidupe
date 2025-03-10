#ifndef VIDEO_H
#define VIDEO_H

#include <QDebug>               //generic includes go here as video.h is used by many files
#include <QRunnable>
#include <QProcess>
#include <QBuffer>
#include <QTemporaryDir>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "prefs.h"
#include "db.h"

class Video : public QObject, public QRunnable
{
    Q_OBJECT

public:
    Video(const Prefs &prefsParam, const QString &filenameParam, const QDateTime &dateMod);
    void run();

    QString filename;
    QString id;
    int64_t size = 0;
    QDateTime modified;
    int64_t duration = 0;
    int bitrate = 0;
    double framerate = 0;
    QString codec;
    QString audio;
    short width = 0;
    short height = 0;
    QByteArray thumbnail;
    cv::Mat grayThumb [16];
    uint64_t hash [16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    bool cachedMetadata = false;
    bool cachedCaptures = true;

    QImage captureAt(const int &percent, const int &ofDuration=100) const;

signals:
    void acceptVideo(Video *addMe);
    void rejectVideo(Video *deleteMe, const QString &reason);

private:
    static Prefs _prefs;
    static int _jpegQuality;

    enum class ScreenCaptureResult { Success, NoFrame, ResolutionMismatch, Exception };

    static constexpr int _okJpegQuality      = 60;
    static constexpr int _lowJpegQuality     = 25;
    static constexpr int _hugeAmountVideos   = 200000;
    static constexpr int _goBackwardsPercent = 6;       //if capture fails, retry but omit this much from end
    static constexpr int _videoStillUsable   = 90;      //90% of video duration is considered usable
    static constexpr int _thumbnailMaxWidth  = 448;     //small size to save memory and cache space
    static constexpr int _thumbnailMaxHeight = 336;
    static constexpr int _pHashSize          = 32;      //phash generated from 32x32 image
    static constexpr int _ssimSize           = 16;      //larger than 16x16 seems to have slower comparison
    static constexpr int _almostBlackBitmap  = 1500;    //monochrome thumbnail if less shades of gray than this

    uint64_t computePhash(const cv::Mat &input) const;
    QImage minimizeImage(const QImage &image) const;
    QString msToHHMMSS(const int64_t &time) const;

    void getMetadata(const QString &filename);
    ScreenCaptureResult takeScreenCaptures(const Db &cache);
    void processThumbnail(QImage &thumbnail, const int &hashes);
    void getBrightest(const QString &filename);
};

#endif // VIDEO_H
