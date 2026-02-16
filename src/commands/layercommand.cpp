#include "layercommand.h"
#include "../layermanager.h"
#include "../layer.h"

LayerCommand::LayerCommand(LayerManager *layerManager, Type type, int layerIndex)
    : m_layerManager(layerManager)
    , m_type(type)
    , m_layerIndex(layerIndex)
    , m_savedLayer(nullptr)
{
}

LayerCommand::~LayerCommand()
{
    if (m_type == Delete) {
        delete m_savedLayer;
    }
}

void LayerCommand::undo()
{
    // TODO: Implement layer command undo
}

void LayerCommand::redo()
{
    // TODO: Implement layer command redo
}

QString LayerCommand::getText() const
{
    switch (m_type) {
        case Add: return "Add Layer";
        case Delete: return "Delete Layer";
        case Duplicate: return "Duplicate Layer";
        case Merge: return "Merge Layers";
        default: return "Layer Command";
    }
}
