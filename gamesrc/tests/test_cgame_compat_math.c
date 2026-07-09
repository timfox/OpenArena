/*
 * Unit tests for OpenArena cgame compatibility math used by native idTech3
 * integration paths.
 */
#include <stdio.h>
#include <string.h>

#include "cg_compat_math.h"

#define ASSERT(cond, msg) do { \
	if (!(cond)) { \
		fprintf(stderr, "FAIL: %s\n", msg); \
		return 1; \
	} \
} while (0)

#define ASSERT_STREQ(a, b, msg) do { \
	if (strcmp((a), (b)) != 0) { \
		fprintf(stderr, "FAIL: %s\n", msg); \
		return 1; \
	} \
} while (0)

static void v3( vec3_t out, float x, float y, float z ) {
	out[0] = x;
	out[1] = y;
	out[2] = z;
}

static int test_game_version_fallback_basic(void) {
	char built[32];

	CG_BuildExpectedGameVersion( "openarena", "baseq3", built, sizeof( built ) );
	ASSERT_STREQ( built, "openarena-1", "openarena game version" );

	CG_BuildExpectedGameVersion( "baseoa", "openarena", built, sizeof( built ) );
	ASSERT_STREQ( built, "baseoa-1", "baseoa game version" );
	return 0;
}

static int test_game_version_fallback_default(void) {
	char built[32];

	CG_BuildExpectedGameVersion( "", "openarena", built, sizeof( built ) );
	ASSERT_STREQ( built, "openarena-1", "empty gamename uses BASEGAME" );

	CG_BuildExpectedGameVersion( NULL, NULL, built, sizeof( built ) );
	ASSERT_STREQ( built, "baseq3-1", "NULL names use hard default" );
	return 0;
}

static int test_spawn_point_bounds_math(void) {
	vec3_t mins, maxs, spawn;

	v3( mins, -1024.0f, -1024.0f, -128.0f );
	v3( maxs, 1024.0f, 1024.0f, 512.0f );
	v3( spawn, 128.0f, -64.0f, 96.0f );
	ASSERT( CG_PointInsideBounds( spawn, mins, maxs ) == qtrue,
		"spawn inside collision bounds" );

	v3( spawn, -1024.0f, 0.0f, 16.0f );
	ASSERT( CG_PointInsideBounds( spawn, mins, maxs ) == qtrue,
		"spawn on boundary stays valid" );

	v3( spawn, 1400.0f, 0.0f, 16.0f );
	ASSERT( CG_PointInsideBounds( spawn, mins, maxs ) == qfalse,
		"spawn outside collision bounds" );
	return 0;
}

static int test_testgun_origin_identity_axes(void) {
	vec3_t vieworg, out;
	vec3_t axis[3];

	v3( vieworg, 100.0f, 200.0f, 300.0f );
	v3( axis[0], 1.0f, 0.0f, 0.0f );
	v3( axis[1], 0.0f, 1.0f, 0.0f );
	v3( axis[2], 0.0f, 0.0f, 1.0f );

	CG_ComputeTestGunOrigin( vieworg, axis, 8.0f, -4.0f, 12.0f, out );
	ASSERT( out[0] == 108.0f && out[1] == 196.0f && out[2] == 312.0f,
		"identity-axis gun origin offset" );
	return 0;
}

static int test_testgun_origin_rotated_axes(void) {
	vec3_t vieworg, out;
	vec3_t axis[3];

	v3( vieworg, 10.0f, 20.0f, 30.0f );
	v3( axis[0], 0.0f, 1.0f, 0.0f );
	v3( axis[1], -1.0f, 0.0f, 0.0f );
	v3( axis[2], 0.0f, 0.0f, 1.0f );

	CG_ComputeTestGunOrigin( vieworg, axis, 4.0f, 2.0f, -6.0f, out );
	ASSERT( out[0] == 8.0f && out[1] == 24.0f && out[2] == 24.0f,
		"rotated-axis gun origin offset" );
	return 0;
}

int main( void ) {
	if ( test_game_version_fallback_basic() ) return 1;
	if ( test_game_version_fallback_default() ) return 1;
	if ( test_spawn_point_bounds_math() ) return 1;
	if ( test_testgun_origin_identity_axes() ) return 1;
	if ( test_testgun_origin_rotated_axes() ) return 1;
	return 0;
}
