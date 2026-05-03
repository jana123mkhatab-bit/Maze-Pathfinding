#include "MazeWidget.h"
#include "MyVector.h"
#include <QPainter>
#include <QFontMetrics>

MazeWidget::MazeWidget(QWidget *parent)
    : QWidget(parent),
    cellSize(15),
    gridOffsetX(10),
    gridOffsetY(40),
    currentVisStep(0),
    isVisualizing(false) {

    visTimer = new QTimer(this);
    visTimer->setInterval(50);
    connect(visTimer, &QTimer::timeout, this, &MazeWidget::onVisualizationStep);

    setMinimumSize(300, 300);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MazeWidget::setMazeData(const MazeData& maze) {
    mazeData = maze;
    displayGrid = maze.grid;
    calculateCellSize();
    update();
}

void MazeWidget::setAlgorithmName(const QString& name) {
    algorithmName = name;
    update();
}

void MazeWidget::setResult(const AlgorithmResult& result) {
    currentResult = result;
    displayGrid = result.solutionGrid;
    visPath = result.path;
    resetVisualization();
    update();
}

void MazeWidget::calculateCellSize() {
    if (mazeData.FULL_R == 0 || mazeData.FULL_C == 0) {
        cellSize = 15;
        return;
    }

    int availableWidth = width() - 40;
    int availableHeight = height() - 130;

    int maxCellWidth = availableWidth / mazeData.FULL_C;
    int maxCellHeight = availableHeight / mazeData.FULL_R;

    cellSize = qMin(maxCellWidth, maxCellHeight);

    if (cellSize < 3) cellSize = 3;
    if (cellSize > 30) cellSize = 30;

    gridOffsetX = (width() - (mazeData.FULL_C * cellSize)) / 2;
    if (gridOffsetX < 10) gridOffsetX = 10;

    gridOffsetY = 40;
}

void MazeWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    calculateCellSize();
    update();
}

void MazeWidget::startVisualization() {
    if (isVisualizing || visPath.empty()) return;

    isVisualizing = true;
    currentVisStep = 0;
    displayGrid = mazeData.grid;
    visTimer->start();
    update();
}

void MazeWidget::stopVisualization() {
    if (!isVisualizing) return;

    visTimer->stop();
    isVisualizing = false;
    displayGrid = currentResult.solutionGrid;
    update();
}

void MazeWidget::resetVisualization() {
    stopVisualization();
    currentVisStep = 0;
    displayGrid = mazeData.grid;
    update();
}

void MazeWidget::onVisualizationStep() {
    if (currentVisStep >= (int)visPath.size()) {
        stopVisualization();
        return;
    }

    Point p = visPath[currentVisStep];
    if (displayGrid[p.r][p.c] != START && displayGrid[p.r][p.c] != GOAL) {
        displayGrid[p.r][p.c] = SHORTEST_PATH;
    }

    currentVisStep++;
    update();
}

QColor MazeWidget::getColorForCell(int cellType, int weight) const {
    switch (cellType) {
    case WALL: return QColor(80, 80, 80);
    case START: return QColor(0, 150, 255);
    case GOAL: return QColor(255, 50, 50);
    case SHORTEST_PATH: return QColor(255, 200, 0);
    case PATH: {
        int pinkBase = 255 - (weight * 20);
        if (pinkBase < 150) pinkBase = 150;
        int red = 255;
        int green = 200 - (weight * 15);
        if (green < 100) green = 100;
        int blue = 220 - (weight * 20);
        if (blue < 100) blue = 100;
        return QColor(red, green, blue);
    }
    default: return Qt::white;
    }
}

void MazeWidget::drawLegend(QPainter& painter) {
    painter.setPen(QColor(60, 60, 60));
    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);

    painter.drawText(10, 20, algorithmName);

    if (currentResult.nodesExpanded > 0) {
        QFont statsFont = font;
        statsFont.setBold(false);
        statsFont.setPointSize(9);
        painter.setFont(statsFont);

        QString statsText;
        if (currentResult.success) {
            statsText = QString("Cost: %1 | Nodes: %2 | Time: %3ms")
            .arg(currentResult.totalCost)
                .arg(currentResult.nodesExpanded)
                .arg(currentResult.timeTakenMs, 0, 'f', 2);
            painter.setPen(QColor(0, 120, 0));
        } else {
            statsText = "No path found";
            painter.setPen(QColor(180, 0, 0));
        }
        painter.drawText(10, 35, statsText);
    }
}

void MazeWidget::drawColorLegend(QPainter& painter) {
    int legendY = height() - 85;
    int legendX = 10;

    painter.setPen(QColor(60, 60, 60));
    QFont legendFont = painter.font();
    legendFont.setPointSize(8);
    legendFont.setBold(true);
    painter.setFont(legendFont);

    painter.drawText(legendX, legendY - 5, "Color Coding:");

    legendFont.setBold(false);
    painter.setFont(legendFont);

    painter.fillRect(legendX, legendY, 10, 10, QColor(0, 150, 255));
    painter.drawRect(legendX, legendY, 10, 10);
    painter.drawText(legendX + 13, legendY + 9, "Start");

    painter.fillRect(legendX + 60, legendY, 10, 10, QColor(255, 50, 50));
    painter.drawRect(legendX + 60, legendY, 10, 10);
    painter.drawText(legendX + 73, legendY + 9, "Goal");

    painter.fillRect(legendX, legendY + 15, 10, 10, QColor(255, 200, 0));
    painter.drawRect(legendX, legendY + 15, 10, 10);
    painter.drawText(legendX + 13, legendY + 24, "Solution");

    painter.fillRect(legendX + 60, legendY + 15, 10, 10, QColor(255, 180, 200));
    painter.drawRect(legendX + 60, legendY + 15, 10, 10);
    painter.drawText(legendX + 73, legendY + 24, "Path");

    painter.fillRect(legendX, legendY + 30, 10, 10, QColor(80, 80, 80));
    painter.drawRect(legendX, legendY + 30, 10, 10);
    painter.drawText(legendX + 13, legendY + 39, "Wall");

    painter.setPen(QColor(100, 100, 100));
    painter.drawText(legendX, legendY + 50, "Dark Pink = High Cost");
}

void MazeWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(255, 228, 225));

    if (mazeData.grid.empty() || mazeData.grid[0].empty()) {
        painter.setPen(QColor(60, 60, 60));
        painter.drawText(rect(), Qt::AlignCenter, "No maze data");
        return;
    }

    for (int r = 0; r < mazeData.FULL_R; ++r) {
        for (int c = 0; c < mazeData.FULL_C; ++c) {
            int x = gridOffsetX + c * cellSize;
            int y = gridOffsetY + r * cellSize;

            QColor color = getColorForCell(displayGrid[r][c], mazeData.weights[r][c]);
            painter.fillRect(x, y, cellSize, cellSize, color);

            if (displayGrid[r][c] == WALL) {
                painter.setPen(QColor(60, 60, 60));
            } else {
                painter.setPen(QColor(200, 200, 200));
            }
            painter.drawRect(x, y, cellSize, cellSize);

            if (cellSize > 8 && (displayGrid[r][c] == PATH || displayGrid[r][c] == SHORTEST_PATH)) {
                painter.setPen(QColor(80, 80, 80));
                QFont smallFont = painter.font();
                smallFont.setPointSize(cellSize > 12 ? 8 : 6);
                painter.setFont(smallFont);

                QString weightStr = QString::number(mazeData.weights[r][c]);
                QRect textRect(x, y, cellSize, cellSize);
                painter.drawText(textRect, Qt::AlignCenter, weightStr);
            }
        }
    }

    drawLegend(painter);
    drawColorLegend(painter);

    if (isVisualizing) {
        painter.setPen(QColor(0, 100, 200));
        painter.setBrush(QColor(255, 255, 255, 200));
        painter.drawRect(width() - 160, 5, 150, 25);
        painter.drawText(width() - 155, 22,
                         QString("Step %1/%2").arg(currentVisStep).arg(visPath.size()));
    }
}
