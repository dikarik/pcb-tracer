#ifndef EDITOR_H
#define EDITOR_H

#include <QGraphicsView>
#include <QUndoStack>
#include <QKeyEvent>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <unordered_map>
#include <vector>
#include <QStatusBar>
#include "TypeChecks.h"

/*
#include "component.h"
#include "link.h"
#include "node.h"
#include "pad.h"
#include "image_layer.h"
#include "text_note.h"
#include "guide_tool.h"
*/
#include "CommunicationHub.h"
#include "IEditorTool.h"
#include "enums.h"
#include "ZoomableGraphicsView.h"
#include "QGraphicsItemLayer.h"
#include "ColorBox.h"
#include "GuideTool.h"
#include "TrackDrawingTool.h"

/*
 * Класс Editor - основной редактор графической сцены
 *
 * Основные функции:
 * 1. instance(QWidget* parent) - получение единственного экземпляра редактора
 * 2. Editor(QWidget* parent) - конструктор класса
 * 3. ~Editor() - деструктор класса
 * 4. showTracingIndicator() - отображение индикатора трассировки
 * 5. hideTracingIndicator() - скрытие индикатора трассировки
 * 6. getCurrentSide() - получение текущей стороны платы
 * 7. setCurrentSide(LinkSide side) - установка текущей стороны платы
 * 8. showStatusMessage(const QString& message) - отображение сообщения в статусной строке
 * 9. zoomIn() - увеличение масштаба
 * 10. zoomOut() - уменьшение масштаба
 * 11. toggleLayerVisibility(LinkSide side, bool visible) - переключение видимости слоя
 * 12. setCurrentTool(IEditorTool*) - установка текущего инструмента
 * 13. enterComponentMode() - переход в режим компонентов
 * 14. enterTrackMode() - переход в режим трассировки
 * 15. enterNotesMode() - переход в режим заметок
 * 16. saveSceneToJson(const QString& filename) - сохранение сцены в JSON
 * 17. loadSceneFromJson(const QString& filename) - загрузка сцены из JSON
 * 18. setStatusBar(QStatusBar* statusBar) - установка статусной строки
 * 19. clean() - очистка сцены
 * 20. getTrackDrawingTool() - получение инструмента рисования трасс
 * 21. findItemByIdAndClass(int itemId) - поиск элемента по ID и классу
 * 22. keyPressEvent(QKeyEvent* event) - обработчик нажатия клавиш
 * 23. keyReleaseEvent(QKeyEvent* event) - обработчик отпускания клавиш
 * 24. mousePressEvent(QMouseEvent* event) - обработчик нажатия мыши
 * 25. mouseDoubleClickEvent(QMouseEvent* event) - обработчик двойного клика мыши
 * 26. mouseMoveEvent(QMouseEvent* event) - обработчик движения мыши
 * 27. mouseReleaseEvent(QMouseEvent* event) - обработчик отпускания кнопки мыши
 * 28. addTracingIndicator() - добавление индикатора трассировки
 */
class ComponentDrawingTool;
class NotesTool;

class Editor : public ZoomableGraphicsView
{
	Q_OBJECT

public:
	static Editor *instance(QWidget *parent = nullptr);

	QGraphicsLineItem *m_tracingIndicator;
	Editor(QWidget *parent = nullptr);
	~Editor();
	void showTracingIndicator();
	void hideTracingIndicator();
	LinkSide getCurrentSide() const { return m_currentSide; }
	void setCurrentSide(LinkSide side);
	void showStatusMessage(const QString &message);
	void zoomIn();
	void zoomOut();
	void toggleLayerVisibility(LinkSide side, bool visible);
	void setCurrentTool(IEditorTool *);
	void enterComponentMode();
	void enterTrackMode();
	void enterNotesMode();
	void saveSceneToJson(const QString &filename);
	void loadSceneFromJson(const QString &filename);
	void setStatusBar(QStatusBar *statusBar);
	void clean();
	TrackDrawingTool *getTrackDrawingTool();

	int padSize;
	LinkSide m_currentSide;
	QUndoStack m_undoStack;
	QMap<LinkSide, QGraphicsItemLayer *> m_layers;
	QGraphicsScene *getScene() const { return m_scene; }
	DrawingState m_state;
	ColorBox *m_colorBox;
	GuideTool *m_guideTool;
	NotesTool *m_notesTool;

	template <typename T>
	T *findItemByIdAndClass(int itemId)
	{
		QList<QGraphicsItem *> items = scene()->items();
		for (QGraphicsItem *item : items)
		{
			if (isDynamicCastableToAny<T>(item))
			{
				T *castedItem = dynamic_cast<T *>(item);
				if (castedItem && castedItem->m_id == itemId)
				{
					return castedItem;
				}
			}
		}
		return nullptr;
	}

protected:
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	QGraphicsLineItem *addTracingIndicator();

private:
	static Editor *m_instance;
	QGraphicsScene *m_scene;
	IEditorTool *m_currentTool;
	TrackDrawingTool *m_trackDrawingTool;
	ComponentDrawingTool *m_componentDrawingTool;

	QStatusBar *m_statusBar;

	/*QObject* currentEditor;

	QJsonObject getSceneElements();
	void showStatusMessage(const QString& message);

	*/
};

#endif // EDITOR_H
