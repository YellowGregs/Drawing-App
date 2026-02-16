#ifndef TEXTTOOL_H
#define TEXTTOOL_H

#include "tool.h"
#include <QFont>

class TextTool : public Tool
{
    Q_OBJECT

public:
    explicit TextTool(Canvas *canvas, QObject *parent = nullptr);
    
    void mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    void mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure) override;
    
    QString getName() const override { return tr("Text"); }
    
    void setFont(const QFont &font) { m_font = font; }
    QFont getFont() const { return m_font; }
    
    void setText(const QString &text) { m_text = text; }
    QString getText() const { return m_text; }

private:
    QFont m_font;
    QString m_text;
    QPoint m_textPosition;
};

#endif // TEXTTOOL_H
