#include "sliceplane.h"

SlicePlane::SlicePlane(const QVector3D& p,
                       int w, int h,
                       float dx, float dy)
{
    setPoint(p);
    setGrid(w,h,dx,dy);

    qDebug() << "a SlicePlane object is constructed";
}

void SlicePlane::setRotationAngles(float roll, float pitch, float yaw)
{
    this->rl = roll;
    this->pch = pitch;
    this->yw = yaw;
    const QQuaternion qRoll  = QQuaternion::fromAxisAndAngle({1,0,0}, roll);
    const QQuaternion qPitch = QQuaternion::fromAxisAndAngle({0,1,0}, pitch);
    const QQuaternion qYaw   = QQuaternion::fromAxisAndAngle({0,0,1}, yaw);

    this->qRot = (qYaw * qPitch * qRoll).normalized();

    this->u = qRot.rotatedVector(QVector3D(1,0,0)).normalized();
    this->v = qRot.rotatedVector(QVector3D(0,1,0)).normalized();
    this->n = QVector3D::crossProduct(u,v);
}

float SlicePlane::roll() const
{
    return this->rl;
}

float SlicePlane::pitch() const
{
    return this->pch;
}

float SlicePlane::yaw() const
{
    return this->yw;
}

void SlicePlane::setGrid(int w, int h, float sx, float sy)
{
    this->gd.w = w;
    this->gd.h = h;
    this->gd.sx = sx;
    this->gd.sy = sy;
}

const SlicePlane::Grid &SlicePlane::grid() const
{
    return this->gd;
}

int SlicePlane::height() const
{
    return this->gd.h;
}

int SlicePlane::width() const
{
    return this->gd.w;
}

void SlicePlane::setPoint(const QVector3D &p0)
{
    this->p0 = p0;
}

QVector3D SlicePlane::point() const
{
    return this->p0;
}

QVector3D SlicePlane::uVector() const
{
    return this->u;
}

QVector3D SlicePlane::vVector() const
{
    return this->v;
}

QQuaternion SlicePlane::rotationQuaternion() const
{
    return this->qRot;
}
