#include "board.h"
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;
#define MAXSIZE 100

struct Node {
    char** board;
    Node* next;
};

bool check(char** board, char** board_2,int size) {
    char a, b;
    for (int i = 0; i < 2*size+1; i++) {
        for (int j = 0; j < 4 * size + 1; j++) {
            a = board[i][j];
            b = board_2[i][j];
            if (board[i][j] != board_2[i][j]) return false;
        }
    }
    return true;
}

void add(Node*& head, Node*& newNode,int size) {
    Node* curr = head;
    if (head == nullptr) {
        head = newNode;
        return;
    }
    bool k = false;
    while (curr->next != nullptr) {
        if (check(curr->board, newNode->board, size)) return;
        curr = curr->next;
    }
    if (check(curr->board, newNode->board, size)) return;
    curr->next = newNode;
}
void boarding(char** board, Node*& head, int vert, int horiz,int x,int y,char turn,int size) {
    int temp_x = x, temp_y = y;
    char c=board[temp_y][temp_x];
    if (board[temp_y][temp_x] != '_') {
        while (board[temp_y + vert][temp_x + horiz] != '_') {
            if (board[temp_y + vert][temp_x + horiz] == '+') {
                return;
            }
            temp_x += horiz;
            temp_y += vert;
            c = board[temp_y][temp_x];
        }
    }
    Node* newNode = new Node;
    newNode->board = new char*[2*size+1];
    for (int i = 0; i < 2 * size + 1; i++) {
        newNode->board[i] = new char[4 * size + 1];
        for (int j = 0; j < 4 * size + 1; j++) {
            newNode->board[i][j] = board[i][j];
        }
    }
    newNode->next = nullptr;
    if (y == 1 && x == 8) {
        c = ' ';
    }
    int start_x = temp_x + horiz, start_y = temp_y + vert;
    if (board[temp_y][temp_x] != '_') {
        do {
            newNode->board[temp_y + vert][temp_x + horiz] = newNode->board[temp_y][temp_x];
            temp_y -= vert;
            temp_x -= horiz;
        } while (board[temp_y][temp_x] != '+');
    }
    else {
        temp_y -= vert;
        temp_x -= horiz;
    }
    newNode->board[temp_y + vert][temp_x + horiz] = turn;
    add(head, newNode,size);
}

Board::Board() {};
void Board::Set(int size, int collection, int whiteAll, int blackAll, int whiteInReserve, int blackInReserve, char turn)
{
    empty = true;
    this->size = size;
    this->collection = collection;
    this->whiteInReserve = whiteInReserve;
    this->whiteAll = whiteAll;
    this->blackInReserve = blackInReserve;
    this->blackAll = blackAll;
    this->turn = turn;
    this->blackOnBoard = 0;
    this->whiteOnBoard = 0;
    int length = size;
    int start = size + 1;
    int occupied = length;
    int size_hor = 4*size + 1;
    int size_vert = 2 * size + 1;
    bool row = false;
    board = new char* [size_vert];
    for (int i = 0; i < size_vert; i++) {
        board[i] = new char[size_hor];
    }
    for (int i = 0; i < size_vert; i++) {
        occupied = 0;
        char line[MAXSIZE];
        char c;
        int k = 0;
        char tog;
        if (i == 0 || i == size_vert - 1) {
            for (int j = 0; j < size_hor; j++) {
                if (j < size || j > size_hor - size - 1) board[i][j] = ' ';
                else {
                    board[i][j++] = '+';
                    board[i][j] = ' ';
                }
            }
        }
        else {
            cin >> c;
            while (c != '\n' && i < MAXSIZE - 1 && c != '\xff') {
                line[k++] = c;
                if (c == '_' || c == 'W' || c == 'B') occupied++;
                c = getchar();
            }
            if (occupied != length) {
                row = true;
            }
            occupied = length;
            int together = 0;
            for (int j = 0; j < size_hor; j++) {
                if (j == start - 2 || j == size_hor - start + 1) board[i][j] = '+';
                else if (j < start || j > size_hor - start || j == size_hor - start) board[i][j] = ' ';
                else {
                    board[i][j] = line[j - start];
                    if (board[i][j] == 'W') whiteOnBoard++;
                    else if (board[i][j] == 'B') blackOnBoard++;
                }
            }
            if (i < size) { length++; start--; }
            else  { length--; start++; }
        }
    }
    int false_rows = check_start();
    if (false_rows > 0) {
        if(false_rows == 1) cout << endl<<"ERROR_FOUND_" << false_rows << "_ROW_OF_LENGTH_K" << endl;
        else cout << endl << "ERROR_FOUND_" << false_rows << "_ROWS_OF_LENGTH_K" << endl;
        return;
    }    if (row) {
        cout << "WRONG_BOARD_ROW_LENGTH" << endl;
        return;
    }
    else if (blackAll - blackOnBoard < blackInReserve) {
        cout << "WRONG_BLACK_PAWNS_NUMBER" << endl;
        return;
    } else if ( whiteAll - whiteOnBoard < whiteInReserve){
        cout << "WRONG_WHITE_PAWNS_NUMBER" << endl;
        return;
    }
    cout <<endl<< "BOARD_STATE_OK" << endl;
    empty = false;
};
void Board::Print() {
    if (empty) {
        cout <<endl<< "EMPTY_BOARD" << endl;
    }
    else {
        cout << endl;
        printf("%d %d %d %d\n", size, collection, whiteAll, blackAll);
        printf("%d %d %c\n", whiteInReserve, blackInReserve, turn);
        for (int i = 1; i < 2 * size; i++) {
            for (int j = 2; j < 4 * size - 1; j++) {
                if (board[i][j] == '+') cout << " ";
                else cout << board[i][j];
            }
            cout << endl;
        }
    }
}
bool Board::move(char letter_1, int num_1, char letter_2, int num_2,  char col, char letter_f, int num_f, char letter_l, int num_l) {
    int pos_1_x = (int)(letter_1 - 'a') + 1;
    int pos_2_x = (int)(letter_2 - 'a') + 1;
    int where = pos_2_x - pos_1_x;
    int x = 0, y = size;
    int x_2 = x, y_2 = y, dir;

    if ((pos_1_x > 2 * size + 1) || (num_1 > 2 * size + 1)) {
        cout <<endl<< "BAD_MOVE_" << letter_1 << num_1 << "_IS_WRONG_INDEX" << endl;;
        return false;
    }
    if ((pos_2_x > 2 * size + 1) || (num_2 > 2 * size + 1)) {
        cout << endl << "BAD_MOVE_" << letter_2 << num_2 << "_IS_WRONG_INDEX" << endl;;
        return false;
    }
    if (abs(where) > 1 || abs(num_2 - num_1) > 1) {
        cout << endl << "UNKNOWN_MOVE_DIRECTION" << endl;;
        return false;
    }

    for (int i = 1; i <= 2 * size + 1; i++) {
        if (pos_1_x == i) {
            if (where == 1) {
                if (y < 2 * size) {
                    x_2 = x+1;
                    y_2 = y+1;
                }
                else {
                    x_2 = x+2;
                    y_2 = y;
                }
            }
            else if (where == -1) {
                if (y < 2 * size) {
                    x_2 = x-1;
                    y_2 = y-1;
                }
                else {
                    x_2 = x-2;
                    y_2 = y;
                }
            }
            else if (where == 0) {
                x_2 = x;
                y_2 = y;
            }
            break;
        }
        else {
            if (y < 2 * size) {
                x++;
                y++;
            }
            else {
                x += 2;
            }
        }
        if (i == 2 * size + 1 && pos_1_x != i) {
            cout << endl << "BAD_MOVE_" << letter_1 << num_1 << "_IS_WRONG_INDEX" << endl;;
            return false;
        }
    }
    int index = 1;
    while (true) {
        if (index == num_1) break;
        if (board[y-1][x+1] != ' ') {
            y--;
            x++;
            index++;
        }
        else break;
    }
    if (num_1 != index) {
        cout << endl << "BAD_MOVE_" << letter_1 << num_1 << "_IS_WRONG_INDEX" << endl;;
        return false;
    }
    else if (board[y][x] != '+') {
        cout << endl << "BAD_MOVE_" << letter_1 << num_1 << "_IS_WRONG_STARTING_FIELD" << endl;;
        return false;
    }
    index = 1;
    while (true) {
        if (index == num_2) break;
        if (board[y_2][x_2] != ' ') {
            y_2--;
            x_2++;
            index++;
        }
        else break;
    }
    if (num_2 != index) {
        cout << endl << "BAD_MOVE_" << letter_2 << num_2 << "_IS_WRONG_INDEX" << endl;;
        return false;
    }
    else if (board[y_2][x_2] == '+') {
        cout << endl << "BAD_MOVE_" << letter_2 << num_2 << "_IS_WRONG_DESTINATION_FIELD" << endl;;
        return false;
    }
    dir = is_neighbour(x, y, x_2, y_2);
    if (dir == -1) return false;
    if (board[y][x] == '_' || board[y_2][x_2] == ' ') return false;
    else if (board[y_2][x_2] != '_') {
        if (!move_chain(x, y, dir, col, letter_f, num_f, letter_l, num_l)) {
            return false;
        }
    }
    if (turn == 'W') {
        whiteInReserve--;
        turn = 'B';
    }
    else {
        blackInReserve--;
        turn = 'W';
    }
    cout << endl << "MOVE_COMMITTED" << endl;
    if (print_after) {
        //Print();
        print_after = false;
    }
    return true;
}
bool Board::move_chain(int x, int y, int dir, char col, char letter_f, int num_f, char letter_l, int num_l) {
    int horiz = 0, vert = 0;
    int temp_x = x, temp_y = y;
    switch (dir)
    {
    case 0: // NE
        horiz = 1;
        vert = -1;
        break;
    case 1: // E
        horiz = 2;
        break;
    case 2: // SE
        horiz = 1;
        vert = 1;
        break;
    case 3: // SW
        horiz = -1;
        vert = 1;
        break;
    case 4: // W
        horiz = -2;
        break;
    case 5: // NW
        horiz = -1;
        vert = -1;
        break;
    }
    while (board[temp_y + vert][temp_x + horiz] != '_') {
        if (board[temp_y + vert][temp_x + horiz] == '+') {
            cout << endl << "BAD_MOVE_ROW_IS_FULL" << endl;
            return false;
        }
        temp_x += horiz;
        temp_y += vert;
    }
    int start_x = temp_x + horiz, start_y = temp_y + vert;
    do {
        board[temp_y + vert][temp_x + horiz] = board[temp_y][temp_x];
        temp_y -= vert;
        temp_x -= horiz;
    } while (x != temp_x || y != temp_y);
    board[temp_y + vert][temp_x + horiz] = turn;
    if (col != ' ') {
        int pos_1_x = (int)(letter_f - 'a') + 1;
        int pos_2_x = (int)(letter_l - 'a') + 1;
        int where = pos_2_x - pos_1_x;
        int find_x = 0, find_y = size;
        int x = -1, y, x_2 = -1, y_2, dir;
        for (int i = 1; i <= 2 * size + 1; i++) {
            if (x != -1 && x_2 != -1) break;
            if (pos_1_x == i) {
                x = find_x;
                y = find_y;
            }
            if (pos_2_x == i) {
                x_2 = find_x;
                y_2 = find_y;
            }
            if (find_y < 2 * size) {
                find_x++;
                find_y++;
            }
            else {
                find_x += 2;
            }
        }
        int index = 1;
        while (true) {
            if (index == num_f) break;
            if (board[y - 1][x + 1] != ' ') {
                y--;
                x++;
                index++;
            }
            else break;
        }
        index = 1;
        while (true) {
            if (index == num_l) break;
            if (board[y_2][x_2] != ' ') {
                y_2--;
                x_2++;
                index++;
            }
        }
        if (x_2 - x > 0) {
            if (y_2 - y > 0) {
                horiz = 1;
                vert = 1;
            }
            else if (y_2 - y < 0) {
                horiz = 1;
                vert = -1;
            }
            else {
                horiz = -2;
                vert = 0;
            }
        }
        else {
            if (y_2 - y > 0) {
                horiz = -1;
                vert = 1;
            }
            else if (y_2 - y < 0) {
                horiz = -1;
                vert = -1;
            }
            else {
                horiz = 2;
                vert = 0;
            }
        }
        if (board[y - vert][x - horiz] == col) {
            cout << "WRONG_INDEX_OF_CHOSEN_ROW" << endl;
            return false;
        }
        do {
            if (board[y][x] != col) {
                cout << "WRONG_COLOR_OF_CHOSEN_ROW" << endl;
                return false;
            }
            x += horiz;
            y += vert;
        } while (x_2 != x || y_2 != y);
        if (board[y + vert][x + horiz] == col) {
            cout << "WRONG_INDEX_OF_CHOSEN_ROW" << endl;
            return false;
        }
        while (board[y - vert][x - horiz] != '_' && board[y - vert][x - horiz] != '+') {
            y -= vert;
            x -= horiz;
        }
        do {
            if (board[y][x] == col) {
                if (col == 'W') whiteInReserve++;
                if (col == 'B') blackInReserve++;
            }
            board[y][x] = '_';
            y += vert;
            x += horiz;
        } while (board[y][x] != '_' && board[y][x] != '+');
        print_after = true;
        return true;
    }
    do {
        check_path(start_x, start_y);
        start_y -= vert;
        start_x -= horiz;
    } while (start_x != temp_x || start_y != temp_y);
    return true;
};
void Board::check_path(int x, int y) {
    int vert[3] = {-1,-1,0};
    int horiz[3] = {-1,1,2};
    char color = board[y][x];
    char curr;
    for (int i = 0; i < 3; i++) {
        int occur = 1;
        int temp_x = x, temp_y = y;
        while (board[temp_y+vert[i]][temp_x+horiz[i]] == color) {
            curr = board[temp_y + vert[i]][temp_x + horiz[i]];
            temp_y += vert[i];
            temp_x += horiz[i];
            occur++;
        }

        temp_x = x, temp_y = y;
        while (board[temp_y - vert[i]][temp_x - horiz[i]] == color) {
            temp_y -= vert[i];
            temp_x -= horiz[i];
            occur++;
        }
        if (occur >= collection) collecting(x, y, i,color);
    }
};
int Board::check_start() {
    int rows = 0, occupied;
    char sign;
    for (int i = 1; i < 2 * size; i++) {
        int occupied = 0;
        sign = '1';
        for (int j = 2; j < 4 * size - 1; j++) {
            if (board[i][j] == ' ' || board[i][j] == '*') continue;
            if (board[i][j] != '_' && occupied == 0) {
                sign = board[i][j];
                occupied++;
            }
            else if(occupied > 0){
                if (board[i][j] == sign) {
                    occupied++;
                }
                else if (board[i][j] == '_') {
                    occupied = 0;
                }
                else {
                    occupied = 1;
                    sign = board[i][j];
                }
                if (occupied == collection) {
                    rows++;
                    break;
                }
            }
        }
    }
    int x = 2, y = size;
    for (int i = 1; i < 2 * size; i++) {
        rows += check_start_right(x, y);
        rows += check_start_left(4 * size - x, y);
        if (y < 2 * size - 1) {
            x++;
            y++;
        }
        else {
            x += 2;
        }
    }
    return rows;
}
int Board::check_start_right(int x, int y) {
    char sign;
    int occupied = 0;
    while (board[y][x] != '+') {
        if (board[y][x] != '_' && occupied == 0) {
            sign = board[y][x];
            occupied++;
        }
        else if (occupied > 0) {
            if (board[y][x] == sign) {
                occupied++;
            }
            else if (board[y][x] == '_') {
                occupied = 0;
            }
            else {
                occupied = 1;
                sign = board[y][x];
            }
            if (occupied == collection) {
                return 1;
            }
        }
        y--;
        x++;
    }
    return 0;

}
void Board::collecting(int x, int y, int dir, char turn)
{
    int vert[3] = { -1,-1,0 };
    int horiz[3] = { -1,1,2 };
    char curr;
    while (board[y - vert[dir]][x - horiz[dir]] != '_' && board[y - vert[dir]][x - horiz[dir]] != '+') {
        y -= vert[dir];
        x -= horiz[dir];
    }
    do {
        if (board[y][x] == turn) {
            if (turn == 'W') whiteInReserve++;
            if (turn == 'B') blackInReserve++;
        }
        board[y][x] = '_';
        y += vert[dir];
        x += horiz[dir];
    } while (board[y][x] != '_' && board[y][x] != '+');
};
int Board::check_start_left(int x, int y) {
    char sign;
    int occupied = 0;
    while (board[y][x] != '+') {
        if (board[y][x] != '_' && occupied == 0) {
            sign = board[y][x];
            occupied++;
        }
        else if (occupied > 0) {
            if (board[y][x] == sign) {
                occupied++;
            }
            else if (board[y][x] == '_') {
                occupied = 0;
            }
            else {
                occupied = 1;
                sign = board[y][x];
            }
            if (occupied == collection) {
                return 1;
            }
        }
        y--;
        x--;
    }
    return 0;
};
int Board::is_neighbour(int x, int y, int x_2, int y_2) {
    int horiz[6] = {-1,-2,-1,1,2,1};
    int vert[6] = {1,0,-1,-1,0,1};
    for (int i = 0; i < 6; i++) {
        if (x_2 + horiz[i] == x && y_2 + vert[i] == y) return i;
    }
    return -1;
}
void Board::all_possible_moves() {
    Node* head = nullptr;
    int x = 2, y = size, mode = 0, curr = 1,unique=0;
    int horiz[6] = {1,2,1,-1,-2,-1};
    int vert[6] = { -1,0,1,1,0,-1 };
    for (int i = 0; i < 6 * size - 6; i++) {
        //cout << x << "\t" << y <<"\t"<<board[y][x]<<endl;
            for (int j = 0; j < 6; j++) {
                if (i == 5 && j == 2) {
                    int dufus=34;
                }
                if (board[y - vert[j]][x - horiz[j]] == '+') {
                    boarding(board, head, vert[j], horiz[j], x, y, turn, size);
                }
            }// i=5 j=2
        x += horiz[mode];
        y += vert[mode];
        curr++;
        if (curr == size) {
            mode++;
            curr = 1;
        }
    }

    Node* current = head;
    while (current != nullptr) {
        unique++;
        current = current->next;
    }
    cout << unique << "_UNIQUE_MOVES" << endl;;
}
Board::~Board() {};