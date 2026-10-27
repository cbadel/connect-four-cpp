#include "ConnectFourPlusUndo.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;
namespace ConnectFourGame {
bool ConnectFourPlusUndo::playUser(char column) {
    // kullanici girisi gecerli mi diye kontrol ediyorum
    if (!isValidColumn(column)) {
        cout << "Invalid column! Use A-" << char('A' + width - 1) << "\n";
        return false;
    }
    
    int col = getColumnIndex(column);//girilen harfi indexe cevirdim
    int row = findEmptyRow(col);// o sutunda en alttaki bos satiri buluyor
    
    if (row == -1) {
        //sutun dolmussa hamle yapilamiyor
        cout << "Column " << (char)('A' + col) << " is full!\n";
        return false;
    }
    Cell::State pieceState;
    if (currentPlayer == 1) {
        pieceState = Cell::USER1;
    } else {
        // user ves user ise user2 koyuyor degilse bilgisayar
        pieceState = (gameMode == MODE_USER1_VS_USER2) ? Cell::USER2 : Cell::COMPUTER;
    }
    // tasi tahtaya koy
    board[row][col].setState(pieceState);
    //undo icin hamleyi kaydettim
    moves.push_back({row, col, pieceState, currentPlayer});
    
    if (checkWin(row, col)) {
        gameOver = true;
        cout << "\n" << getCurrentPlayerName() << " WINS!\n";
        return true;
    }
    //tahtanin dolulugu kontrolü
    bool boardFull = true;
    for (int c = 0; c < width; c++) {
        if (board[0][c].getState() == Cell::EMPTY) {
            boardFull = false;
            break;
        }
    }
    
    if (boardFull) {
        gameOver = true;
        draw = true;
        cout << "\nDRAW! Board is full.\n";
        return true;
    }
    // oyuncuyu degistir
    switchPlayer();
    return true;
}

bool ConnectFourPlusUndo::playComputer() {
    if (gameMode != MODE_USER_VS_COMPUTER || currentPlayer != 2) {
        return false;
    }
    
    cout << "Computer is thinking...\n";
    //bilgisayarin secebilecegi bos sutunlari topladim
    vector<int> availableColumns;
    for (int c = 0; c < width; c++) {
        if (board[0][c].getState() == Cell::EMPTY) {
            availableColumns.push_back(c);
        }
    }
    // bos sutun yoksa oyun bitmistir
    if (availableColumns.empty()) {
        draw = true;
        gameOver = true;
        cout << "No moves available for computer!\n";
        return false;
    }
    //pc random bir sutun seciyor
    int randomIndex = rand() % availableColumns.size();
    int col = availableColumns[randomIndex];
    int row = findEmptyRow(col);
    //bilgisayar tasini yerlestir
    board[row][col].setState(Cell::COMPUTER);
    moves.push_back({row, col, Cell::COMPUTER, currentPlayer});
    
    cout << "Computer plays column " << char('A' + col) << "\n";
    
    if (checkWin(row, col)) {
        gameOver = true;
        cout << "\nComputer WINS!\n";
        return true;
    }
    // beraber kontrolu
    bool boardFull = true;
    for (int c = 0; c < width; c++) {
        if (board[0][c].getState() == Cell::EMPTY) {
            boardFull = false;
            break;
        }
    }
    
    if (boardFull) {
        gameOver = true;
        draw = true;
        cout << "\nDRAW! Board is full.\n";
        return true;
    }
    //pc den sonra sira userda
    switchPlayer();
    return true;
}

void ConnectFourPlusUndo::undo() {
    //undo yapacak hamle yoksa uyari
    if (moves.empty()) { 
        cout << "No moves to undo\n"; 
        return; 
    }
    
    Move lastMove = moves.back();
    moves.pop_back();
    // tekrar bos yaptim undo yapilan yeri
    board[lastMove.r][lastMove.c].setState(Cell::EMPTY);
    currentPlayer = lastMove.player;
    gameOver = false;
    draw = false;
    
    cout << "Undid " << ((lastMove.player == 1) ? "Player 1" : 
                        (gameMode == MODE_USER1_VS_USER2 ? "Player 2" : "Computer")) 
         << "'s move at " << char('A' + lastMove.c) << (lastMove.r + 1) << "\n";
}

void ConnectFourPlusUndo::undoAll() {
    // tum hamleleri geri aliyorum
    while (!moves.empty()) {
        undo();
    }
    cout << "All moves undone. Board reset.\n";
}

void ConnectFourPlusUndo::playGame() {
    cout << "Enter board width and height (e.g., 7 6): ";
    cin >> width >> height;
    // eger duzgun boyut girmezse 7x6 yapiyorum
    if (width < 4 || width > 26 || height < 4 || height > 20) {
        cout << "Invalid dimensions. Using default 7x6.\n";
        width = 7;
        height = 6;
    }
    //eski boardi siliyorum
    for (int i = 0; i < 6; i++) 
        delete[] board[i];
    delete[] board;
    //yeni board
    board = new Cell*[height];
    for (int r = 0; r < height; r++) {
        board[r] = new Cell[width];
        for (int c = 0; c < width; c++) {
            board[r][c].setPosition('A' + c, r + 1);
            board[r][c].setState(Cell::EMPTY);
        }
    }
    
    moves.clear();
    
    displayBoard();//tahtayi ekrana basiyorum
    string input;

    while (!isGameFinished()) {
        cout << "\n" << getCurrentPlayerName() << "'s turn\n";
        
        if (gameMode == MODE_USER_VS_COMPUTER && currentPlayer == 2) {
            playComputer();
            displayBoard();
            continue;
        }
        
        cout << "Enter column (A-" << char('A' + width - 1) << "), UNDO, UNDOALL, SAVE, LOAD, or QUIT: ";
        cin >> input;

        for (char &ch : input) ch = toupper(ch);

        if (input == "UNDO") {
            undo();
            displayBoard();
            continue;
        }
        else if (input == "UNDOALL") {
            undoAll();
            displayBoard();
            continue;
        }
        else if (input == "SAVE") {
            cout << "Enter filename: ";
            string f;
            cin >> f;
            writeToFile(f);
            cout << "Game saved to " << f << "\n";
            continue;
        }
        else if (input == "LOAD") {
            cout << "Enter filename: ";
            string f;
            cin >> f;
            readFromFile(f);
            moves.clear();
            displayBoard();
            continue;
        }
        else if (input == "QUIT") {
            cout << "Game ended by user.\n";
            return;
        }

        if (input.length() != 1) {
            cout << "Invalid input!\n";
            continue;
        }

        if (!playUser(input[0])) {
            continue;
        }

        displayBoard();
        
        if (isGameFinished() && draw) {
            cout << "\nGame ended in a DRAW!\n";
        }
    }
}
}