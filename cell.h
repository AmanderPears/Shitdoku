//Amander Pears
#ifndef CELL_H
#define CELL_H
#include <QLabel>
#include <vector>

class Cell
{
public:
    int currentValue;
    bool givenValue;
    QLabel* cellLabel;
    std::vector<Cell*> neighbors;

    Cell() {
        currentValue = 0;
        givenValue = false;
        cellLabel = new QLabel;
        cellLabel->setFixedWidth(25);
        cellLabel->setFixedHeight(25);
        cellLabel->setAlignment(Qt::AlignCenter);
        //cellLabel->setStyleSheet("QLabel{background-color:white;}");
    }

    void initial_set(int i)
    {
        currentValue = i;
        givenValue = true;
        cellLabel->setNum(i);
        cellLabel->setDisabled(true);
    }

    bool next_value()
    {
        if (!givenValue)
        {
            do
            {
                if (currentValue == 9)
                {
                    currentValue = 0;
                    return false;
                }
                currentValue++;
            } while (any_of(neighbors.begin(), neighbors.end(), [&](Cell *c) {return c->currentValue==currentValue; }));
            cellLabel->setNum(currentValue);
            return true;
        }
        return false;
    }
};

#endif // CELL_H
