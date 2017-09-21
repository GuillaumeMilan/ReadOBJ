#include <windows.h>
#include<gl/gl.h>
#include <stdio.h>
#include "ReadOBJ.h"
#include <vector>
#include <string.h>

//#include<gl/glut.h>
//globals
float elephantrot;
//other functions and main
//wavefront .obj loader code begins
GLuint loadObj(char *fname)
{
    GLuint elephant;
    FILE *fp;
    int read;
    GLfloat x, y, z;
    char ch;
    elephant=glGenLists(1);
    fp=fopen(fname,"r");
    if (!fp)
    {
        printf("can't open file %s\n", fname);
        PostQuitMessage(1);
    }
    glPointSize(2.0);
    glNewList(elephant, GL_COMPILE);
    {
        //glPushMatrix();
        glBegin(GL_POINTS);
        while(!(feof(fp)))
        {
            read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
            if(read==4&&ch=='v')
            {
                glVertex3f(x,y,z);
            }
        }
        glEnd();
    }
    //glPopMatrix();
    glEndList();
    fclose(fp);
    return elephant;
}
void drawTriangle() {
    GLuint essai_de_ouf = glGenLists(1);
    glNewList(essai_de_ouf, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    {
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f(1.0f,0.0f,0.0f);
        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(0.0f,0.0f,1.0f);
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f(0.0f,1.0f,0.0f);
    }
    glEnd();
    glEndList();
}
GLuint creatList(char *fname)
/* Open the file ... +.obj and +.mtl */
{
    /* CLEAR BUFFER TO AVOID MULTI COLORING AFFECTIN SHAPE */
    glClear(GL_COLOR_BUFFER_BIT);
    GLuint shapeList;
    char chaine[4][255];
    FILE *objFile,*mtl;
    int read;
    GLfloat x, y, z;
    std::vector<FILE*> mtlFiles;
    std::vector<GLfloat> vx;
    std::vector<GLfloat> vy;
    std::vector<GLfloat> vz;
    std::vector<GLfloat> vnx;
    std::vector<GLfloat> vny;
    std::vector<GLfloat> vnz;
    std::vector<GLfloat> vtx;
    std::vector<GLfloat> vty;
    std::vector<GLfloat> vtz;
    /* INIT */
    shapeList=glGenLists(1);
    /* OPEN .obj file */
    objFile=fopen(fname,"r");
    if (!objFile)
    {
        printf("can't open file %s\n", fname);
        PostQuitMessage(1);
    }
    /* RECHERCHE library */
    int i=0;
    while(!(feof(objFile)))
    {
        i++;
        read = fscanf(objFile,"%s",chaine[i%2]);
        if(strcmp(chaine[i%2],"mtllib")==0)
        {
            printf("mtllib : detected \n");
            i++;
            read = fscanf(objFile,"%s",chaine[i%2]);
            printf("mtllib file:%s\n",chaine[i%2]);
            mtl=fopen(chaine[i%2],"r");
            if(!mtl)
                printf("materials file: %s missing\n",chaine[i%2]);
            else
                mtlFiles.push_back(mtl);
        }
    }
    printf("fin detect lib\n");
    fseek(objFile,0,SEEK_SET);
    /* CREATION list */
    glNewList(shapeList, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    while(!(feof(objFile)))
    {
        //printf("DEBUT lecture fichier obj\n");
    /* CREATION 1 objet */
        while(!(feof(objFile)))
        {
            fgets(chaine[0],255,objFile);
            read = sscanf(chaine[0],"%s %f %f %f",chaine[1],&x,&y,&z);
            if(read==4)
            {
                if(0==strcmp(chaine[1],"v"))
                {
                    //printf("trouve v\n");
                    vx.push_back(x);
                    vy.push_back(y);
                    vz.push_back(z);
                }
                else if(0==strcmp(chaine[1],"vn"))
                {
                    //printf("trouve vn\n");
                    vnx.push_back(x);
                    vny.push_back(y);
                    vnz.push_back(z);
                }
                else if(0==strcmp(chaine[1],"vt"))
                {
                    //printf("trouve vt\n"); w parametere enable
                    vtx.push_back(x);
                    vty.push_back(y);
                    vtz.push_back(z);
                }
            }
            if(read==3)
            {
                if(0==strcmp(chaine[1],"vt"))
                vtx.push_back(x);
                vty.push_back(y);
                vtz.push_back(0.0);
            }
            /*FIND MATERIALS */
            if(0==strcmp(chaine[1],"usemtl"))
            {
                //printf("FOUND usemtl\n");
                sscanf(chaine[0],"%s %s",chaine[1],chaine[3]);
                /* FIND chaine[3] mtl */
                bool found=false;
                printf("je rentre\n");
                for(UINT  j=0;j<mtlFiles.size();j++)
                {
                    fseek(mtlFiles.at(j),1,SEEK_SET);
                    printf("fichier reseted\n");
                    /*PARCOURT fichier */
                    while(!feof(mtlFiles.at(j))&&!found)
                    {
                        //printf("lecture fichier\n");
                        fgets(chaine[0],255,mtlFiles.at(j));
                        read = sscanf(chaine[0],"%s %s",chaine[1],chaine[2]);
                        if(read==2 && strcmp(chaine[1],"newmtl")==0)
                        {
                            if(found)
                                break;
                            if(0==strcmp(chaine[2],chaine[3]))
                            {
                            /*FOUND next lines are materials*/
                                found=true;
                                //printf("trouve materials\n");
                                while(strcmp("newmtl",chaine[2])&&!feof(mtlFiles.at(j)))//pas les memes chaines
                                {
                                    GLfloat transparency=0.0f;
                                    fgets(chaine[0],255,mtlFiles.at(j));
                                    read = sscanf(chaine[0],"%s %f %f %f",chaine[2],&x,&y,&z);
                                    if(read==2)
                                    {
                                        if(0==strcmp("Ns",chaine[2]))
                                        {
                                            glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,x/100.0);
                                        }
                                        else if(0==strcmp("Ni",chaine[2]))
                                        {
                                            //refraction no handle for the moment
                                        }
                                        else if(0==strcmp("Tr",chaine[2]))
                                        {
                                            transparency = 1.0-x;
                                        }
                                        else if(0==strcmp("d",chaine[2]))
                                        {
                                            transparency = x;
                                        }
                                        else if(0==strcmp("illum",chaine[2]))
                                        {
                                            //not handle
                                        }
                                    }
                                    if(read==4)
                                    {
                                        if(0==strcmp("Ka",chaine[2]))
                                        {
                                            GLfloat l[4] = {x,y,z,transparency};
                                            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,l);
                                        }
                                        else if(0==strcmp("Kd",chaine[2]))
                                        {
                                            GLfloat l[4] = {x,y,z,transparency};
                                            glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,l);
                                            glColor3f(x,y,z);
                                            //printf("je change de couleur\n");
                                        }
                                        else if(0==strcmp("Ks",chaine[2]))
                                        {
                                            GLfloat l[4] = {x,y,z,transparency};
                                            glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,l);
                                        }
                                        else if(0==strcmp("Ke",chaine[2]))
                                        {
                                            GLfloat l[4] = {x,y,z,transparency};
                                            glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,l);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if(found)
                        break;
                }
                //printf("j'ai reussi a sortir\n");
            }
            else if(0==strcmp(chaine[1],"f"))
            {
                int v1,v2,v3,vn1,vn2,vn3,vt1,vt2,vt3;
                char a,b,c,d,e,f,g;
                read = sscanf(chaine[0],"%c %d%c%d%c%d %d%c%d%c%d %d%c%d%c%d",&a,&v1,&b,&vt1,&c,&vn1,&v2,&d,&vt2,&e,&vn2,&v3,&f,&vt3,&g,&vn3);

                if(read!=16)
                {
                    read = sscanf(chaine[0],"%c %d%c%c%d %d%c%c%d %d%c%c%d",&a,&v1,&b,&c,&vn1,&v2,&d,&e,&vn2,&v3,&f,&g,&vn3);
                    if(read != 13)
                        printf("ERROR : format face not understood\n%s\n%d\n",chaine[0],read);
                }
                if(!(v1<=vx.size()&&v2<=vx.size()&&v3<=vx.size()))
                {
                    printf("depassement de taille v:%d : %d :%d size:%d\n",v1,v2,v3,vx.size());
                }
                else if(!(vn1<=vnx.size()&&vn2<=vnx.size()&&vn3<=vnx.size()))
                {
                    printf("depassement de taille vn:%d : %d :%d size:%d\n",vn1,vn2,vn3,vnx.size());
                }
                else
                {
                    GLfloat x1,x2,x3,y1,y2,y3,z1,z2,z3;
                    x1=vx.at(v1-1);
                    y1=vy.at(v1-1);
                    z1=vz.at(v1-1);
                    x2=vx.at(v2-1);
                    y2=vy.at(v2-1);
                    z2=vz.at(v2-1);
                    x3=vx.at(v3-1);
                    y3=vy.at(v3-1);
                    z3=vz.at(v3-1);
                    glTexCoord2d(0,0);
                    glNormal3f(vnx.at(vn1-1),vny.at(vn1-1),vnz.at(vn1-1));
                    glVertex3f(x1,y1,z1);
                    glTexCoord2d(1,0);
                    glNormal3f(vnx.at(vn2-1),vny.at(vn2-1),vnz.at(vn2-1));
                    glVertex3d(x2,y2,z2);
                    glTexCoord2d(1,1);
                    glNormal3f(vnx.at(vn3-1),vny.at(vn3-1),vnz.at(vn3-1));
                    glVertex3d(x3,y3,z3);
                }
            }
        }
    }
    glEndList();
    glEnd();
    printf("FINI\n");
    return shapeList;
}

