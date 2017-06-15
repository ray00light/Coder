//
// Created by ramon on 08/06/17.
//

#ifndef CODER_BYTEGF_H
#define CODER_BYTEGF_H

#include <iostream>
#include <vector>
#include "GaloisField.h"




class ByteGF{

public:
    ByteGF(GaloisField* _gf = NULL, GFSymbol v = -1);
    ByteGF(const ByteGF& gfe);

    ByteGF(int value);

    ~ByteGF(){}


    inline ByteGF& operator=(const ByteGF& gfe)
    {
        if (this == &gfe)
            return *this;


        gf          = gfe.gf;
        poly_value  = gfe.poly_value;

        return *this;
    }


    inline ByteGF& operator=(const GFSymbol& v)
    {
        poly_value  = v & gf->size();
        return *this;
    }


    inline ByteGF& operator+=(const ByteGF& gfe)
    {
        poly_value ^= gfe.poly_value;
        return *this;
    }


    inline ByteGF& operator+=(const GFSymbol& v)
    {
        poly_value ^= v;
        return *this;
    }

    inline ByteGF& operator-=(const ByteGF& gfe)
    {
        *this += gfe;
        return *this;
    }


    inline ByteGF& operator-=(const GFSymbol& v)
    {
        *this += v;
        return *this;
    }


    inline ByteGF& operator*=(const ByteGF& gfe)
    {
        poly_value = gf->mul(poly_value,gfe.poly_value);
        return *this;
    }


    inline ByteGF& operator*=(const GFSymbol& v)
    {
        poly_value = gf->mul(poly_value,v);
        return *this;
    }


    inline ByteGF& operator/=(const ByteGF& gfe)
    {
        poly_value = gf->div(poly_value,gfe.poly_value);
        return *this;
    }


    inline ByteGF& operator/=(const GFSymbol& v)
    {
        poly_value = gf->div(poly_value,v);
        return *this;
    }


    inline ByteGF& operator^=(const int& n)
    {
        poly_value = gf->exp(poly_value,n);
        return *this;
    }


    inline bool operator==(const ByteGF& gfe) const
    {
        return (
                (gf  == gfe.gf) &&
                (poly_value == gfe.poly_value)
        );
    }


    inline bool operator==(const GFSymbol& v) const
    {
        return (poly_value == v);
    }


    inline bool operator!=(const ByteGF& gfe) const
    {
        return (
                (gf  != gfe.gf) ||
                (poly_value != gfe.poly_value)
        );
    }


    inline bool operator!=(const GFSymbol& v) const
    {
        return (poly_value != v);
    }


    inline bool operator<(const ByteGF& gfe)
    {
        return (poly_value < gfe.poly_value);
    }


    inline bool operator<(const GFSymbol& v)
    {
        return (poly_value < v);
    }

    inline bool operator>(const ByteGF& gfe)
    {
        return (poly_value > gfe.poly_value);
    }


    inline bool operator>(const GFSymbol& v)
    {
        return (poly_value > v);
    }

    inline GFSymbol index() const
    {
        return gf->index(poly_value);
    }


    inline GFSymbol poly() const
    {
        return poly_value;
    }


    inline GaloisField* field() const
    {
        return gf;
    }


    inline GFSymbol inverse() const
    {
        return gf->inverse(poly_value);
    }


    friend std::ostream& operator << (std::ostream& os, const ByteGF& gfe);

private:
    GaloisField* gf;
    GFSymbol     poly_value;
    //to use by default
    static unsigned int prim_poly[9];
    static GaloisField gf8;

};

ByteGF operator + (const ByteGF& a, const ByteGF& b);
ByteGF operator - (const ByteGF& a, const ByteGF& b);
ByteGF operator * (const ByteGF& a, const ByteGF& b);
ByteGF operator * (const ByteGF& a, const GFSymbol& b);
ByteGF operator * (const GFSymbol& a, const ByteGF& b);
ByteGF operator / (const ByteGF& a, const ByteGF& b);
ByteGF operator ^ (const ByteGF& a, const int& b);




#endif //CODER_BYTEGF_H
