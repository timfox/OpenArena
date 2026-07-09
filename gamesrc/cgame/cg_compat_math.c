#include <stdio.h>

#include "cg_compat_math.h"

size_t CG_BuildExpectedGameVersion( const char *gamename, const char *defaultGamename,
	char *out, size_t outSize ) {
	const char *baseName;
	int written;

	if ( !out || outSize == 0 ) {
		return 0;
	}

	baseName = ( gamename && gamename[0] ) ? gamename : defaultGamename;
	if ( !baseName || !baseName[0] ) {
		baseName = "baseq3";
	}

	written = snprintf( out, outSize, "%s-1", baseName );
	if ( written < 0 ) {
		out[0] = '\0';
		return 0;
	}

	if ( (size_t)written >= outSize ) {
		return outSize - 1;
	}

	return (size_t)written;
}

qboolean CG_PointInsideBounds( const vec3_t point, const vec3_t mins, const vec3_t maxs ) {
	return (qboolean)(
		point[0] >= mins[0] && point[0] <= maxs[0] &&
		point[1] >= mins[1] && point[1] <= maxs[1] &&
		point[2] >= mins[2] && point[2] <= maxs[2] );
}

void CG_ComputeTestGunOrigin( const vec3_t vieworg, const vec3_t viewaxis[3],
	float gunX, float gunY, float gunZ, vec3_t out ) {
	int i;

	VectorCopy( vieworg, out );
	for ( i = 0; i < 3; i++ ) {
		out[i] += viewaxis[0][i] * gunX;
		out[i] += viewaxis[1][i] * gunY;
		out[i] += viewaxis[2][i] * gunZ;
	}
}
