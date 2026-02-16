#include "drawcommand.h"
#include "../layer.h"

DrawCommand::DrawCommand(Layer *layer, const QImage &beforeImage, const QImage &afterImage)
    : m_layer(layer)
    , m_beforeImage(beforeImage)
    , m_afterImage(afterImage)
{
}

DrawCommand::~DrawCommand()
{
}

void DrawCommand::undo()
{
    if (m_layer) {
        *m_layer->getImage() = m_beforeImage;
    }
}

void DrawCommand::redo()
{
    if (m_layer) {
        *m_layer->getImage() = m_afterImage;
    }
}
