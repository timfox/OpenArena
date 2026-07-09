#ifndef CG_COMPAT_MATH_H
#define CG_COMPAT_MATH_H

#include "../qcommon/q_shared.h"

size_t CG_BuildExpectedGameVersion( const char *gamename, const char *defaultGamename,
	char *out, size_t outSize );
qboolean CG_PointInsideBounds( const vec3_t point, const vec3_t mins, const vec3_t maxs );
void CG_ComputeTestGunOrigin( const vec3_t vieworg, const vec3_t viewaxis[3],
	float gunX, float gunY, float gunZ, vec3_t out );

#endif
