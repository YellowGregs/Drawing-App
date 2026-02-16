#include "tool.h"

Tool::Tool(Canvas *canvas, QObject *parent)
    : QObject(parent)
    , m_canvas(canvas)
    , m_size(10)
    , m_opacity(1.0)
    , m_hardness(0.5)
    , m_spacing(0.25)
    , m_isDrawing(false)
{
}

Tool::~Tool()
{
}

void Tool::drawOverlay(QPainter &painter)
{
    // Default: no overlay
}
