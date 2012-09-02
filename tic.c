
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <stdlib.h>

#include <stdio.h>
#include "board.h"
#include "geometry.h"

GLuint boardDim = 5;
GLdouble cameraRadius = 3.0;
GLdouble cameraHeight = 7.0;
GLfloat cellSpacing = 0.02;

GLubyte none_color[] = { 100, 100, 100 };
GLubyte x_color[] = { 0, 100, 0 };
GLubyte o_color[] = { 100, 0, 0 };

GLubyte selector_color[] = { 0, 200, 100 };

struct game_state {
    struct board *board;

    /* board geometry */
    GLfloat *board_vertices;
    GLuint *board_indices;
    GLubyte *board_colors;

    /* selector geometry */
    GLfloat *selector_vertices;
    GLuint *selector_indices;
    GLubyte *selector_colors;

    int selector_row;
    int selector_col;

    enum piece current_player;
} state;

void resetGameState(void)
{
    reset(state.board);
    state.selector_row = state.selector_col = 0;
    state.current_player = PIECE_X;
}

int getVertexBaseIndex(int x, int y)
{
    return (x * state.board->dim + y) * 4 * 3;
}

void createBoard(void)
{
    state.board = new_board(boardDim);

    createBoardGeometry(state.board->dim,
                        cellSpacing,
                        &(state.board_vertices),
                        &(state.board_indices));

    state.board_colors = malloc(sizeof(GLubyte) *
                                state.board->dim * state.board->dim *
                                4 * 3);

    // Selector is a basic quad.
    state.selector_vertices = malloc(sizeof(GLfloat) * 4 * 3);
    state.selector_vertices[0] = 0.0;
    state.selector_vertices[1] = 0.0;
    state.selector_vertices[2] = 0.0;

    state.selector_vertices[3] = 1.0;
    state.selector_vertices[4] = 0.0;
    state.selector_vertices[5] = 0.0;

    state.selector_vertices[6] = 1.0;
    state.selector_vertices[7] = 0.0;
    state.selector_vertices[8] = 1.0;

    state.selector_vertices[9] = 0.0;
    state.selector_vertices[10] = 0.0;
    state.selector_vertices[11] = 1.0;

    // Pick indices in counter-clockwise order.
    state.selector_indices = malloc(sizeof(GLuint) * 4);
    state.selector_indices[0] = 0;
    state.selector_indices[1] = 1;
    state.selector_indices[2] = 2;
    state.selector_indices[3] = 3;

    // Set the color of every selector vertex.
    state.selector_colors = malloc(sizeof(GLubyte) * 3 * 4);
    state.selector_colors[0] = selector_color[0];
    state.selector_colors[1] = selector_color[1];
    state.selector_colors[2] = selector_color[2];

    state.selector_colors[3] = selector_color[0];
    state.selector_colors[4] = selector_color[1];
    state.selector_colors[5] = selector_color[2];

    state.selector_colors[6] = selector_color[0];
    state.selector_colors[7] = selector_color[1];
    state.selector_colors[8] = selector_color[2];

    state.selector_colors[9] = selector_color[0];
    state.selector_colors[10] = selector_color[1];
    state.selector_colors[11] = selector_color[2];

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,
              cameraHeight,
              cameraRadius,
              0, 0, 0, 0, 1, 0);

    glPushMatrix();
    glTranslatef(-(state.board->dim / 2.0), 0.0, -(state.board->dim / 2.0));

    glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 3, state.board_vertices);
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(GLubyte) * 3, state.board_colors);
    glDrawElements(GL_QUADS, state.board->dim * state.board->dim * 4, GL_UNSIGNED_INT,
                   state.board_indices);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(state.board->dim / 2.0) + state.selector_col,
                 -0.01,
                 -(state.board->dim / 2.0) + state.selector_row);
    glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 3, state.selector_vertices);
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(GLubyte) * 3, state.selector_colors);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, state.selector_indices);
    glPopMatrix();

    /* Camera */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 0.1, 30.0);

    glutSwapBuffers();
}

void idle(void)
{
    glutPostRedisplay();
}

void updatePresentation()
{
    // Set vertex colors from game state and selector state.

    int i, j, base;
    GLubyte *color_ptr;
    enum piece p;

    for (i = 0; i < state.board->dim; i++) {
        for (j = 0; j < state.board->dim; j++) {
            p = get(state.board, i, j);
            color_ptr = (p == PIECE_NONE ? none_color :
                         p == PIECE_X ? x_color :
                         o_color);

            base = getVertexBaseIndex(i, j);

            state.board_colors[base] = color_ptr[0];
            state.board_colors[base + 1] = color_ptr[1];
            state.board_colors[base + 2] = color_ptr[2];

            state.board_colors[base + 3] = color_ptr[0];
            state.board_colors[base + 4] = color_ptr[1];
            state.board_colors[base + 5] = color_ptr[2];

            state.board_colors[base + 6] = color_ptr[0];
            state.board_colors[base + 7] = color_ptr[1];
            state.board_colors[base + 8] = color_ptr[2];

            state.board_colors[base + 9] = color_ptr[0];
            state.board_colors[base + 10] = color_ptr[1];
            state.board_colors[base + 11] = color_ptr[2];
        }
    }
}

void handleKey(unsigned char key, int x, int y)
{
    struct win *w;

    switch (key) {
    case 'w':
        state.selector_row = (state.selector_row == 0 ? 0 : state.selector_row - 1);
        break;
        ;;
    case 's':
        state.selector_row = (state.selector_row == state.board->dim - 1 ?
                              state.board->dim - 1 : state.selector_row + 1);
        break;
        ;;
    case 'a':
        state.selector_col = (state.selector_col == 0 ? 0 : state.selector_col - 1);
        break;
        ;;
    case 'd':
        state.selector_col = (state.selector_col == state.board->dim - 1 ?
                              state.board->dim - 1 : state.selector_col + 1);
        break;
        ;;
    case ' ':
        if (place(state.board, state.selector_row, state.selector_col, state.current_player) == 0) {
            state.current_player = state.current_player == PIECE_X ? PIECE_O : PIECE_X;
        }
        break;
        ;;
    }

    w = find_win(state.board);
    if (w != NULL) {
        printf("Player %s won on %s %d\n",
               w->winning_piece == PIECE_X ? "X" : "O",
               w->type == WIN_DIAGONAL ? "diagonal" : w->type == WIN_ROW ? "row" : "column",
               w->which);
        free_win(w);

        resetGameState();
    } else if (!moves_possible(state.board)) {
        printf("Stalemate, resetting\n");
        resetGameState();
    }

    updatePresentation();
}

void init(void)
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 128.0 };
    GLfloat ambient0_intensity[] = { 0.8, 0.8, 0.8, 0.0 };

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 0.1, 30.0);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

/*     glLightfv(GL_LIGHT0, GL_POSITION, light0_position); */
/*     glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0_intensity); */

/*     glEnable(GL_LIGHTING); */
/*     glEnable(GL_LIGHT0); */
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    printf("Starting up\n");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Tic");
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKey);
    glutIdleFunc(idle);
    glewInit();
    if (!GLEW_VERSION_2_0) {
        printf("OpenGL 2.0 unavailable\n");
        return 1;
    }

    printf("Initializing\n");
    init();

    printf("Creating board geometry\n");
    createBoard();

    printf("Initializing game state\n");
    resetGameState();
    updatePresentation();

    printf("Running\n");
    glutMainLoop();

    return 0;
}
