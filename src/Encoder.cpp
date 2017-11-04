#include "Encoder.h"

#include <cstdlib>
#include <string>

std::string Encoder::encode(const void * data, size_t data_size)
{
    int i;
    unsigned char * dst = new unsigned char[data_size * 2 + 1];
    const unsigned char * src = (const unsigned char *) data;

    for (i = 0; i < (signed int) data_size; i++) {
        dst[2 * i] = ((src[i] & 0xF0) >> 4) | 64;
        dst[2 * i + 1] = (src[i] & 0x0F) | 64;
    }
    dst[2 * i] = 0; //null na koncu stringa

    std::string s = std::string((const char *)dst);
    delete[] dst;
    return s;
}

void Encoder::decode(const std::string & s, void * data)
{
    unsigned char * dst = (unsigned char *) data;
    const unsigned char * src = (const unsigned char *) s.c_str();
    int i;
    int n = s.size() / 2;

    for (i = 0; i < n; i++)
        dst[i] = ((src[2 * i] & 0x0F) << 4) | (src[2 * i + 1] & 0x0F);
}

DataArray Encoder::decode(const std::string & s)
{
    int n = s.size() / 2;
    DataArray dst = DataArray(n, 0);
    const unsigned char * src = (const unsigned char *) s.c_str();
    int i;

    for (i = 0; i < n; i++)
        dst[i] = ((src[2 * i] & 0x0F) << 4) | (src[2 * i + 1] & 0x0F);

    return dst;
}
