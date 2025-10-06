#ifndef VOLUME3D_H
#define VOLUME3D_H

#include <QVector3D>
#include <QVector>
#include <QObject>
#include <random>


class Volume3D
{

public:
    Volume3D(int nx = 64, int ny = 64, int nz = 64);

    // Getter for dimensions of the Volume (in pixels)
        int xVoxelCount() const {return nx;};
        int yVoxelCount() const {return ny;};
        int zVoxelCount() const {return nz;};

    // Setter for dimensions
        void resize(int nx, int ny, int nz);

    // Read and write the value of a voxel
        uchar& set(int x, int y, int z);
        uchar at(int x, int y, int z) const;

    // Checks if the voxel is within the volume
        bool inBound(int x, int y, int z) const;


    void fill(uchar f);
    void makeSpherePhantom(int mode, float innerFrac, float outerFrac);

private:
    int nx = 0;
    int ny = 0;
    int nz = 0;

    // Vector containing the volume values
        QVector<uchar> m_vol;
    void addRandomBlackDot(float xc, float yc, float zc,
                           float r1, float r2, int radiusVox = 0);
};

#endif // VOLUME3D_H
