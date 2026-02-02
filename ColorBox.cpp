#include "ColorBox.h"
#include "Editor.h"
#include "Config.h"
#include <QDebug>

ColorBox::ColorBox()
{
    // Создаем рамку для отображения цвета
    m_colorFrame = new QFrame(this);
    m_colorFrame->setFixedSize(60, 25); // Устанавливаем размер цветовой рамки

    // Создаем метку для отображения текста
    m_textLabel = new QLabel("", this);
    m_textLabel->setStyleSheet("color: black;");
    m_textLabel->setAlignment(Qt::AlignCenter); // Выравниваем текст по центру

    // Создаем layout для рамки, чтобы разместить в ней текст
    QVBoxLayout *frameLayout = new QVBoxLayout(m_colorFrame);
    frameLayout->addWidget(m_textLabel, 0, Qt::AlignCenter); // Центрируем текст

    // Устанавливаем layout для рамки
    m_colorFrame->setLayout(frameLayout);

    // Создаем основной layout для виджета ColorBox
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_colorFrame);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Устанавливаем основной layout для виджета
    setLayout(mainLayout);

    // Устанавливаем начальную сторону (лицевая)
    setSide(LinkSide::FRONT);
}

void ColorBox::setSide(LinkSide side)
{
    QString text;
    QColor color;

    // Определяем текст и цвет в зависимости от стороны платы
    switch (side)
    {
    case LinkSide::BACK:
        text = "BACK";
        color = QColor(Config::instance()->color(Color::BACK));
        break;
    case LinkSide::FRONT:
        text = "FRONT";
        color = QColor(Config::instance()->color(Color::FRONT));
        break;
    case LinkSide::WIP:
        text = "WIP";
        color = QColor(Config::instance()->color(Color::WIP));
        break;
    case LinkSide::NOTES:
        text = "NOTES";
        color = QColor(Config::instance()->color(Color::NOTES));
        break;
    default:
        text = "";
        color = Qt::white;
        break;
    }

    // Устанавливаем текст и цвет
    m_textLabel->setText(text);
    m_colorFrame->setStyleSheet(QString("background-color: %1;").arg(color.name()));
}
