#ifndef POSITION_H
#define POSITION_H
#include <gl/gl.h>
#include <gl/glu.h>

bool operator==(Position const& a, Position const& b);
class Position
{
    public:
        Position();
        virtual ~Position();
        GLfloat getX()const;
        GLfloat getY()const;
        GLfloat getZ()const;

    protected:
    private:
        GLfloat x;
        GLfloat y;
        GLfloat z;
};

#endif // POSITION_H
