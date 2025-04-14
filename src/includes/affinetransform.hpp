#pragma once

#include <iostream>
#include <iomanip>
#include <memory>

#include <vector2d.hpp>

namespace Core
{
    class Matrix4;

    class AffineTransform; // Forward decl just for 'using' statement.
    using affineShPtr = std::shared_ptr<AffineTransform>;

    class AffineTransform
    {
    private:
        /* data */
    public:
        AffineTransform(/* args */) = default;
        AffineTransform(bool setToIdentity)
        {
            if (setToIdentity)
                toIdentity();
        }
        ~AffineTransform() = default;

        static const int ma = 0; // a
        static const int mb = 1; // b
        static const int m2 = 2; // 0
        static const int m3 = 3; // 0
        static const int mc = 4; // c
        static const int md = 5; // d
        static const int m6 = 6; // 0
        static const int m7 = 7; // 0

        static const int m8 = 8;   // 0
        static const int m9 = 9;   // 0
        static const int m10 = 10; // 1
        static const int m11 = 11; // 0
        static const int me = 12;  // e
        static const int mf = 13;  // f
        static const int m14 = 14; // 0
        static const int m15 = 15; // 1

        // [a, b, 0, 0, c, d, 0, 0, 0, 0, 1, 0, 0, e, f, 0, 1]
        double m[16] = {};

        // These getters are marked as 'const' to indicate they
        // will not mutate this class. This allows them to be used as getter
        // input to another method.
        double getA() const;
        double getB() const;
        double getC() const;
        double getD() const;
        double getTx() const;
        double getTy() const;

        void setA(double v);
        void setB(double v);
        void setC(double v);
        void setD(double v);
        void setTx(double v);
        void setTy(double v);

        void set(double a, double b, double c, double d, double tx, double ty);
        void set(const AffineTransform &atf);
        void set(const affineShPtr &atf);
        void set(const Matrix4 &m);

        void transform(Vector2D &point) const;
        void transform(const Vector2D &point, Vector2D &out) const;
        void transform(double x, double y, Vector2D &out) const;

        void makeTranslate(double x, double y);
        void makeTranslate(const Vector2D &point);

        void scale(double sx, double sy);
        void makeScale(double sx, double sy);
        double getPsuedoScale() const;

        void rotate(double radianAngle);
        void makeRotate(double radianAngle);

        static void multiplyPre(const AffineTransform &m, AffineTransform &n);
        static void multiplyPost(const AffineTransform &m, AffineTransform &n);
        static void multiplyPost(const AffineTransform &m, AffineTransform &n, AffineTransform &out);

        void invert();
        void invert(AffineTransform &out) const;

        void transpose();

        void toIdentity();

        // This is equivalent to toString()
        friend std::ostream &operator<<(std::ostream &os, const AffineTransform &aft)
        {
            using std::endl;
            using std::setw;

            os << endl
               << std::fixed << std::setprecision(5)
               << "|" << setw(10) << aft.getA() << "," << setw(10) << aft.getC() << "," << setw(10) << aft.getTx() << "|\n"
               << "|" << setw(10) << aft.getB() << "," << setw(10) << aft.getD() << "," << setw(10) << aft.getTy() << "|\n";
            return os;
        }

        /// @brief
        /// ```
        /// | a c 0 e |      | ma mc m8  me  |
        /// | b d 0 f | <==> | mb md m9  mf  |
        /// | 0 0 1 0 |      | m2 m6 m10 m14 |
        /// | 0 0 0 1 |      | m3 m7 m11 m15 |
        ///```
        std::string toString4x4();
    };

}
