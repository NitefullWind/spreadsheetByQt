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
    setAttribute(Qt::WA_DeleteOnClose);     //设置窗口关闭时删除对象，防止内存泄露
/*
    spreadsheet = new Spreadsheet;
    setCentralWidget(spreadsheet);
*/
    createActions();
    createMenus();
//    createContextMenu();
    createToolBars();
//    createStatusBar();

    readSettings();

    findDialog = 0;
    setWindowIcon(QIcon(":images/icon.jpg"));
    setCurrentFile("");
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

    closeAction = new QAction(tr("&Close"),this);
    closeAction->setShortcut(QKeySequence::Close);
    closeAction->setStatusTip(tr("Close this window"));
    connect(closeAction,SIGNAL(triggered()),this,SLOT(close()));

    //退出程序。明确指定键序列（快捷键）
    exitAction = new QAction(tr("E&xit"),this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction,SIGNAL(triggered()),qApp,SLOT(closeAllWindows()));

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

    aboutAction = new QAction(tr("About Speadsheet"),this);
    aboutAction->setStatusTip(tr("Show the balabala"));
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(about()));
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

    separatorAction = fileMenu->addSeparator();     //插入一个间隔器(separator)。使用指针可以操作其隐藏或显示。
    for(int i=0; i<MaxRecentFiles; i++)
        fileMenu->addAction(recentFileActions[i]);
    fileMenu->addSeparator();

    fileMenu->addAction(closeAction);
    fileMenu->addAction(exitAction);

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

void MainWindow::newFile()
{
    MainWindow *mainWin = new MainWindow;
    mainWin->show();
}
bool MainWindow::okToContinue()
{
    if(isWindowModified()){
        int t = QMessageBox::warning(this,tr("Spreadsheet"),
                                     tr("The document has been modified.\n"
                                        "Do you want to save your change?"),
                                     QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(t == QMessageBox::Yes){
            return save();
        }
        else if (t == QMessageBox::No){
            return false();
        }
    }
    return true;
}

void MainWindow::open()
{
    if(okToContinue()){
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open Spreadsheet"),".",
                                                        tr("Spreadsheet files (*.sp)\n"
                                                           "Comma-separated values files (*.csv)\n"
                                                           "Lotus 1-2-3 files (*.wkl *.wks)"));
        if(!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::loadFile(const QString &fileName)
{
    statusBar()->showMessage(tr("File loaded"),2000);
    setCurrentFile(fileName);       //设置窗口标题
    return false;
}

void MainWindow::openRecentFile()
{
    if(okToContinue()){
        QAction* action = qobject_cast<QAction *>(sender());    //将QObeject::sender指针强制转换为QAction指针
        if(action)                                              //不能转换时返回0
            loadFile(action->data().toString());                //data()提取文件全名（与setData()对应)
    }
}
bool MainWindow::save()
{
    if(curFile.isEmpty()){
        return saveAs();
    }else{
        return saveFile(curFile);
    }
}
bool MainWindow::saveFile(const QString &fileName)
{
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"),2000);
    return true;
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Saave Spreadsheet"),".",
                                                    tr("Spreadsheet files (*.sp)"));
    if(fileName.isEmpty())
        return false;
    return saveFile(fileName);
}
//重写close事件，可选择接受或忽略
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(okToContinue()){
        event->accept();
        writeSettings();
    }else{
        event->ignore();
    }
}

void MainWindow::find()
{
    if(!findDialog)
    {
        findDialog = new FindDialog(this);
    }
    findDialog->show();
    findDialog->raise();        //设置为顶层窗口
    findDialog->activateWindow();   //设置为激活状态
}
void MainWindow::goToCell()
{
    GoToCellDialog *goToCellDialog = new GoToCellDialog;
    if(goToCellDialog->exec()){       //设置为模态的,如果对话框被接受，返回true。
        QString str = goToCellDialog->lineEdit->text().toUpper();
        statusBar()->showMessage(tr("跳转到：%1").arg(str),2000);
    }
    delete goToCellDialog;
}
void MainWindow::sort()
{
    SortDialog dialog(this);
    if(dialog.exec()){
        statusBar()->showMessage(tr("排序"),2000);
    }
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName = tr("Untitled");
    if(!curFile.isEmpty()){
        shownName = strippedName(curFile);
        recentFiles.removeAll(curFile);     //将已有的curFile从链表中移除
        recentFiles.prepend(curFile);       //重新放入链表
        foreach (QWidget *win, QApplication::topLevelWidgets()){
            if(MainWindow *mainWin = qobject_cast<MainWindow *>(win))
                mainWin->updateRecentFileActions();          //更新“最近打开的文档”
        }
    }
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Spreadsheet")));
}
QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();      //去掉文件路径返回文件名
}
void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);      //设置一个链表指针，
    while(i.hasNext()){                             //如果不是文件名则从链表中移除
        if(!QFile::exists(i.next()))
            i.remove();
    }
    for(int j=0; j<MaxRecentFiles;++j){
        if(j<recentFiles.count()){
            QString text = tr("&%1 %2").arg(j+1).arg(strippedName(recentFiles[j]));
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        }else{
            recentFileActions[j]->setVisible(false);
        }
    }
    separatorAction->setVisible(!recentFiles.isEmpty());
}
void MainWindow::about()
{
    QMessageBox::about(this,tr("About Spreadsheet"),
                       tr("<h2>Spreadsheet 1.1</h2>"
                          "<p>Copyright &copy; 2008 Software Inc."
                          "<p>Spreadsheet is a small application that "
                          "demonstrates QAction, QMainWindow, QMenueBar,"
                          "and many other Qt classes"));
}
//windows平台的设置信息存储在注册表中
void MainWindow::writeSettings()
{
    QSettings settings("Software Inc.","Spreadsheet");              //组织的名字和程序的名字
    settings.setValue("recentFiles",recentFiles);
    settings.setValue("showGrid",showGridAction->isChecked());
}
void MainWindow::readSettings()
{
    QSettings settings("Software Inc.","Spreadsheet");

    recentFiles = settings.value("recentFiles").toStringList();
    foreach (QWidget *win, QApplication::topLevelWidgets()){        //每个窗口都调用updateRecentFileActions函数
        if(MainWindow *mainWin = qobject_cast<MainWindow *>(win))
            mainWin->updateRecentFileActions();          //更新“最近打开的文档”
    }

    bool showGrid = settings.value("showGrid",true).toBool();       //没设置的情况下指定默认值为true
    showGridAction->setChecked(showGrid);
}

QStringList MainWindow::recentFiles = QStringList();

void MainWindow::selectAll(){}
void MainWindow::selectRow(){}
void MainWindow::selectColumn(){}
void MainWindow::setShowGrid(bool x){x=true;}
void MainWindow::cut(){}
void MainWindow::copy(){}
void MainWindow::paste(){}
void MainWindow::editDelete(){}
MainWindow::~MainWindow()
{
    delete ui;
}

