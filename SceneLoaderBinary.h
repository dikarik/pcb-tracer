#ifndef SCENELOADERBINARY_H
#define SCENELOADERBINARY_H

#include <QString>
#include <QJsonObject>
#include <QDataStream>
#include <QMap>

// Предварительные объявления классов
class Component;
class Link;
class Node;
class Pad;
class ImageLayer;
class TextNote;

/**
 * @brief Перечисление типов элементов сцены для бинарного формата
 */
enum class SceneElementType : quint8
{
    Component = 1,  ///< Компонент
    Link = 2,       ///< Связь
    Pad = 3,        ///< Контакт
    Node = 4,       ///< Узел
    ImageLayer = 5, ///< Слой изображения
    TextNote = 6,   ///< Текстовая заметка
    LastIds = 7,    ///< Последние ID
    Config = 8      ///< Конфигурация
};

/**
 * @brief Класс бинарного загрузчика сцены
 *
 * SceneLoaderBinary предоставляет статические методы для загрузки и сохранения
 * сцены в бинарном формате (.pcb).
 */
class SceneLoaderBinary
{
public:
    /**
     * @brief Загружает сцену из бинарного файла
     * @param filename Путь к файлу сцены
     * @return true если загрузка успешна, false в противном случае
     */
    static bool loadSceneFromBinary(const QString &filename);

    /**
     * @brief Сохраняет сцену в бинарный файл
     * @param filename Путь к файлу для сохранения
     * @return true если сохранение успешно, false в противном случае
     */
    static bool saveSceneToBinary(const QString &filename);

private:
    /**
     * @brief Записывает компонент в бинарный поток
     * @param out Бинарный поток для записи
     * @param component Указатель на компонент
     */
    static void writeComponentToBinary(QDataStream &out, Component *component);

    /**
     * @brief Записывает контакт в бинарный поток
     * @param out Бинарный поток для записи
     * @param pad Указатель на контакт
     */
    static void writePadToBinary(QDataStream &out, Pad *pad);

    /**
     * @brief Записывает связь в бинарный поток
     * @param out Бинарный поток для записи
     * @param link Указатель на связь
     */
    static void writeLinkToBinary(QDataStream &out, Link *link);

    /**
     * @brief Записывает узел в бинарный поток
     * @param out Бинарный поток для записи
     * @param node Указатель на узел
     */
    static void writeNodeToBinary(QDataStream &out, Node *node);

    /**
     * @brief Записывает текстовую заметку в бинарный поток
     * @param out Бинарный поток для записи
     * @param textNote Указатель на текстовую заметку
     */
    static void writeTextNoteToBinary(QDataStream &out, TextNote *textNote);

    /**
     * @brief Записывает слой изображения в бинарный поток
     * @param out Бинарный поток для записи
     * @param imageLayer Указатель на слой изображения
     */
    static void writeImageLayerToBinary(QDataStream &out, ImageLayer *imageLayer);

    /**
     * @brief Записывает последние ID в бинарный поток
     * @param out Бинарный поток для записи
     */
    static void writeLastIds(QDataStream &out);

    /**
     * @brief Записывает конфигурацию в бинарный поток
     * @param out Бинарный поток для записи
     */
    static void writeConfigToBinary(QDataStream &out);

    /**
     * @brief Читает конфигурацию из бинарного потока
     * @param in Бинарный поток для чтения
     */
    static void readConfigFromBinary(QDataStream &in);

    /**
     * @brief Читает компонент из бинарного потока
     * @param in Бинарный поток для чтения
     * @param nodeMap Словарь узлов
     */
    static void readComponentFromBinary(QDataStream &in, QMap<int, Node *> &nodeMap);

    /**
     * @brief Читает контакт из бинарного потока
     * @param in Бинарный поток для чтения
     * @return Указатель на созданный контакт
     */
    static Pad *readPadFromBinary(QDataStream &in);

    /**
     * @brief Читает связь из бинарного потока
     * @param in Бинарный поток для чтения
     * @param nodeMap Словарь узлов
     */
    static void readLinkFromBinary(QDataStream &in, const QMap<int, Node *> &nodeMap);

    /**
     * @brief Читает узел из бинарного потока
     * @param in Бинарный поток для чтения
     * @param nodeMap Словарь узлов
     */
    static void readNodeFromBinary(QDataStream &in, QMap<int, Node *> &nodeMap);

    /**
     * @brief Читает слой изображения из бинарного потока
     * @param in Бинарный поток для чтения
     */
    static void readImageLayerFromBinary(QDataStream &in);

    /**
     * @brief Читает текстовую заметку из бинарного потока
     * @param in Бинарный поток для чтения
     */
    static void readTextNoteFromBinary(QDataStream &in);

    /**
     * @brief Читает последние ID из бинарного потока
     * @param in Бинарный поток для чтения
     */
    static void readLastIds(QDataStream &in);

    /**
     * @brief Конструктор бинарного загрузчика сцены
     *
     * Приватный конструктор для предотвращения создания экземпляров класса.
     */
    SceneLoaderBinary() = delete; // Prevent instantiation

    /**
     * @brief Конструктор копирования бинарного загрузчика сцены
     *
     * Удален для предотвращения копирования.
     */
    SceneLoaderBinary(const SceneLoaderBinary &) = delete; // Prevent copy

    /**
     * @brief Оператор присваивания бинарного загрузчика сцены
     *
     * Удален для предотвращения присваивания.
     */
    SceneLoaderBinary &operator=(const SceneLoaderBinary &) = delete; // Prevent assignment
};

#endif // SCENELOADERBINARY_H