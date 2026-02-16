#ifndef DRAWCOMMAND_H
#define DRAWCOMMAND_H

#include "../commandhistory.h"
#include <QImage>

class Layer;

class DrawCommand : public Command
{
public:
    DrawCommand(Layer *layer, const QImage &beforeImage, const QImage &afterImage);
    ~DrawCommand();
    
    void undo() override;
    void redo() override;
    QString getText() const override { return "Draw"; }

private:
    Layer *m_layer;
    QImage m_beforeImage;
    QImage m_afterImage;
};

#endif // DRAWCOMMAND_H
