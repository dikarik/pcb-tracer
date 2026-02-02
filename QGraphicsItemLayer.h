#ifndef QGRAPHICSITEMLAYER_H
#define QGRAPHICSITEMLAYER_H

#include <QGraphicsItem>
#include <QGraphicsOpacityEffect>
#include <QPainterPath>

/**
 * @brief Класс слоя графических элементов
 *
 * QGraphicsItemLayer представляет собой слой для группировки графических элементов
 * с возможностью управления прозрачностью всего слоя.
 */
class QGraphicsItemLayer : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    /**
     * @brief Конструктор слоя графических элементов
     * @param parent Родительский элемент (опционально)
     */
    QGraphicsItemLayer(QGraphicsItem *parent = nullptr);

    /**
     * @brief Возвращает ограничивающий прямоугольник слоя
     * @return Ограничивающий прямоугольник
     */
    QRectF boundingRect() const override;

    /**
     * @brief Отрисовывает слой
     * @param painter Указатель на рисовальщика
     * @param option Указатель на опции стиля
     * @param widget Указатель на виджет (опционально)
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    /**
     * @brief Возвращает форму слоя для обработки событий
     * @return Путь формы слоя
     */
    QPainterPath shape() const override;

    /**
     * @brief Устанавливает прозрачность слоя
     * @param opacity Значение прозрачности (0.0 - полностью прозрачный, 1.0 - непрозрачный)
     */
    void setOpacity(qreal opacity);

private:
    QGraphicsOpacityEffect *m_opacityEffect; ///< Эффект прозрачности для слоя
};

#endif // QGRAPHICSITEMLAYER_H