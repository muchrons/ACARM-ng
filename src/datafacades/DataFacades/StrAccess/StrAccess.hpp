/*
 * StrAccess.hpp
 *
 * main header for this module. it includes everything that is needed for the facility to work.
 * to use it one must define callback object in form:
 *
 * struct MyCallbackHandle
 * {
 *   // called when collection size is to be returned to user
 *   bool collectionSize(size_t size)
 *   {
 *     // user implementaiton
 *   }
 *   // called when (non-NULL) value is to be returned to user
 *   bool value(const std::string &v)
 *   {
 *     // user implementation
 *   }
 *   // called when NULL is found on path (arguments indicated place in path where it happened)
 *   bool nullOnPath(const std::string &where)
 *   {
 *     // user implementation
 *   }
 * }; // struct MyCallbackHandle
 *
 * return value is user-defined. by deafult it is returned back to caller. by design
 * however it can be used to signal that further processing is nor required (for example: when
 * iterating over a collection).
 *
 * using this facility boils down to declaring proper Params<> template specification and running
 * dispatcher to process given path, using user-provided callbacks. in a simples form it may
 * look like this:
 *
 * namespace StrFc=Persnstency::Facades::StrAccess;
 * typedef StrFc::Params<StrFc::DefaultHandleMap, MyCallbackHandle> Params;
 * MyCallbackHandle mcb;
 * Params           p(StrFc::Path("metaalert.id"), mcb);
 * StrFc::MainDispatcher::process(someNode, p); // from here callacks are called
 * // here mcb can be used to obtain gathered data, if it was saved, etc...
 *
 * method of accessing elements can be controlled by modyfing StrFc::DefaultHandleMap
 * (boost::mpl::map<>) and adding proper handles for given types (Alert, Host, etc..)
 * and/or conditions (CollectionIndexHandle, TermHandle, etc...).
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCESS_STRACCESS_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCESS_STRACCESS_HPP_FILE

/* public header */

#include "DataFacades/StrAccess/Params.hpp"
#include "DataFacades/StrAccess/DefaultHandleMap.hpp"
#include "DataFacades/StrAccess/MainDispatcher.hpp"

#endif
