#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegExpValidator>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    delete ui->statusBar;
    this->setWindowTitle("Shitdoku Solver");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::visual()
{
    //setup grid layout
    grid = new QGridLayout;
    QWidget *windowCentralDock = new QWidget;
    windowCentralDock->setLayout(grid);
    this->setCentralWidget(windowCentralDock);

    //setup input
    input = new QLineEdit;
    grid->addWidget(input,0,0,1,11);
    input->setMaxLength(81);
    QRegExp inputRegex("[.1-9]{81}");
    QValidator *inputValidator = new QRegExpValidator(inputRegex, this);
    input->setValidator(inputValidator);
    connect(input,SIGNAL(textEdited(const QString&)),this,SLOT(inputChanged(const QString&)));

    //setup cells
    cells = std::vector<Cell>(81);
    int row = 1, col = 0;
    for (size_t i = 0; i < 81; i++)
    {
        if (i!=0 && i%9==0)
        {
            row++;
            col=0;
        }
        if (col==3 || col==7) col++;
        if (row==4 || row==8) row++;

        grid->addWidget(cells[i].cellLabel,row,col++,1,1);
    }
    //borders
    QLabel* borders;
    //horizontal
    for (int i = 1; i <= 2; i++)
    {
        borders = new QLabel;
        borders->setMaximumHeight(2);
        borders->setStyleSheet("QLabel{background-color:black};");
        grid->addWidget(borders,4*i,0,1,11);
    }
    //vertical
    for (int i = 1; i <= 2; i++)
    {
        borders = new QLabel;
        borders->setMaximumWidth(2);
        borders->setStyleSheet("QLabel{background-color:black};");
        grid->addWidget(borders,1,(4*i)-1,11,1);
    }

    //test label
    test = new QLabel;
    test->setText("TEST");
//    grid->addWidget(test,12,0,1,5);
}

void MainWindow::inputChanged(const QString& qss)
{
    if (qss.size()==81)
    {
        //read from input
        int inputValue;
        for (size_t c = 0; c < 81; c++)
        {
            inputValue = qss.at(static_cast<int>(c)).digitValue();
            if (inputValue!=-1)
            {
                cells[c].initial_set(inputValue);
            }
        }
        //setup cells neighbors
        setUpCells();
        //fill
        bool error = false;
        for (size_t g = 0; g < noGivens.size(); g = error ? g - 1 : g + 1)
        {
            error = !noGivens[g]->next_value();
        }

        //test label stuff
        test->setNum(static_cast<int>(noGivens.size()));
    }
}

void MainWindow::setUpCells()
{
    std::vector<Cell*> *used;
    int nextRow = 0, nextCol = 0, grid = 0, gridr = 0, gridc = 0;
    for (int ss = 0; ss < 81; ss++)
    {
        //horizontal
        if (ss != 0 && ss % 9 == 0)
        {
            nextRow += 9;
        }
        for (int row = 0 + nextRow; row < (9 + nextRow); row++)
        {
            if (row != ss)
            {
                //used.push_back(row);
                cells[ss].neighbors.push_back(&cells[row]);
                //cells[ss].addNeighbors(&cells[row]);
            }
        }

        //vertical
        if (ss != 0 && ss % 9 == 0)
        {
            nextCol = 0;
        }
        for (int col = 0 + nextCol; col <= 72 + nextCol; col += 9)
        {
            if (col != ss) {
                //used.push_back(col);
                //cells[ss].addNeighbors(&cells[col]);
                cells[ss].neighbors.push_back(&cells[col]);
            }
        }
        nextCol++;

        //grid
        if (ss != 0 && ss % 3 == 0)
        {
            gridc += 3;
            gridc = gridc <= 6 ? gridc : 0;
        }
        if (ss != 0 && ss % 27 == 0)
        {
            gridr += 27;
        }
        used = &cells[ss].neighbors;
        for (int gr = 0; gr < 3; gr++)
        {
            for (int gc = 0 + gridc; gc < (3 + gridc); gc++)
            {
                grid = gr * 9 + gridr + gc;
                if (grid != ss)
                {
                    //need to compare address
                    if (!any_of(used->begin(), used->end(), [&](Cell *i) { return i == &cells[grid]; }))
                    {
                        //used.push_back(grid);
                        //cells[ss].addNeighbors(&cells[grid]);
                        cells[ss].neighbors.push_back(&cells[grid]);
                    }
                }
            }
        }
    }

    //set up no givens
    std::for_each(cells.begin(), cells.end(), [&](Cell &c)
    {
        if (!c.givenValue)
        {
            noGivens.push_back(&c);
        }
    });
}
