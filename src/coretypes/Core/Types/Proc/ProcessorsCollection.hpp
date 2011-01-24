/*
 * ProcessorsCollection.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_PROC_PROCESSORSCOLLECTION_HPP_FILE
#define INCLUDE_CORE_TYPES_PROC_PROCESSORSCOLLECTION_HPP_FILE

/* public header */

#include <vector>

#include "Core/Types/Proc/Processor.hpp"

namespace Core
{
namespace Types
{
namespace Proc
{

/** \brief collection of processors. */
typedef std::vector<ProcessorPtrNN> ProcessorsCollection;

} // namespace Proc
} // namespace Types
} // namespace Core

#endif
