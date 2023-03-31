#ifndef PXLFILE_H
#define PXLFILE_H

#include <vector>
#include <QPixmap>

#include "PixelGenFormat.h"


struct Frame
{
    file_frame_t header;
    std::vector <file_pixel_t> pixels;
};


class PxlFile
{
    // PXL file data
    file_header_t header;
    std::vector <Frame> frames;
    bool empty { true };

    // convertors to strip format
    size_t xStripPos(size_t pos) const;
    size_t yStripPos(size_t pos) const;

    // convertors to color format
    file_pixel_t convertColor(uint16_t skip, QColor c) const;
    QColor convertColor(file_pixel_t c) const;

public:
    PxlFile();

    // Load & save
    bool loadPxlFile(std::string file_name);
    void savePxlFile(std::string file_name) const;

    // Image convertors
    void importFramesFromImage(QImage img, uint16_t frames_timeout);
    QImage frameToImage(uint16_t frame_number) const;

    // Setters
    void setSize(uint8_t width, uint8_t height);
    void setStrip(format_strip_t strip, format_color_t color);
    void setFramesData(uint16_t frames_count, uint8_t repeats);

    // Getters
    QSize    getSize() const;
    uint16_t getFramesCount() const;
    uint8_t  getRepeats() const;
    bool     isEmpty() const;
    std::vector<uint16_t> getFramesTimeouts() const;

};

#endif // PXLFILE_H
