#ifndef PXLWRITER_H
#define PXLWRITER_H

#include <QPainter>


class PxlWriter
{
    QSize size { 0, 0 };
    uint  pixel_size { 0 };
    bool  black_background { true };
    QImage background {};

    void rebuild_background();

public:
    PxlWriter();

    void resize(const QSize& newSize);
    void setPixelSize(uint pixel_size_);
    void setBlackBackground(bool black_background_);

    QImage draw(const QImage& img);
};

#endif // PXLWRITER_H
