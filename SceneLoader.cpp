#include "SceneLoader.h"
#include "Editor.h"
#include "Config.h"
#include "Component.h"
#include "Node.h"
#include "Link.h"
#include "ImageLayer.h"
#include "CommunicationHub.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include "NotesTool.h"
#include <QDebug>

bool SceneLoader::loadSceneFromJson(const QString &filename)
{
    try
    {
        Editor *editor = Editor::instance();
        editor->showStatusMessage(QString("Loading %1").arg(filename));
        qDebug() << "Loading" << filename;

        // Открываем файл для чтения
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
        {
            throw std::runtime_error("Could not open file");
        }

        // Читаем и парсим JSON-документ
        QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
        if (jsonDoc.isNull())
        {
            throw std::runtime_error("Invalid JSON in file");
        }

        QJsonObject sceneData = jsonDoc.object();

        // Создаем словарь для хранения узлов
        QMap<int, Node *> nodes;
        int maxNodeId = 0;
        int maxGraphId = 0;
        int maxComponentId = 0;

        // Воссоздаем компоненты
        QJsonArray componentsArray = sceneData["components"].toArray();
        for (const QJsonValue &componentValue : componentsArray)
        {
            QJsonObject componentData = componentValue.toObject();
            maxComponentId = qMax(maxComponentId, componentData["id"].toInt());
            Component *component = new Component(componentData["name"].toString(), componentData["id"].toInt());
            component->setPos(componentData["position"].toObject()["x"].toDouble(),
                              componentData["position"].toObject()["y"].toDouble());

            // Воссоздаем контакты для этого компонента
            QJsonArray padsArray = componentData["pads"].toArray();
            for (const QJsonValue &padValue : padsArray)
            {
                QJsonObject padData = padValue.toObject();
                Pad *pad = new Pad(padData["name"].toString(), padData["id"].toInt(),
                                   QPoint(padData["x"].toDouble(), padData["y"].toDouble()),
                                   padData["number"].toInt());
                component->addPad(pad);
                nodes[pad->m_id] = pad;
                maxNodeId = std::max(maxNodeId, pad->m_id);
            }

            component->addToScene(editor->scene());
            qDebug() << "Adding component" << componentData["name"].toString();
            CommunicationHub::instance().publish(HubEvent::COMPONENT_CREATED, component);
        }

        Component::setComponentCount(maxComponentId + 1);

        // Воссоздаем узлы
        QJsonArray nodesArray = sceneData["nodes"].toArray();
        for (const QJsonValue &nodeValue : nodesArray)
        {
            QJsonObject nodeData = nodeValue.toObject();
            maxNodeId = std::max(maxNodeId, nodeData["id"].toInt());
            Node *node = new Node(nodeData["id"].toInt());
            node->setPos(nodeData["position"].toObject()["x"].toDouble(),
                         nodeData["position"].toObject()["y"].toDouble());
            node->setSide(LinkSide::NODE); // setSide adds it to the scene
            nodes[nodeData["id"].toInt()] = node;
        }

        Node::setNodeCount(maxNodeId + 1);

        // Воссоздаем связи
        int maxLinkId = 0;
        QJsonArray linksArray = sceneData["links"].toArray();
        for (const QJsonValue &linkValue : linksArray)
        {
            QJsonObject linkData = linkValue.toObject();
            maxLinkId = qMax(maxLinkId, linkData["id"].toInt());
            maxGraphId = qMax(maxGraphId, linkData["graph_id"].toInt());
            Node *fromNode = nodes[linkData["from_node_id"].toInt()];
            Node *toNode = nodes[linkData["to_node_id"].toInt()];
            Link *link = new Link(linkData["id"].toInt());
            link->setFromNode(fromNode);
            link->setToNode(toNode);
            editor->scene()->addItem(link);
            link->setSide(LinkSideUtils::fromString(linkData["side"].toString()));
            link->setGraphId(linkData["graph_id"].toInt());
            link->refresh();
        }

        Link::setLinkCount(maxLinkId + 1);

        TrackGraph::setTrackGraphCount(maxGraphId + 1);

        qDebug() << "After loading:" << Link::genLinkId() << TrackGraph::genTrackGraphId();

        // Уведомляем об изменениях связей для всех узлов
        for (Node *node : nodes)
        {
            node->notifyLinkChanges();
        }

        qDebug() << nodes.size() << "nodes loaded";

        // Воссоздаем слои изображений
        QJsonArray imageLayersArray = sceneData["image_layers"].toArray();
        for (const QJsonValue &imageLayerValue : imageLayersArray)
        {
            QJsonObject imageData = imageLayerValue.toObject();
            editor->m_guideTool->setImageLayer(static_cast<LinkSide>(imageData["id"].toInt()),
                                               imageData["image_path"].toString());
        }

        // Загружаем текстовые заметки
        QJsonArray notesArray = sceneData["notes"].toArray();
        for (const QJsonValue &noteValue : notesArray)
        {
            QJsonObject noteData = noteValue.toObject();
            QJsonObject rect = noteData["rect"].toObject();
            QRectF rectf(rect["x"].toDouble(), rect["y"].toDouble(), rect["width"].toDouble(), rect["height"].toDouble());
            TextNote *textNote = new TextNote(rectf, Config::instance()->color(Color::NOTES));
            textNote->m_id = NotesTool::genNoteId();
            textNote->setText(noteData["text"].toString());
            textNote->setParentItem(editor->m_layers[LinkSide::NOTES]);

            // Уведомляем о создании заметки
            CommunicationHub::instance().publish(HubEvent::NOTE_CREATED, textNote);
        }

        editor->showStatusMessage("Scene loaded successfully");
        qDebug() << "Scene loaded from" << filename;

        return true;
    }
    catch (const std::exception &e)
    {
        qDebug() << "An error occurred while loading the scene:" << e.what();
        QMessageBox::critical(nullptr, "Error", QString("An error occurred while loading the scene: %1").arg(e.what()));
        return false;
    }
}

QJsonObject SceneLoader::getSceneElements()
{
    // Создаем JSON-объект для хранения элементов сцены
    QJsonObject sceneData;
    QJsonArray components, links, pads, nodes, imageLayers, notes;

    // Проходим по всем элементам сцены
    for (QGraphicsItem *item : Editor::instance()->scene()->items())
    {
        if (auto component = dynamic_cast<Component *>(item))
        {
            // Сохраняем данные компонента
            QJsonObject componentData;
            componentData["id"] = component->m_id;
            componentData["name"] = component->m_name;
            componentData["position"] = QJsonObject{
                {"x", component->pos().x()},
                {"y", component->pos().y()}};

            // Сохраняем данные контактов компонента
            QJsonArray padsData;
            for (const auto &pad : component->m_pads)
            {
                QJsonObject padData;
                padData["id"] = pad->m_id;
                padData["x"] = pad->pos().x();
                padData["y"] = pad->pos().y();
                padData["component_id"] = component->m_id;
                padData["number"] = pad->m_number;
                padData["name"] = pad->m_name;
                padsData.append(padData);
            }
            componentData["pads"] = padsData;
            components.append(componentData);
        }
        else if (auto link = dynamic_cast<Link *>(item))
        {
            // Сохраняем данные связи
            QJsonObject linkData;
            linkData["id"] = link->m_id;
            linkData["from_node_id"] = link->fromNode()->m_id;
            linkData["to_node_id"] = link->toNode()->m_id;
            linkData["graph_id"] = link->m_graphId;
            linkData["side"] = LinkSideUtils::toString(link->m_side);
            linkData["width"] = 2;
            links.append(linkData);
        }
        else if (auto imageLayer = dynamic_cast<ImageLayer *>(item))
        {
            // Сохраняем данные слоя изображения
            QJsonObject imageData;
            imageData["id"] = static_cast<int>(imageLayer->m_id);
            imageData["image_path"] = imageLayer->m_imagePath;
            imageData["position"] = QJsonObject{
                {"x", imageLayer->pos().x()},
                {"y", imageLayer->pos().y()}};
            imageData["opacity"] = imageLayer->opacity();
            imageLayers.append(imageData);
        }
        else if (auto node = dynamic_cast<Node *>(item))
        {
            // Сохраняем данные узла (если это не контакт)
            if (!dynamic_cast<Pad *>(node))
            {
                QJsonObject nodeData;
                nodeData["id"] = node->m_id;
                nodeData["position"] = QJsonObject{
                    {"x", node->pos().x()},
                    {"y", node->pos().y()}};
                nodes.append(nodeData);
            }
        }
        else if (auto textNote = dynamic_cast<TextNote *>(item))
        {
            // Сохраняем данные текстовой заметки
            QJsonObject textNoteData;
            textNoteData["id"] = textNote->m_id;
            textNoteData["rect"] = QJsonObject{
                {"x", textNote->rect().x()},
                {"y", textNote->rect().y()},
                {"width", textNote->rect().width()},
                {"height", textNote->rect().height()}};
            textNoteData["text"] = textNote->m_text;
            notes.append(textNoteData);
        }
    }

    // Добавляем все массивы в объект сцены
    sceneData["components"] = components;
    sceneData["links"] = links;
    sceneData["pads"] = pads;
    sceneData["nodes"] = nodes;
    sceneData["image_layers"] = imageLayers;
    sceneData["notes"] = notes;

    return sceneData;
}

bool SceneLoader::saveSceneToJson(const QString &filename)
{
    // Добавляем расширение .jpcb если его нет
    QString actualFilename = filename;
    if (!actualFilename.toLower().endsWith(".jpcb"))
    {
        actualFilename += ".jpcb";
    }

    // Получаем элементы сцены и создаем JSON-документ
    QJsonObject sceneData = SceneLoader::getSceneElements();
    QJsonDocument doc(sceneData);
    QFile file(actualFilename);

    // Пытаемся открыть файл для записи
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toJson());
        file.close();
        qDebug() << "Scene data saved to" << actualFilename;
        return true;
    }
    else
    {
        qDebug() << "Failed to save scene data to" << actualFilename;
        return false;
    }
}
