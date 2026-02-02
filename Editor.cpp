#include <QDebug>
#include "Editor.h"
#include "ConnectionAnalyzer.h"
#include "TrackDrawingTool.h"
#include "ComponentDrawingTool.h"
#include "ZoomableGraphicsView.h"
#include <QGraphicsLineItem>
#include "QGraphicsItemLayer.h"
#include "NotesTool.h"

/*
 * Функция Editor::m_instance - статическая переменная для хранения единственного экземпляра редактора
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   указатель на единственный экземпляр редактора
 */
Editor *Editor::m_instance = nullptr;

/*
 * Функция Editor::instance - получение единственного экземпляра редактора
 * Входные параметры:
 *   parent - родительский виджет
 * Выходные данные:
 *   указатель на единственный экземпляр редактора
 */
Editor *Editor::instance(QWidget *parent)
{
    if (m_instance == nullptr)
    {
        m_instance = new Editor(parent);
    }
    return m_instance;
}

/*
 * Функция Editor::Editor - конструктор редактора
 * Входные параметры:
 *   parent - родительский виджет
 * Выходные данные:
 *   отсутствуют
 */
Editor::Editor(QWidget *parent) : ZoomableGraphicsView(parent)
{
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);
    m_scene->setBackgroundBrush(QBrush(QColor(28, 30, 32)));

    setMouseTracking(true);

    m_tracingIndicator = addTracingIndicator();
    m_trackDrawingTool = new TrackDrawingTool(this);
    m_componentDrawingTool = new ComponentDrawingTool();
    m_notesTool = new NotesTool(this);
    m_currentTool = m_trackDrawingTool;
    m_currentSide = LinkSide::FRONT;
    m_state = DrawingState::TRACKS;
    m_guideTool = new GuideTool();
    enterTrackMode();

    m_layers = {
        {LinkSide::BACK, new QGraphicsItemLayer()},
        {LinkSide::FRONT, new QGraphicsItemLayer()},
        {LinkSide::NODE, new QGraphicsItemLayer()},
        {LinkSide::WIP, new QGraphicsItemLayer()},
        {LinkSide::NOTES, new QGraphicsItemLayer()}};

    m_scene->addItem(m_layers[LinkSide::FRONT]);
    m_scene->addItem(m_layers[LinkSide::BACK]);
    m_scene->addItem(m_layers[LinkSide::WIP]);
    m_scene->addItem(m_layers[LinkSide::NODE]);
    m_scene->addItem(m_layers[LinkSide::NOTES]);

    m_layers[LinkSide::BACK]->setZValue(0);
    m_layers[LinkSide::FRONT]->setZValue(1);
    m_layers[LinkSide::WIP]->setZValue(2);
    m_layers[LinkSide::NODE]->setZValue(3);
    m_layers[LinkSide::NOTES]->setZValue(5);
}

/*
 * Функция Editor::~Editor - деструктор редактора
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   отсутствуют
 */
Editor::~Editor()
{
    delete m_guideTool;
    delete m_trackDrawingTool;
    delete m_tracingIndicator;
    delete m_componentDrawingTool;
    delete m_notesTool;
}

/*
 * Функция Editor::keyPressEvent - обработчик нажатия клавиш
 * Входные параметры:
 *   event - событие нажатия клавиши
 * Выходные данные:
 *   отсутствуют
 */
void Editor::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Z && event->modifiers() & Qt::ControlModifier)
    {
        if (event->modifiers() & Qt::ShiftModifier)
        {
            m_undoStack.redo();
        }
        else
        {
            m_undoStack.undo();
        }
    }
    else if (!m_currentTool->onKeyPress(event))
    {
        ZoomableGraphicsView::keyPressEvent(event);
    }
}

/*
 * Функция Editor::keyReleaseEvent - обработчик отпускания клавиш
 * Входные параметры:
 *   event - событие отпускания клавиши
 * Выходные данные:
 *   отсутствуют
 */
void Editor::keyReleaseEvent(QKeyEvent *event)
{
    if (!m_currentTool->onKeyRelease(event))
    {
        ZoomableGraphicsView::keyReleaseEvent(event);
    }
}

/*
 * Функция Editor::mouseDoubleClickEvent - обработчик двойного клика мыши
 * Входные параметры:
 *   event - событие двойного клика мыши
 * Выходные данные:
 *   отсутствуют
 */
void Editor::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (!m_currentTool->onMouseDoubleClick(event))
    {
        ZoomableGraphicsView::mouseDoubleClickEvent(event);
    }
}

/*
 * Функция Editor::mousePressEvent - обработчик нажатия мыши
 * Входные параметры:
 *   event - событие нажатия мыши
 * Выходные данные:
 *   отсутствуют
 */
void Editor::mousePressEvent(QMouseEvent *event)
{
    if (!m_currentTool->onMousePress(event))
    {
        ZoomableGraphicsView::mousePressEvent(event);
    }
}

/*
 * Функция Editor::mouseMoveEvent - обработчик движения мыши
 * Входные параметры:
 *   event - событие движения мыши
 * Выходные данные:
 *   отсутствуют
 */
void Editor::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_currentTool->onMouseMove(event))
    {
        ZoomableGraphicsView::mouseMoveEvent(event);
    }
}

/*
 * Функция Editor::mouseReleaseEvent - обработчик отпускания кнопки мыши
 * Входные параметры:
 *   event - событие отпускания кнопки мыши
 * Выходные данные:
 *   отсутствуют
 */
void Editor::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_currentTool->onMouseRelease(event))
    {
        ZoomableGraphicsView::mouseReleaseEvent(event);
    }
}

/*
 * Функция Editor::setStatusBar - установка статусной строки
 * Входные параметры:
 *   statusBar - указатель на статусную строку
 * Выходные данные:
 *   отсутствуют
 */
void Editor::setStatusBar(QStatusBar *statusBar)
{
    m_statusBar = statusBar;
}

/*
 * Функция Editor::zoomIn - увеличение масштаба
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   отсутствуют
 */
void Editor::zoomIn()
{
    scale(1.2, 1.2);
}

/*
 * Функция Editor::zoomOut - уменьшение масштаба
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   отсутствуют
 */
void Editor::zoomOut()
{
    scale(0.8, 0.8);
}

/*
 * Функция Editor::addTracingIndicator - добавление индикатора трассировки
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   указатель на элемент индикатора трассировки
 */
QGraphicsLineItem *Editor::addTracingIndicator()
{
    QGraphicsLineItem *line = new QGraphicsLineItem(0, 0, 0, 0);
    QPen pen(Qt::gray);
    pen.setStyle(Qt::DashLine);
    pen.setWidth(1);
    line->setPen(pen);
    line->setVisible(false);
    m_scene->addItem(line);
    return line;
}

/*
 * Функция Editor::showTracingIndicator - отображение индикатора трассировки
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   отсутствуют
 */
void Editor::showTracingIndicator()
{
    if (m_tracingIndicator)
    {
        m_tracingIndicator->setVisible(true);
    }
}

/*
 * Функция Editor::hideTracingIndicator - скрытие индикатора трассировки
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   отсутствуют
 */
void Editor::hideTracingIndicator()
{
    if (m_tracingIndicator)
    {
        m_tracingIndicator->setVisible(false);
    }
}

/*
 * Функция Editor::toggleLayerVisibility - переключение видимости слоя
 * Входные параметры:
 *   side - сторона платы
 *   visible - флаг видимости
 * Выходные данные:
 *   отсутствуют
 */
void Editor::toggleLayerVisibility(LinkSide side, bool visible)
{
    auto layer = m_layers[side];
    layer->setVisible(visible);
}

/*
 * Функция Editor::showStatusMessage - отображение сообщения в статусной строке
 * Входные параметры:
 *   message - текст сообщения
 * Выходные данные:
 *   отсутствуют
 */
void Editor::showStatusMessage(const QString &message)
{
    if (m_statusBar)
    {
        m_statusBar->showMessage(message);
    }
}

/*
 * Функция Editor::clean - очистка сцены
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   отсутствуют
 */
void Editor::clean()
{
    m_undoStack.clear();
    TrackGraph::setTrackGraphCount(0);
    Link::setLinkCount(0);
    Node::setNodeCount(0);
    NotesTool::setNoteCount(0);
    Component::setComponentCount(0);

    // remove all items
    for (auto it = m_layers.constBegin(); it != m_layers.constEnd(); ++it)
    {
        QList<QGraphicsItem *> childItems = it.value()->childItems();
        for (QGraphicsItem *item : childItems)
        {
            m_scene->removeItem(item);
        }
    }

    m_guideTool->clear();
    m_trackDrawingTool->clean();

    // notify all listeners about the scene clean
    CommunicationHub::instance().publish(HubEvent::SCENE_CLEAN, nullptr);
}

/*
 * Функция Editor::setCurrentSide - установка текущей стороны платы
 * Входные параметры:
 *   side - сторона платы
 * Выходные данные:
 *   отсутствуют
 */
void Editor::setCurrentSide(LinkSide side)
{
    m_colorBox->setSide(side);
    m_currentSide = side;
}

/*
 * Функция Editor::setCurrentTool - установка текущего инструмента
 * Входные параметры:
 *   tool - указатель на инструмент
 * Выходные данные:
 *   отсутствуют
 */
void Editor::setCurrentTool(IEditorTool *tool)
{
    m_currentTool->exitMode();
    m_currentTool = tool;
    m_currentTool->enterMode();
}

/*
 * Функция Editor::enterTrackMode - переход в режим трассировки
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   отсутствуют
 */
void Editor::enterTrackMode()
{
    m_state = DrawingState::TRACKS;
    setCurrentTool(m_trackDrawingTool);
}

/*
 * Функция Editor::enterComponentMode - переход в режим компонентов
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   отсутствуют
 */
void Editor::enterComponentMode()
{
    m_state = DrawingState::IC;
    setCurrentTool(m_componentDrawingTool);
}

/*
 * Функция Editor::enterNotesMode - переход в режим заметок
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   отсутствуют
 */
void Editor::enterNotesMode()
{
    m_state = DrawingState::NOTES;
    setCurrentTool(m_notesTool);
}

/*
 * Функция Editor::getTrackDrawingTool - получение инструмента рисования трасс
 * Входные параметры:
 *   отсутствуют
 * Выходные данные:
 *   указатель на инструмент рисования трасс
 */
TrackDrawingTool *Editor::getTrackDrawingTool()
{
    return m_trackDrawingTool;
}
