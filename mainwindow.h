#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QLabel;
class FindDialog;
class GoToCellDialog;
class SortDialog;
//class Spreadsheet;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void open();
    void openRecentFile();
    bool save();
    bool saveAs();
    void cut();
    void copy();
    void paste();
    void editDelete();
    void find();
    void goToCell();
    void sort();
    void selectRow();
    void selectColumn();
    void selectAll();
    void setShowGrid(bool x);
    void about();
//    void updateStatusBar();
private:
//    void spreadsheetModified();
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool okToContinue();
    bool loadFile(const QString& fileName);
    bool saveFile(const QString& fileName);
    void setCurrentFile(const QString& fileName);
    void updateRecentFileActions();
    QString strippedName(const QString& fullFileName);

    FindDialog *findDialog;

    enum { MaxRecentFiles = 5 };
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;

//    Spreadsheet *spreadsheet;
    QLabel *locationLabel;
    QLabel *formulaLabel;
    QStringList recentFiles;
    QString curFile;


    QMenu *fileMenu, *editMenu, *selectSubMenu, *optionsMenu, *helpMenu;
    QToolBar *fileToolBar, *editToolBar;
    QAction *newAction, *openAction, *saveAction, *saveAsAction;
    QAction *cutAction, *copyAction, *pasteAction, *deleteAction;
    QAction *selectRowAction, *selectColumnAction, *selectAllAction;
    QAction *findAction, *goToCellAction, *sortAction;
    QAction *aboutAction;
    QAction *exitAction;
    QAction *showGridAction;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
