#include "Config.h"
#include "Link.h"
#include "Node.h"
#include "Editor.h"
#include "Component.h"
#include <QDebug>

Config *Config::m_instance = nullptr;

Config *Config::instance()
{
    // Реализация паттерна Singleton
    if (!m_instance)
    {
        m_instance = new Config();
    }
    return m_instance;
}

Config::Config()
{
    // Инициализируем цвета по умолчанию
    m_colors = {
        {Color::FRONT, "#FF0000"},       // Красный для лицевой стороны
        {Color::BACK, "#00FF00"},        // Зеленый для обратной стороны
        {Color::WIP, "#00FFFF"},         // Голубой для незавершенных работ
        {Color::NOTES, "#FFFFFF"},       // Белый для заметок
        {Color::HIGHLIGHTED, "#FFFFFF"}, // Белый для выделенных элементов
        {Color::NODE, "#00FF00"}         // Зеленый для узлов
    };

    // Инициализируем другие настройки по умолчанию
    m_linkWidth = 6; // Ширина линий связей
    m_padSize = 12;  // Размер контактных площадок
}

void Config::apply()
{
    int nodeRadius = m_padSize / 2;

    // Применяем настройки ко всем элементам сцены
    for (QGraphicsItem *item : Editor::instance()->scene()->items())
    {
        if (auto *link = dynamic_cast<Link *>(item))
        {
            // Устанавливаем цвет для связей в зависимости от стороны
            link->setColor(ColorUtils::fromLinkSide(link->m_side));
        }
        else if (auto *node = dynamic_cast<Node *>(item))
        {
            // Устанавливаем цвет для узлов
            node->setColor(Color::NODE);
        }
        else if (auto *pad = dynamic_cast<Pad *>(item))
        {
            // Устанавливаем цвет для контактов
            pad->setColor(Color::NODE);
        }
    }

    // Перерисовываем цветовую панель
    Editor::instance()->setCurrentSide(Editor::instance()->m_currentSide);
}

void Config::updateFromConfigDialog(const QVariantMap &dialogConfig)
{
    // Обновляем цвета из данных диалога настроек
    QVariantMap colorMap = dialogConfig["colors"].toMap();
    for (auto it = colorMap.begin(); it != colorMap.end(); ++it)
    {
        Color color = ColorUtils::fromString(it.key());
        m_colors[color] = it.value().toString();
    }

    // Обновляем другие настройки
    m_linkWidth = dialogConfig["link_width"].toInt();
    m_padSize = dialogConfig["pad_size"].toInt();
}

void Config::readConfigFromBinary(QDataStream &in)
{
    // Читаем конфигурацию из бинарного потока
    in >> m_colors[Color::FRONT] >> m_colors[Color::BACK] >> m_colors[Color::HIGHLIGHTED] >> m_colors[Color::NODE] >> m_colors[Color::NOTES] >> m_colors[Color::WIP] >> m_linkWidth >> m_padSize;
}

QString Config::color(LinkSide side) const
{
    // Возвращаем цвет для указанной стороны платы
    return m_colors.value(ColorUtils::fromLinkSide(side), "#000000");
}

QString Config::color(Color color) const
{
    // Возвращаем цвет для указанного типа элемента
    return m_colors.value(color, "#000000");
}

QVariantMap Config::toDict() const
{
    // Преобразуем конфигурацию в словарь
    QVariantMap result;
    QVariantMap colorMap;

    // Заполняем словарь цветов
    for (auto it = m_colors.begin(); it != m_colors.end(); ++it)
    {
        colorMap[QString::number(static_cast<int>(it.key()))] = it.value();
    }

    result["colors"] = colorMap;
    result["link_width"] = m_linkWidth;
    result["pad_size"] = m_padSize;
    return result;
}