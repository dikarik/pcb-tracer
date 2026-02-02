#ifndef IMAGELAYER_H
#define IMAGELAYER_H

#include <QGraphicsPixmapItem>
#include <QString>

/**
 * @brief Класс слоя изображения
 *
 * ImageLayer представляет собой слой изображения на печатной плате,
 * который может содержать изображение лицевой или обратной стороны платы.
 */
class ImageLayer : public QGraphicsPixmapItem
{
public:
    /**
     * @brief Конструктор слоя изображения
     * @param id Уникальный идентификатор слоя
     */
    explicit ImageLayer(int id);

    /**
     * @brief Загружает изображение в слой
     * @param imagePath Путь к файлу изображения
     * @return true если изображение успешно загружено, false в противном случае
     */
    bool loadImage(const QString &imagePath);

    int m_id;            ///< Уникальный идентификатор слоя
    QString m_imagePath; ///< Путь к файлу изображения
};

#endif // IMAGELAYER_H