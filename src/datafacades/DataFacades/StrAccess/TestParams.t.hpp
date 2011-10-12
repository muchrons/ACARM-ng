/*
 * TestParams.t.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_SACCESS_TESTPARAMS_T_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_SACCESS_TESTPARAMS_T_HPP_FILE

#include "Persistency/Facades/StrAccess/Params.hpp"
#include "Persistency/Facades/StrAccess/DefaultHandleMap.hpp"
#include "Persistency/Facades/StrAccess/TestCallback.t.hpp"

namespace
{

typedef Persistency::Facades::StrAccess::Params<Persistency::Facades::StrAccess::DefaultHandleMap,
                                                Persistency::Facades::StrAccess::TestCallback>     TestParams;

} // unnamed namespace

#endif
