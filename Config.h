#ifndef CONFIG_H
#define CONFIG_H

#include <QColor>
#include <QVariantMap>
#include <QMap>
#include "enums.h"

class Editor;
class Link;

/**
 * @brief Класс конфигурации приложения
 *
 * Config реализует паттерн Singleton и предоставляет доступ к настройкам приложения,
 * включая цвета, размеры элементов и другие параметры.
 */
class Config
{
public:
    /**
     * @brief Получает экземпляр конфигурации (Singleton)
     * @return Указатель на экземпляр Config
     */
    static Config *instance();

    /**
     * @brief Получает цвет для указанной стороны платы
     * @param side Сторона платы
     * @return Цвет в формате строки
     */
    QString color(LinkSide side) const;

    /**
     * @brief Получает цвет для указанного типа элемента
     * @param color Тип цвета
     * @return Цвет в формате строки
     */
    QString color(Color color) const;

    /**
     * @brief Применяет конфигурацию ко всем элементам сцены
     */
    void apply();

    /**
     * @brief Обновляет конфигурацию из данных диалога настроек
     * @param dialogConfig Данные конфигурации из диалога
     */
    void updateFromConfigDialog(const QVariantMap &dialogConfig);

    /**
     * @brief Обновляет конфигурацию из словаря
     * @param kwargs Словарь с параметрами конфигурации
     */
    void update(const QVariantMap &kwargs);

    /**
     * @brief Преобразует конфигурацию в словарь
     * @return Словарь с параметрами конфигурации
     */
    QVariantMap toDict() const;

    /**
     * @brief Читает конфигурацию из бинарного потока
     * @param in Бинарный поток для чтения
     */
    void readConfigFromBinary(QDataStream &in);

    int m_linkWidth; ///< Ширина линий связей
    int m_padSize;   ///< Размер контактных площадок

private:
    /**
     * @brief Конструктор конфигурации
     */
    Config();

    Config(const Config &) = delete;            ///< Запрещаем копирование
    Config &operator=(const Config &) = delete; ///< Запрещаем присваивание

    static Config *m_instance; ///< Указатель на единственный экземпляр (Singleton)

    QMap<Color, QString> m_colors; ///< Словарь цветов для разных типов элементов
};

#endif // CONFIG_H