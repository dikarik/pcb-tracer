#include "ImageLayer.h"
#include <QPixmap>
#include <QDebug>

ImageLayer::ImageLayer(int id) : QGraphicsPixmapItem(), m_id(id)
{
    setZValue(-1); // Устанавливаем слой позади других элементов
    setPos(0, 0);  // Позиционируем изображение в левом верхнем углу
    setOpacity(1); // Устанавливаем непрозрачность 100%
}

bool ImageLayer::loadImage(const QString &imagePath)
{
    m_imagePath = imagePath;

    // Загружаем изображение
    QPixmap pixmap;
    if (!pixmap.load(imagePath))
    {
        qDebug() << "Failed to load image:" << imagePath;
        return false;
    }

    // Устанавливаем изображение в слой
    if (!pixmap.isNull())
    {
        setPixmap(pixmap);
        return true;
    }
    else
    {
        qDebug() << "Failed to load image:" << imagePath;
        return false;
    }
}
