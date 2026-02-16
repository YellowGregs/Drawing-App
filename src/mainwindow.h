#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QDockWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QActionGroup>

class Canvas;
class LayerWidget;
class ColorPicker;
class ToolOptionsWidget;
class Tool;
class CommandHistory;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    // File menu
    void newDocument();
    void openDocument();
    void saveDocument();
    void saveDocumentAs();
    void exportImage();
    void print();
    
    // Edit menu
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void selectAll();
    void deselectAll();
    
    // View menu
    void zoomIn();
    void zoomOut();
    void zoomFit();
    void zoomActual();
    void toggleGrid();
    void toggleRulers();
    
    // Image menu
    void resizeCanvas();
    void cropCanvas();
    void rotateImage();
    void flipHorizontal();
    void flipVertical();
    
    // Layer menu
    void newLayer();
    void duplicateLayer();
    void deleteLayer();
    void mergeDown();
    void flattenImage();
    
    // Filter menu
    void applyBlur();
    void applySharpen();
    void adjustBrightness();
    void adjustContrast();
    void adjustHueSaturation();
    
    // Tools menu
    void showPreferences();
    
    // Help menu
    void showAbout();
    void showHelp();
    
    // Tool selection
    void selectTool();
    
    // Update UI
    void updateTitle();
    void updateStatusBar();
    void updateUndoRedoActions();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createDockWidgets();
    void createStatusBar();
    void setupConnections();
    void loadSettings();
    void saveSettings();
    
    // Central widget
    Canvas *m_canvas;
    
    // Dock widgets
    QDockWidget *m_layerDock;
    QDockWidget *m_colorDock;
    QDockWidget *m_toolOptionsDock;
    
    LayerWidget *m_layerWidget;
    ColorPicker *m_colorPicker;
    ToolOptionsWidget *m_toolOptionsWidget;
    
    // Toolbars
    QToolBar *m_fileToolBar;
    QToolBar *m_editToolBar;
    QToolBar *m_toolsToolBar;
    QToolBar *m_viewToolBar;
    
    // Command history
    CommandHistory *m_commandHistory;
    
    // Actions - File
    QAction *m_newAction;
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_saveAsAction;
    QAction *m_exportAction;
    QAction *m_printAction;
    QAction *m_exitAction;
    
    // Actions - Edit
    QAction *m_undoAction;
    QAction *m_redoAction;
    QAction *m_cutAction;
    QAction *m_copyAction;
    QAction *m_pasteAction;
    QAction *m_selectAllAction;
    QAction *m_deselectAction;
    
    // Actions - View
    QAction *m_zoomInAction;
    QAction *m_zoomOutAction;
    QAction *m_zoomFitAction;
    QAction *m_zoomActualAction;
    QAction *m_gridAction;
    QAction *m_rulersAction;
    
    // Actions - Image
    QAction *m_resizeAction;
    QAction *m_cropAction;
    QAction *m_rotateAction;
    QAction *m_flipHAction;
    QAction *m_flipVAction;
    
    // Actions - Layer
    QAction *m_newLayerAction;
    QAction *m_duplicateLayerAction;
    QAction *m_deleteLayerAction;
    QAction *m_mergeDownAction;
    QAction *m_flattenAction;
    
    // Actions - Tools
    QActionGroup *m_toolActionGroup;
    QAction *m_brushAction;
    QAction *m_penAction;
    QAction *m_pencilAction;
    QAction *m_eraserAction;
    QAction *m_lineAction;
    QAction *m_rectAction;
    QAction *m_ellipseAction;
    QAction *m_polygonAction;
    QAction *m_selectAction;
    QAction *m_textAction;
    QAction *m_fillAction;
    QAction *m_sprayAction;
    QAction *m_calligraphyAction;
    
    // Actions - Filters
    QAction *m_blurAction;
    QAction *m_sharpenAction;
    QAction *m_brightnessAction;
    QAction *m_contrastAction;
    QAction *m_hueSatAction;
    
    // Actions - Settings
    QAction *m_preferencesAction;
    
    // Actions - Help
    QAction *m_aboutAction;
    QAction *m_helpAction;
    
    // Current file path
    QString m_currentFilePath;
    bool m_isModified;
};

#endif // MAINWINDOW_H
