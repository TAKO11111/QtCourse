#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionabout_triggered();

    void on_checkaction_triggered();

    void on_repacesaction_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_actionEdit_triggered();

    void on_actionUndo_triggered();

    void on_actioncopy_triggered();

    void on_actionCut_triggered();

    void on_actionpaste_triggered();



    void on_actionredo_triggered();

    void on_TextEdit_copyAvailable(bool b);

    void on_TextEdit_redoAvailable(bool b);

    void on_TextEdit_undoAvailable(bool b);

    void on_action_color_triggered();

    void on_actionbeicolor_triggered();

    void on_actionzibeicolor_triggered();

    void on_actionwrap_triggered();

    void on_actionFont_triggered();

    void on_actionstatus_triggered();

    void on_actiontools_triggered();

    void on_actionselectall_triggered();

    void on_TextEdit_textChanged();

    void on_TextEdit_cursorPositionChanged();


    void on_actionnum_triggered(bool checked);

private:
    Ui::MainWindow *ui;
    QLabel statusCursorLabel;
    QLabel statuLabel;

    QString filePath;
    bool textChange;
};
#endif // MAINWINDOW_H
