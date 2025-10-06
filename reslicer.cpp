#include "reslicer.h"

QImage Reslicer::resliceToImage(const Volume3D& vol, const SlicePlane& plane)
{
    // Getting the dimensions from the plane and instanciating an empty image
        int H = std::max(1,plane.height());
        int W = std::max(1, plane.width());
        QImage img(W,H,QImage::Format_Grayscale8);

    // Calculating the center of the plane
        const float xCenter = (W-1) * 0.5f;
        const float yCenter = (H-1) * 0.5f;

    // Getting the most important properties of a plane: "a point and two vectors"
        QVector3D P0 = plane.point();
        QVector3D U = plane.uVector().normalized();
        QVector3D V = plane.vVector().normalized();

    // "Point P" will move around the plane
        QVector3D P = P0;

    //
        for (int y = 0; y < H; ++y)
        {
            uchar* row = img.scanLine(y);
            const float dy = (y - yCenter) * plane.grid().sy;

            for (int x = 0; x < W; ++x)
            {
                const float dx = (x - xCenter) * plane.grid().sx;

                // The "Point P" is going through every pixel of the plane
                // Vector U moves the point P in horizontal direction
                // Vector V moves the point P in vertical direction
                // dx and dy indicate how big is the steps between P0 (center) and the P
                P = P0 +  dx * U + dy * V;

                // cast the P into integer because voxels of the volume are addressed by ints
                int xVolIdx = static_cast<int>(std::round(P.x()));
                int yVolIdx = static_cast<int>(std::round(P.y()));
                int zVolIdx = static_cast<int>(std::round(P.z()));

                // By default every voxel is dark
                uchar val = static_cast<uchar>(0.0f);;

                // If the P is within the boundries of the volume, the pixels get values
                if (vol.inBound(xVolIdx, yVolIdx, zVolIdx))
                    val = vol.at(xVolIdx, yVolIdx, zVolIdx);

                // The pixel values are written to the image
                row[x] = static_cast<uchar>(val);
            }

        }

    return img;
}
