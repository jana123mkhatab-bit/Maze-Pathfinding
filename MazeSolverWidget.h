#ifndef MAZESOLVERWIDGET_H
#define MAZESOLVERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QTableWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "MazeWidget.h"
#include "MazeAlgorithms.h"

class MazeSolverWidget : public QWidget {
    Q_OBJECT

public:
    explicit MazeSolverWidget(QWidget *parent = nullptr);

private slots:
    void generateAndSolve();
    void onVisualizeClicked();
    void onStopVisualization();
    void onResetVisualization();
    void updateSpinBoxRanges();

private:
    void setupUI();
    void updateResultsTable();

    QVBoxLayout *mainLayout;
    QGridLayout *mazeLayout;
    QHBoxLayout *controlLayout;

    MazeWidget *dijkstraWidget;
    MazeWidget *bucketWidget;
    MazeWidget *astarWidget;

    QGroupBox *configGroup;
    QSpinBox *rowsSpinBox;
    QSpinBox *colsSpinBox;
    QSpinBox *startRowSpinBox;
    QSpinBox *startColSpinBox;
    QSpinBox *goalRowSpinBox;
    QSpinBox *goalColSpinBox;
    QPushButton *solveButton;

    // SIMPLIFIED Visualization controls - SINGLE button
    QGroupBox *visGroup;
    QPushButton *visStartButton;
    QPushButton *visStopButton;
    QPushButton *visResetButton;

    // Results display
    QGroupBox *resultsGroup;
    QTableWidget *resultsTable;

    // Current data
    MazeData currentMaze;
    AlgorithmResult dijkstraResult;
    AlgorithmResult bucketResult;
    AlgorithmResult astarResult;
};

#endif // MAZESOLVERWIDGET_H
