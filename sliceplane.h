#ifndef SLICEPLANE_H
#define SLICEPLANE_H

#include <QVector3D>
#include <QQuaternion>


class SlicePlane
{

private:

    QVector3D p0, u, v, n;
    QQuaternion qRot;
    float rl, pch, yw;


    struct Grid {
        int w = 64;
        int h = 64;
        float sx = 1.0f;
        float sy = 1.0f;
    };

    Grid gd;

public:
        SlicePlane(const QVector3D& p = {0,0,0},
                   int w = 256, int h = 256,
                   float sx = 1.0f, float sy = 1.0f);

    // Setters for the plane Point, Angels, and Grid
        void setPoint(const QVector3D& p0);
        void setRotationAngles(float roll, float pitch, float yaw);
        void setGrid(int w, int h, float sx, float sy);

    // Getters
        QVector3D point() const;
        QVector3D uVector() const;
        QVector3D vVector() const;
        QQuaternion rotationQuaternion() const;
        float roll() const;
        float pitch() const;
        float yaw() const;
        const Grid& grid() const;

        int height() const;
        int width() const;
};

#endif // SLICEPLANE_H
