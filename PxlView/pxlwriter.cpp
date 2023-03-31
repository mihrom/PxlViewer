#include "pxlwriter.h"


PxlWriter::PxlWriter()
{

}


void PxlWriter::rebuild_background()
{
    if(size.width() == 0 || size.height() == 0 )
        return;

    QColor bg_color1(200, 200, 200, 255);
    QColor bg_color2(220, 220, 220, 255);

    if(black_background){
        bg_color1 = QColor(0, 0, 0, 255);
        bg_color2 = QColor(20, 20, 20, 255);
    }

    QImage new_background(size*2, QImage::Format_RGBA8888);
    new_background.fill(bg_color1);

    QPainter fon_painter( &new_background );
    fon_painter.setBrush(QBrush(bg_color2, Qt::Dense4Pattern));
    fon_painter.drawRect(-1, -1, size.width()*2 + 1, size.height()*2 + 1);
    fon_painter.end();

    background = new_background.scaled(size*pixel_size, Qt::KeepAspectRatio);
}


void PxlWriter::resize(const QSize &newSize)
{
    size = newSize;
    rebuild_background();
}


void PxlWriter::setPixelSize(uint pixel_size_)
{
    pixel_size = pixel_size_;
    rebuild_background();
}


void PxlWriter::setBlackBackground(bool black_background_)
{
    black_background = black_background_;
    rebuild_background();
}


QImage PxlWriter::draw(const QImage &img)
{
    QImage dest = background;
    QPainter painter( &dest );
    painter.drawImage( 0, 0, img.scaled(size*pixel_size,Qt::KeepAspectRatio));
    painter.end();
    return dest;
}

