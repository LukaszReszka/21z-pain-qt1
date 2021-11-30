#include "gamewindow.h"
#include "gameboard.h"
#include "./ui_gamewindow.h"
#include <QUrl>
#include <QDesktopServices>

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui = new Ui::GameWindow;
    ui->setupUi(this);

    GameBoard *board = new GameBoard(this);
    setCentralWidget(board);
    setWindowIcon(QIcon(":/logo.png"));

    engine = new GameEngine(board);
    QObject::connect(board, &GameBoard::putPawnOnField,
                     engine, &GameEngine::processPlayersMove);

    QObject::connect(ui->createNewGame, &QAction::triggered,
                     engine, &GameEngine::resetGame);

    QObject::connect(engine, &GameEngine::setStatusBarText,
                     ui->statusBar, &QStatusBar::showMessage);

    QObject::connect(ui->getInstructions, &QAction::triggered,
            [](){
        QUrl instruction_url("https://www.kurnik.pl/reversi/zasady.phtml");
        QDesktopServices::openUrl(instruction_url);
    });
}

GameWindow::~GameWindow()
{
    delete ui;
    delete engine;
}
