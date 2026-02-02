#include <QGraphicsScene>
#include <QPainter>
#include "Component.h"

int Component::s_componentCount = 0;

PhantomPad::PhantomPad(const QString &name, const QPointF &position) : Node(-1)
{
    m_name = name;
    setPos(position);
    setFlags(QGraphicsItem::ItemIsSelectable);
    setOpacity(1);
    m_showOnHover = false;
}

Pad::Pad(const QString &name, int id, const QPointF &position, int number) : Node(id), m_id(id), m_name(name), m_number(number)
{
    m_componentId = -1;
    setPos(position);
    setFlags(QGraphicsItem::ItemIsSelectable);
    m_showOnHover = false;
    setOpacity(1);
}

Component::Component(const QString &name, int id) : m_name(name), m_id(id) {}

int Component::numberOfPads() const
{
    return m_pads.size();
}

QString Component::toString() const
{
    return QString("Component %1 (ID: %2, Pads: %3)").arg(m_name).arg(m_id).arg(m_pads.size());
}

void Component::addPad(Pad *pad)
{
    pad->m_componentId = m_id;
    m_pads.push_back(pad);
}

void Component::addToScene(QGraphicsScene *scene)
{
    scene->addItem(this);
    for (size_t i = 0; i < m_pads.size(); ++i)
    {
        scene->addItem(m_pads[i]);
        m_pads[i]->setSide(LinkSide::NODE);
    }
}

QRectF Component::boundingRect() const
{
    // Если контактов нет, возвращаем пустой прямоугольник
    if (m_pads.empty())
    {
        return QRectF();
    }

    // Находим границы всех контактов
    qreal left = m_pads[0]->x();
    qreal top = m_pads[0]->y();
    qreal right = left;
    qreal bottom = top;

    for (const Pad *pad : m_pads)
    {
        qreal x = pad->x();
        qreal y = pad->y();
        if (x < left)
            left = x;
        if (x > right)
            right = x;
        if (y < top)
            top = y;
        if (y > bottom)
            bottom = y;
    }

    // Добавляем отступ
    qreal margin = 10.0;
    return QRectF(left - margin, top - margin,
                  right - left + 2 * margin,
                  bottom - top + 2 * margin);
}

void Component::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Получаем ограничивающий прямоугольник
    QRectF rect = boundingRect();

    // Рисуем контур компонента
    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(QBrush(Qt::lightGray, Qt::Dense4Pattern));
    painter->drawRect(rect);

    // Рисуем имя компонента
    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial", 8));
    painter->drawText(rect, Qt::AlignCenter, m_name);
}

void Component::remove(QGraphicsScene *scene)
{
    // Удаляем все контакты
    for (auto pad : m_pads)
    {
        scene->removeItem(pad);
    }
    m_pads.clear();

    // Удаляем сам компонент
    scene->removeItem(this);
}

int Component::genComponentId()
{
    return ++s_componentCount;
}

int Component::getLastComponentId()
{
    return s_componentCount;
}

void Component::setComponentCount(int count)
{
    if (count >= 0)
    {
        s_componentCount = count;
    }
    else
    {
        throw std::invalid_argument("component_count must be a non-negative integer");
    }
}