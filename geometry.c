
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include "geometry.h"

void createBoardGeometry(int board_side_length, GLfloat spacing,
                         GLfloat **_vertices, GLuint **_indices)
{
    int i, j, base, i_base;
    GLfloat space_width = 1.0;
    int num_spaces = board_side_length * board_side_length;
    GLfloat *vertices;
    GLuint *indices;

    // Allocate and populate buffers
    vertices = malloc(sizeof(GLfloat) * num_spaces * 4 * 3);
    indices = malloc(sizeof(GLuint) * num_spaces * 4);

    for (i = 0; i < board_side_length; i++) {
        for (j = 0; j < board_side_length; j++) {
            base = (i * board_side_length + j) * 12;
            i_base = (i * board_side_length + j) * 4;

            // X, Y, Z coordinates of each corner of this space on the
            // board.

            vertices[base] = j * space_width + spacing;
            vertices[base + 1] = 0.0;
            vertices[base + 2] = i * space_width + spacing;

            vertices[base + 3] = j * space_width + spacing;
            vertices[base + 4] = 0.0;
            vertices[base + 5] = (i + 1) * space_width - spacing;

            vertices[base + 6] = (j + 1) * space_width - spacing;
            vertices[base + 7] = 0.0;
            vertices[base + 8] = (i + 1) * space_width - spacing;

            vertices[base + 9] = (j + 1) * space_width - spacing;
            vertices[base + 10] = 0.0;
            vertices[base + 11] = i * space_width + spacing;

            indices[i_base] = i_base + 3;
            indices[i_base + 1] = i_base + 2;
            indices[i_base + 2] = i_base + 1;
            indices[i_base + 3] = i_base;
        }
    }

    *_vertices = vertices;
    *_indices = indices;
}
