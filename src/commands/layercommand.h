#ifndef LAYERCOMMAND_H
#define LAYERCOMMAND_H

#include "../commandhistory.h"

class LayerManager;
class Layer;

class LayerCommand : public Command
{
public:
    enum Type {
        Add,
        Delete,
        Duplicate,
        Merge
    };

    LayerCommand(LayerManager *layerManager, Type type, int layerIndex);
    ~LayerCommand();
    
    void undo() override;
    void redo() override;
    QString getText() const override;

private:
    LayerManager *m_layerManager;
    Type m_type;
    int m_layerIndex;
    Layer *m_savedLayer;
};

#endif // LAYERCOMMAND_H
