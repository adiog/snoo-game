#ifndef __Encoder_h__
#define __Encoder_h__

#include <cstdlib>
#include <string>
#include <vector>

typedef std::vector<unsigned char> DataArray;

class Encoder {
    public:
        /// Zamienia dowolny ciąg bajtów na napis
        /// składający się tylko z liter i znaków @
        static std::string encode(const void *data, size_t data_size);

        /// Zamienia napis wygenerowany przez
        /// encode na pierwotny ciąg bajtów
        static void decode(const std::string &, void *data);

        static DataArray decode(const std::string &);
};

#endif
