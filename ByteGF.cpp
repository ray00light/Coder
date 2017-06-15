//
// Created by ramon on 08/06/17.
//

#include "ByteGF.h"


unsigned int ByteGF::prim_poly[9] = {1,1,1,0,0,0,0,1,1};
GaloisField ByteGF::gf8(8,prim_poly);


ByteGF::ByteGF(int value) {
    gf = &gf8;
    poly_value = value;
}

ByteGF::ByteGF(GaloisField* _gf, GFSymbol v)
{
    if (_gf != NULL)
    {
        gf         = _gf;
        poly_value = v & gf->size();
    }
    else
        gf = &gf8;
    poly_value = 0;
}


ByteGF::ByteGF(const ByteGF& gfe)
{
    gf          = gfe.gf;
    poly_value  = gfe.poly_value;
}


std::ostream& operator << (std::ostream& os, const ByteGF& gfe)
{
    os << gfe.poly_value;
    return os;
}


ByteGF operator+(const ByteGF& a, const ByteGF& b)
{
    ByteGF result  = a;
    result += b;
    return result;
}


ByteGF operator-(const ByteGF& a, const ByteGF& b)
{
    ByteGF result  = a;
    result -= b;
    return result;
}


ByteGF operator*(const ByteGF& a, const ByteGF& b)
{
    ByteGF result  = a;
    result *= b;
    return result;
}


ByteGF operator*(const ByteGF& a, const GFSymbol& b)
{
    ByteGF result  = a;
    result *= b;
    return result;
}


ByteGF operator*(const GFSymbol& a, const ByteGF& b)
{
    ByteGF result  = b;
    result *= a;
    return result;
}


ByteGF operator/(const ByteGF& a, const ByteGF& b)
{
    ByteGF result  = a;
    result /= b;
    return result;
}


ByteGF operator^(const ByteGF& a, const int& b)
{
    ByteGF result  = a;
    result ^= b;
    return result;
}