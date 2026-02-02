#include "Node.h"
#include "Link.h"
#include "Editor.h"
#include "Component.h"
#include "actions/MoveNode.h"

int Node::node_count = 0;

Node::Node(int id) : QGraphicsEllipseItem(), m_id(id), m_size(0), m_side(LinkSide::FRONT), m_links()
{
    Editor *editor = Editor::instance();
    m_size = 20; // editor->padSize();

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);

    setOpacity(1);
    m_showOnHover = true;

    setColor(Color::NODE);

    if (m_showOnHover)
    {
        setZValue(0.1);
        setOpacity(0.001);
    }
    setAcceptHoverEvents(true);
}

void Node::notifyLinkChanges()
{
    // Проверяем, что это не Pad (так как Pad имеет собственную логику)
    if (typeid(*this).name() != typeid(Pad).name())
    {
        // Публикуем событие в зависимости от количества связей
        if (m_links.size() > 1)
        {
            CommunicationHub::instance().publish(HubEvent::NODE_MADE_MULTIPLE_LINKS, this);
        }
        else
        {
            CommunicationHub::instance().publish(HubEvent::NODE_MADE_SINGLE_LINK, this);
        }
    }
}

void Node::willBeDeleted()
{
    // Публикуем событие удаления узла
    CommunicationHub::instance().publish(HubEvent::NODE_DELETED, this);
}

void Node::addLink(Link *link)
{
    // Проверяем, что ссылка не нулевая
    if (link == nullptr)
    {
        return;
    }

    // Проверяем, что такая связь еще не добавлена
    if (std::find(m_links.begin(), m_links.end(), link) == m_links.end())
    {
        try
        {
            m_links.push_back(link);
        }
        catch (const std::exception &e)
        {
            qDebug() << "Exception caught while adding link:" << e.what();
        }
        catch (...)
        {
            qDebug() << "Unknown exception caught while adding link";
        }
    }
}

void Node::removeLink(Link *link)
{
    // Ищем связь в векторе и удаляем её
    auto it = std::find(m_links.begin(), m_links.end(), link);
    if (it != m_links.end())
    {
        m_links.erase(it);
    }
}

void Node::removeLinkById(int link_id)
{
    // Ищем связь по ID и удаляем её
    auto it = std::find_if(m_links.begin(), m_links.end(),
                           [link_id](const Link *link)
                           { return link->m_id == link_id; });
    if (it != m_links.end())
    {
        m_links.erase(it);
    }
}

void Node::setPosition(float x, float y)
{
    // Устанавливаем позицию и обновляем все связи
    setPos(x, y);
    updateLinks();
}

void Node::setPosition(QPointF &position)
{
    // Устанавливаем позицию через координаты точки
    setPosition(position.x(), position.y());
}

int Node::getGrade() const
{
    // Возвращаем количество связей (степень узла)
    return m_links.size();
}

std::vector<Link *> Node::getLinks() const
{
    // Возвращаем копию вектора связей
    return m_links;
}

void Node::updateLinks()
{
    // Обновляем позиции всех связей, подключенных к узлу
    for (Link *link : m_links)
    {
        link->trackNodes();
    }
}

bool Node::shouldReactToHover() const
{
    Editor *editor = Editor::instance();
    // Проверяем, есть ли связи на текущей стороне платы или в процессе работы
    for (const Link *link : m_links)
    {
        if (link->m_side == editor->m_currentSide || link->m_side == LinkSide::WIP)
        {
            return true;
        }
    }
    return false;
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    // При изменении позиции обновляем все связи
    if (change == QGraphicsItem::ItemPositionChange)
    {
        for (Link *link : m_links)
        {
            link->trackNodes();
        }
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}

void Node::enable()
{
    // Включаем возможность перемещения узла
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
}

void Node::disable()
{
    // Отключаем возможность перемещения узла
    setFlags(QGraphicsItem::GraphicsItemFlags(0));
}

void Node::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    // Для обычных узлов проверяем, должны ли они реагировать на наведение
    if (typeid(*this).name() == typeid(Node).name())
    {
        if (shouldReactToHover())
        {
            enable();
        }
        else
        {
            disable();
            return;
        }
    }

    // При наведении показываем узел и отображаем сообщение в статусной строке
    if (m_showOnHover)
    {
        setOpacity(1);
        Editor::instance()->showStatusMessage(QString("Node %1").arg(m_id));
    }

    // Для контактных площадок отображаем имя
    if (Pad *pad = dynamic_cast<Pad *>(this))
    {
        Editor::instance()->showStatusMessage(pad->m_name);
    }
}

void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    // При уходе мыши скрываем узел (если это нужно) и очищаем статусную строку
    if (m_showOnHover)
    {
        setOpacity(0.001);
        Editor::instance()->showStatusMessage("");
    }

    // Для контактных площадок также очищаем статусную строку
    if (typeid(*this).name() == typeid(Pad).name())
    {
        Editor::instance()->showStatusMessage("");
    }

    QGraphicsEllipseItem::hoverLeaveEvent(event);
}

QPainterPath Node::shape() const
{
    // Возвращаем расширенную форму для более удобного взаимодействия
    QPainterPath path = QGraphicsEllipseItem::shape();
    path.addEllipse(QRectF(-10, -10, 20, 20));
    return path;
}

void Node::setSide(LinkSide side)
{
    // Устанавливаем сторону платы и перемещаем узел в соответствующий слой
    m_side = side;
    setParentItem(Editor::instance()->m_layers[side]);
}

void Node::refresh()
{
    std::vector<LinkSide> sides;

    // Получаем список сторон, к которым подключены связи
    for (const Link *link : m_links)
    {
        sides.push_back(link->m_side);
    }

    // Для обычных узлов: если они соединяют несколько сторон,
    // отключаем эффект наведения и показываем всегда
    if (typeid(*this) == typeid(Node))
    {
        if (std::adjacent_find(sides.begin(), sides.end(), std::not_equal_to<>()) != sides.end())
        {
            m_showOnHover = false;
            setOpacity(1);
        }
        else
        {
            m_showOnHover = true;
            setOpacity(0.01);
        }
    }
}

void Node::setColor(Color color)
{
    // Устанавливаем размер узла
    int size = m_size.has_value() ? m_size.value() : Config::instance()->m_padSize;
    setRect(-size / 2, -size / 2, size, size);

    // Устанавливаем цвет контура и заливки
    setPen(QPen(QColor(Config::instance()->color(color)), 0));
    setBrush(QBrush(QColor(Config::instance()->color(color))));
}

int Node::genNodeId()
{
    // Генерируем уникальный ID для узла
    return ++node_count;
}

int Node::getLastNodeId()
{
    // Возвращаем последний использованный ID
    return node_count;
}

void Node::setNodeCount(int count)
{
    // Устанавливаем счетчик узлов с проверкой на отрицательное значение
    if (count >= 0)
    {
        node_count = count;
    }
    else
    {
        throw std::invalid_argument("node_count must be a non-negative integer");
    }
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Сохраняем начальную позицию для отслеживания перетаскивания
    m_drag_start_pos = pos();
    QGraphicsEllipseItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // При отпускании мыши проверяем, было ли перетаскивание
    if (m_drag_start_pos != QPointF())
    {
        QPointF drag_end_pos = pos();
        dragFinished(m_drag_start_pos, drag_end_pos);
        m_drag_start_pos = QPointF();
    }
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}

void Node::dragFinished(const QPointF &startPos, const QPointF &endPos)
{
    // Если позиция изменилась, создаем действие отмены/повтора
    if (startPos != endPos)
    {
        MoveNodeMeta meta{m_id, startPos, endPos};
        MoveNode *action = new MoveNode(meta);
        Editor::instance()->m_undoStack.push(action);
    }
}