#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>
#include <ctime>
#include <cmath>
#include <gl/glu.h>
#include "ReadOBJ.h"
#include "sdlglutils.h"
#include "textures.h"
#define ALPHA 5.0/180.0


LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
void light (void);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    /* initialize local var */
    GLdouble eyeX,eyeY,eyeZ,directionX,directionY,directionZ,upX,upY,upZ,droiteX,droiteY,droiteZ;
    eyeX=-1.0;
    eyeY=0.0;
    eyeZ=0.0;
    directionX = 1.0;
    directionY = 0.0;
    directionZ = 0.0;
    upX=0.0;
    upY=1.0;
    upZ=0.0;
    droiteX=0.0;
    droiteY=0.0;
    droiteZ=1.0;
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;
    float direction =0.0f;
    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          700,
                          700,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    /* Compile GL List */
    GLuint tie = creatList("starwars-tie-fighter.obj");
    GLuint xwing = creatList("star-wars-x-wing.obj");

    /* Load Textures */
    GLuint texture2 = loadBMP_custom("1.bmp");
    GLuint texture1 = noTexture();
    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) //gestion des messages
        {
            /* handle or dispatch messages */
            GLfloat dx = directionX;
            GLfloat dy = directionY;
            GLfloat dz = directionZ;
            GLfloat ux = upX;
            GLfloat uy = upY;
            GLfloat uz = upZ;
            switch(msg.message)
            {
            case WM_QUIT :
                bQuit = TRUE;
                printf("BB\n");
                break;
            case WM_LBUTTONDOWN :
                printf("CLICK CLICK CLICK\n");
                break;
            case WM_CHAR :
                printf("0x%x : ",msg.wParam);
                printf("0x%x : ",msg.lParam);
                printf("(%d;%d) : ",msg.pt.x,msg.pt.y);
                printf("%d\n",msg.time/CLOCKS_PER_SEC);
                break;
            case WM_GETDLGCODE :
                printf("0x%x \n",msg.wParam);
                break;
            case WM_KEYDOWN :
                printf("DOWN : 0x%x\n",msg.wParam);
                switch(msg.wParam) {
                case 0x5a :
                    //Z
                    directionX = directionX*cos(ALPHA)-upX*sin(ALPHA);
                    directionY = directionY*cos(ALPHA)-upY*sin(ALPHA);
                    directionZ = directionZ*cos(ALPHA)-upZ*sin(ALPHA);
                    upX = dx*sin(ALPHA)+upX*cos(ALPHA);
                    upY = dy*sin(ALPHA)+upY*cos(ALPHA);
                    upZ = dz*sin(ALPHA)+upZ*cos(ALPHA);
                    //printf("UP VAUT : %f %f %f\n",upX,upY,upZ);
                    break;
                case 0x44 :
                    //D
                    directionX = directionX*cos(ALPHA)+droiteX*sin(ALPHA);
                    directionY = directionY*cos(ALPHA)+droiteY*sin(ALPHA);
                    directionZ = directionZ*cos(ALPHA)+droiteZ*sin(ALPHA);
                    droiteX = -dx*sin(ALPHA)+droiteX*cos(ALPHA);
                    droiteY = -dy*sin(ALPHA)+droiteY*cos(ALPHA);
                    droiteZ = -dz*sin(ALPHA)+droiteZ*cos(ALPHA);
                    break;
                case 0x53 :
                    //S
                    directionX = directionX*cos(ALPHA)+upX*sin(ALPHA);
                    directionY = directionY*cos(ALPHA)+upY*sin(ALPHA);
                    directionZ = directionZ*cos(ALPHA)+upZ*sin(ALPHA);
                    upX = -dx*sin(ALPHA)+upX*cos(ALPHA);
                    upY = -dy*sin(ALPHA)+upY*cos(ALPHA);
                    upZ = -dz*sin(ALPHA)+upZ*cos(ALPHA);
                    break;
                case 0x51 :
                    //Q
                    directionX = directionX*cos(ALPHA)-droiteX*sin(ALPHA);
                    directionY = directionY*cos(ALPHA)-droiteY*sin(ALPHA);
                    directionZ = directionZ*cos(ALPHA)-droiteZ*sin(ALPHA);
                    droiteX = dx*sin(ALPHA)+droiteX*cos(ALPHA);
                    droiteY = dy*sin(ALPHA)+droiteY*cos(ALPHA);
                    droiteZ = dz*sin(ALPHA)+droiteZ*cos(ALPHA);
                    break;
                case 0x41 :
                    //A
                    upX = upX*cos(ALPHA)-droiteX*sin(ALPHA);
                    upY = upY*cos(ALPHA)-droiteY*sin(ALPHA);
                    upZ = upZ*cos(ALPHA)-droiteZ*sin(ALPHA);
                    droiteX = ux*sin(ALPHA)+droiteX*cos(ALPHA);
                    droiteY = uy*sin(ALPHA)+droiteY*cos(ALPHA);
                    droiteZ = uz*sin(ALPHA)+droiteZ*cos(ALPHA);
                    break;
                case 0x45 :
                    //E
                    upX = upX*cos(ALPHA)+droiteX*sin(ALPHA);
                    upY = upY*cos(ALPHA)+droiteY*sin(ALPHA);
                    upZ = upZ*cos(ALPHA)+droiteZ*sin(ALPHA);
                    droiteX = -ux*sin(ALPHA)+droiteX*cos(ALPHA);
                    droiteY = -uy*sin(ALPHA)+droiteY*cos(ALPHA);
                    droiteZ = -uz*sin(ALPHA)+droiteZ*cos(ALPHA);
                    break;
                case 0x57 :
                    eyeX += 0.1*directionX;
                    eyeY += 0.1*directionY;
                    eyeZ += 0.1*directionZ;
                    break;
                case 0x58 :
                    eyeX += -0.1*directionX;
                    eyeY += -0.1*directionY;
                    eyeZ += -0.1*directionZ;
                    break;
                case 0x52 :
                    printf("POSITION : %f , %f , %f\n",eyeX,eyeY,eyeZ);
                default :
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                break;
            case WM_KEYUP :
                printf("UP : 0x%x\n",msg.wParam);
                switch(msg.wParam)
                {
                    case VK_UP:
                        direction +=1.0f;
                        break;
                    case VK_DOWN:
                        direction +=-1.0f;
                        break;
                }
            default :
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                break;
            }
        }
        else //affichage
        {
            /* OpenGL animation code goes here */
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            light();
            gluPerspective (60.0, 250/(float)250, 0.1, 500.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
			gluLookAt(eyeX,eyeY,eyeZ,eyeX+directionX,eyeY+directionY,eyeZ+directionZ,upX,upY,upZ);
            //origin
            /*glPointSize(2.0);
            glPushMatrix();
            glBegin(GL_POINTS);
                glColor3f(1.0f,0.0f,0.0f);
                glVertex3f(0.0f,0.0f,0.0f);
            glEnd();
            glPopMatrix();
            //triangle
            glPushMatrix();
            glRotatef(theta, 1.0f, 0.0f, 0.0f);

            glBegin(GL_TRIANGLES);

                glColor3f(1.0f, 0.0f, 0.0f);   glVertex3f(0.0f,   1.0f, 0.0f);
                glColor3f(0.0f, 1.0f, 0.0f);   glVertex3f(0.87f,  -0.5f, 0.0f);
                glColor3f(0.0f, 0.0f, 1.0f);   glVertex3f(-0.87f, -0.5f, 0.0f);

            glEnd();
            glPopMatrix();*/
            //glPushMatrix();
            glPushMatrix();
            glRotated(45,0,0,1);
            glTranslated(0,0,50);
            glBindTexture(GL_TEXTURE_2D,texture1);
            glCallList(xwing); //ICI
            //glPopMatrix();
            /*glBegin(GL_QUADS);
            {
                glColor3f(1.0f,1.0f,0.0f);
                glVertex3f(-1.0f,-1.0f,0.0f);
                glVertex3f(1.0f,-1.0f,0.0f);
                glColor3f(1.0f,0.0f,0.0f);
                glVertex3f(1.0f,1.0f,0.0f);
                glVertex3f(-1.0f,1.0f,0.0f);
            }*/
            glEnd();
            glPopMatrix();
                glColor3d(1,1,1);
                glDisable(GL_LIGHTING);
                glBindTexture(GL_TEXTURE_2D,texture2);
                GLUquadric* params = gluNewQuadric();
                gluQuadricDrawStyle(params,GLU_FILL);
                gluQuadricTexture(params,GL_TRUE);
                gluSphere(params,100,20,20);
                gluDeleteQuadric(params);
                glEnable(GL_LIGHTING);
            glBindTexture(GL_TEXTURE_2D,texture1);
            glCallList(tie); //ICI
            glLineWidth(1.0);
            glBegin(GL_LINES);
                glColor3f(1.0f,0.0f,0.0f);
                glVertex3f(0.0f,0.0f,0.0f);
                glVertex3f(1.0f,0.0f,0.0f);

                glColor3f(0.0f,1.0f,0.0f);
                glVertex3f(0.0f,0.0f,0.0f);
                glVertex3f(0.0f,1.0f,0.0f);

                glColor3f(0.0f,0.0f,1.0f);
                glVertex3f(0.0f,0.0f,0.0f);
                glVertex3f(0.0f,0.0f,1.0f);
            glEnd();
            glLineWidth(2.0);
            glBegin(GL_LINES);
                glColor3f(1.0f,0.0f,0.0f);
                glVertex3f(eyeX+1.0*directionX,eyeY+1.0*directionY,eyeZ+1.0*directionZ);
                glVertex3f(eyeX+2.0*directionX,eyeY+2.0*directionY,eyeZ+2.0*directionZ);

                glColor3f(0.0f,1.0f,0.0f);
                glVertex3f(eyeX+1.0*directionX,eyeY+1.0*directionY,eyeZ+1.0*directionZ);
                glVertex3f(eyeX+1.0*directionX+upX,eyeY+1.0*directionY+upY,eyeZ+1.0*directionZ+upZ);

                glColor3f(0.0f,0.0f,1.0f);
                glVertex3f(eyeX+1.0*directionX,eyeY+1.0*directionY,eyeZ+1.0*directionZ);
                glVertex3f(eyeX+1.0*directionX+droiteX,eyeY+1.0*directionY+droiteY,eyeZ+1.0*directionZ+droiteZ);

                /*glColor3f(1.0f,0.0f,0.0f);
                glVertex3f(eyeX+0.1*directionX,eyeY+0.1*directionY,eyeZ+0.1*directionZ);
                glVertex3f(eyeX+1.0+0.1*directionX,eyeY+0.1*directionY,eyeZ+0.1*directionZ);

                glColor3f(0.0f,1.0f,0.0f);
                glVertex3f(eyeX+0.1*directionX,eyeY+0.1*directionY,eyeZ+0.1*directionZ);
                glVertex3f(eyeX+0.1*directionX,eyeY+1.0+0.1*directionY,eyeZ+0.1*directionZ);

                glColor3f(0.0f,0.0f,1.0f);
                glVertex3f(eyeX+0.1*directionX,eyeY+0.1*directionY,eyeZ+0.1*directionZ);
                glVertex3f(eyeX+0.1*directionX,eyeY+0.1*directionY,eyeZ+1.0+0.1*directionZ);*/
            glEnd();
            glPointSize(5.0);
            glBegin(GL_POINTS);
                glColor3f(1.0f,0.0f,0.0f);
                glVertex3f(-1.0f,0.0f,0.0f);
            glEnd();


            SwapBuffers(hDC);

            theta += direction*1.0f;
            Sleep (1);
        }
    }

    /* shutdown OpenGL */
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_TEXTURE_2D);
    DisableOpenGL(hwnd, hDC, hRC);
    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);
    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

void light (void) {
    GLfloat whiteSpecularLight[] = {0.2, 0.2, 0.2};
    GLfloat blackAmbientLight[] = {0.0, 0.0, 0.0};
    GLfloat whiteDiffuseLight[] = {0.1, 0.1, 0.1};
    glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);
}
