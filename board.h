
#ifndef __BOARD_H__
#define __BOARD_H__

enum piece {
    PIECE_NONE,
    PIECE_X,
    PIECE_O
};

enum win_type {
    WIN_DIAGONAL,
    WIN_ROW,
    WIN_COLUMN
};

struct board {
    enum piece *grid;
    int dim;
};

struct win {
    enum piece winning_piece;
    enum win_type type;
    int which;
};

struct board * new_board(int dim);
int reset(struct board *b);
int place(struct board *b, int row, int col, enum piece p);
int free_board(struct board *b);
struct win * find_win(struct board *b);
int free_win(struct win *w);
enum piece get(struct board *b, int row, int col);
int moves_possible(struct board *b);

#endif
