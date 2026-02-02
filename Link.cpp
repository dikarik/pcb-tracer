#include "Link.h"
#include "Node.h"
#include "Editor.h"
#include "Config.h"

/*
 * Статическая переменная TrackGraph::count - счетчик графов трассировки
 */
int TrackGraph::count = 0;

/*
 * Статическая переменная Link::link_count - счетчик связей
 */
int Link::link_count = 0;

/*
 * Функция Link::Link - конструктор связи
 * Входные параметры:
 *   id - идентификатор связи
 * Выходные данные:
 *   отсутствуют
 */
Link::Link(int id) : QGraphicsLineItem(), m_id(id), m_graphId(-1), m_my_from_node(nullptr), m_my_to_node(nullptr), m_side(LinkSide::FRONT)
{
    Editor *editor = Editor::instance();

    m_text_item = new QGraphicsSimpleTextItem(QString::number(m_graphId), this);
    m_text_item->setBrush(QColor("#FFFFFF"));
    m_text_item->setZValue(1);

    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    m_text_item->setFont(font);
    m_text_item->setVisible(false);
    editor->scene()->addItem(m_text_item);
    updateTextPosition();
}

/*
 * Функция Link::~Link - деструктор связи
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   отсутствуют
 */
Link::~Link()
{
    delete m_text_item;
}

/*
 * Функция Link::updateTextPosition - обновление позиции текста
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   отсутствуют
 */
void Link::updateTextPosition()
{
    QLineF line = this->line();
    qreal line_length = line.length();
    qreal text_width = m_text_item->boundingRect().width();
    qreal text_x = line.pointAt(0.5).x() - text_width / 2;
    qreal text_y = line.pointAt(0.5).y() - m_text_item->boundingRect().height() / 2;
    m_text_item->setPos(text_x, text_y);
    m_text_item->setOpacity(0);
}

/*
 * Функция Link::setGraphId - установка ID графа
 * Входные параметры:
 *   graphId - ID графа
 * Выходные данные:
 *   отсутствуют
 */
void Link::setGraphId(int graphId)
{
    m_graphId = graphId;
    m_text_item->setText(QString::number(graphId));
}

/*
 * Функция Link::setFromNode - установка начального узла
 * Входные параметры:
 *   node - указатель на узел
 * Выходные данные:
 *   отсутствуют
 */
void Link::setFromNode(Node *node)
{
    m_my_from_node = node;
    m_my_from_node->addLink(this);
    trackNodes();
}

/*
 * Функция Link::setToNode - установка конечного узла
 * Входные параметры:
 *   node - указатель на узел
 * Выходные данные:
 *   отсутствуют
 */
void Link::setToNode(Node *node)
{
    m_my_to_node = node;
    m_my_to_node->addLink(this);
    trackNodes();
}

/*
 * Функция Link::fromNode - получение начального узла
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   Node* - указатель на начальный узел
 */
Node *Link::fromNode() const
{
    return m_my_from_node;
}

/*
 * Функция Link::toNode - получение конечного узла
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   Node* - указатель на конечный узел
 */
Node *Link::toNode() const
{
    return m_my_to_node;
}

/*
 * Функция Link::setSide - установка стороны
 * Входные параметры:
 *   side - сторона
 * Выходные данные:
 *   отсутствуют
 */
void Link::setSide(LinkSide side)
{
    m_side = side;
    qDebug() << "Link::setSide: " << LinkSideUtils::toString(side);
    setParentItem(Editor::instance()->m_layers[side]);
    trackNodes();
    refresh();
}

/*
 * Функция Link::remove - удаление связи
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   отсутствуют
 */
void Link::remove()
{
    if (m_my_from_node)
    {
        m_my_from_node->removeLinkById(m_id);
    }
    if (m_my_to_node)
    {
        m_my_to_node->removeLinkById(m_id);
    }
    Editor::instance()->getScene()->removeItem(this);
    Editor::instance()->getScene()->removeItem(m_text_item);
}

/*
 * Функция Link::setColor - установка цвета
 * Входные параметры:
 *   color - цвет
 * Выходные данные:
 *   отсутствуют
 */
void Link::setColor(Color color)
{
    int width = m_width.has_value() ? m_width.value() : Config::instance()->m_linkWidth;
    QPen pen(QColor(Config::instance()->color(color)), width);
    pen.setCapStyle(Qt::RoundCap);
    setPen(pen);
}

/*
 * Функция Link::color - получение цвета
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   QColor - цвет
 */
QColor Link::color() const
{
    return pen().color();
}

/*
 * Функция Link::trackNodes - отслеживание узлов
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   отсутствуют
 */
void Link::trackNodes()
{
    if (m_my_from_node && m_my_to_node)
    {
        setLine(QLineF(m_my_from_node->pos(), m_my_to_node->pos()));
        // qDebug() << "trackNodes: from=" << m_my_from_node->pos() << ", to=" << m_my_to_node->pos();
        updateTextPosition();
    }
}

/*
 * Функция Link::refresh - обновление
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   отсутствуют
 */
void Link::refresh()
{
    // qDebug() << "Link::refresh, tiene side: " << LinkSideUtils::toString(m_side) << "usa color: " << Config::instance()->color(m_side);
    setColor(ColorUtils::fromLinkSide(m_side));
    if (m_my_from_node)
    {
        m_my_from_node->refresh();
    }
    if (m_my_to_node)
    {
        m_my_to_node->refresh();
    }
}

/*
 * Функция Link::setHighlighted - установка подсветки
 * Входные параметры:
 *   is_highlighted - флаг подсветки
 * Выходные данные:
 *   отсутствуют
 */
void Link::setHighlighted(bool is_highlighted)
{
    if (is_highlighted)
    {
        setColor(Color::HIGHLIGHTED);
    }
    else
    {
        refresh();
    }
}

/*
 * Функция Link::notifyLinkChanges - уведомление об изменениях
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   отсутствуют
 */
void Link::notifyLinkChanges()
{
    if (m_my_from_node)
    {
        m_my_from_node->notifyLinkChanges();
    }
    if (m_my_to_node)
    {
        m_my_to_node->notifyLinkChanges();
    }
}

/*
 * Функция Link::genLinkId - генерация ID связи
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   int - новый ID связи
 */
int Link::genLinkId()
{
    link_count++;
    return link_count;
}

/*
 * Функция Link::getLastLinkId - получение последнего ID связи
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   int - последний ID связи
 */
int Link::getLastLinkId()
{
    return link_count;
}

/*
 * Функция Link::setLinkCount - установка счетчика связей
 * Входные параметры:
 *   count - новое значение счетчика
 * Выходные данные:
 *   отсутствуют
 */
void Link::setLinkCount(int count)
{
    if (count >= 0)
    {
        link_count = count;
    }
    else
    {
        throw std::invalid_argument("link_count must be a non-negative integer");
    }
}

/*
 * Функция Link::updateTextItem - обновление текстового элемента
 * Входные параметры:
 *   text - текст
 * Выходные данные:
 *   отсутствуют
 */
void Link::updateTextItem(const QString &text)
{
    qDebug() << "Link::updateTextItem: " << text;
}
