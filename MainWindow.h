#pragma once

#include <QMainWindow>
#include <QStatusBar>
#include <QToolBar>
#include <QSlider>
#include <QAction>
#include <QActionGroup>
#include <QUndoStack>
#include "Editor.h"
#include "ColorBox.h"
#include "Sidebar.h"
#include "enums.h"

/*
 * Класс MainWindow - главное окно приложения PCB Tracer
 *
 * Основные функции:
 * 1. MainWindow(QWidget* parent) - конструктор класса
 * 2. ~MainWindow() - деструктор класса
 * 3. sliderValueChanged(int value) - обработчик изменения значения слайдера
 * 4. exit() - выход из приложения
 * 5. saveProject() - сохранение проекта
 * 6. saveProjectAs() - сохранение проекта с новым именем
 * 7. handleLoadProject() - обработчик загрузки проекта
 * 8. loadProject() - загрузка проекта
 * 9. setFrontSideImage() - установка изображения лицевой стороны платы
 * 10. setBackSideImage() - установка изображения обратной стороны платы
 * 11. viewConnections() - отображение соединений
 * 12. addTrackButtonAction(bool checked) - обработчик кнопки добавления трасс
 * 13. addComponentButtonAction(bool checked) - обработчик кнопки добавления компонентов
 * 14. addNotesButtonAction(bool checked) - обработчик кнопки добавления заметок
 * 15. showAboutDialog() - отображение диалога "О программе"
 * 16. frontSideToggleButtonAction(bool checked) - обработчик переключения видимости лицевой стороны
 * 17. backSideToggleButtonAction(bool checked) - обработчик переключения видимости обратной стороны
 * 18. zoomIn() - увеличение масштаба
 * 19. zoomOut() - уменьшение масштаба
 * 20. resetLinkWidth() - сброс ширины трасс
 * 21. openConfigDialog() - открытие диалога настроек
 * 22. cleanProject() - очистка проекта
 * 23. newProject() - создание нового проекта
 * 24. autoSaveProject() - автоматическое сохранение проекта
 * 25. closeEvent(QCloseEvent* event) - обработчик события закрытия окна
 * 26. keyPressEvent(QKeyEvent* event) - обработчик нажатия клавиш
 * 27. keyReleaseEvent(QKeyEvent* event) - обработчик отпускания клавиш
 * 28. showEvent(QShowEvent* event) - обработчик события отображения окна
 * 29. checkAndLoadAutosave(QString originalFilePath) - проверка и загрузка автосохранения
 * 30. loadProjectFromFile(const QString& filePath, bool isAutoLoad) - загрузка проекта из файла
 * 31. createToolbarActions() - создание действий для панели инструментов
 * 32. setCurrentFilePath(const QString& filePath) - установка текущего пути к файлу
 * 33. saveToFile(const QString& filePath, bool isAutoLoad) - сохранение в файл
 * 34. getAutosaveFilePath(QString filePath) - получение пути к файлу автосохранения
 * 35. promptForUnsavedChanges() - запрос о несохраненных изменениях
 * 36. removeAutosaveFile() - удаление файла автосохранения
 * 37. renameToAutosaveFile(QString filePath) - переименование в файл автосохранения
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void sliderValueChanged(int value);
    void exit();
    void saveProject();
    void saveProjectAs();
    void handleLoadProject();
    void loadProject();
    void setFrontSideImage();
    void setBackSideImage();
    void viewConnections();
    void addTrackButtonAction(bool checked);
    void addComponentButtonAction(bool checked);
    void addNotesButtonAction(bool checked);
    void showAboutDialog();
    void frontSideToggleButtonAction(bool checked);
    void backSideToggleButtonAction(bool checked);
    void zoomIn();
    void zoomOut();
    void resetLinkWidth();
    void openConfigDialog();
    void cleanProject();
    void newProject();

    void autoSaveProject();

private:
    QString m_windowBaseTitle;
    QString m_currentFilePath;
    Editor *m_editor;
    QToolBar *m_toolbar;
    QToolBar *m_sideToolbar;
    QSlider *m_slider;
    ColorBox *m_colorBox;
    Sidebar *m_sidebar;
    QTimer *m_autoSaveTimer;
    bool m_changesSinceLastAutosave;
    bool m_wasJustAutosaved;

    QAction *m_addTrackAction;
    QAction *m_addComponentAction;
    QAction *m_addNotesAction;
    QAction *m_frontSideAction;
    QAction *m_backSideAction;
    QAction *m_flipHAction;
    QAction *m_flipVAction;

    bool checkAndLoadAutosave(QString originalFilePath);
    void loadProjectFromFile(const QString &filePath, bool isAutoLoad = false);
    void createToolbarActions();
    void setCurrentFilePath(const QString &filePath);
    void saveToFile(const QString &filePath, bool isAutoLoad = false);
    QString getAutosaveFilePath(QString filePath);
    bool promptForUnsavedChanges();
    void removeAutosaveFile();
    void renameToAutosaveFile(QString filePath);
};