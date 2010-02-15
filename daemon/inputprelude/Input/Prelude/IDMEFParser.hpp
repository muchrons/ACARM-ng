/*
 * IDMEFParser.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_IDMEFPARSER_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_IDMEFPARSER_HPP_FILE

#include <prelude.h>

#include "Persistency/Alert.hpp"
#include "Persistency/Timestamp.hpp"
#include "Persistency/Host.hpp"

namespace Input
{
namespace Prelude
{
  /**
   * @brief Allows to extract information from IDMEF Messages
   *
   */
class IDMEFParser
{
public:
  /**
   * @brief c-tor creates parser object from idmef_message_t
   */
  IDMEFParser(idmef_message_t *msg);
  /**
   * @brief gets host name
   */
  Persistency::Host::Name getName();

  /**
   * @brief gets alert creation time
   */
  Persistency::Timestamp getCreateTime();

private:    
  idmef_alert_t *alert_;
};

}
}

#endif

