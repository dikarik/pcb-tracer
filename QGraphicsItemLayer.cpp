#include "QGraphicsItemLayer.h"
#include "Editor.h"

QGraphicsItemLayer::QGraphicsItemLayer(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    // Создаем эффект прозрачности и устанавливаем его для слоя
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    m_opacityEffect->setOpacity(1); // По умолчанию слой непрозрачный
    setGraphicsEffect(m_opacityEffect);
}

QRectF QGraphicsItemLayer::boundingRect() const
{
    // Возвращаем пустой прямоугольник, так как слой сам по себе не имеет размеров
    return QRectF(0, 0, 0, 0);
}

void QGraphicsItemLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Пустая реализация, так как слой сам по себе ничего не рисует
}

QPainterPath QGraphicsItemLayer::shape() const
{
    // Возвращаем пустой путь, так как слой не должен перехватывать события
    return QPainterPath();
}

void QGraphicsItemLayer::setOpacity(qreal opacity)
{
    // Устанавливаем прозрачность для всего слоя
    m_opacityEffect->setOpacity(opacity);
}