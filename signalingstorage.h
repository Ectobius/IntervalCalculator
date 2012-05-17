#ifndef SIGNALINGSTORAGE_H
#define SIGNALINGSTORAGE_H

#include "object_storage.h"
#include "signalizer.h"

class SignalingStorage : public int_calc::map_object_storage
{
public:
    Signalizer changedSignal;

    virtual void addObject(const std::string &, int_calc::stored_object *);
    virtual void deleteObject(const std::string &);
    virtual void clear();



};

#endif // SIGNALINGSTORAGE_H
