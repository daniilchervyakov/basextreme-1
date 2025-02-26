#ifndef NX_FOUNDATION_NXFPU
#define NX_FOUNDATION_NXFPU
/*----------------------------------------------------------------------------*\
|
|						Public Interface to NovodeX Technology
|
|							     www.novodex.com
|
\*----------------------------------------------------------------------------*/

#include "Nx.h"

#ifndef WIN32
#include <math.h>
#endif

	#define	NX_SIGN_BITMASK		0x80000000

	//Integer representation of a floating-point value.
	#define NX_IR(x)			((NxU32&)(x))

	//Floating-point representation of a integer value.
	#define NX_FR(x)			((NxF32&)(x))

	//Absolute integer representation of a floating-point value
	#define NX_AIR(x)			(NX_IR(x)&0x7fffffff)

#endif
