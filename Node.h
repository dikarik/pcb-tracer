#ifndef NODE_H
#define NODE_H

#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include <vector>
#include "Config.h"
#include "enums.h"
#include "CommunicationHub.h"

class Link;

/**
 * @brief Класс узла на печатной плате
 *
 * Node представляет собой точку соединения на печатной плате, к которой могут быть подключены трассы (Link).
 * Узлы могут быть контактами компонентов (Pad) или виртуальными точками соединения (Node).
 */
class Node : public QGraphicsEllipseItem
{
public:
    static int node_count; ///< Статический счетчик узлов для генерации уникальных ID
    bool m_showOnHover;    ///< Флаг, определяющий отображение узла при наведении мыши

    /**
     * @brief Конструктор узла
     * @param id Уникальный идентификатор узла
     */
    Node(int id);

    /**
     * @brief Уведомляет о изменениях в связях узла
     *
     * Публикует событие через CommunicationHub о количестве связей узла
     */
    void notifyLinkChanges();

    /**
     * @brief Уведомляет о удалении узла
     *
     * Публикует событие через CommunicationHub о удалении узла
     */
    void willBeDeleted();

    /**
     * @brief Добавляет связь к узлу
     * @param link Указатель на добавляемую связь
     */
    void addLink(Link *link);

    /**
     * @brief Удаляет связь из узла
     * @param link Указатель на удаляемую связь
     */
    void removeLink(Link *link);

    /**
     * @brief Удаляет связь по идентификатору
     * @param link_id Идентификатор удаляемой связи
     */
    void removeLinkById(int link_id);

    /**
     * @brief Устанавливает позицию узла
     * @param x Координата X
     * @param y Координата Y
     */
    void setPosition(float x, float y);

    /**
     * @brief Устанавливает позицию узла
     * @param position Точка с координатами
     */
    void setPosition(QPointF &position);

    /**
     * @brief Возвращает степень узла (количество связей)
     * @return Количество связей, подключенных к узлу
     */
    int getGrade() const;

    /**
     * @brief Возвращает список связей узла
     * @return Вектор указателей на связи
     */
    std::vector<Link *> getLinks() const;

    /**
     * @brief Обновляет все связи узла
     *
     * Вызывает trackNodes() для каждой связи, подключенной к узлу
     */
    void updateLinks();

    /**
     * @brief Проверяет, должен ли узел реагировать на наведение
     * @return true если узел должен реагировать на наведение, false в противном случае
     */
    bool shouldReactToHover() const;

    /**
     * @brief Включает возможность перемещения узла
     */
    void enable();

    /**
     * @brief Отключает возможность перемещения узла
     */
    void disable();

    /**
     * @brief Устанавливает сторону платы для узла
     * @param side Сторона платы (FRONT или BACK)
     */
    void setSide(LinkSide side);

    /**
     * @brief Обновляет отображение узла
     *
     * Обновляет видимость узла в зависимости от подключенных связей
     */
    void refresh();

    /**
     * @brief Устанавливает цвет узла
     * @param color Цвет из перечисления Color
     */
    void setColor(Color color);

    /**
     * @brief Генерирует уникальный ID для узла
     * @return Уникальный идентификатор узла
     */
    static int genNodeId();

    /**
     * @brief Возвращает последний использованный ID узла
     * @return Последний идентификатор узла
     */
    static int getLastNodeId();

    /**
     * @brief Устанавливает счетчик узлов
     * @param count Новое значение счетчика
     * @throws std::invalid_argument если count отрицательный
     */
    static void setNodeCount(int count);

    int m_id;                  ///< Уникальный идентификатор узла
    std::optional<int> m_size; ///< Размер узла (опциональный)

protected:
    /**
     * @brief Обработчик изменения свойств элемента
     * @param change Тип изменения
     * @param value Новое значение
     * @return QVariant с результатом изменения
     */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    /**
     * @brief Обработчик события наведения мыши
     * @param event Событие наведения
     */
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    /**
     * @brief Обработчик события ухода мыши
     * @param event Событие ухода мыши
     */
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    /**
     * @brief Возвращает форму элемента для обработки событий
     * @return QPainterPath с формой элемента
     */
    QPainterPath shape() const override;

    /**
     * @brief Обработчик события нажатия мыши
     * @param event Событие нажатия мыши
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    /**
     * @brief Обработчик события отпускания мыши
     * @param event Событие отпускания мыши
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    std::vector<Link *> m_links; ///< Вектор связей, подключенных к узлу
    LinkSide m_side;             ///< Сторона платы, к которой принадлежит узел
    QPointF m_drag_start_pos;    ///< Начальная позиция перетаскивания

    /**
     * @brief Обработчик завершения перетаскивания
     * @param startPos Начальная позиция перетаскивания
     * @param endPos Конечная позиция перетаскивания
     */
    void dragFinished(const QPointF &startPos, const QPointF &endPos);
};

#endif // NODE_H