#include "signalizer.h"

void Signalizer::sendSignal()
{
    emit signal();
}
