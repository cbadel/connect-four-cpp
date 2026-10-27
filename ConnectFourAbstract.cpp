#include "ConnectFourAbstract.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;
namespace ConnectFourGame {
ConnectFourAbstract::ConnectFourAbstract() {
    width = 7;//default width
    height = 6;//default height
    gameOver = false;
    draw = false;
    gameMode = MODE_USER_VS_COMPUTER;
    currentPlayer = 1;
    srand(time(0));

    board = new Cell*[height];//tahta dinamik olarak olusturulur
    for (int r = 0; r < height; r++) {
        board[r] = new Cell[width];
        for (int c = 0; c < width; c++) {
            board[r][c].setPosition('A' + c, r + 1);
            //baslangicta tum hucreler bos
            board[r][c].setState(Cell::EMPTY);
        }
    }
}

ConnectFourAbstract::~ConnectFourAbstract() {
    for (int i = 0; i < height; i++)
        delete[] board[i];
    delete[] board;
}

void ConnectFourAbstract::setGameType(int t) {
    gameMode = (t == 1) ? MODE_USER1_VS_USER2 : MODE_USER_VS_COMPUTER;
    currentPlayer = 1;
}

string ConnectFourAbstract::getCurrentPlayerName() const {
    if (gameMode == MODE_USER1_VS_USER2) {
        return (currentPlayer == 1) ? "Player 1 (X)" : "Player 2 (O)";
    } else {
        return (currentPlayer == 1) ? "Player (X)" : "Computer (O)";
    }
}

void ConnectFourAbstract::switchPlayer() {
    // oyuncu degisimini sagliyor 1den 2 ye 2 den 1 e
    currentPlayer = (currentPlayer == 1) ? 2 : 1;
}

void ConnectFourAbstract::displayBoard() const {
    //tahta ekrana yazdiriliyor
    cout << "\n  ";
    for (int c = 0; c < width; c++)
        cout << (char)('A' + c) << " ";
    cout << "\n";
    
    for (int r = 0; r < height; r++) {
        cout << (r+1) << " ";
        for (int c = 0; c < width; c++) {
            Cell::State s = board[r][c].getState();
            if (s == Cell::EMPTY) cout << ". ";
            else if (s == Cell::USER1) cout << "X ";
            else if (s == Cell::USER2) cout << "O ";
            else cout << "C ";
        }
        cout << "\n";
    }
    cout << "\n";
}

bool ConnectFourAbstract::isValidColumn(char column) const {
    //kullanicin girdigi harf tahta genisligi icinde mi
    int col = toupper(column) - 'A';
    return (col >= 0 && col < width);
}

int ConnectFourAbstract::getColumnIndex(char column) const {
    // harfi indexe cevir
    return toupper(column) - 'A';
}

int ConnectFourAbstract::findEmptyRow(int col) const {
    //kolonda ilk bos yer araniyor
    for (int r = height - 1; r >= 0; r--) {
        if (board[r][col].getState() == Cell::EMPTY)
            return r;
    }
    return -1;
}

bool ConnectFourAbstract::playUser(char column) {
    if (!isValidColumn(column)) {
        cout << "Invalid column! Use A-" << char('A' + width - 1) << "\n";
        return false;
    }
    
    int col = getColumnIndex(column);
    int row = findEmptyRow(col);
    // sutun dolmussa uyari ver
    if (row == -1) {
        cout << "Column " << (char)('A' + col) << " is full!\n";
        return false;
    }
    
    if (currentPlayer == 1) {
        board[row][col].setState(Cell::USER1);
    } else {
        board[row][col].setState((gameMode == MODE_USER1_VS_USER2) ? Cell::USER2 : Cell::COMPUTER);
    }
    
    if (checkWin(row, col)) {
        gameOver = true;
        cout << "\n" << getCurrentPlayerName() << " WINS!\n";
        return true;
    }
    // barabere durumu icin tahatnin doluluguna bakiyorum
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
    
    switchPlayer();
    return true;
}

bool ConnectFourAbstract::playComputer() {
    if (gameMode != MODE_USER_VS_COMPUTER || currentPlayer != 2) {
        return false;
    }
    
    cout << "Computer is thinking...\n";
    //bilgisayarin rastgele secebilecegi bos sutunlari topluyorum
    vector<int> availableColumns;
    for (int c = 0; c < width; c++) {
        if (board[0][c].getState() == Cell::EMPTY) {
            availableColumns.push_back(c);
        }
    }
    
    if (availableColumns.empty()) {
        draw = true;
        gameOver = true;
        cout << "No moves available for computer!\n";
        return false;
    }
    //bilgisayar rastgele  bir sutun seciyor
    int randomIndex = rand() % availableColumns.size();
    int col = availableColumns[randomIndex];
    int row = findEmptyRow(col);
    //tasi yerlestirdim
    board[row][col].setState(Cell::COMPUTER);
    
    cout << "Computer plays column " << char('A' + col) << "\n";
    
    if (checkWin(row, col)) {
        gameOver = true;
        cout << "\nComputer WINS!\n";
        return true;
    }
    
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
    
    switchPlayer();
    return true;
}

bool ConnectFourAbstract::isGameFinished() const {
    //sadece gameover flagi donduruluyor
    return gameOver;
}
void ConnectFourAbstract::readFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cout << "Cannot open file: " << filename << "\n";
        return;
    }

    int newW, newH;
    in >> newW >> newH;
    if (newW <= 0 || newH <= 0) {
        std::cout << "Invalid dimensions in file\n";
        return;
    }

    //eski boardu sildim
    for (int r = 0; r < height; ++r) {
        delete[] board[r];
    }
    delete[] board;
    board = nullptr;

    //yeni boyutlari ayarla
    width = newW;
    height = newH;

    // yeni boyutlari allocate ettim
    board = new Cell*[height];
    for (int r = 0; r < height; ++r) {
        board[r] = new Cell[width];
        for (int c = 0; c < width; ++c)
            board[r][c].setPosition('A' + c, r + 1);
    }
    in.ignore(10000, '\n');

    std::string line;
    for (int r = 0; r < height; ++r) {
        if (!std::getline(in, line)) {
            for (int c = 0; c < width; ++c)
                board[r][c].setState(Cell::EMPTY);
            continue;
        }
        // satir icinde bosluk varsa onlari atliyorum ve sadece karakterleri okuyorum.
        int colIdx = 0;
        for (size_t i = 0; i < line.size() && colIdx < width; ++i) {
            char ch = line[i];
            if (ch == ' ' || ch == '\t') continue; 

            char lc = (char)std::tolower(static_cast<unsigned char>(ch));
            //karakterlerin hangi oyuncuya ait oldugunu belirliyoruz
            if (lc == 'x') board[r][colIdx].setState(Cell::USER1);
            else if (lc == 'o') board[r][colIdx].setState(Cell::USER2);
            else if (lc == 'c') board[r][colIdx].setState(Cell::COMPUTER);
            else board[r][colIdx].setState(Cell::EMPTY);

            ++colIdx;
        }

        // eger satirdaki karakter sayisi board genisliginden azsa kalan hucreleri bos olarak tamamlıyoruz
        for (int c = colIdx; c < width; ++c)
            board[r][c].setState(Cell::EMPTY);
    }

    gameOver = false; //dosyadan yukledikten sonra oyun durumunu sifirliyoruz
}


void ConnectFourAbstract::writeToFile(const string& filename) const {
    ofstream out(filename);
    out << width << " " << height << "\n";

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            Cell::State s = board[r][c].getState();
            if (s == Cell::EMPTY) out << '.';
            else if (s == Cell::USER1) out << 'X';
            else if (s == Cell::USER2) out << 'O';
            else out << 'C';
        }
        out << "\n";
    }
}

void ConnectFourAbstract::playGame() {
    cout << "Enter board width and height (e.g., 7 6): ";
    cin >> width >> height;
    // kullanici mantikli deger girmezse diye default degerler belirledim
    if (width < 4 || width > 26 || height < 4 || height > 20) {
        cout << "Invalid dimensions. Using default 7x6.\n";
        width = 7;
        height = 6;
    }
    //onceki board temizlenir
    for (int i = 0; i < 6; i++) 
        delete[] board[i];
    delete[] board;
    // yeni boyutlara gore board yeniden olusturulur
    board = new Cell*[height];
    for (int r = 0; r < height; r++) {
        board[r] = new Cell[width];
        for (int c = 0; c < width; c++) {
            //hucrelerin koordinatlari ayarliyoruz
            board[r][c].setPosition('A' + c, r + 1);
            board[r][c].setState(Cell::EMPTY);
        }
    }
    //bos boardi ekrana bastiriyorum
    displayBoard();
    string input;
    //oyun bitene kadar onen an oyun dongusu
    while (!isGameFinished()) {
        cout << "\n" << getCurrentPlayerName() << "'s turn\n";
        
        if (gameMode == MODE_USER_VS_COMPUTER && currentPlayer == 2) {
            playComputer();
            displayBoard();
            continue;
        }
        
        cout << "Enter column (A-" << char('A' + width - 1) << "), SAVE, LOAD, or QUIT: ";
        cin >> input;

        for (char &ch : input) ch = toupper(ch);
        //oyunu kaydetme opsiyonu
        if (input == "SAVE") {
            cout << "Enter filename: ";
            string f;
            cin >> f;
            writeToFile(f);//dosyaya kaydetme
            cout << "Game saved to " << f << "\n";
            continue;
        }
        //oyun yukleme secenegi
        else if (input == "LOAD") {
            cout << "Enter filename: ";
            string f;
            cin >> f;
            readFromFile(f);//dosyadan okuma
            displayBoard();//yuklenen oyunu gosterme
            continue;
        }
        //oyunu bitirmek icin
        else if (input == "QUIT") {
            cout << "Game ended by user.\n";
            return;
        }
        //user tek karakter girmediyse hata alir
        if (input.length() != 1) {
            cout << "Invalid input! Enter a single column letter.\n";
            continue;
        }
        //dolu kolona oynadiysa tekrar giris istiyorum
        if (!playUser(input[0])) {
            continue;
        }

        displayBoard();
        //oyun bittiyse ve kims ekazanmadiysa beraberlik mesaji
        if (isGameFinished() && draw) {
            cout << "\nGame ended in a DRAW!\n";
        }
    }
}
}