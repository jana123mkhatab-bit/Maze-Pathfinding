#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include "Maze.h"
#include "MazeAlgorithms.h"
#include "MyVector.h"

class MazeWidget : public QWidget {
    Q_OBJECT

public:
    explicit MazeWidget(QWidget *parent = nullptr);
    void setMazeData(const MazeData& maze);
    void setAlgorithmName(const QString& name);
    void setResult(const AlgorithmResult& result);

    // Visualization controls
    void startVisualization();
    void stopVisualization();
    void resetVisualization();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onVisualizationStep();

private:
    void calculateCellSize();
    QColor getColorForCell(int cellType, int weight) const;
    void drawLegend(QPainter& painter);
    void drawColorLegend(QPainter& painter);

    MazeData mazeData;
    AlgorithmResult currentResult;
    QString algorithmName;

    // Visualization
    QTimer* visTimer;
    int currentVisStep;
    bool isVisualizing;
    MyVector<MyVector<int>> displayGrid;
    MyVector<Point> visPath;

    // Drawing parameters
    int cellSize;
    int gridOffsetX;
    int gridOffsetY;
};

#endif // MAZEWIDGET_H
