#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLineEdit>
#include <vector>
#include "cell.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QGridLayout* grid;
    QLineEdit* input;
    std::vector<Cell> cells;
    std::vector<Cell*> noGivens;

    QLabel* test;

    explicit MainWindow(QWidget *parent = nullptr);
    void visual();
    void setUpCells();
    ~MainWindow();

public slots:
    void inputChanged(const QString&);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
