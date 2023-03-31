#include <fstream>
#include <string>

#include "pxlfile.h"

using namespace std;


PxlFile::PxlFile()
{
}


size_t PxlFile::xStripPos(size_t pos) const
{
    switch(header.format.strip){
    case (STRIP_ZIGZAG):
        return pos / header.size_y;
    case (STRIP_LINE):
    case (STRIP_X01):
    default:
        return pos % header.size_x;
    }
}


size_t PxlFile::yStripPos(size_t pos) const
{
    switch(header.format.strip){
    case (STRIP_ZIGZAG):
        if((pos / header.size_y) % 2 == 0)
            return pos % header.size_y;
        else
            return header.size_y - (pos % header.size_y) - 1;
    case (STRIP_LINE):
    case (STRIP_X01):
    default:
        return pos / header.size_x;
    }
}


file_pixel_t PxlFile::convertColor(uint16_t skip, QColor c) const
{
    uint8_t r = c.red();
    uint8_t g = c.green();
    uint8_t b = c.blue();
    uint8_t a = c.alpha();

    switch(header.format.color){
    case(COLOR_RGB):
        return file_pixel_t{skip, r, g, b, 255};
    case(COLOR_GRBA):
        return file_pixel_t{skip, g, r, b, a};
    case(COLOR_RGBA):
    default:
        return file_pixel_t{skip, r, g, b, a};
    }
}


QColor PxlFile::convertColor(file_pixel_t c) const
{
    switch(header.format.color){
    case(COLOR_RGB):
        return QColor(c.color1, c.color2, c.color3, 255);
    case(COLOR_GRBA):
        return QColor(c.color2, c.color3, c.color1, c.color4);
    case(COLOR_RGBA):
    default:
        return QColor(c.color1, c.color2, c.color3, c.color4);
    }
}


bool PxlFile::loadPxlFile(string file_name)
{
    empty = true;

    ifstream fin(file_name, ios::binary);

    fin.read((char*)&header, sizeof (header));

    if(string(header.type, 3) != "PXL")
        return false;

    frames.resize(header.img_frames);

    for(auto& f: frames) {
        fin.read(reinterpret_cast<char*>(&(f.header)), sizeof(file_frame_t));
        f.pixels.resize(f.header.frame_pixels);
        fin.read(reinterpret_cast<char*>(f.pixels.data()), sizeof(file_pixel_t)*f.header.frame_pixels);
    }

    empty = false;

    return true;
}


void PxlFile::savePxlFile(string file_name) const
{
    ofstream oin(file_name, ios::binary);
    oin.write((char*)&header, sizeof (header));

    for(const auto& f: frames) {
        oin.write(reinterpret_cast<const char*>(&(f.header)), sizeof(file_frame_t));
        oin.write(reinterpret_cast<const char*>(f.pixels.data()), sizeof(file_pixel_t)*f.header.frame_pixels);
    }
}


void PxlFile::importFramesFromImage(QImage img, uint16_t frames_timeout)
{
    header.type[0] = 'P';
    header.type[1] = 'X';
    header.type[2] = 'L';

    header.version = 0x01;

    frames.clear();
    frames.resize(header.img_frames);

    size_t pixel_count = header.size_x * header.size_y;

    for(int i=0; i<header.img_frames; ++i)
    {
        frames[i].header.frame_pixels = pixel_count;
        frames[i].header.frame_timeout = frames_timeout;

        size_t frame_shift = i * header.size_y;
        uint16_t skip=0;

        for(size_t pos=0; pos < pixel_count; ++pos)
        {
            QColor c = img.pixelColor(xStripPos(pos), yStripPos(pos) + frame_shift);
            uint8_t a = c.alpha();
            if(a == 0) {
                skip++;
            }
            else {
                frames[i].pixels.emplace_back(convertColor(skip, c));
                skip = 0;
            }
        }

        frames[i].header.frame_pixels = frames[i].pixels.size();
    }

    empty = false;
}


QImage PxlFile::frameToImage(uint16_t frame_number) const
{
    if(empty)
        return QImage(0,0, QImage::Format_RGBA8888);

    QImage img (header.size_x, header.size_y, QImage::Format_RGBA8888);
    img.fill(QColor(0, 0, 0, 0));

    const auto& pixels = frames[frame_number].pixels;
    size_t frame_pixels = frames[frame_number].header.frame_pixels;
    size_t pos = 0;

    for(size_t i=0; i<frame_pixels; ++i) {
        pos += pixels[i].skip;

        img.setPixelColor(
            xStripPos(pos),
            yStripPos(pos),
            convertColor(pixels[i])
        );

        ++pos;
    }

    return img;
}


void PxlFile::setSize(uint8_t width, uint8_t height)
{
    header.size_x = width;
    header.size_y = height;
}


void PxlFile::setStrip(format_strip_t strip, format_color_t color)
{
    header.format.strip = strip;
    header.format.color = color;
}


void PxlFile::setFramesData(uint16_t frames_count, uint8_t repeats)
{
    header.img_frames = frames_count;
    header.img_repeats = repeats;
}


QSize PxlFile::getSize() const
{
    if(empty)
        return { 0, 0 };

    return { header.size_x, header.size_y };
}


uint16_t PxlFile::getFramesCount() const
{
    return empty ? 0 : frames.size();
}


uint8_t PxlFile::getRepeats() const
{
    return empty ? 0 : header.img_repeats;
}


bool PxlFile::isEmpty() const
{
    return empty;
}

vector<uint16_t> PxlFile::getFramesTimeouts() const
{
    vector<uint16_t> timeouts;

    for(const auto& f: frames)
        timeouts.push_back(f.header.frame_timeout);

    return timeouts;
}

