#ifndef TEXTURES_H
#define TEXTURES_H
#define RETURN_ERROR 1
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <stdlib.h>
GLuint loadBMP_custom(const char * imagepath);
GLuint noTexture();
#endif // TEXTURES_H
