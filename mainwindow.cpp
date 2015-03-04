#include <QtWidgets>
#include "finddialog.h"
#include "gotocelldialog.h"
#include "sortdialog.h"
#include "spreadsheet.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
/*
    spreadsheet = new Spreadsheet;
    setCentralWidget(spreadsheet);
*/
    createActions();
    createMenus();
//    createContextMenu();
    createToolBars();
//    createStatusBar();

//    readSettings();

//    findDialog = 0;
    setWindowIcon(QIcon(":images/icon.jpg"));
//    setCurrentFile("");
}

//创建动作
void MainWindow::createActions()
{
    //file
    newAction = new QAction(tr("&New"),this);
    newAction->setIcon(QIcon(":/images/images/filenew.png"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Creat a new spreadsheet file"));
    connect(newAction,SIGNAL(triggered()),this,SLOT(newFile()));

    openAction = new QAction(tr("&Open"),this);
    openAction->setIcon(QIcon(":/images/images/fileopen.png"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open a spreadsheet file"));
    connect(openAction,SIGNAL(triggered()),this,SLOT(open()));

    saveAction = new QAction(tr("&Save"),this);
    saveAction->setIcon(QIcon(":images/images/filesave.png"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the file"));
    connect(saveAction,SIGNAL(triggered()),this,SLOT(save()));

    saveAsAction = new QAction(tr("Save as ..."),this);
    saveAsAction->setIcon(QIcon(":images/images/filesaveas.png"));
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save the file as ..."));
    connect(saveAsAction,SIGNAL(triggered()),this,SLOT(saveAs()));

    //最近打开的文件
    for(int i=0; i<MaxRecentFiles; i++) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i],SIGNAL(triggered()),this,SLOT(openRecentFile()));
    }

    //退出程序。明确指定键序列（快捷键）
    exitAction = new QAction(tr("E&xit"),this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

    //edit
    copyAction = new QAction(tr("Copy"),this);
    copyAction->setIcon(QIcon(":/images/images/editcopy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction,SIGNAL(triggered()),this,SLOT(copy()));

    cutAction = new QAction(tr("Cut"),this);
    cutAction->setIcon(QIcon(":/images/images/editcut.png"));
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction,SIGNAL(triggered()),this,SLOT(cut()));

    pasteAction = new QAction(tr("Paste"),this);
    pasteAction->setIcon(QIcon(":/images/images/editpaste.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction,SIGNAL(triggered()),this,SLOT(paste()));

    deleteAction = new QAction(tr("Delete"),this);
    deleteAction->setIcon(QIcon(":/images/images/editdelete.png"));
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(editDelete()));

    selectRowAction = new QAction(tr("&Row"),this);
    selectRowAction->setStatusTip(tr("Select one row of the cells in the " "spreadsheet"));
    connect(selectRowAction,SIGNAL(triggered()),this,SLOT(selectRow()));

    selectColumnAction = new QAction(tr("&Column"),this);
    selectColumnAction->setStatusTip(tr("Select one column of the cells in the " "spreadsheet"));
    connect(selectColumnAction,SIGNAL(triggered()),this,SLOT(selectColumn()));

    selectAllAction = new QAction(tr("&All"),this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    selectAllAction->setStatusTip(tr("Select all the cells in the " "spreadsheet"));
    connect(selectAllAction,SIGNAL(triggered()),this,SLOT(selectAll()));                //为测试

    findAction = new QAction(tr("&Find"),this);
    findAction->setShortcut(QKeySequence::Find);
    connect(findAction,SIGNAL(triggered()),this,SLOT(find()));

    goToCellAction = new QAction(tr("&Go To Cell"),this);
    connect(goToCellAction,SIGNAL(triggered()),this,SLOT(goToCell()));

    sortAction = new QAction(tr("Sort"),this);
    connect(sortAction,SIGNAL(triggered()),this,SLOT(sort()));

    //是否显示网格。复选框，切换。
    showGridAction = new QAction(tr("&Show Grid"),this);
    showGridAction->setCheckable(true);
    showGridAction->setChecked(true);   //spreadsheet->showGrid()
    showGridAction->setStatusTip(tr("Show or hide the spreadsheet's ""grid"));
    connect(showGridAction,SIGNAL(toggled(bool)),this,SLOT(setShowGrid(bool)));         //为测试

    aboutAction = new QAction(tr("About Me"),this);
    aboutAction->setStatusTip(tr("Show the balabala"));
    connect(aboutAction,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
}

//创建菜单
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    separatorAction = fileMenu->addSeparator();     //插入一个间隔器(separator)。使用指针可以操作其隐藏或显示。
    for(int i=0; i<MaxRecentFiles; i++)
        fileMenu->addAction(recentFileActions[i]);
    fileMenu->addSeparator();

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(copyAction);
    editMenu->addAction(cutAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(deleteAction);
    //添加子菜单
    selectSubMenu = editMenu->addMenu(tr("&Select"));
    selectSubMenu->addAction(selectRowAction);
    selectSubMenu->addAction(selectColumnAction);
    selectSubMenu->addAction(selectAllAction);
    editMenu->addSeparator();
    editMenu->addAction(findAction);
    editMenu->addAction(goToCellAction);
    editMenu->addAction(sortAction);

    optionsMenu = menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(showGridAction);

    menuBar()->addSeparator();      //菜单之间插入分割器，某些风格会把help菜单放到最右端
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
}

/*
//上下文菜单
void MainWindow::createContextMenu()
{
    spreadsheet->addAction(cutAction);
    spreadsheet->addAction(copyAction);
    spreadsheet->addAction(pasteAction);
    spreadsheet->setContexMenuePolicy(Qt::ActionsContextMenu);
}
*/
void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);

    editToolBar = addToolBar(tr("&Edit"));
    editToolBar->addAction(copyAction);
    editToolBar->addAction(cutAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(findAction);
    editToolBar->addAction(goToCellAction);
    editToolBar->addAction(sortAction);
}
/*
void MainWindow::createStatusBar()
{
    locationLabel = new QLabel("W999");
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());

    formulaLabel = new QLabel;
    formulaLabel->setIndent(3);

    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formulaLabel,1);

    connect(spreadsheet,SIGNAL(currentCellChanged(int,int,int,int)),
            this,SLOT(updateStatusBar()));
    connect(spreadsheet,SIGNAL(modified()),
            this,SLOT(spreadsheetModified()));
    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
    locationLabel->setText(spreadsheet->currentLocation());
    formulaLabel->setText(spreadsheet->currentFormula());
}
void MainWindow::spreadsheetModified()
{
    setWindowModified(true);
    updateStatusBar();
}
*/

void MainWindow::newFile(){}
void MainWindow::open(){}
void MainWindow::openRecentFile(){}
bool MainWindow::save(){return false;}
bool MainWindow::saveAs(){return false;}
void MainWindow::cut(){}
void MainWindow::copy(){}
void MainWindow::paste(){}
void MainWindow::editDelete(){}
void MainWindow::find()
{
    FindDialog *findDialog = new FindDialog;
    findDialog->show();
}
void MainWindow::goToCell()
{
    GoToCellDialog *goToCellDialog = new GoToCellDialog;
    goToCellDialog->show();
}
void MainWindow::sort()
{
    SortDialog *sortDialog = new SortDialog;
    sortDialog->show();
}
void MainWindow::selectAll(){}
void MainWindow::selectRow(){}
void MainWindow::selectColumn(){}
void MainWindow::setShowGrid(bool x){x=true;}
MainWindow::~MainWindow()
{
    delete ui;
}

