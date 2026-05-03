#include <QApplication>
#include "MazeSolverWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MazeSolverWidget window;
    window.setWindowTitle("Maze Algorithm Comparison");
    window.resize(1600, 900);
    window.show();


    return app.exec();
}
