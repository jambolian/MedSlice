#ifndef RESLICER_H
#define RESLICER_H

#include <QImage>
#include "sliceplane.h"
#include "volume3d.h"

// Reslicer is a stateless function. It's only a service.
// It takes a volume and a plane inside the volume,
// and makes an image of the volume mapped on the plane

namespace Reslicer
{
    QImage resliceToImage(const Volume3D& vol, const SlicePlane& plane);
};

#endif // RESLICER_H
