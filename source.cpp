#include <iostream>
#include <stdlib.h>
#include <string>
#include "board.h"

using namespace std;
int decypher(string text) {
    int ten = 1;
    int num=0;
    for (int i = text.length() - 1; i >= 0; i--) {
        num += int(text[i] - '0') * ten;
        ten *= 10;
    }
    return num;
}

int main() {
    int size,collection,white,black,white_res,black_res;
    char turn;
    string positions,prev_pos, next_pos;
    Board board;
    string command;
    while (cin >> command) {
        if (command == "LOAD_GAME_BOARD") {
            cin >> size >> collection >> white >> black >> white_res >> black_res >> turn;
            board.Set(size, collection, white, black, white_res, black_res, turn);
        }
        else if (command == "PRINT_GAME_BOARD") {
            board.Print();
        }
        else if (command == "DO_MOVE") {
            cin >> positions;
            char c = getchar();
            if (c == ' ') {
                string color, first, last;
                cin >> color >> first >> last;
                int index = positions.find('-');
                prev_pos = positions.substr(0, index);
                next_pos = positions.substr(index + 1);
                board.move(prev_pos[0], decypher(prev_pos.substr(1)), next_pos[0], decypher(next_pos.substr(1)),color[0] - 32, first[0], decypher(first.substr(1)),last[0], decypher(last.substr(1)));
            }
            else {
                int index = positions.find('-');
                prev_pos = positions.substr(0, index);
                next_pos = positions.substr(index + 1);
                board.move(prev_pos[0], decypher(prev_pos.substr(1)), next_pos[0], decypher(next_pos.substr(1)),' ', ' ', 0, ' ', 0);

            }
        }
        else if (command == "GEN_ALL_POS_MOV_NUM") {
            board.all_possible_moves();
        }
    }

    //board.Print();
    return 0;
}
/* 
     * * * * * *         0
    * - - - - - *        1  
   * - - - - - - *       2
  * - - - - - - - *      3
 * - - - - - - - - *     4
* - - - - - - - - - *    5
 * - - - - - - - - *     6
  * - - - - - - - *      7
   * - - - - - - *       8
    * - - - - - *        9
     * * * * * *         10
     * 
     01234567890123456
0        * * * * *    
1       * - - - - *
2      * - - - - - *
3     * - - - - - - *        x: 2   3
4    * - - - - - - - *   coord: 14  13 ...
5     * - - - - - - *
6      * - - - - - *
7       * - - - - *
8        * * * * *


17
21

>15 || >= 16
21 - 6

2 * (2 * (size + 1) - 1) - 1

*/