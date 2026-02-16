#ifndef COMMANDHISTORY_H
#define COMMANDHISTORY_H

#include <QObject>
#include <QStack>

class Command;

class CommandHistory : public QObject
{
    Q_OBJECT

public:
    explicit CommandHistory(QObject *parent = nullptr);
    ~CommandHistory();

    void push(Command *command);
    void undo();
    void redo();
    void clear();
    
    bool canUndo() const { return !m_undoStack.isEmpty(); }
    bool canRedo() const { return !m_redoStack.isEmpty(); }
    
    int undoCount() const { return m_undoStack.count(); }
    int redoCount() const { return m_redoStack.count(); }

signals:
    void canUndoChanged(bool canUndo);
    void canRedoChanged(bool canRedo);
    void commandExecuted();

private:
    QStack<Command*> m_undoStack;
    QStack<Command*> m_redoStack;
    int m_maxStackSize;
};

class Command
{
public:
    virtual ~Command() {}
    virtual void undo() = 0;
    virtual void redo() = 0;
    virtual QString getText() const = 0;
};

#endif // COMMANDHISTORY_H
