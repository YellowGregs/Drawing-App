#include "mainwindow.h"
#include "canvas.h"
#include "layermanager.h"
#include "widgets/layerwidget.h"
#include "colorpicker.h"
#include "widgets/tooloptionswidget.h"
#include "commandhistory.h"
#include "dialogs/newdocumentdialog.h"
#include "dialogs/preferencesdialog.h"
#include "dialogs/aboutdialog.h"
#include "tools/brushtool.h"
#include "tools/pentool.h"
#include "tools/penciltool.h"
#include "tools/erasertool.h"
#include "tools/linetool.h"
#include "tools/rectangletool.h"
#include "tools/ellipsetool.h"
#include "tools/polygontool.h"
#include "tools/selecttool.h"
#include "tools/texttool.h"
#include "tools/filltool.h"
#include "tools/spraytool.h"
#include "tools/calligraphytool.h"
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLabel>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_canvas(nullptr)
    , m_commandHistory(nullptr)
    , m_isModified(false)
{
    setWindowTitle("Drawing App");
    resize(1400, 900);
    
    m_canvas = new Canvas(this);
    setCentralWidget(m_canvas);
    
    m_commandHistory = new CommandHistory(this);
    m_canvas->setCommandHistory(m_commandHistory);
    
    createActions();
    createMenus();
    createToolBars();
    createDockWidgets();
    createStatusBar();
    setupConnections();

    loadSettings();
    
    m_brushAction->setChecked(true);
    selectTool();
    
    updateTitle();
    updateUndoRedoActions();
}

MainWindow::~MainWindow()
{
    saveSettings();
}

void MainWindow::createActions()
{
    // File actions
    m_newAction = new QAction(QIcon(":/icons/new.png"), tr("&New"), this);
    m_newAction->setShortcut(QKeySequence::New);
    connect(m_newAction, &QAction::triggered, this, &MainWindow::newDocument);
    
    m_openAction = new QAction(QIcon(":/icons/open.png"), tr("&Open..."), this);
    m_openAction->setShortcut(QKeySequence::Open);
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openDocument);
    
    m_saveAction = new QAction(QIcon(":/icons/save.png"), tr("&Save"), this);
    m_saveAction->setShortcut(QKeySequence::Save);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveDocument);
    
    m_saveAsAction = new QAction(tr("Save &As..."), this);
    m_saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::saveDocumentAs);
    
    m_exportAction = new QAction(tr("&Export Image..."), this);
    connect(m_exportAction, &QAction::triggered, this, &MainWindow::exportImage);
    
    m_printAction = new QAction(QIcon(":/icons/print.png"), tr("&Print..."), this);
    m_printAction->setShortcut(QKeySequence::Print);
    connect(m_printAction, &QAction::triggered, this, &MainWindow::print);
    
    m_exitAction = new QAction(tr("E&xit"), this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);
    
    // Edit actions
    m_undoAction = new QAction(QIcon(":/icons/undo.png"), tr("&Undo"), this);
    m_undoAction->setShortcut(QKeySequence::Undo);
    connect(m_undoAction, &QAction::triggered, this, &MainWindow::undo);
    
    m_redoAction = new QAction(QIcon(":/icons/redo.png"), tr("&Redo"), this);
    m_redoAction->setShortcut(QKeySequence::Redo);
    connect(m_redoAction, &QAction::triggered, this, &MainWindow::redo);
    
    m_cutAction = new QAction(QIcon(":/icons/cut.png"), tr("Cu&t"), this);
    m_cutAction->setShortcut(QKeySequence::Cut);
    connect(m_cutAction, &QAction::triggered, this, &MainWindow::cut);
    
    m_copyAction = new QAction(QIcon(":/icons/copy.png"), tr("&Copy"), this);
    m_copyAction->setShortcut(QKeySequence::Copy);
    connect(m_copyAction, &QAction::triggered, this, &MainWindow::copy);
    
    m_pasteAction = new QAction(QIcon(":/icons/paste.png"), tr("&Paste"), this);
    m_pasteAction->setShortcut(QKeySequence::Paste);
    connect(m_pasteAction, &QAction::triggered, this, &MainWindow::paste);
    
    m_selectAllAction = new QAction(tr("Select &All"), this);
    m_selectAllAction->setShortcut(QKeySequence::SelectAll);
    connect(m_selectAllAction, &QAction::triggered, this, &MainWindow::selectAll);
    
    m_deselectAction = new QAction(tr("&Deselect"), this);
    m_deselectAction->setShortcut(Qt::CTRL | Qt::Key_D);
    connect(m_deselectAction, &QAction::triggered, this, &MainWindow::deselectAll);
    
    // View actions
    m_zoomInAction = new QAction(QIcon(":/icons/zoom-in.png"), tr("Zoom &In"), this);
    m_zoomInAction->setShortcut(QKeySequence::ZoomIn);
    connect(m_zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);
    
    m_zoomOutAction = new QAction(QIcon(":/icons/zoom-out.png"), tr("Zoom &Out"), this);
    m_zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    connect(m_zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);
    
    m_zoomFitAction = new QAction(tr("Zoom to &Fit"), this);
    m_zoomFitAction->setShortcut(Qt::CTRL | Qt::Key_0);
    connect(m_zoomFitAction, &QAction::triggered, this, &MainWindow::zoomFit);
    
    m_zoomActualAction = new QAction(tr("&Actual Size"), this);
    m_zoomActualAction->setShortcut(Qt::CTRL | Qt::Key_1);
    connect(m_zoomActualAction, &QAction::triggered, this, &MainWindow::zoomActual);
    
    m_gridAction = new QAction(tr("Show &Grid"), this);
    m_gridAction->setCheckable(true);
    connect(m_gridAction, &QAction::triggered, this, &MainWindow::toggleGrid);
    
    m_rulersAction = new QAction(tr("Show &Rulers"), this);
    m_rulersAction->setCheckable(true);
    connect(m_rulersAction, &QAction::triggered, this, &MainWindow::toggleRulers);
    
    // Tool actions
    m_toolActionGroup = new QActionGroup(this);
    
    m_brushAction = new QAction(QIcon(":/icons/brush.png"), tr("&Brush"), this);
    m_brushAction->setCheckable(true);
    m_brushAction->setShortcut(Qt::Key_B);
    m_toolActionGroup->addAction(m_brushAction);
    
    m_penAction = new QAction(QIcon(":/icons/pen.png"), tr("&Pen"), this);
    m_penAction->setCheckable(true);
    m_penAction->setShortcut(Qt::Key_P);
    m_toolActionGroup->addAction(m_penAction);
    
    m_pencilAction = new QAction(QIcon(":/icons/pencil.png"), tr("Pe&ncil"), this);
    m_pencilAction->setCheckable(true);
    m_pencilAction->setShortcut(Qt::Key_N);
    m_toolActionGroup->addAction(m_pencilAction);
    
    m_eraserAction = new QAction(QIcon(":/icons/eraser.png"), tr("&Eraser"), this);
    m_eraserAction->setCheckable(true);
    m_eraserAction->setShortcut(Qt::Key_E);
    m_toolActionGroup->addAction(m_eraserAction);
    
    m_lineAction = new QAction(QIcon(":/icons/line.png"), tr("&Line"), this);
    m_lineAction->setCheckable(true);
    m_lineAction->setShortcut(Qt::Key_L);
    m_toolActionGroup->addAction(m_lineAction);
    
    m_rectAction = new QAction(QIcon(":/icons/rectangle.png"), tr("&Rectangle"), this);
    m_rectAction->setCheckable(true);
    m_rectAction->setShortcut(Qt::Key_R);
    m_toolActionGroup->addAction(m_rectAction);
    
    m_ellipseAction = new QAction(QIcon(":/icons/ellipse.png"), tr("Elli&pse"), this);
    m_ellipseAction->setCheckable(true);
    m_ellipseAction->setShortcut(Qt::Key_O);
    m_toolActionGroup->addAction(m_ellipseAction);
    
    m_polygonAction = new QAction(QIcon(":/icons/polygon.png"), tr("Pol&ygon"), this);
    m_polygonAction->setCheckable(true);
    m_polygonAction->setShortcut(Qt::Key_Y);
    m_toolActionGroup->addAction(m_polygonAction);
    
    m_selectAction = new QAction(QIcon(":/icons/select.png"), tr("&Select"), this);
    m_selectAction->setCheckable(true);
    m_selectAction->setShortcut(Qt::Key_S);
    m_toolActionGroup->addAction(m_selectAction);
    
    m_textAction = new QAction(QIcon(":/icons/text.png"), tr("&Text"), this);
    m_textAction->setCheckable(true);
    m_textAction->setShortcut(Qt::Key_T);
    m_toolActionGroup->addAction(m_textAction);
    
    m_fillAction = new QAction(QIcon(":/icons/fill.png"), tr("&Fill"), this);
    m_fillAction->setCheckable(true);
    m_fillAction->setShortcut(Qt::Key_F);
    m_toolActionGroup->addAction(m_fillAction);
    
    m_sprayAction = new QAction(QIcon(":/icons/spray.png"), tr("S&pray"), this);
    m_sprayAction->setCheckable(true);
    m_sprayAction->setShortcut(Qt::Key_A);
    m_toolActionGroup->addAction(m_sprayAction);
    
    m_calligraphyAction = new QAction(QIcon(":/icons/calligraphy.png"), tr("&Calligraphy"), this);
    m_calligraphyAction->setCheckable(true);
    m_calligraphyAction->setShortcut(Qt::Key_C);
    m_toolActionGroup->addAction(m_calligraphyAction);
    
    connect(m_toolActionGroup, &QActionGroup::triggered, this, &MainWindow::selectTool);
    
    // Layer actions
    m_newLayerAction = new QAction(QIcon(":/icons/layer-new.png"), tr("&New Layer"), this);
    m_newLayerAction->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_N);
    connect(m_newLayerAction, &QAction::triggered, this, &MainWindow::newLayer);
    
    m_duplicateLayerAction = new QAction(tr("&Duplicate Layer"), this);
    connect(m_duplicateLayerAction, &QAction::triggered, this, &MainWindow::duplicateLayer);
    
    m_deleteLayerAction = new QAction(QIcon(":/icons/layer-delete.png"), tr("D&elete Layer"), this);
    connect(m_deleteLayerAction, &QAction::triggered, this, &MainWindow::deleteLayer);
    
    m_mergeDownAction = new QAction(tr("&Merge Down"), this);
    m_mergeDownAction->setShortcut(Qt::CTRL | Qt::Key_E);
    connect(m_mergeDownAction, &QAction::triggered, this, &MainWindow::mergeDown);
    
    m_flattenAction = new QAction(tr("&Flatten Image"), this);
    connect(m_flattenAction, &QAction::triggered, this, &MainWindow::flattenImage);
    
    // Preferences and Help
    m_preferencesAction = new QAction(tr("&Preferences..."), this);
    connect(m_preferencesAction, &QAction::triggered, this, &MainWindow::showPreferences);
    
    m_aboutAction = new QAction(tr("&About"), this);
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    
    m_helpAction = new QAction(tr("&Help"), this);
    m_helpAction->setShortcut(QKeySequence::HelpContents);
    connect(m_helpAction, &QAction::triggered, this, &MainWindow::showHelp);
}

void MainWindow::createMenus()
{
    QMenuBar *menuBar = this->menuBar();
    
    // File menu
    QMenu *fileMenu = menuBar->addMenu(tr("&File"));
    fileMenu->addAction(m_newAction);
    fileMenu->addAction(m_openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_saveAction);
    fileMenu->addAction(m_saveAsAction);
    fileMenu->addAction(m_exportAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_printAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_exitAction);
    
    // Edit menu
    QMenu *editMenu = menuBar->addMenu(tr("&Edit"));
    editMenu->addAction(m_undoAction);
    editMenu->addAction(m_redoAction);
    editMenu->addSeparator();
    editMenu->addAction(m_cutAction);
    editMenu->addAction(m_copyAction);
    editMenu->addAction(m_pasteAction);
    editMenu->addSeparator();
    editMenu->addAction(m_selectAllAction);
    editMenu->addAction(m_deselectAction);
    
    // View menu
    QMenu *viewMenu = menuBar->addMenu(tr("&View"));
    viewMenu->addAction(m_zoomInAction);
    viewMenu->addAction(m_zoomOutAction);
    viewMenu->addAction(m_zoomFitAction);
    viewMenu->addAction(m_zoomActualAction);
    viewMenu->addSeparator();
    viewMenu->addAction(m_gridAction);
    viewMenu->addAction(m_rulersAction);
    
    // Layer menu
    QMenu *layerMenu = menuBar->addMenu(tr("&Layer"));
    layerMenu->addAction(m_newLayerAction);
    layerMenu->addAction(m_duplicateLayerAction);
    layerMenu->addAction(m_deleteLayerAction);
    layerMenu->addSeparator();
    layerMenu->addAction(m_mergeDownAction);
    layerMenu->addAction(m_flattenAction);
    
    // Tools menu
    QMenu *toolsMenu = menuBar->addMenu(tr("&Tools"));
    toolsMenu->addAction(m_brushAction);
    toolsMenu->addAction(m_penAction);
    toolsMenu->addAction(m_pencilAction);
    toolsMenu->addAction(m_eraserAction);
    toolsMenu->addSeparator();
    toolsMenu->addAction(m_lineAction);
    toolsMenu->addAction(m_rectAction);
    toolsMenu->addAction(m_ellipseAction);
    toolsMenu->addAction(m_polygonAction);
    toolsMenu->addSeparator();
    toolsMenu->addAction(m_selectAction);
    toolsMenu->addAction(m_textAction);
    toolsMenu->addAction(m_fillAction);
    toolsMenu->addSeparator();
    toolsMenu->addAction(m_sprayAction);
    toolsMenu->addAction(m_calligraphyAction);
    toolsMenu->addSeparator();
    toolsMenu->addAction(m_preferencesAction);
    
    // Help menu
    QMenu *helpMenu = menuBar->addMenu(tr("&Help"));
    helpMenu->addAction(m_helpAction);
    helpMenu->addSeparator();
    helpMenu->addAction(m_aboutAction);
}

void MainWindow::createToolBars()
{
    // File toolbar
    m_fileToolBar = addToolBar(tr("File"));
    m_fileToolBar->setObjectName("FileToolBar");
    m_fileToolBar->addAction(m_newAction);
    m_fileToolBar->addAction(m_openAction);
    m_fileToolBar->addAction(m_saveAction);
    
    // Edit toolbar
    m_editToolBar = addToolBar(tr("Edit"));
    m_editToolBar->setObjectName("EditToolBar");
    m_editToolBar->addAction(m_undoAction);
    m_editToolBar->addAction(m_redoAction);
    
    // Tools toolbar
    m_toolsToolBar = addToolBar(tr("Tools"));
    m_toolsToolBar->setObjectName("ToolsToolBar");
    m_toolsToolBar->addAction(m_brushAction);
    m_toolsToolBar->addAction(m_penAction);
    m_toolsToolBar->addAction(m_pencilAction);
    m_toolsToolBar->addAction(m_eraserAction);
    m_toolsToolBar->addSeparator();
    m_toolsToolBar->addAction(m_lineAction);
    m_toolsToolBar->addAction(m_rectAction);
    m_toolsToolBar->addAction(m_ellipseAction);
    m_toolsToolBar->addAction(m_polygonAction);
    m_toolsToolBar->addSeparator();
    m_toolsToolBar->addAction(m_selectAction);
    m_toolsToolBar->addAction(m_textAction);
    m_toolsToolBar->addAction(m_fillAction);
    m_toolsToolBar->addSeparator();
    m_toolsToolBar->addAction(m_sprayAction);
    m_toolsToolBar->addAction(m_calligraphyAction);
    
    // View toolbar
    m_viewToolBar = addToolBar(tr("View"));
    m_viewToolBar->setObjectName("ViewToolBar");
    m_viewToolBar->addAction(m_zoomInAction);
    m_viewToolBar->addAction(m_zoomOutAction);
}

void MainWindow::createDockWidgets()
{
    // Layer dock widget
    m_layerDock = new QDockWidget(tr("Layers"), this);
    m_layerDock->setObjectName("LayerDock");
    m_layerWidget = new LayerWidget(m_canvas->getLayerManager(), this);
    m_layerDock->setWidget(m_layerWidget);
    addDockWidget(Qt::RightDockWidgetArea, m_layerDock);
    
    // Color picker dock widget
    m_colorDock = new QDockWidget(tr("Colors"), this);
    m_colorDock->setObjectName("ColorDock");
    m_colorPicker = new ColorPicker(this);
    m_colorDock->setWidget(m_colorPicker);
    addDockWidget(Qt::RightDockWidgetArea, m_colorDock);
    
    // Tool options dock widget
    m_toolOptionsDock = new QDockWidget(tr("Tool Options"), this);
    m_toolOptionsDock->setObjectName("ToolOptionsDock");
    m_toolOptionsWidget = new ToolOptionsWidget(this);
    m_toolOptionsDock->setWidget(m_toolOptionsWidget);
    addDockWidget(Qt::LeftDockWidgetArea, m_toolOptionsDock);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::setupConnections()
{
    connect(m_canvas, &Canvas::modified, this, [this]() {
        m_isModified = true;
        updateTitle();
    });
    
    connect(m_commandHistory, &CommandHistory::canUndoChanged, this, &MainWindow::updateUndoRedoActions);
    connect(m_commandHistory, &CommandHistory::canRedoChanged, this, &MainWindow::updateUndoRedoActions);
    connect(m_commandHistory, &CommandHistory::commandExecuted, m_canvas, 
            static_cast<void (QWidget::*)()>(&QWidget::update));
    
    connect(m_colorPicker, &ColorPicker::colorChanged, m_canvas, &Canvas::setPrimaryColor);
    connect(m_canvas, &Canvas::cursorPositionChanged, this, &MainWindow::updateStatusBar);
    connect(m_canvas, &Canvas::zoomChanged, this, [this](qreal zoom) {
        statusBar()->showMessage(tr("Zoom: %1%").arg(zoom * 100, 0, 'f', 0), 2000);
    });
    
    connect(m_canvas->getLayerManager(), &LayerManager::layersChanged, m_canvas, 
            static_cast<void (QWidget::*)()>(&QWidget::update));
    connect(m_canvas->getLayerManager(), &LayerManager::currentLayerChanged, m_canvas,
            static_cast<void (QWidget::*)()>(&QWidget::update));
}

void MainWindow::newDocument()
{
    NewDocumentDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        m_canvas->newDocument(dialog.width(), dialog.height(), dialog.backgroundColor());
        m_currentFilePath.clear();
        m_isModified = false;
        updateTitle();
    }
}

void MainWindow::openDocument()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Document"),
        QString(), tr("Images (*.png *.jpg *.bmp);;All Files (*)"));
    
    if (!fileName.isEmpty()) {
        if (m_canvas->loadDocument(fileName)) {
            m_currentFilePath = fileName;
            m_isModified = false;
            updateTitle();
        }
    }
}

void MainWindow::saveDocument()
{
    if (m_currentFilePath.isEmpty()) {
        saveDocumentAs();
    } else {
        if (m_canvas->saveDocument(m_currentFilePath)) {
            m_isModified = false;
            updateTitle();
        }
    }
}

void MainWindow::saveDocumentAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Document"),
        QString(), tr("PNG Image (*.png);;JPEG Image (*.jpg);;BMP Image (*.bmp)"));
    
    if (!fileName.isEmpty()) {
        if (m_canvas->saveDocument(fileName)) {
            m_currentFilePath = fileName;
            m_isModified = false;
            updateTitle();
        }
    }
}

void MainWindow::exportImage() { saveDocumentAs(); }
void MainWindow::print() { /* TODO: Implement printing */ }

// Edit menu 
void MainWindow::undo() { m_commandHistory->undo(); }
void MainWindow::redo() { m_commandHistory->redo(); }
void MainWindow::cut() { /* TODO */ }
void MainWindow::copy() { /* TODO */ }
void MainWindow::paste() { /* TODO */ }
void MainWindow::selectAll() { /* TODO */ }
void MainWindow::deselectAll() { m_canvas->clearSelection(); }

// View menu 
void MainWindow::zoomIn() { m_canvas->zoomIn(); }
void MainWindow::zoomOut() { m_canvas->zoomOut(); }
void MainWindow::zoomFit() { m_canvas->zoomToFit(); }
void MainWindow::zoomActual() { m_canvas->zoomToActual(); }
void MainWindow::toggleGrid() { m_canvas->setGridVisible(m_gridAction->isChecked()); }
void MainWindow::toggleRulers() { m_canvas->setRulersVisible(m_rulersAction->isChecked()); }

// Image menu (stubs)
void MainWindow::resizeCanvas() { /* TODO */ }
void MainWindow::cropCanvas() { /* TODO */ }
void MainWindow::rotateImage() { /* TODO */ }
void MainWindow::flipHorizontal() { /* TODO */ }
void MainWindow::flipVertical() { /* TODO */ }

// Layer menu
void MainWindow::newLayer() { m_canvas->getLayerManager()->addLayer(); }
void MainWindow::duplicateLayer() { m_canvas->getLayerManager()->duplicateCurrentLayer(); }
void MainWindow::deleteLayer() { m_canvas->getLayerManager()->deleteCurrentLayer(); }
void MainWindow::mergeDown() { m_canvas->getLayerManager()->mergeDown(); }
void MainWindow::flattenImage() { m_canvas->getLayerManager()->flattenImage(); }

// Filter menu (stubs)
void MainWindow::applyBlur() { /* TODO */ }
void MainWindow::applySharpen() { /* TODO */ }
void MainWindow::adjustBrightness() { /* TODO */ }
void MainWindow::adjustContrast() { /* TODO */ }
void MainWindow::adjustHueSaturation() { /* TODO */ }

// Settings and Help
void MainWindow::showPreferences()
{
    PreferencesDialog dialog(this);
    dialog.exec();
}

void MainWindow::showAbout()
{
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::showHelp()
{
    QMessageBox::information(this, tr("Help"),
        tr("Drawing App v1.0\n\n"
           "Tool Shortcuts:\n"
           "B - Brush Tool\n"
           "P - Pen Tool\n"
           "N - Pencil Tool\n"
           "E - Eraser Tool\n"
           "L - Line Tool\n"
           "R - Rectangle Tool\n"
           "O - Ellipse Tool\n"
           "S - Select Tool\n"
           "T - Text Tool\n"
           "F - Fill Tool\n"
           "A - Spray Tool\n"
           "C - Calligraphy Tool\n\n"
           "File Operations:\n"
           "Ctrl+N - New Document\n"
           "Ctrl+O - Open\n"
           "Ctrl+S - Save\n"
           "Ctrl+Shift+S - Save As\n\n"
           "Edit Operations:\n"
           "Ctrl+Z - Undo\n"
           "Ctrl+Y - Redo\n"
           "Ctrl+D - Deselect\n\n"
           "View Operations:\n"
           "Ctrl++ - Zoom In\n"
           "Ctrl+- - Zoom Out\n"
           "Ctrl+0 - Zoom to Fit\n"
           "Ctrl+1 - Actual Size\n"
           "Ctrl+Wheel - Zoom\n"
           "Shift+Drag / Middle Mouse - Pan\n\n"
           "Layer Operations:\n"
           "Ctrl+Shift+N - New Layer\n"
           "Ctrl+E - Merge Down\n\n"
           "Brush Adjustments:\n"
           "[ - Decrease Brush Size\n"
           "] - Increase Brush Size"));
}

void MainWindow::selectTool()
{
    QAction *action = qobject_cast<QAction*>(sender());
    
    if (!action) {
        action = m_toolActionGroup->checkedAction();
    }
    
    if (!action) return;
    
    Tool *tool = nullptr;
    
    if (action == m_brushAction) tool = new BrushTool(m_canvas);
    else if (action == m_penAction) tool = new PenTool(m_canvas);
    else if (action == m_pencilAction) tool = new PencilTool(m_canvas);
    else if (action == m_eraserAction) tool = new EraserTool(m_canvas);
    else if (action == m_lineAction) tool = new LineTool(m_canvas);
    else if (action == m_rectAction) tool = new RectangleTool(m_canvas);
    else if (action == m_ellipseAction) tool = new EllipseTool(m_canvas);
    else if (action == m_polygonAction) tool = new PolygonTool(m_canvas);
    else if (action == m_selectAction) tool = new SelectTool(m_canvas);
    else if (action == m_textAction) tool = new TextTool(m_canvas);
    else if (action == m_fillAction) tool = new FillTool(m_canvas);
    else if (action == m_sprayAction) tool = new SprayTool(m_canvas);
    else if (action == m_calligraphyAction) tool = new CalligraphyTool(m_canvas);
    
    if (tool) {
        m_canvas->setCurrentTool(tool);
        m_toolOptionsWidget->setTool(tool);
        statusBar()->showMessage(tr("Tool: %1").arg(tool->getName()), 2000);
    }
}

void MainWindow::updateTitle()
{
    QString title = "Drawing App";
    if (!m_currentFilePath.isEmpty()) {
        QFileInfo fileInfo(m_currentFilePath);
        title = fileInfo.fileName() + " - " + title;
    } else {
        title = "Untitled - " + title;
    }
    if (m_isModified) {
        title += " *";
    }
    setWindowTitle(title);
}

void MainWindow::updateStatusBar()
{
    QPoint pos = m_canvas->getCursorPosition();
    qreal zoom = m_canvas->getZoomLevel();
    statusBar()->showMessage(tr("Position: %1, %2 | Zoom: %3%")
        .arg(pos.x()).arg(pos.y()).arg(zoom * 100, 0, 'f', 0));
}

void MainWindow::updateUndoRedoActions()
{
    m_undoAction->setEnabled(m_commandHistory->canUndo());
    m_redoAction->setEnabled(m_commandHistory->canRedo());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_isModified) {
        QMessageBox::StandardButton reply = QMessageBox::question(this,
            tr("Unsaved Changes"),
            tr("Do you want to save your changes?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        
        if (reply == QMessageBox::Save) {
            saveDocument();
            if (m_isModified) {
                event->ignore();
                return;
            }
        } else if (reply == QMessageBox::Cancel) {
            event->ignore();
            return;
        }
    }
    
    saveSettings();
    event->accept();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_BracketLeft) {
        Tool *tool = m_canvas->getCurrentTool();
        if (tool) {
            int newSize = qMax(1, tool->getSize() - 5);
            tool->setSize(newSize);
            m_toolOptionsWidget->setTool(tool);
            statusBar()->showMessage(tr("Brush Size: %1").arg(newSize), 1000);
            m_canvas->update();
        }
        event->accept();
    } else if (event->key() == Qt::Key_BracketRight) {
        Tool *tool = m_canvas->getCurrentTool();
        if (tool) {
            int newSize = qMin(500, tool->getSize() + 5);
            tool->setSize(newSize);
            m_toolOptionsWidget->setTool(tool);
            statusBar()->showMessage(tr("Brush Size: %1").arg(newSize), 1000);
            m_canvas->update();
        }
        event->accept();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::loadSettings()
{
    QSettings settings("DrawingApp", "DrawingApp");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}

void MainWindow::saveSettings()
{
    QSettings settings("DrawingApp", "DrawingApp");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
}
