#ifndef HASREADER_H_INCLUDED
#define HASREADER_H_INCLUDED

//========================================================================
// HasReader - dependancies
//========================================================================

    #include "readsomein.h"

//========================================================================
// HasReader - definitions
//========================================================================

    class HasReader;

//========================================================================
// HasReader - implementations
//========================================================================

    class HasReader {

    //====================================================================

    	protected:

            static Reader* reader;

    //====================================================================

    };

//========================================================================

    Reader* HasReader::reader = new ReadSomeIn();

//========================================================================

#endif // HASREADER_H_INCLUDED
