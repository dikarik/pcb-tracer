#pragma once

#include <QGraphicsItem>
#include <QPointF>
#include <QString>
#include <vector>
#include "Node.h"
#include "enums.h"

/**
 * @brief Класс виртуального контакта компонента
 *
 * PhantomPad представляет собой виртуальный контакт, который не имеет физического
 * представления на плате, но используется для логической связи компонентов.
 */
class PhantomPad : public Node
{
public:
    QString m_name; ///< Имя виртуального контакта

    /**
     * @brief Конструктор виртуального контакта
     * @param name Имя контакта
     * @param position Позиция контакта на сцене
     */
    PhantomPad(const QString &name, const QPointF &position);
};

/**
 * @brief Класс физического контакта компонента
 *
 * Pad представляет собой физический контакт компонента на печатной плате.
 */
class Pad : public Node
{
public:
    /**
     * @brief Конструктор контакта
     * @param name Имя контакта
     * @param id Уникальный идентификатор контакта
     * @param position Позиция контакта на сцене
     * @param number Номер контакта в компоненте
     */
    Pad(const QString &name, int id, const QPointF &position, int number);

    int m_componentId; ///< Идентификатор компонента, к которому принадлежит контакт
    int m_id;          ///< Уникальный идентификатор контакта
    QString m_name;    ///< Имя контакта
    int m_number;      ///< Номер контакта в компоненте
};

/**
 * @brief Класс компонента на печатной плате
 *
 * Component представляет собой электронный компонент на печатной плате,
 * состоящий из корпуса и контактов (Pad).
 */
class Component : public QGraphicsItem
{
public:
    /**
     * @brief Конструктор компонента
     * @param name Имя компонента
     * @param id Уникальный идентификатор компонента
     */
    Component(const QString &name, int id);

    /**
     * @brief Получает количество контактов компонента
     * @return Количество контактов
     */
    int numberOfPads() const;

    /**
     * @brief Преобразует компонент в строковое представление
     * @return Строковое представление компонента
     */
    QString toString() const;

    /**
     * @brief Добавляет контакт к компоненту
     * @param pad Указатель на добавляемый контакт
     */
    void addPad(Pad *pad);

    /**
     * @brief Добавляет компонент на графическую сцену
     * @param scene Указатель на графическую сцену
     */
    void addToScene(QGraphicsScene *scene);

    /**
     * @brief Возвращает ограничивающий прямоугольник компонента
     * @return Ограничивающий прямоугольник
     */
    QRectF boundingRect() const override;

    /**
     * @brief Отрисовывает компонент
     * @param painter Указатель на рисовальщика
     * @param option Указатель на опции стиля
     * @param widget Указатель на виджет (опционально)
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    /**
     * @brief Удаляет компонент со сцены
     * @param scene Указатель на графическую сцену
     */
    void remove(QGraphicsScene *scene);

    /**
     * @brief Генерирует уникальный ID для компонента
     * @return Уникальный идентификатор компонента
     */
    static int genComponentId();

    /**
     * @brief Устанавливает счетчик компонентов
     * @param count Новое значение счетчика
     * @throws std::invalid_argument если count отрицательный
     */
    static void setComponentCount(int count);

    /**
     * @brief Возвращает последний использованный ID компонента
     * @return Последний идентификатор компонента
     */
    static int getLastComponentId();

    int m_id;                  ///< Уникальный идентификатор компонента
    std::vector<Pad *> m_pads; ///< Вектор контактов компонента
    QString m_name;            ///< Имя компонента

private:
    static int s_componentCount; ///< Статический счетчик компонентов для генерации уникальных ID
};