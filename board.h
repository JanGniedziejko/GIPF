#pragma once

#ifndef Board_h
#define Board_h

class Board {
private:
	int size;
	int collection;
	int whiteAll, blackAll;
	int whiteOnBoard=0, blackOnBoard=0;
	int whiteInReserve, blackInReserve;
	char turn;
    bool print_after = false,empty=false;
	char** board;
public:
	Board();
	Board(int size);
	void Set(int size, int collection, int white, int black,int whiteInReserve, int blackInReserve, char turn);
	void Print();
	bool move(char letter_1, int num_1, char letter_2, int num_2,char col,char letter_f, int num_f, char letter_l, int num_l);
    bool move_chain(int x, int y, int dir, char col, char letter_f, int num_f, char letter_l, int num_l);
    void check_path(int x, int y);
    int check_start();
    int check_start_left(int x, int y);
    int check_start_right(int x, int y);
	int is_neighbour(int x, int y, int x_2, int y_2);
    void collecting(int x, int y, int dir, char turn);
    void all_possible_moves();
	~Board();
};

#endif

/*
case 1: // NE
        horiz = 1;
        vert = -1;
        break;
    case 2: // E
        horiz = 2;
        break;
    case 3: // SE
        horiz = 1;
        vert = 1;
        break;
    case 4: // SW
        horiz = -1;
        vert = 1;
        break;
    case 5: // W
        horiz = -2;
        break;
    case 6: // NW
        horiz = -1;*/