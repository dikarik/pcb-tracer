#ifndef COMMUNICATION_HUB_H
#define COMMUNICATION_HUB_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>
#include <any>

/**
 * @brief Перечисление типов событий системы обмена сообщениями
 */
enum class HubEvent
{
    NODE_MADE_SINGLE_LINK,    ///< Узел создал одну связь
    NODE_MADE_MULTIPLE_LINKS, ///< Узел создал несколько связей
    NODE_DELETED,             ///< Узел удален
    COMPONENT_CREATED,        ///< Компонент создан
    COMPONENT_DELETED,        ///< Компонент удален
    NOTE_CREATED,             ///< Заметка создана
    NOTE_DELETED,             ///< Заметка удалена
    SCENE_CLEAN               ///< Сцена очищена
};

/**
 * @brief Класс центра обмена сообщениями
 *
 * CommunicationHub реализует паттерн Observer для обмена сообщениями
 * между компонентами приложения.
 */
class CommunicationHub
{
public:
    /**
     * @brief Тип функции обратного вызова
     *
     * Callback принимает тип события и указатель на данные события
     */
    using Callback = std::function<void(HubEvent, const void *)>;

    /**
     * @brief Получает экземпляр центра обмена сообщениями (Singleton)
     * @return Ссылка на экземпляр CommunicationHub
     */
    static CommunicationHub &instance()
    {
        static CommunicationHub instance;
        return instance;
    }

    /**
     * @brief Подписывается на событие
     * @param event Тип события
     * @param callback Функция обратного вызова
     */
    void subscribe(HubEvent event, Callback callback)
    {
        subscribers[event].push_back(std::move(callback));
    }

    /**
     * @brief Публикует событие
     * @param event Тип события
     * @param data Указатель на данные события
     */
    void publish(HubEvent event, const void *data)
    {
        auto it = subscribers.find(event);
        if (it != subscribers.end())
        {
            for (auto &callback : it->second)
            {
                callback(event, data);
            }
        }
    }

private:
    /**
     * @brief Конструктор центра обмена сообщениями
     */
    CommunicationHub() = default;

    std::unordered_map<HubEvent, std::vector<Callback>> subscribers; ///< Словарь подписчиков на события
};

#endif // COMMUNICATION_HUB_H