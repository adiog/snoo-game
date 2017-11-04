#ifndef __R3_h__
#define __R3_h__

#include <iostream>
#include <cmath>

/** Wyjątek rzucany przy próbie uzyskania dostępu do
 czwarej i kolejnych współrzędnych wektora typu R3  */
class OutOfRangeException {};

/** Pomocnicza funkcja - sig - znak liczby */
static double sig(double val) 
{
    if (val>0)
        return 1.0;
    else if (val<0)
        return -1.0;
    else return 0.0;
}

/**
 * Klasa reprezentująca wektor w przestrzeni R^3 lub R^2
 */
class R3 {
    public:

        /** Tworzy wektor [0 0 0] */
        inline R3()
        {
            v[0] = v[1] = v[2] = 0.0;
        }
     
        /** Tworzy wektor [s s s] */
        inline R3(const double s)
        {
            v[0] = v[1] = v[2] = s;
        }
        
        /** Tworzy wektor [x y 0] */
        inline R3(const double x, const double y)
        {
            v[0] = x;
            v[1] = y;
            v[2] = 0.0;
        }
        
        /** Tworzy wektor [x y z] */
        inline R3(const double x, const double y, const double z)
        {
            v[0] = x;
            v[1] = y;
            v[2] = z;
        }

        /** Konstruktor kopiujący */
        inline R3(const R3 & w)
        {
            v[0] = w.v[0];
            v[1] = w.v[1];
            v[2] = w.v[2];
        }
        
        ~R3()
        {
        }

        /** Przypisanie wektorów */
        inline R3 & operator=  (const R3 & w)
        {
            for (int i = 0; i < 3; i++)
                v[i] = w.v[i];
            return *this;
        }

        inline R3 & operator+= (const R3 & w)
        {
            for (int i = 0; i < 3; i++)
                v[i] += w.v[i];
            return *this;
        }
        
        inline R3 & operator-= (const R3 & w)
        {
            for (int i = 0; i < 3; i++)
                v[i] -= w.v[i];
            return *this;
        }
        
        inline R3 & operator*= (const double d)
        {
            for (int i = 0; i < 3; i++)
                v[i] *= d;
            return *this;
        }
        
        inline R3 & operator*= (const R3 & w)
        {
            for (int i = 0; i < 3; i++)
                v[i] *= w[i];
            return *this;
        }
        
        inline R3 & operator/= (const double d)
        {
            for (int i = 0; i < 3; i++)
                v[i] /= d;
            return *this;
        }
        
        inline R3 operator- (void) const
        {
            R3 w;
            w -= *this;
            return w;
        }
        
        inline R3 operator- (const R3 & w) const
        {
            R3 u(*this);
            u -= w;
            return u;
        }
        
        inline R3 operator+ (const R3 & w) const
        {
            R3 u(*this);
            u += w;
            return u;
        }
        
        inline R3 operator* (const double d) const
        {
            R3 u(*this);
            u *= d;
            return u;
        }
     
        /** Mnożenie po współrzędnych */
        inline R3 operator* (const R3 & w) const
        {
            R3 u(*this);
            u *= w;
            return u;
        }

        /** Dzielenie przez stałą */
        inline R3 operator/ (const double d) const
        {
            R3 u(*this);
            u /= d;
            return u;
        }

        /** konwersja na double t[3] */
        inline operator double * (void)
        {
            return v;
        }

        /** dostęp do współrzędnych wektora */
        inline double & operator[] (const int i)
        {
#ifndef SNOO_UNSAFE
            if (i > 2) {
                throw OutOfRangeException();
            }
#endif
            return v[i];
        }
        
        /** dostęp do współrzędnych wektora */
        inline const double & operator[] (const int i) const
        {
#ifndef SNOO_UNSAFE
            if (i > 2) {
                throw OutOfRangeException();
            }
#endif
            return v[i];
        }

        /** porównywanie po współrzędnych */
        inline bool operator< (const R3 & w) const
        {
            return (v[0] < w[0] && v[1] < w[1] && v[2] < w[2]);
        }

        /** porównywanie po współrzędnych */
        inline bool operator> (const R3 & w) const
        {
            return (w[0] < v[0] && w[1] < v[1] && w[2] < v[2]);
        }

        friend std::ostream & operator<<(std::ostream &out, const R3 &w)
        {
            out << w.v[0] << ' ' << w.v[1] << ' ' << w.v[2];
            return out;
        }

        /** iloczyn skalarny */
        inline double dotProd(const R3 & w) const
        {
            return v[0] * w.v[0] + v[1] * w.v[1] + v[2] * w.v[2];
        }

        /** iloczyn wektorowy */
        inline R3 crossProd(const R3 & w) const
        {
            return R3(
                    v[1] * w[2] - v[2] * w[1],
                    v[2] * w[0] - v[0] * w[2],
                    v[0] * w[1] - v[1] * w[0]
            );
        }

        /** rzut wektora na dany wektor w */
        inline R3 projected(const R3 & w) const
        {
            return w * (this->dotProd(w)) / w.lengthSquared();
        }

        /** długość wektora */
        inline double length(void) const
        {
            return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
        }

        /** kwadrat długości wektora */
        inline double lengthSquared(void) const
        {
            return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
        }

        /** Odleglosc punktow */
        inline double distanceTo(const R3 & w) const
        {
            return (w - *this).length();
        }

        /** P.approaches(v, B) <=> jesli punkt P porusza się z prędkościa v,
         to jego ogległość do punktu B maleje */
        inline bool approaches(const R3 & v, const R3 & point) const
        {
            R3 distance = point - (*this);
            return (v.length() > 0 &&
                    v.projected(distance).dotProd(distance) > 0);
        }

        /** wartość bezwzględna po współrzędnych */
        inline R3 absolute(void) const
        {
            return R3(fabs(v[0]), fabs(v[1]), fabs(v[2]));
        }

        /** znak po wspolrzednych */
        inline R3 signum(void)
        {
            return R3(sig(v[0]), sig(v[1]), sig(v[2]));
        }

        /** Zwraca wektor o identycznym kierunku i zwrocie, ale o długości 1 */
        inline R3 unit(void)
        {
            if (length() == 0)
                return ZERO;
            else 
                return (*this) / length();
        }

        /** wektor obrócony wokół osi o kąty w radianach */
        R3 rotate(const R3 &angles)
        {
            int i;
            double sins[3], coss[3], x, y;
            R3 ret = *this;
            for(i=0; i<3; i++)
            {
                sins[i] = sin(angles.v[i]);
                coss[i] = cos(angles.v[i]);
            }
            x = ret[0]; y = ret[1];
            ret[0] = (coss[2] * x) - (sins[2] * y);
            ret[1] = (sins[2] * x) + (coss[2] * y);
            x = ret[1]; y = ret[2];
            ret[1] = (coss[0] * x) - (sins[0] * y);
            ret[2] = (sins[0] * x) + (coss[0] * y);
            x = ret[2]; y = ret[0];
            ret[2] = (coss[1] * x) - (sins[1] * y);
            ret[0] = (sins[1] * x) + (coss[1] * y);
            return ret;
        }

        /** Odwrócenie współrzędnej: v[i] = -v[i] */
        inline R3 & flipAxis(const int i)
        {
            v[i] = -v[i];
            return *this;
        }

        /** Obcina wektor po współrzędnych */
        inline R3 & limit(const R3 & a, const R3 & b)
        {
            for (int i = 0; i < 3; i++) {
                if (v[i] < a[i])
                    v[i] = a[i];
                if (v[i] > b[i])
                    v[i] = b[i];
            }
            return *this;
        }

        /** Kąty, o jakie trzeba obrócić wektor, by dostac [1 0 0] */
        R3 anglesTo100(void) const
        {
            R3 v;
            R3 w = *this;
            const double PI = 3.1415926535897932384626433;

            if  (w[0] < 1e-6 && w[0] > -1e-6) {
                if (w[1] > 0)
                    v[2] = -PI / 2;
                else
                    v[2] = PI / 2;
            } else if (w[0] > 0) 
                v[2] = -atan(w[1] / w[0]);
            else if (w[0] < 0)
                v[2] = -atan(w[1] / w[0]) + PI;

            w = w.rotate(v); // obracamy wokol osi Z
            
            if  (w[0] < 1e-6 && w[0] > -1e-6) {
                if (w[2] > 0)
                    v[1] = PI / 2;
                else
                    v[1] = -PI / 2;
            } else if (w[0] > 0) 
                v[1] = atan(w[2] / w[0]);
            else if (w[0] < 0)
                v[1] = atan(w[2] / w[0]) + PI;

            return v;
        }
        
        /** Max po wspolrzednych */
        inline R3 max(const R3 & v1, const R3 & v2)
        {
            R3 ret = v1;
            for(int i=0; i<3; i++)
                if(v2[i]>ret[i])
                    ret[i]=v2[i];
            return ret;
        }

        /** Min po wspolrzednych */
        inline R3 min(const R3 & v1, const R3 & v2)
        {
            R3 ret = v1;
            for(int i=0; i<3; i++)
                if(v2[i]<ret[i])
                    ret[i]=v2[i];
            return ret;
        }

        /** Stała - wektor [0 0 0] */
        static const R3 ZERO;
    private:
        double v[3];
};


#endif
