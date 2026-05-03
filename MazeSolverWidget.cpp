#include "MazeSolverWidget.h"
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>

MazeSolverWidget::MazeSolverWidget(QWidget *parent) : QWidget(parent) {
    setupUI();
    QTimer::singleShot(100, this, &MazeSolverWidget::generateAndSolve);
}

void MazeSolverWidget::setupUI() {
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // ROSE BACKGROUND for entire widget
    setStyleSheet("QWidget { background-color: rgb(255, 228, 225); }");

    // Title with dark gray text
    QLabel *titleLabel = new QLabel("Maze Algorithm Comparison", this);
    titleLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: rgb(60, 60, 60); }");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("QFrame { color: rgb(180, 180, 180); }");
    mainLayout->addWidget(line);

    // Maze widgets in grid with gray background containers
    mazeLayout = new QGridLayout();
    mazeLayout->setSpacing(15);

    // Create containers for each algorithm with gray background
    QFrame* dijkstraFrame = new QFrame(this);
    dijkstraFrame->setFrameStyle(QFrame::StyledPanel);
    dijkstraFrame->setStyleSheet("QFrame { background-color: rgb(220, 220, 220); border-radius: 10px; }");
    QVBoxLayout* dijkstraContainerLayout = new QVBoxLayout(dijkstraFrame);
    dijkstraContainerLayout->setContentsMargins(10, 10, 10, 10);
    dijkstraWidget = new MazeWidget(dijkstraFrame);
    dijkstraContainerLayout->addWidget(dijkstraWidget);
    dijkstraWidget->setAlgorithmName("Dijkstra Algorithm");

    QFrame* bucketFrame = new QFrame(this);
    bucketFrame->setFrameStyle(QFrame::StyledPanel);
    bucketFrame->setStyleSheet("QFrame { background-color: rgb(220, 220, 220); border-radius: 10px; }");
    QVBoxLayout* bucketContainerLayout = new QVBoxLayout(bucketFrame);
    bucketContainerLayout->setContentsMargins(10, 10, 10, 10);
    bucketWidget = new MazeWidget(bucketFrame);
    bucketContainerLayout->addWidget(bucketWidget);
    bucketWidget->setAlgorithmName("Bucket/Stepping Algorithm");

    QFrame* astarFrame = new QFrame(this);
    astarFrame->setFrameStyle(QFrame::StyledPanel);
    astarFrame->setStyleSheet("QFrame { background-color: rgb(220, 220, 220); border-radius: 10px; }");
    QVBoxLayout* astarContainerLayout = new QVBoxLayout(astarFrame);
    astarContainerLayout->setContentsMargins(10, 10, 10, 10);
    astarWidget = new MazeWidget(astarFrame);
    astarContainerLayout->addWidget(astarWidget);
    astarWidget->setAlgorithmName("A* Algorithm");

    mazeLayout->addWidget(dijkstraFrame, 0, 0);
    mazeLayout->addWidget(bucketFrame, 0, 1);
    mazeLayout->addWidget(astarFrame, 0, 2);

    mazeLayout->setColumnStretch(0, 1);
    mazeLayout->setColumnStretch(1, 1);
    mazeLayout->setColumnStretch(2, 1);

    mainLayout->addLayout(mazeLayout, 1);

    // Control area with gray background containers
    controlLayout = new QHBoxLayout();
    controlLayout->setSpacing(15);

    // Configuration Panel with dark gray text in gray container
    QFrame* configFrame = new QFrame(this);
    configFrame->setFrameStyle(QFrame::StyledPanel);
    configFrame->setStyleSheet("QFrame { background-color: rgb(220, 220, 220); border-radius: 10px; }");
    QVBoxLayout* configFrameLayout = new QVBoxLayout(configFrame);
    configFrameLayout->setContentsMargins(10, 10, 10, 10);

    configGroup = new QGroupBox("Maze Configuration", configFrame);
    configGroup->setMaximumWidth(300);
    configGroup->setStyleSheet("QGroupBox { color: rgb(60, 60, 60); font-weight: bold; background-color: transparent; }");
    QFormLayout *configForm = new QFormLayout(configGroup);
    configForm->setSpacing(8);

    rowsSpinBox = new QSpinBox(configGroup);
    rowsSpinBox->setRange(5, 50);
    rowsSpinBox->setValue(15);
    rowsSpinBox->setStyleSheet("QSpinBox { color: rgb(60, 60, 60); }");
    connect(rowsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MazeSolverWidget::updateSpinBoxRanges);
    configForm->addRow("Rows:", rowsSpinBox);

    colsSpinBox = new QSpinBox(configGroup);
    colsSpinBox->setRange(5, 50);
    colsSpinBox->setValue(15);
    colsSpinBox->setStyleSheet("QSpinBox { color: rgb(60, 60, 60); }");
    connect(colsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MazeSolverWidget::updateSpinBoxRanges);
    configForm->addRow("Columns:", colsSpinBox);

    startRowSpinBox = new QSpinBox(configGroup);
    startRowSpinBox->setRange(0, 14);
    startRowSpinBox->setValue(0);
    startRowSpinBox->setStyleSheet("QSpinBox { color: rgb(60, 60, 60); }");
    configForm->addRow("Start Row:", startRowSpinBox);

    startColSpinBox = new QSpinBox(configGroup);
    startColSpinBox->setRange(0, 14);
    startColSpinBox->setValue(0);
    startColSpinBox->setStyleSheet("QSpinBox { color: rgb(60, 60, 60); }");
    configForm->addRow("Start Column:", startColSpinBox);

    goalRowSpinBox = new QSpinBox(configGroup);
    goalRowSpinBox->setRange(0, 14);
    goalRowSpinBox->setValue(14);
    goalRowSpinBox->setStyleSheet("QSpinBox { color: rgb(60, 60, 60); }");
    configForm->addRow("Goal Row:", goalRowSpinBox);

    goalColSpinBox = new QSpinBox(configGroup);
    goalColSpinBox->setRange(0, 14);
    goalColSpinBox->setValue(14);
    goalColSpinBox->setStyleSheet("QSpinBox { color: rgb(60, 60, 60); }");
    configForm->addRow("Goal Column:", goalColSpinBox);

    solveButton = new QPushButton("Generate & Solve Maze", configGroup);
    solveButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    padding: 8px 15px;"
        "    font-weight: bold;"
        "    border-radius: 4px;"
        "}"
        "QPushButton:hover { background-color: #45a049; }"
        );
    connect(solveButton, &QPushButton::clicked, this, &MazeSolverWidget::generateAndSolve);
    configForm->addRow(solveButton);

    configFrameLayout->addWidget(configGroup);
    controlLayout->addWidget(configFrame);

    // VERTICAL Visualization Controls with dark gray text in gray container
    QFrame* visFrame = new QFrame(this);
    visFrame->setFrameStyle(QFrame::StyledPanel);
    visFrame->setStyleSheet("QFrame { background-color: rgb(220, 220, 220); border-radius: 10px; }");
    QVBoxLayout* visFrameLayout = new QVBoxLayout(visFrame);
    visFrameLayout->setContentsMargins(10, 10, 10, 10);

    visGroup = new QGroupBox("Visualization Controls", visFrame);
    visGroup->setMaximumWidth(200);
    visGroup->setStyleSheet("QGroupBox { color: rgb(60, 60, 60); font-weight: bold; background-color: transparent; }");
    QVBoxLayout *visLayout = new QVBoxLayout(visGroup);

    QLabel *visLabel = new QLabel("Visualize All Algorithms", visGroup);
    visLabel->setAlignment(Qt::AlignCenter);
    visLabel->setStyleSheet("QLabel { font-weight: bold; color: rgb(60, 60, 60); margin-bottom: 5px; }");
    visLayout->addWidget(visLabel);

    // VERTICAL layout for buttons
    visStartButton = new QPushButton("▶ Start All", visGroup);
    visStopButton = new QPushButton("⏹ Stop All", visGroup);
    visResetButton = new QPushButton("↺ Reset All", visGroup);

    visStartButton->setEnabled(false);
    visStopButton->setEnabled(false);
    visResetButton->setEnabled(false);

    // Style visualization buttons
    QString buttonStyle = "QPushButton { padding: 10px 15px; border-radius: 4px; margin: 3px; font-size: 11px; color: white; }";
    visStartButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #2196F3; }");
    visStopButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #f44336; }");
    visResetButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #FF9800; }");

    connect(visStartButton, &QPushButton::clicked, this, &MazeSolverWidget::onVisualizeClicked);
    connect(visStopButton, &QPushButton::clicked, this, &MazeSolverWidget::onStopVisualization);
    connect(visResetButton, &QPushButton::clicked, this, &MazeSolverWidget::onResetVisualization);

    // Add buttons VERTICALLY
    visLayout->addWidget(visStartButton);
    visLayout->addWidget(visStopButton);
    visLayout->addWidget(visResetButton);

    // Add spacer to push buttons to top
    visLayout->addStretch();

    visFrameLayout->addWidget(visGroup);
    controlLayout->addWidget(visFrame);

    // Results Table in gray container
    QFrame* resultsFrame = new QFrame(this);
    resultsFrame->setFrameStyle(QFrame::StyledPanel);
    resultsFrame->setStyleSheet("QFrame { background-color: rgb(220, 220, 220); border-radius: 10px; }");
    QVBoxLayout* resultsFrameLayout = new QVBoxLayout(resultsFrame);
    resultsFrameLayout->setContentsMargins(10, 10, 10, 10);

    resultsGroup = new QGroupBox("Algorithm Comparison Results", resultsFrame);
    resultsGroup->setMinimumWidth(400);
    resultsGroup->setStyleSheet("QGroupBox { color: rgb(60, 60, 60); font-weight: bold; background-color: transparent; }");
    QVBoxLayout *resultsVLayout = new QVBoxLayout(resultsGroup);

    resultsTable = new QTableWidget(4, 5, resultsGroup);
    resultsTable->setHorizontalHeaderLabels({"Algorithm", "Success", "Cost", "Nodes", "Time (ms)"});
    resultsTable->verticalHeader()->setVisible(false);

    // Disable alternating row colors so our custom colors show
    resultsTable->setAlternatingRowColors(false);

    // Style the table - ROSE BACKGROUND and dark gray text
    resultsTable->setStyleSheet(
        "QTableWidget {"
        "    gridline-color: rgb(200, 180, 180);"
        "    font-size: 9pt;"
        "    color: rgb(60, 60, 60);"
        "    background-color: rgb(255, 240, 240);"  // Lighter rose for table
        "}"
        "QHeaderView::section {"
        "    background-color: rgb(240, 220, 220);"
        "    padding: 5px;"
        "    border: 1px solid rgb(200, 180, 180);"
        "    font-weight: bold;"
        "    color: rgb(60, 60, 60);"
        "}"
        "QTableWidget::item {"
        "    padding: 4px;"
        "    border: 1px solid rgb(220, 200, 200);"
        "}"
        );

    // Set column widths
    resultsTable->setColumnWidth(0, 120);
    resultsTable->setColumnWidth(1, 70);
    resultsTable->setColumnWidth(2, 80);
    resultsTable->setColumnWidth(3, 80);
    resultsTable->horizontalHeader()->setStretchLastSection(true);

    // Initialize table rows with STATIC COLORS - FIXED FIRST ROW
    QStringList algorithms = {"Dijkstra", "Bucket/Stepping", "A*", "Best"};
    QList<QColor> rowColors = {
        QColor(250, 235, 235),  // Dijkstra - light rose (NO WHITE)
        QColor(250, 235, 235),  // Bucket - light rose
        QColor(250, 235, 235),  // A* - light rose
        QColor(255, 245, 220)   // Best - light yellow (static)
    };

    QList<QColor> successColors = {
        QColor(230, 255, 230),  // Success color - light green
        QColor(255, 230, 230)   // Failure color - light red
    };

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) {
            QTableWidgetItem *item = new QTableWidgetItem("");
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setTextAlignment(Qt::AlignCenter);
            item->setForeground(QBrush(QColor(60, 60, 60)));  // Dark gray text

            if (j == 0 && i < 3) {
                item->setText(algorithms[i]);
                item->setBackground(rowColors[i]);  // FIXED: Set proper background
            } else if (i == 3 && j == 0) {
                item->setText("Best");
                item->setBackground(rowColors[3]);  // FIXED: Set proper background
            } else if (j == 1) { // Success column
                item->setBackground(successColors[0]);
            } else {
                item->setBackground(rowColors[i < 3 ? i : 3]);  // FIXED: Set background for all cells
            }

            resultsTable->setItem(i, j, item);
        }
    }

    resultsVLayout->addWidget(resultsTable);
    resultsFrameLayout->addWidget(resultsGroup);
    controlLayout->addWidget(resultsFrame, 1);

    mainLayout->addLayout(controlLayout, 0);

    // Status bar at bottom with gray background container
    QFrame *statusFrame = new QFrame(this);
    statusFrame->setFrameStyle(QFrame::StyledPanel);
    statusFrame->setStyleSheet("QFrame { background-color: rgb(220, 220, 220); border-radius: 10px; margin-top: 10px; }");
    QHBoxLayout *statusLayout = new QHBoxLayout(statusFrame);
    statusLayout->setContentsMargins(15, 8, 15, 8);

    QLabel *statusLabel = new QLabel("Ready", statusFrame);
    statusLabel->setStyleSheet("QLabel { color: rgb(80, 80, 80); }");
    statusLayout->addWidget(statusLabel);
    statusLayout->addStretch();

    QLabel *infoLabel = new QLabel("Maze Algorithm Comparison Tool", statusFrame);
    infoLabel->setStyleSheet("QLabel { color: rgb(120, 120, 120); font-size: 9pt; }");
    statusLayout->addWidget(infoLabel);

    mainLayout->addWidget(statusFrame);
}

void MazeSolverWidget::updateSpinBoxRanges() {
    int rows = rowsSpinBox->value();
    int cols = colsSpinBox->value();

    // Update ranges for start/goal positions
    startRowSpinBox->setMaximum(rows - 1);
    startColSpinBox->setMaximum(cols - 1);
    goalRowSpinBox->setMaximum(rows - 1);
    goalColSpinBox->setMaximum(cols - 1);

    // Ensure current values are within new ranges
    if (startRowSpinBox->value() >= rows) startRowSpinBox->setValue(rows - 1);
    if (startColSpinBox->value() >= cols) startColSpinBox->setValue(cols - 1);
    if (goalRowSpinBox->value() >= rows) goalRowSpinBox->setValue(rows - 1);
    if (goalColSpinBox->value() >= cols) goalColSpinBox->setValue(cols - 1);
}

void MazeSolverWidget::generateAndSolve() {
    // Get configuration
    int R = rowsSpinBox->value();
    int C = colsSpinBox->value();
    Point start = {startRowSpinBox->value(), startColSpinBox->value()};
    Point goal = {goalRowSpinBox->value(), goalColSpinBox->value()};

    // Validate
    if (start.r == goal.r && start.c == goal.c) {
        QMessageBox::warning(this, "Invalid Configuration",
                             "Start and goal positions cannot be the same.");
        return;
    }

    // Stop any ongoing visualization
    onStopVisualization();

    // Generate new maze
    currentMaze = generateMaze(R, C, start, goal);

    // Solve with all algorithms
    dijkstraResult = solveDijkstra(currentMaze);
    bucketResult = solveBucket(currentMaze);
    astarResult = solveAStar(currentMaze);

    // Update maze displays
    dijkstraWidget->setMazeData(currentMaze);
    dijkstraWidget->setResult(dijkstraResult);

    bucketWidget->setMazeData(currentMaze);
    bucketWidget->setResult(bucketResult);

    astarWidget->setMazeData(currentMaze);
    astarWidget->setResult(astarResult);

    // Update results table
    updateResultsTable();

    // Enable visualization buttons
    visStartButton->setEnabled(true);
    visResetButton->setEnabled(true);
}

void MazeSolverWidget::updateResultsTable() {
    // Predefined static colors
    QColor successColor(230, 255, 230);  // Light green
    QColor failureColor(255, 230, 230);  // Light red
    QColor bestRowColor(255, 245, 220);  // Light yellow
    QColor normalRowColor(250, 235, 235); // Light rose

    // Update Dijkstra results - WITH STATIC COLORS
    resultsTable->item(0, 1)->setText(dijkstraResult.success ? "✓ Yes" : "✗ No");
    resultsTable->item(0, 2)->setText(dijkstraResult.success ?
                                          QString::number(dijkstraResult.totalCost) : "N/A");
    resultsTable->item(0, 3)->setText(QString::number(dijkstraResult.nodesExpanded));
    resultsTable->item(0, 4)->setText(QString::number(dijkstraResult.timeTakenMs, 'f', 3));

    // Set STATIC colors for success/failure
    resultsTable->item(0, 1)->setBackground(dijkstraResult.success ? successColor : failureColor);
    // Keep other columns with static background
    for (int col = 0; col < 5; col++) {
        if (col != 1) { // Don't change color for success column (already set)
            resultsTable->item(0, col)->setBackground(normalRowColor);
        }
    }

    // Update Bucket results - WITH STATIC COLORS
    resultsTable->item(1, 1)->setText(bucketResult.success ? "✓ Yes" : "✗ No");
    resultsTable->item(1, 2)->setText(bucketResult.success ?
                                          QString::number(bucketResult.totalCost) : "N/A");
    resultsTable->item(1, 3)->setText(QString::number(bucketResult.nodesExpanded));
    resultsTable->item(1, 4)->setText(QString::number(bucketResult.timeTakenMs, 'f', 3));

    resultsTable->item(1, 1)->setBackground(bucketResult.success ? successColor : failureColor);
    for (int col = 0; col < 5; col++) {
        if (col != 1) {
            resultsTable->item(1, col)->setBackground(normalRowColor);
        }
    }

    // Update A* results - WITH STATIC COLORS
    resultsTable->item(2, 1)->setText(astarResult.success ? "✓ Yes" : "✗ No");
    resultsTable->item(2, 2)->setText(astarResult.success ?
                                          QString::number(astarResult.totalCost) : "N/A");
    resultsTable->item(2, 3)->setText(QString::number(astarResult.nodesExpanded));
    resultsTable->item(2, 4)->setText(QString::number(astarResult.timeTakenMs, 'f', 3));

    resultsTable->item(2, 1)->setBackground(astarResult.success ? successColor : failureColor);
    for (int col = 0; col < 5; col++) {
        if (col != 1) {
            resultsTable->item(2, col)->setBackground(normalRowColor);
        }
    }

    // Determine and highlight best algorithm - WITH STATIC HIGHLIGHT
    if (dijkstraResult.success && bucketResult.success && astarResult.success) {
        // Find fastest algorithm
        double bestTime = dijkstraResult.timeTakenMs;
        int bestRow = 0;

        if (bucketResult.timeTakenMs < bestTime) {
            bestTime = bucketResult.timeTakenMs;
            bestRow = 1;
        }
        if (astarResult.timeTakenMs < bestTime) {
            bestTime = astarResult.timeTakenMs;
            bestRow = 2;
        }

        // Update best row
        resultsTable->item(3, 1)->setText("✓ Yes");
        resultsTable->item(3, 2)->setText(QString::number(bestTime, 'f', 3) + " ms");
        resultsTable->item(3, 3)->setText("Fastest");
        resultsTable->item(3, 4)->setText(QString("Row %1").arg(bestRow + 1));

        // Highlight the best algorithm row with STATIC COLOR
        for (int col = 0; col < 5; col++) {
            resultsTable->item(bestRow, col)->setBackground(bestRowColor);
        }

        // Set best row background (row 3 is always "Best" row)
        for (int col = 0; col < 5; col++) {
            resultsTable->item(3, col)->setBackground(bestRowColor);
        }
    } else {
        resultsTable->item(3, 1)->setText("N/A");
        resultsTable->item(3, 2)->setText("N/A");
        resultsTable->item(3, 3)->setText("N/A");
        resultsTable->item(3, 4)->setText("N/A");

        // Keep "Best" row with static color even when N/A
        for (int col = 0; col < 5; col++) {
            resultsTable->item(3, col)->setBackground(bestRowColor);
        }
    }
}

void MazeSolverWidget::onVisualizeClicked() {
    // Stop any ongoing visualization
    onStopVisualization();

    // Start visualization for ALL algorithms simultaneously
    dijkstraWidget->startVisualization();
    bucketWidget->startVisualization();
    astarWidget->startVisualization();

    // Update button states
    visStartButton->setEnabled(false);
    visStopButton->setEnabled(true);
    visResetButton->setEnabled(true);
}

void MazeSolverWidget::onStopVisualization() {
    // Stop all visualizations
    dijkstraWidget->stopVisualization();
    bucketWidget->stopVisualization();
    astarWidget->stopVisualization();

    // Update button states
    visStartButton->setEnabled(true);
    visStopButton->setEnabled(false);
    visResetButton->setEnabled(true);
}

void MazeSolverWidget::onResetVisualization() {
    // Reset all visualizations
    dijkstraWidget->resetVisualization();
    bucketWidget->resetVisualization();
    astarWidget->resetVisualization();

    // Update button states
    visStartButton->setEnabled(true);
    visStopButton->setEnabled(false);
}
