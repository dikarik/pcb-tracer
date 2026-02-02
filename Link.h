#ifndef LINK_H
#define LINK_H

#include <QGraphicsLineItem>
#include <QGraphicsSimpleTextItem>
#include <QPen>
#include <QColor>
#include <QFont>
#include "Config.h"
#include "enums.h"

class Node;

/*
 * Класс TrackGraph - граф трассировки
 *
 * Основные функции:
 * 1. genTrackGraphId() - генерация ID графа трассировки
 * 2. setTrackGraphCount(int count) - установка счетчика графов трассировки
 */
class TrackGraph
{
public:
    static int count;

    static int genTrackGraphId()
    {
        count++;
        return count;
    }

    static void setTrackGraphCount(int count)
    {
        if (count >= 0)
        {
            TrackGraph::count = count;
        }
        else
        {
            throw std::invalid_argument("count must be a non-negative integer");
        }
    }
};

/*
 * Класс Link - связь между узлами
 *
 * Основные функции:
 * 1. Link(int id) - конструктор класса
 * 2. ~Link() - деструктор класса
 * 3. setGraphId(int graph_id) - установка ID графа
 * 4. setFromNode(Node* node) - установка начального узла
 * 5. setToNode(Node* node) - установка конечного узла
 * 6. fromNode() const - получение начального узла
 * 7. toNode() const - получение конечного узла
 * 8. setSide(LinkSide side) - установка стороны
 * 9. remove() - удаление связи
 * 10. setColor(Color color) - установка цвета
 * 11. color() const - получение цвета
 * 12. trackNodes() - отслеживание узлов
 * 13. refresh() - обновление
 * 14. setHighlighted(bool is_highlighted) - установка подсветки
 * 15. notifyLinkChanges() - уведомление об изменениях
 * 16. genLinkId() - генерация ID связи
 * 17. getLastLinkId() - получение последнего ID связи
 * 18. setLinkCount(int count) - установка счетчика связей
 * 19. updateTextItem(const QString& text) - обновление текстового элемента
 * 20. updateTextPosition() - обновление позиции текста
 */
class Link : public QGraphicsLineItem
{
public:
    static int link_count;

    Link(int id);
    ~Link();

    void setGraphId(int graph_id);
    void setFromNode(Node *node);
    void setToNode(Node *node);
    Node *fromNode() const;
    Node *toNode() const;
    void setSide(LinkSide side);
    void remove();
    void setColor(Color color);
    QColor color() const;
    void trackNodes();
    void refresh();
    void setHighlighted(bool is_highlighted);
    void notifyLinkChanges();

    static int genLinkId();
    static int getLastLinkId();
    static void setLinkCount(int count);

    void updateTextItem(const QString &text);

    LinkSide m_side;
    int m_id;
    int m_graphId;
    std::optional<int> m_width;

private:
    void updateTextPosition();

    Node *m_my_from_node;
    Node *m_my_to_node;
    QGraphicsSimpleTextItem *m_text_item;
};

#endif // LINK_H