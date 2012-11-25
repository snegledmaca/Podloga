#ifndef ELEMENTS_H_INCLUDED
#define ELEMENTS_H_INCLUDED

//======================================================================
// Elements - dependancies
//======================================================================

    #include "memoryparts/frompool.h"
    #include "container.h"

//======================================================================
// Elements - definitions
//======================================================================

    struct DataPoint;
    struct ClassPoint;
    struct ElementPoint;

    class Interactive;
    class Part;
    class Element;

//======================================================================
// Elements - implementations
//======================================================================

    struct DataPoint: public FromPool {
        char* name;
        char* value;
        ClassPoint* class_point;
    };

    struct ClassPoint: public FromPool {
        char* name;
        DataPoint** data;
    };

    struct ElementPoint: public FromPool {
        DataPoint** data;

        ElementPoint* previous;
        ElementPoint* parent;
        ElementPoint* next;
        ElementPoint* first_child;
    };

//=====================================================================

#endif // ELEMENTS_H_INCLUDED
