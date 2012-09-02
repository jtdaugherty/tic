
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"

struct board* new_board(int dim)
{
    struct board * b;

    b = malloc(sizeof(struct board));
    if (b == NULL)
        return NULL;

    b->dim = dim;
    b->grid = malloc(sizeof(enum piece) * dim * dim);
    if (b->grid == NULL) {
        free(b);
        return NULL;
    }

    reset(b);
    return b;
}

int reset(struct board *b)
{
    if (b == NULL)
        return 1;

    for (int i = 0; i < b->dim * b->dim; i++)
        b->grid[i] = PIECE_NONE;

    return 0;
}

int free_board(struct board *b)
{
    if (b == NULL)
        return 1;

    free(b->grid);
    free(b);

    return 0;
}

enum piece get(struct board *b, int row, int col)
{
    return b->grid[row * b->dim + col];
}

int place(struct board *b, int row, int col, enum piece p)
{
    int pos;

    assert(b != NULL);

    pos = row * b->dim + col;
    assert(pos >= 0 && pos < b->dim * b->dim);

    if (b->grid[pos] == PIECE_NONE) {
        b->grid[pos] = p;
        return 0;
    } else {
        return 1;
    }
}

int find_win_diagonal(struct board *b, struct win *w)
{
    enum piece p;
    int i, j;

    // Check first diagonal (top left to bottom right)
    p = get(b, 0, 0);
    for (i = 1; i < b->dim; i++) {
        if (get(b, i, i) != p)
            p = PIECE_NONE;
    }

    if (p != PIECE_NONE) {
        w->winning_piece = p;
        w->type = WIN_DIAGONAL;
        w->which = 0;
        return 0;
    }

    // Check second diagonal (top right to bottom left)
    p = get(b, b->dim - 1, 0); // Start with bottom left board cell
    for (i = b->dim - 2, j = 1; i >= 0 && j < b->dim; i--, j++) {
        if (get(b, i, j) != p)
            p = PIECE_NONE;
    }

    if (p != PIECE_NONE) {
        w->winning_piece = p;
        w->type = WIN_DIAGONAL;
        w->which = 1;
        return 0;
    }

    return 1;
}

int find_win_row(struct board *b, struct win *w)
{
    enum piece p;
    int i, j;

    for (i = 0; i < b->dim; i++) {
        p = get(b, i, 0);
        for (j = 1; j < b->dim; j++) {
            if (get(b, i, j) != p)
                p = PIECE_NONE;
        }

        if (p != PIECE_NONE) {
            w->winning_piece = p;
            w->type = WIN_ROW;
            w->which = i;
            return 0;
        }
    }

    return 1;
}

int find_win_column(struct board *b, struct win *w)
{
    enum piece p;
    int i, j;

    for (i = 0; i < b->dim; i++) {
        p = get(b, 0, i);
        for (j = 1; j < b->dim; j++) {
            if (get(b, j, i) != p)
                p = PIECE_NONE;
        }

        if (p != PIECE_NONE) {
            w->winning_piece = p;
            w->type = WIN_COLUMN;
            w->which = j;
            return 0;
        }
    }

    return 1;
}

struct win * find_win(struct board *b)
{
    struct win *w = malloc(sizeof(struct win));

    // Is there a winning diagonal?
    if (find_win_diagonal(b, w) == 0)
        return w;

    // Is there a winning row?
    if (find_win_row(b, w) == 0)
        return w;

    // Is there a winning column?
    if (find_win_column(b, w) == 0)
        return w;

    free_win(w);
    return NULL;
}

int free_win(struct win *w)
{
    if (w == NULL)
        return 1;

    free(w);

    return 0;
}

int moves_possible(struct board *b)
{
    int i, j;

    for (i = 0; i < b->dim; i++)
        for (j = 0; j < b->dim; j++)
            if (get(b, i, j) == PIECE_NONE)
                return 1;

    return 0;
}
