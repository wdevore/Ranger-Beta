#include <cmath>

#include <constants.hpp>
#include <matrix4.hpp>
#include <vector3.hpp>
#include <affinetransform.hpp>

namespace Core
{
    Matrix4::Matrix4(/* args */)
    {
        tM0_.toIdentity();
        mulM_.toIdentity();
    }

    // Matrix4::~Matrix4()
    // {
    // }

    Matrix4 Matrix4::createAsIdentity()
    {
        Matrix4 m4 = Matrix4{};
        m4.toIdentity();
        return m4;
    }

    void Matrix4::set(const Matrix4 &m)
    {
        e[m00] = m.e[m00];
        e[m01] = m.e[m01];
        e[m02] = m.e[m02];
        e[m03] = m.e[m03];

        e[m10] = m.e[m10];
        e[m11] = m.e[m11];
        e[m12] = m.e[m12];
        e[m13] = m.e[m13];

        e[m20] = m.e[m20];
        e[m21] = m.e[m21];
        e[m22] = m.e[m22];
        e[m23] = m.e[m23];

        e[m30] = m.e[m30];
        e[m31] = m.e[m31];
        e[m32] = m.e[m32];
        e[m33] = m.e[m33];
    }

    void Matrix4::set(const AffineTransform &aft)
    {
        e[0] = aft.m[0];
        e[1] = aft.m[1];
        e[2] = aft.m[2];
        e[3] = aft.m[3];
        e[4] = aft.m[4];
        e[5] = aft.m[5];
        e[6] = aft.m[6];
        e[7] = aft.m[7];
        e[8] = aft.m[8];
        e[9] = aft.m[9];
        e[10] = aft.m[10];
        e[11] = aft.m[11];
        e[12] = aft.m[12];
        e[13] = aft.m[13];
        e[14] = aft.m[14];
        e[15] = aft.m[15];
    }

    void Matrix4::translate(const Vector3 &v)
    {
        translate(v.x, v.y, v.z);
    }

    inline void Matrix4::translate(double x, double y, double z)
    {
        tM0_.e[m00] = 1.0f;
        tM0_.e[m01] = 0.0f;
        tM0_.e[m02] = 0.0f;
        tM0_.e[m03] = x;
        tM0_.e[m10] = 0.0f;
        tM0_.e[m11] = 1.0f;
        tM0_.e[m12] = 0.0f;
        tM0_.e[m13] = y;
        tM0_.e[m20] = 0.0f;
        tM0_.e[m21] = 0.0f;
        tM0_.e[m22] = 1.0f;
        tM0_.e[m23] = z;
        tM0_.e[m30] = 0.0f;
        tM0_.e[m31] = 0.0f;
        tM0_.e[m32] = 0.0f;
        tM0_.e[m33] = 1.0f;

        multiply(this, tM0_, mulM_);
        set(mulM_);
    }

    /// @brief Resets the matrix to Identity and then
    ///        sets translational components in the 4th column.
    /// @param v Vector3
    void Matrix4::setTranslate(const Vector3 &v)
    {
        setTranslate(v.x, v.y, v.z);
    }

    /// @brief Resets the matrix to Identity and then
    ///        sets translational components in the 4th column.
    /// @param x double
    /// @param y double
    /// @param z double
    inline void Matrix4::setTranslate(double x, double y, double z)
    {
        toIdentity();
        e[m03] = x;
        e[m13] = y;
        e[m23] = z;
    }

    /// @brief Returns the translational components in 'out' Vector3 field.
    /// @param out Vector3
    void Matrix4::getTranslation(const Matrix4 &m, Vector3 &out)
    {
        out.x = m.e[m03];
        out.y = m.e[m13];
        out.z = m.e[m23];
    }

    /// @brief Set a rotation matrix about Z axis. [angle] is specified
    ///        in radians.
    ///
    ///      ```
    ///      [  m00  m01   _    _   ]
    ///      [  m10  m11   _    _   ]
    ///      [   _    _    _    _   ]
    ///      [   _    _    _    _   ]
    ///      ```
    void Matrix4::setRotation(double radianAngle)
    {
        if (fabs(radianAngle) < Core::EPSILON)
            return;

        toIdentity();

        // Column major
        double c = cos(radianAngle);
        double s = sin(radianAngle);

        e[m00] = c;
        e[m01] = -s;
        e[m10] = s;
        e[m11] = c;
    }

    void Matrix4::rotate(double radianAngle)
    {
        if (radianAngle == 0.0)
            return;

        // Column major
        double c = cos(radianAngle);
        double s = sin(radianAngle);

        tM0_.e[m00] = c;
        tM0_.e[m01] = -s;
        tM0_.e[m02] = 0.0f;
        tM0_.e[m03] = 0.0f;
        tM0_.e[m10] = s;
        tM0_.e[m11] = c;
        tM0_.e[m12] = 0.0f;
        tM0_.e[m13] = 0.0f;
        tM0_.e[m20] = 0.0f;
        tM0_.e[m21] = 0.0f;
        tM0_.e[m22] = 1.0f;
        tM0_.e[m23] = 0.0f;
        tM0_.e[m30] = 0.0f;
        tM0_.e[m31] = 0.0f;
        tM0_.e[m32] = 0.0f;
        tM0_.e[m33] = 1.0f;

        multiply(this, tM0_, mulM_);
        set(mulM_);
    }

    /// @brief Overwrites the diagonal scale components.
    /// @param vector
    void Matrix4::setScale(const Vector3 &vector)
    {
        toIdentity();
        e[m00] = vector.x;
        e[m11] = vector.y;
        e[m22] = vector.z;
    }

    /// @brief Overwrites the scale components of an identity matrix and captures
    ///        scale values into Scale property where Z component = 1.0.
    /// @param sx
    /// @param sy
    void Matrix4::setScale(double sx, double sy)
    {
        toIdentity();

        e[m00] = sx;
        e[m11] = sy;
        e[m22] = 1.0f;
    }

    /// @brief Overwrites the diagonal scale components.
    /// @param sx
    /// @param sy
    /// @param sz
    void Matrix4::setScale(double sx, double sy, double sz)
    {
        toIdentity();

        e[m00] = sx;
        e[m11] = sy;
        e[m22] = sz;
    }

    /// @brief Creates Scale matrix and multiplies into "this" matrix.
    /// @param sx
    /// @param sy
    /// @param sz
    void Matrix4::scaleBy(double sx, double sy, double sz)
    {
        tM0_.e[m00] = sx;
        tM0_.e[m01] = 0.0f;
        tM0_.e[m02] = 0.0f;
        tM0_.e[m03] = 0.0f;
        tM0_.e[m10] = 0.0f;
        tM0_.e[m11] = sy;
        tM0_.e[m12] = 0.0f;
        tM0_.e[m13] = 0.0f;
        tM0_.e[m20] = 0.0f;
        tM0_.e[m21] = 0.0f;
        tM0_.e[m22] = sz;
        tM0_.e[m23] = 0.0f;
        tM0_.e[m30] = 0.0f;
        tM0_.e[m31] = 0.0f;
        tM0_.e[m32] = 0.0f;
        tM0_.e[m33] = 1.0f;

        multiply(this, tM0_, mulM_);
        set(mulM_);
    }

    /// @brief This is a rough approximation under certain conditions. Use it with
    ///        extreme caution!
    /// @return double
    double Matrix4::getPsuedoScale()
    {
        return e[m00];
    }

    void Matrix4::transformVertices(const std::vector<double> &inV, std::vector<double> &out)
    {
        for (size_t i = 0; i < inV.size(); i++)
        {
            double x = inV[i];
            double y = inV[i + 1];
            double z = inV[i + 2];

            // http://www.c-jump.com/bcc/common/Talk3/Math/Matrices/Matrices.html
            out[i] = e[m00] * x + e[m01] * y + e[m02] * z + e[m03];     // x
            out[i + 1] = e[m10] * x + e[m11] * y + e[m12] * z + e[m13]; // y
            // e[m20]*x + e[m21]*y + e[m22]*z + e[m23]
            out[i + 2] = 0.0; // z
        }
    }

    void Matrix4::transformVector(const Vector3 &vIn, Vector3 &vOut)
    {
        //    M(Cell/Row)
        // |M00 M01 M02 M03|
        // |M10 M11 M12 M13|
        // |               |
        // |               |

        vOut.x = e[m00] * vIn.x + e[m01] * vIn.y + e[m02] * vIn.z + e[m03]; // x
        vOut.y = e[m10] * vIn.x + e[m11] * vIn.y + e[m12] * vIn.z + e[m13]; // y
        vOut.z = 0.0f;
    }

    void Matrix4::setToOrtho(double left, double right,
                             double bottom, double top,
                             double near, double far)
    {
        double xorth = 2.0 / (right - left);
        double yorth = 2.0 / (top - bottom);
        double zorth = 2.0 / (near - far);

        double tx = (right + left) / (left - right);
        double ty = (top + bottom) / (bottom - top);
        double tz = (far + near) / (far - near);

        e[m00] = xorth;
        e[m10] = 0.0f;
        e[m20] = 0.0f;
        e[m30] = 0.0f;
        e[m01] = 0.0f;
        e[m11] = yorth;
        e[m21] = 0.0f;
        e[m31] = 0.0f;
        e[m02] = 0.0f;
        e[m12] = 0.0f;
        e[m22] = zorth;
        e[m32] = 0.0f;
        e[m03] = tx;
        e[m13] = ty;
        e[m23] = tz;
        e[m33] = 1.0f;
    }

    /// @brief multiply multiplies a * b and places result into 'out', (i.e. out = a * b)
    /// @param a Matrix4
    /// @param b Matrix4
    /// @param out Matrix4
    void multiply(const Matrix4 *a, const Matrix4 &b, Matrix4 &out)
    {
        out.e[Matrix4::m00] = a->e[Matrix4::m00] * b.e[Matrix4::m00] +
                              a->e[Matrix4::m01] * b.e[Matrix4::m10] +
                              a->e[Matrix4::m02] * b.e[Matrix4::m20] +
                              a->e[Matrix4::m03] * b.e[Matrix4::m30];
        out.e[Matrix4::m01] = a->e[Matrix4::m00] * b.e[Matrix4::m01] +
                              a->e[Matrix4::m01] * b.e[Matrix4::m11] +
                              a->e[Matrix4::m02] * b.e[Matrix4::m21] +
                              a->e[Matrix4::m03] * b.e[Matrix4::m31];
        out.e[Matrix4::m02] = a->e[Matrix4::m00] * b.e[Matrix4::m02] +
                              a->e[Matrix4::m01] * b.e[Matrix4::m12] +
                              a->e[Matrix4::m02] * b.e[Matrix4::m22] +
                              a->e[Matrix4::m03] * b.e[Matrix4::m32];
        out.e[Matrix4::m03] = a->e[Matrix4::m00] * b.e[Matrix4::m03] +
                              a->e[Matrix4::m01] * b.e[Matrix4::m13] +
                              a->e[Matrix4::m02] * b.e[Matrix4::m23] +
                              a->e[Matrix4::m03] * b.e[Matrix4::m33];
        out.e[Matrix4::m10] = a->e[Matrix4::m10] * b.e[Matrix4::m00] +
                              a->e[Matrix4::m11] * b.e[Matrix4::m10] +
                              a->e[Matrix4::m12] * b.e[Matrix4::m20] +
                              a->e[Matrix4::m13] * b.e[Matrix4::m30];
        out.e[Matrix4::m11] = a->e[Matrix4::m10] * b.e[Matrix4::m01] +
                              a->e[Matrix4::m11] * b.e[Matrix4::m11] +
                              a->e[Matrix4::m12] * b.e[Matrix4::m21] +
                              a->e[Matrix4::m13] * b.e[Matrix4::m31];
        out.e[Matrix4::m12] = a->e[Matrix4::m10] * b.e[Matrix4::m02] +
                              a->e[Matrix4::m11] * b.e[Matrix4::m12] +
                              a->e[Matrix4::m12] * b.e[Matrix4::m22] +
                              a->e[Matrix4::m13] * b.e[Matrix4::m32];
        out.e[Matrix4::m13] = a->e[Matrix4::m10] * b.e[Matrix4::m03] +
                              a->e[Matrix4::m11] * b.e[Matrix4::m13] +
                              a->e[Matrix4::m12] * b.e[Matrix4::m23] +
                              a->e[Matrix4::m13] * b.e[Matrix4::m33];
        out.e[Matrix4::m20] = a->e[Matrix4::m20] * b.e[Matrix4::m00] +
                              a->e[Matrix4::m21] * b.e[Matrix4::m10] +
                              a->e[Matrix4::m22] * b.e[Matrix4::m20] +
                              a->e[Matrix4::m23] * b.e[Matrix4::m30];
        out.e[Matrix4::m21] = a->e[Matrix4::m20] * b.e[Matrix4::m01] +
                              a->e[Matrix4::m21] * b.e[Matrix4::m11] +
                              a->e[Matrix4::m22] * b.e[Matrix4::m21] +
                              a->e[Matrix4::m23] * b.e[Matrix4::m31];
        out.e[Matrix4::m22] = a->e[Matrix4::m20] * b.e[Matrix4::m02] +
                              a->e[Matrix4::m21] * b.e[Matrix4::m12] +
                              a->e[Matrix4::m22] * b.e[Matrix4::m22] +
                              a->e[Matrix4::m23] * b.e[Matrix4::m32];
        out.e[Matrix4::m23] = a->e[Matrix4::m20] * b.e[Matrix4::m03] +
                              a->e[Matrix4::m21] * b.e[Matrix4::m13] +
                              a->e[Matrix4::m22] * b.e[Matrix4::m23] +
                              a->e[Matrix4::m23] * b.e[Matrix4::m33];
        out.e[Matrix4::m30] = a->e[Matrix4::m30] * b.e[Matrix4::m00] +
                              a->e[Matrix4::m31] * b.e[Matrix4::m10] +
                              a->e[Matrix4::m32] * b.e[Matrix4::m20] +
                              a->e[Matrix4::m33] * b.e[Matrix4::m30];
        out.e[Matrix4::m31] = a->e[Matrix4::m30] * b.e[Matrix4::m01] +
                              a->e[Matrix4::m31] * b.e[Matrix4::m11] +
                              a->e[Matrix4::m32] * b.e[Matrix4::m21] +
                              a->e[Matrix4::m33] * b.e[Matrix4::m31];
        out.e[Matrix4::m32] = a->e[Matrix4::m30] * b.e[Matrix4::m02] +
                              a->e[Matrix4::m31] * b.e[Matrix4::m12] +
                              a->e[Matrix4::m32] * b.e[Matrix4::m22] +
                              a->e[Matrix4::m33] * b.e[Matrix4::m32];
        out.e[Matrix4::m33] = a->e[Matrix4::m30] * b.e[Matrix4::m03] +
                              a->e[Matrix4::m31] * b.e[Matrix4::m13] +
                              a->e[Matrix4::m32] * b.e[Matrix4::m23] +
                              a->e[Matrix4::m33] * b.e[Matrix4::m33];
    }

    void Matrix4::toIdentity()
    {
        e[m00] = 1.0f;
        e[m01] = 0.0f;
        e[m02] = 0.0f;
        e[m03] = 0.0f;

        e[m10] = 0.0f;
        e[m11] = 1.0f;
        e[m12] = 0.0f;
        e[m13] = 0.0f;

        e[m20] = 0.0f;
        e[m21] = 0.0f;
        e[m22] = 1.0f;
        e[m23] = 0.0f;

        e[m30] = 0.0f;
        e[m31] = 0.0f;
        e[m32] = 0.0f;
        e[m33] = 1.0f;
    }
}
