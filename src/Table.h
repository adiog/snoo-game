#ifndef __Table_h__
#define __Table_h__

#include "ComplexCushion.h"

class Table {
    public:
        virtual ~Table();
        
        /** Zwraca referencję na singleton stołu */
        static Table & getSingleton(void);

        /** Bandy stołu jako jedna złożona banda */
        ComplexCushion & getCushions(void);

        /** @TODO: kieszenie */
    protected:
        ComplexCushion cushions;
        Table();
};

#endif
