#include "commandhistory.h"

CommandHistory::CommandHistory(QObject *parent)
    : QObject(parent)
    , m_maxStackSize(100)
{
}

CommandHistory::~CommandHistory()
{
    clear();
}

void CommandHistory::push(Command *command)
{
    if (!command) return;
    
    qDeleteAll(m_redoStack);
    m_redoStack.clear();
    
    m_undoStack.push(command);
    
    while (m_undoStack.size() > m_maxStackSize) {
        delete m_undoStack.takeFirst();
    }
    
    emit canUndoChanged(canUndo());
    emit canRedoChanged(canRedo());
}

void CommandHistory::undo()
{
    if (!canUndo()) return;
    
    Command *command = m_undoStack.pop();
    command->undo();
    m_redoStack.push(command);
    
    emit canUndoChanged(canUndo());
    emit canRedoChanged(canRedo());
    emit commandExecuted();
}

void CommandHistory::redo()
{
    if (!canRedo()) return;
    
    Command *command = m_redoStack.pop();
    command->redo();
    m_undoStack.push(command);
    
    emit canUndoChanged(canUndo());
    emit canRedoChanged(canRedo());
    emit commandExecuted();
}

void CommandHistory::clear()
{
    qDeleteAll(m_undoStack);
    qDeleteAll(m_redoStack);
    m_undoStack.clear();
    m_redoStack.clear();
    
    emit canUndoChanged(canUndo());
    emit canRedoChanged(canRedo());
}
