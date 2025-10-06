#include "volume3d.h"


Volume3D::Volume3D(int nx, int ny, int nz)
    : nx(nx), ny(ny), nz(nz), m_vol((nx*ny*nz),0)
{
}

void Volume3D::resize(int nx, int ny, int nz)
{
    this->nx = nx;
    this->ny = ny;
    this->nz = nz;
    m_vol.resize(nx*ny*nz);
}

uchar &Volume3D::set(int x, int y, int z)
{
    return m_vol[(z * this->ny * this->nx) + (y * this->nx) + x];
}

uchar Volume3D::at(int x, int y, int z) const
{
    return m_vol[(z * ny * nx) + (y * nx) + x];
}

void Volume3D::fill(uchar f)
{
    std::fill(m_vol.begin(), m_vol.end(), f);
}

bool Volume3D::inBound(int x, int y, int z) const
{
    return ( (x >= 0)&&(x < this->nx) &&
             (y >= 0)&&(y < this->ny) &&
             (z >= 0)&&(z < this->nz)    );
}



void Volume3D::makeSpherePhantom(int mode, float innerFrac, float outerFrac)
{
    fill(0);

    // Center of the Cube and the Fractions
    const float xc = (this->nx-1)*0.5f;
    const float yc = (this->ny-1)*0.5f;
    const float zc = (this->nz-1)*0.5f;
    const float minDim = static_cast<float>(std::min( {this->nx,this->ny,this->nz}));

    const float r1 = innerFrac * 0.5f * minDim;
    const float r2 = outerFrac * 0.5f * minDim;
    const float r1sq = r1*r1;
    const float r2sq = r2*r2;

    for (int z = 0; z < this->nz; ++z) {
        float dz = z - zc;
        //dz *= 1.5f; Oval Shape!

        for (int y = 0; y < this->ny; ++y) {
            float dy = y-yc;
            //dy *= 0.8f; Oval Shape!

            for (int x = 0; x < this->nx; ++x) {
                float dx = x - xc;
                //dx *= 1.5f; // Oval Shape!

                float rsq = dx*dx + dy*dy + dz*dz;

                float voxelVal = 0.0f;

            // the mode determines the gradiant style:
            // along x, y, z, or r direction

                switch (mode)
                {
                case 0: // Radial Gradient

                default:
                    if ( (rsq > r1sq) && (rsq < r2sq) )
                    {
                        const float r = std::sqrt(rsq);
                        float ratio = (r-r1) / (r2-r1);
                        ratio = pow(ratio,1.2); //for a better contrast
                        voxelVal = 255.0f * (1-ratio);
                    }
                    break;

                case 1: // Gradiant along X axis
                    if ( (rsq > r1sq) && (rsq < r2sq) )
                    {
                        float ratio;
                        float xlim1 = (nx-r2-r2)*0.5f;
                        float xlim2 = ((nx-r2-r2)*0.5f)+r2+r2;
                        ratio =  (float)(x-xlim1) / (float)(xlim2-xlim1);
                        ratio = pow(ratio,1.6); //for a better contrast
                        voxelVal = 255.0f * (ratio);
                    }
                    break;

                case 2: // Gradiant along Y axis
                    if ( (rsq > r1sq) && (rsq < r2sq) )
                    {
                        float ratio;
                        float ylim1 = (ny-r2-r2)*0.5f;
                        float ylim2 = ((ny-r2-r2)*0.5f)+r2+r2;
                        ratio =  (float)(y-ylim1) / (float)(ylim2-ylim1);
                        ratio = pow(ratio,1.6); //for a better contrast
                        voxelVal = 255.0f * (ratio);
                    }
                    break;

                case 3: // Gradiant along Z axis
                    if ( (rsq > r1sq) && (rsq < r2sq) )
                    {
                        float ratio;
                        float zlim1 = (nz-r2-r2)*0.5f;
                        float zlim2 = ((nz-r2-r2)*0.5f)+r2+r2;
                        ratio =  (float)(z-zlim1) / (float)(zlim2-zlim1);
                        ratio = pow(ratio,1.6); //for a better contrast
                        voxelVal = 255.0f * (ratio);
                    }
                    break;

                }

                set(x,y,z) = static_cast<uchar>(voxelVal + 0.5f);
            }
        }
    }

    // Add one random black dot (radius = 3).
    addRandomBlackDot(xc, yc, zc, r1, r2, 3);
}

void Volume3D::addRandomBlackDot(float xc, float yc, float zc, float r1, float r2, int radiusVox)
{

    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<int> X(0, nx - 1);
    std::uniform_int_distribution<int> Y(0, ny - 1);
    std::uniform_int_distribution<int> Z(0, nz - 1);

    const float r1sq = r1 * r1;
    const float r2sq = r2 * r2;
    float rsq;

    int px = 0, py = 0, pz = 0;

    do
    {
        px = X(rng);
        py = Y(rng);
        pz = Z(rng);
        float dx = px - xc;
        float dy = py - yc;
        float dz = pz - zc;
        rsq = dx*dx + dy*dy + dz*dz;
    } while (rsq <= r1sq || rsq >= r2sq);

    const int r = std::max(0, radiusVox);
    const int r2vox = r * r;

    const int z0 = std::max(0, pz - r);
    const int z1 = std::min(nz - 1, pz + r);
    const int y0 = std::max(0, py - r);
    const int y1 = std::min(ny - 1, py + r);
    const int x0 = std::max(0, px - r);
    const int x1 = std::min(nx - 1, px + r);

    for (int z = z0; z <= z1; z++)
        for (int y = y0; y <= y1; y++)
            for (int x = x0; x <= x1; x++)
            {
                const int dx = x - px, dy = y - py, dz = z - pz;
                if (dx*dx + dy*dy + dz*dz <= r2vox)
                    set(x, y, z) = 0;
            }
}
