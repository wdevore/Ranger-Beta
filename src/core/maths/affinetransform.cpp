#include <cmath>

#include <affinetransform.hpp>
#include <matrix4.hpp>

namespace Core
{

    float AffineTransform::getA() const { return m[ma]; }
    float AffineTransform::getB() const { return m[mb]; }
    float AffineTransform::getC() const { return m[mc]; }
    float AffineTransform::getD() const { return m[md]; }
    float AffineTransform::getTx() const { return m[me]; }
    float AffineTransform::getTy() const { return m[mf]; }

    void AffineTransform::setA(float v) { m[ma] = v; }
    void AffineTransform::setB(float v) { m[mb] = v; }
    void AffineTransform::setC(float v) { m[mc] = v; }
    void AffineTransform::setD(float v) { m[md] = v; }
    void AffineTransform::setTx(float v) { m[me] = v; }
    void AffineTransform::setTy(float v) { m[mf] = v; }

    void AffineTransform::set(float a, float b, float c, float d, float tx, float ty)
    {
        m[ma] = a;
        m[mb] = b;
        m[mc] = c;
        m[md] = d;
        m[me] = tx;
        m[mf] = ty;
    }

    void AffineTransform::set(const AffineTransform &atf)
    {
        m[ma] = atf.getA();
        m[mb] = atf.getB();
        m[mc] = atf.getC();
        m[md] = atf.getD();
        m[me] = atf.getTx();
        m[mf] = atf.getTy();
    }

    void AffineTransform::set(const affineShPtr &atf)
    {
        m[ma] = atf->getA();
        m[mb] = atf->getB();
        m[mc] = atf->getC();
        m[md] = atf->getD();
        m[me] = atf->getTx();
        m[mf] = atf->getTy();
    }

    void AffineTransform::set(const Matrix4 &m4)
    {
        m[0] = m4.e[0];
        m[1] = m4.e[1];
        m[2] = m4.e[2];
        m[3] = m4.e[3];
        m[4] = m4.e[4];
        m[5] = m4.e[5];
        m[6] = m4.e[6];
        m[7] = m4.e[7];
        m[8] = m4.e[8];
        m[9] = m4.e[9];
        m[10] = m4.e[10];
        m[11] = m4.e[11];
        m[12] = m4.e[12];
        m[13] = m4.e[13];
        m[14] = m4.e[14];
        m[15] = m4.e[15];
    }

    /// @brief Transforms 'point'
    /// @param point Vector2D
    void AffineTransform::transform(Vector2D &point) const
    {
        point.set((m[ma] * point.x) + (m[mc] * point.y) + m[me],
                  (m[mb] * point.x) + (m[md] * point.y) + m[mf]);
    }

    /// @brief Transform 'point' into 'out'
    /// @param point Vector2D
    /// @param out Vector2D
    void AffineTransform::transform(const Vector2D &point, Vector2D &out) const
    {
        out.set((m[ma] * point.x) + (m[mc] * point.y) + m[me],
                (m[mb] * point.x) + (m[md] * point.y) + m[mf]);
    }

    /// @brief Transforms x,y into 'out'
    /// @param x
    /// @param y
    /// @param out
    void AffineTransform::transform(float x, float y, Vector2D &out) const
    {
        out.x = (m[ma] * x) + (m[mc] * y) + m[me];
        out.y = (m[mb] * x) + (m[md] * y) + m[mf];
    }

    void AffineTransform::translate(float x, float y)
    {
        m[me] += (m[ma] * x) + (m[mc] * y);
        m[mf] += (m[mb] * x) + (m[md] * y);
    }

    void AffineTransform::makeTranslate(float x, float y)
    {
        m[ma] = 1.0;
        m[mb] = 0.0;
        m[mc] = 0.0;
        m[md] = 1.0;
        m[me] = x;
        m[mf] = y;
    }

    void AffineTransform::makeTranslate(const Vector2D &point)
    {
        m[ma] = 1.0;
        m[mb] = 0.0;
        m[mc] = 0.0;
        m[md] = 1.0;
        m[me] = point.x;
        m[mf] = point.y;
    }

    /// @brief Scales 'this' transform
    /// @param sx
    /// @param sy
    void AffineTransform::scale(float sx, float sy)
    {
        m[ma] *= sx;
        m[mb] *= sx;
        m[mc] *= sy;
        m[md] *= sy;
    }

    void AffineTransform::makeScale(float sx, float sy)
    {
        m[ma] = sx;
        m[mb] = 0.0;
        m[mc] = 0.0;
        m[md] = sy;
        m[me] = 0.0;
        m[mf] = 0.0;
    }

    float AffineTransform::getPsuedoScale() const
    {
        return m[ma];
    }

    /// @brief Concatinate a rotation [radians] onto this transform.
    ///
    /// Rotation is just a matter of perspective. A CW rotation can be seen as
    /// CCW depending on what you are talking about rotating. For example,
    /// if the coordinate system is thought as rotating CCW then objects are
    /// seen as rotating CW, and that is what the 2x2 matrix below represents.
    ///
    /// It is also the frame of reference we use. In this library +Y axis is downward
    ///     ```
    ///     [cos  -sin]   object appears to rotate CW.
    ///     [sin   cos]
    ///     ```
    /// In the matrix below the object appears to rotate CCW.
    ///     ```
    ///     [cos  sin]
    ///     [-sin cos]
    ///
    ///     [a  c]    [cos  -sin]
    ///     [b  d]  x [sin   cos]
    ///     ```
    /// If Y axis is downward (default for SDL and Image) then:
    ///
    ///     - +angle yields a CW rotation
    ///
    ///     - -angle yeilds a CCW rotation
    ///
    /// else
    ///
    ///     - -angle yields a CW rotation
    ///
    ///     - +angle yeilds a CCW rotation.
    /// @param radianAngle
    void AffineTransform::rotate(float radianAngle)
    {
        float s = sin(radianAngle);
        float cr = cos(radianAngle);

        // Capture value BEFORE they are modified
        float a = m[ma];
        float b = m[mb];
        float c = m[mc];
        float d = m[md];

        m[ma] = a * cr + c * s;
        m[mb] = b * cr + d * s;
        m[mc] = c * cr - a * s;
        m[md] = d * cr - b * s;
    }

    void AffineTransform::makeRotate(float radianAngle)
    {
        float s = sin(radianAngle);
        float c = cos(radianAngle);
        m[ma] = c;
        m[mb] = s;
        m[mc] = -s;
        m[md] = c;
        m[me] = 0.0;
        m[mf] = 0.0;
    }

    /// @brief multiplyPre performs: n = n * m.
    /// @param m input
    /// @param n output
    void AffineTransform::multiplyPre(const AffineTransform &m, AffineTransform &n)
    {
        // Copy values BEFORE modifying them.
        float na = n.getA();
        float nb = n.getB();
        float nc = n.getC();
        float nd = n.getD();

        float ja = m.getA();
        float jb = m.getB();
        float jc = m.getC();
        float jd = m.getD();
        float mtx = m.getTx();
        float mty = m.getTy();

        // Now perform multiplication
        n.setA(na * ja + nb * jc);
        n.setB(na * jb + nb * jd);
        n.setC(nc * ja + nd * jc);
        n.setD(nc * jb + nd * jd);
        n.setTx(na * mtx + nc * m.getTy() + mtx);
        n.setTy(nb * mtx + nd * m.getTy() + mty);
    }

    /// @brief multiplyPost performs: n = m * n.
    /// @param m input
    /// @param n output
    void AffineTransform::multiplyPost(const AffineTransform &m, AffineTransform &n)
    {
        // Copy values BEFORE modifying them.
        float na = n.getA();
        float nb = n.getB();
        float nc = n.getC();
        float nd = n.getD();
        float ntx = n.getTx();
        float nty = n.getTy();

        float ja = m.getA();
        float jb = m.getB();
        float jc = m.getC();
        float jd = m.getD();

        // Now perform multiplication
        n.setA(ja * na + jb * nc);
        n.setB(ja * nb + jb * nd);
        n.setC(jc * na + jd * nc);
        n.setD(jc * nb + jd * nd);
        n.setTx(ja * ntx + jc * n.getTy() + ntx);
        n.setTy(jb * ntx + jd * n.getTy() + nty);
    }

    /// @brief multiplyPost performs: out = m * n
    /// @param m
    /// @param n
    /// @param out
    void AffineTransform::multiplyPost(const AffineTransform &m, AffineTransform &n, AffineTransform &out)
    {
        // Copy values BEFORE modifying them.
        float na = n.getA();
        float nb = n.getB();
        float nc = n.getC();
        float nd = n.getD();
        float ntx = n.getTx();
        float nty = n.getTy();

        float ja = m.getA();
        float jb = m.getB();
        float jc = m.getC();
        float jd = m.getD();
        float jtx = m.getTx();
        float jty = m.getTy();

        // Now perform multiplication
        out.setA(ja * na + jb * nc);
        out.setB(ja * nb + jb * nd);
        out.setC(jc * na + jd * nc);
        out.setD(jc * nb + jd * nd);
        out.setTx((jtx * na) + (jty * nc) + ntx);
        out.setTy((jtx * nb) + (jty * nd) + nty);
    }

    /// @brief This can silently fail if the determinant denominator = 0
    void AffineTransform::invert()
    {
        // Copy values BEFORE modifying them.
        float a = m[ma];
        float b = m[mb];
        float c = m[mc];
        float d = m[md];
        float tx = m[me];
        float ty = m[mf];

        float denom = a * d - b * c;
        if (denom == 0)
        {
            std::cout << "Invert failed. Determinant denominator = 0" << std::endl;
            return;
        }
        float determinant = 1.0 / denom;

        m[ma] = determinant * d;
        m[mb] = -determinant * b;
        m[mc] = -determinant * c;
        m[md] = determinant * a;
        m[me] = determinant * (c * ty - d * tx);
        m[mf] = determinant * (b * tx - a * ty);
    }

    /// @brief This can silently fail if the determinant denominator = 0
    void AffineTransform::invert(AffineTransform &out) const
    {
        float a = m[ma];
        float b = m[mb];
        float c = m[mc];
        float d = m[md];
        float tx = m[me];
        float ty = m[mf];

        float denom = a * d - b * c;
        if (denom == 0)
        {
            std::cout << "Invert failed. Determinant denominator = 0" << std::endl;
            return;
        }
        float determinant = 1.0 / denom;

        out.setA(determinant * d);
        out.setB(-determinant * b);
        out.setC(-determinant * c);
        out.setD(determinant * a);
        out.setTx(determinant * (c * ty - d * tx));
        out.setTy(determinant * (b * tx - a * ty));
    }

    /// @brief [transpose] c and d elements.
    ///
    /// Converts either from or to pre or post multiplication.
    /// ```
    ///         a c
    ///         b d
    ///```
    ///  to
    ///```
    ///         a b
    ///         c d
    /// ```
    void AffineTransform::transpose()
    {
        float c = getC();
        setC(getB());
        setB(c);
    }

    /// @brief
    /// ```
    /// | 1 0 0 0 |     | a c 0 e |  | ma mc m8  me  |
    /// | 0 1 0 0 | ==> | b d 0 f |  | mb md m9  mf  |
    /// | 0 0 1 0 |     | 0 0 1 0 |  | m2 m6 m10 m14 |
    /// | 0 0 0 1 |     | 0 0 0 1 |  | m3 m7 m11 m15 |
    ///```
    void AffineTransform::toIdentity()
    {
        m[ma] = 1.0;
        m[mb] = 0.0;
        m[m2] = 0.0;
        m[m3] = 0.0;

        m[mc] = 0.0;
        m[md] = 1.0;
        m[m6] = 0.0;
        m[m7] = 0.0;

        m[m8] = 0.0;
        m[m9] = 0.0;
        m[m10] = 1.0;
        m[m11] = 0.0;

        m[me] = 0.0;
        m[mf] = 0.0;
        m[m14] = 0.0;
        m[m15] = 1.0;
    }

    /// @brief
    /// ```
    /// | a c 0 e |      | ma mc m8  me  |
    /// | b d 0 f | <==> | mb md m9  mf  |
    /// | 0 0 1 0 |      | m2 m6 m10 m14 |
    /// | 0 0 0 1 |      | m3 m7 m11 m15 |
    ///```
    std::string AffineTransform::toString4x4()
    {
        using std::endl;
        using std::setw;
        using at = AffineTransform;
        std::ostringstream oss;

        oss << std::endl
            << std::fixed << std::setprecision(5)
            << "|" << setw(10) << getA() << "," << setw(10) << getC() << "," << setw(10) << m[at::m8] << setw(10) << getTx() << "|\n"
            << "|" << setw(10) << getB() << "," << setw(10) << getD() << "," << setw(10) << m[at::m9] << setw(10) << getTy() << "|\n"
            << "|" << setw(10) << m[at::m2] << "," << setw(10) << m[at::m6] << "," << setw(10) << m[at::m10] << setw(10) << m[at::m14] << "|\n"
            << "|" << setw(10) << m[at::m3] << "," << setw(10) << m[at::m7] << "," << setw(10) << m[at::m11] << setw(10) << m[at::m15] << "|\n";

        return oss.str();
    }

}
