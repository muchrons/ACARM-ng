#ifndef _LIBPRELUDE_PRELUDE_CLIENT_EASY_HXX
#define _LIBPRELUDE_PRELUDE_CLIENT_EASY_HXX

#include "LibPrelude/prelude.h"
#include "LibPrelude/CppBind/idmef.hpp"
#include "LibPrelude/CppBind/prelude-client.hpp"


namespace Prelude {
        class ClientEasy : public Client {
            private:
                void setup_analyzer(idmef_analyzer *analyzer,
                                    const char *_model,
                                    const char *_class,
                                    const char *_manufacturer,
                                    const char *version);

            public:
                ClientEasy(const char *profile,
                           int permission = Client::IDMEF_WRITE,
                           const char *_model = "Unknown model",
                           const char *_class = "Unknown class",
                           const char *_manufacturer = "Unknown manufacturer",
                           const char *_version = "Unknown version");
        };
};

#endif
/* public header */
