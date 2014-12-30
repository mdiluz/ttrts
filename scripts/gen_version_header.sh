HEADER="// Auto generated ttrts version header
// do not edit manually
#ifndef _TTRTS_VERSION_H_
#define _TTRTS_VERSION_H_

#define TTRTS_VERSION_MAJOR $1
#define TTRTS_VERSION_MINOR $2
#define TTRTS_VERSION_PATCH $3
#define TTRTS_VERSION_STRING \"v$1.$2.$3\"

#endif //_TTRTS_VERSION_H_"

echo "$HEADER" > "version.h"