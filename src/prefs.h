#ifndef PREFS_H
#define PREFS_H

#include "thumbnail.h"

class Prefs
{
public:
    enum _modes { _PHASH, _SSIM };

    class MainWindow *_mainwPtr = nullptr;               //pointer to MainWindow, for connecting signals to it's slots

    int _comparisonMode = _PHASH;
    int _thumbnails = thumb12;
    int _numberOfVideos = 0;
    int _ssimBlockSize = 16;

    double _thresholdSSIM = 0.89;
    int _thresholdPhash = 57;

    double _thresholdSSIMMax = 100;
    int _thresholdPhashMax = 100;

    int _differentDurationModifier = 4;
    int _sameDurationModifier = 1;
    int _cacheLoadPageSize = 300;
};

#endif // PREFS_H
