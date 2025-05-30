#pragma once

#include <vector>
#include <iostream>
#include <iomanip>

// Column-major vs row-major in openGL
// https://austinmorlan.com/posts/opengl_matrices/

//    2x3          3x3            4x4         OpenGL Array Index      M(Cell/Row)
// | a c e |    | a c e |      | a c 0 e |      |00 04 08 12|     |M00 M01 M02 M03|
// | b d f |    | b d f |  =>  | b d 0 f | ==>  |01 05 09 13| ==> |M10 M11 M12 M13|
// 	            | 0 0 1 |      | 0 0 1 0 |      |02 06 10 14|     |M20 M21 M22 M23|
// 	                           | 0 0 0 1 |      |03 07 11 15|     |M30 M31 M32 M33|
//                                                    |
// The data is stored in Column-major order  --------/.

namespace Core
{
    class Vector3; // Forward decleration to avoid cyclic dependency.
    class AffineTransform;

    class Matrix4 final
    {
    private:
        /* data */
    public:
        Matrix4(/* args */);
        Matrix4(bool setToIdentity)
        {
            if (setToIdentity)
                toIdentity();
        }
        ~Matrix4() = default;

        // Array indices
        // M00 XX: Typically the unrotated X component for scaling, also the cosine of the
        // angle when rotated on the Y and/or Z axis. On
        // Vector3 multiplication this value is multiplied with the source X component
        // and added to the target X component.
        static const int m00 = 0;
        // M01 XY: Typically the negative sine of the angle when rotated on the Z axis.
        // On Vector3 multiplication this value is multiplied
        // with the source Y component and added to the target X component.
        static const int m01 = 4;
        // M02 XZ: Typically the sine of the angle when rotated on the Y axis.
        // On Vector3 multiplication this value is multiplied with the
        // source Z component and added to the target X component.
        static const int m02 = 8;
        // M03 XW: Typically the translation of the X component.
        // On Vector3 multiplication this value is added to the target X component.
        static const int m03 = 12;

        // M10 YX: Typically the sine of the angle when rotated on the Z axis.
        // On Vector3 multiplication this value is multiplied with the
        // source X component and added to the target Y component.
        static const int m10 = 1;
        // M11 YY: Typically the unrotated Y component for scaling, also the cosine
        // of the angle when rotated on the X and/or Z axis. On
        // Vector3 multiplication this value is multiplied with the source Y
        // component and added to the target Y component.
        static const int m11 = 5;
        // M12 YZ: Typically the negative sine of the angle when rotated on the X axis.
        // On Vector3 multiplication this value is multiplied
        // with the source Z component and added to the target Y component.
        static const int m12 = 9;
        // M13 YW: Typically the translation of the Y component.
        // On Vector3 multiplication this value is added to the target Y component.
        static const int m13 = 13;

        // M20 ZX: Typically the negative sine of the angle when rotated on the Y axis.
        // On Vector3 multiplication this value is multiplied
        // with the source X component and added to the target Z component.
        static const int m20 = 2;
        // M21 ZY: Typical the sine of the angle when rotated on the X axis.
        // On Vector3 multiplication this value is multiplied with the
        // source Y component and added to the target Z component.
        static const int m21 = 6;
        // M22 ZZ: Typically the unrotated Z component for scaling, also the cosine of the
        // angle when rotated on the X and/or Y axis.
        // On Vector3 multiplication this value is multiplied with the source Z component
        // and added to the target Z component.
        static const int m22 = 10;
        // M23 ZW: Typically the translation of the Z component.
        // On Vector3 multiplication this value is added to the target Z component.
        static const int m23 = 14;

        // M30 WX: Typically the value zero. On Vector3 multiplication this value is ignored.
        static const int m30 = 3;
        // M31 WY: Typically the value zero. On Vector3 multiplication this value is ignored.
        static const int m31 = 7;
        // M32 WZ: Typically the value zero. On Vector3 multiplication this value is ignored.
        static const int m32 = 11;
        // M33 WW: Typically the value one. On Vector3 multiplication this value is ignored.
        static const int m33 = 15;

        float e[16] = {};

        // Factory
        static Matrix4 createAsIdentity();

        void set(const Matrix4 &m);
        void set(const AffineTransform &aft);

        const float *data() const { return (const float *)&e[0]; }

        void translate(const Vector3 &v);
        void translate(double x, double y, double z);
        void setTranslate(const Vector3 &v);
        void setTranslate(double x, double y, double z);
        static void getTranslation(const Matrix4 &m, Vector3 &out);

        void setRotation(double radianAngle);
        void rotate(double radianAngle);

        void setScale(const Vector3 &vector);
        void setScale(double sx, double sy);
        void setScale(double sx, double sy, double sz);
        void scaleBy(double sx, double sy, double sz);
        double getPsuedoScale();

        void transformVertices(const std::vector<double> &inV, std::vector<double> &out);
        void transformVector(const Vector3 &vector, Vector3 &out);

        void setToOrtho(double left, double right,
                        double bottom, double top,
                        double near, double far);

        void toIdentity();

        // This is equivalent to toString()
        friend std::ostream &operator<<(std::ostream &os, const Matrix4 &m)
        {
            os << std::endl
               << std::fixed << std::setprecision(5)
               << "|" << std::setw(10) << m.e[m00] << "," << std::setw(10) << m.e[m01] << "," << std::setw(10) << m.e[m02] << "," << std::setw(10) << m.e[m03] << "|\n"
               << "|" << std::setw(10) << m.e[m10] << "," << std::setw(10) << m.e[m11] << "," << std::setw(10) << m.e[m12] << "," << std::setw(10) << m.e[m13] << "|\n"
               << "|" << std::setw(10) << m.e[m20] << "," << std::setw(10) << m.e[m21] << "," << std::setw(10) << m.e[m22] << "," << std::setw(10) << m.e[m23] << "|\n"
               << "|" << std::setw(10) << m.e[m30] << "," << std::setw(10) << m.e[m31] << "," << std::setw(10) << m.e[m32] << "," << std::setw(10) << m.e[m33] << "|\n";
            return os;
        }
    };

    /// @brief
    /// multiply multiplies a * b and places result into 'out', (i.e. out = a * b)
    ///
    /// I use a pointer for "a" because I will use "this" which is a pointer.
    /// @param a input
    /// @param b input
    /// @param out output
    void multiply(const Matrix4 *a, const Matrix4 &b, Matrix4 &out);

    // Temporary matricies for multiplication
    static Matrix4 tM0_ = Matrix4{};
    static Matrix4 mulM_ = Matrix4{};

}