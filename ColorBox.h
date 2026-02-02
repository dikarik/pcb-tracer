#pragma once

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include "Link.h"

/**
 * @brief Класс виджета цветовой индикации
 *
 * ColorBox отображает текущую сторону платы и соответствующий ей цвет.
 */
class ColorBox : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор виджета цветовой индикации
     */
    ColorBox();

    /**
     * @brief Устанавливает сторону платы для отображения
     * @param side Сторона платы (FRONT, BACK, WIP, NOTES)
     */
    void setSide(LinkSide side);

private:
    QFrame *m_colorFrame; ///< Рамка для отображения цвета
    QLabel *m_textLabel;  ///< Метка для отображения текста
};