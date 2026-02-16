#include "texttool.h"
#include "../canvas.h"
#include "../layermanager.h"
#include "../layer.h"
#include <QPainter>
#include <QInputDialog>

TextTool::TextTool(Canvas *canvas, QObject *parent)
    : Tool(canvas, parent)
    , m_font("Arial", 12)
{
}

void TextTool::mousePressEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    if (event->button() == Qt::LeftButton) {
        m_textPosition = pos;
        
        bool ok;
        QString text = QInputDialog::getText(nullptr, tr("Enter Text"),
                                            tr("Text:"), QLineEdit::Normal,
                                            m_text, &ok);
        
        if (ok && !text.isEmpty()) {
            m_text = text;
            
            Layer *currentLayer = m_canvas->getLayerManager()->getCurrentLayer();
            if (currentLayer && !currentLayer->isLocked()) {
                QPainter painter(currentLayer->getImage());
                painter.setRenderHint(QPainter::Antialiasing, true);
                painter.setFont(m_font);
                painter.setPen(m_canvas->getPrimaryColor());
                painter.setOpacity(m_opacity);
                painter.drawText(m_textPosition, m_text);
            }
        }
    }
}

void TextTool::mouseMoveEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    // Not used for text tool
}

void TextTool::mouseReleaseEvent(QMouseEvent *event, const QPoint &pos, qreal pressure)
{
    // Not used for text tool
}
