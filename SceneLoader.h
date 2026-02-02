#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <QString>
#include <QJsonObject>

/**
 * @brief Класс загрузчика сцены
 *
 * SceneLoader предоставляет статические методы для загрузки и сохранения
 * сцены в формате JSON.
 */
class SceneLoader
{
public:
    /**
     * @brief Загружает сцену из JSON-файла
     * @param filename Путь к файлу сцены
     * @return true если загрузка успешна, false в противном случае
     */
    static bool loadSceneFromJson(const QString &filename);

    /**
     * @brief Сохраняет сцену в JSON-файл
     * @param filename Путь к файлу для сохранения
     * @return true если сохранение успешно, false в противном случае
     */
    static bool saveSceneToJson(const QString &filename);

    /**
     * @brief Получает элементы сцены в виде JSON-объекта
     * @return JSON-объект с элементами сцены
     */
    static QJsonObject getSceneElements();

private:
    /**
     * @brief Конструктор загрузчика сцены
     *
     * Приватный конструктор для предотвращения создания экземпляров класса.
     */
    SceneLoader() = delete; // Prevent instantiation

    /**
     * @brief Конструктор копирования загрузчика сцены
     *
     * Удален для предотвращения копирования.
     */
    SceneLoader(const SceneLoader &) = delete; // Prevent copy

    /**
     * @brief Оператор присваивания загрузчика сцены
     *
     * Удален для предотвращения присваивания.
     */
    SceneLoader &operator=(const SceneLoader &) = delete; // Prevent assignment
};

#endif // SCENELOADER_H