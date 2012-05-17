#include "signalingstorage.h"

using namespace int_calc;

void SignalingStorage::addObject(const std::string &nam, stored_object *obj)
{
    map_object_storage::addObject(nam, obj);
    changedSignal.sendSignal();
}

void SignalingStorage::deleteObject(const std::string &nam)
{
    map_object_storage::deleteObject(nam);
    changedSignal.sendSignal();
}

void SignalingStorage::clear()
{
    map_object_storage::clear();
    changedSignal.sendSignal();
}
