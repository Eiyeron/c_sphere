#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "pd_api.h"

#include "mat_math.h"
#include "sphere.h"

// Some tweakables to alter the scene.
#define SPHERE_R 40
#define Z_OFFSET -70

// Runtime data
// Yes, globals are supposed to be code smell but in this situation,
// it's a simple enough project to not be worth the refactor.
// Plus, it's single-threaded, so no worries about data races.

// And I don't know if the stack will be bigh enough for 400 vec4 and 200 vec3
// (8'800 bytes, if I got my math right).

vec4 projected_sphere_vertices[SPHERE_VERTS];

vec3 stars[200];
vec4 star_buffer_1[200];
vec4 star_buffer_2[200];
// Buffer ping-pong. Instead of copying one array into another, just swap between them.
vec4(*projected_stars)[200];
vec4(*previous_projected_stars)[200];

extern const uint8_t dither_pattern[16];

vec3 camera_position;

// The static part of the 3D transform chain:
// - The projection matrix
// - the sphere/star base rotation (to tilt the whole scene)
// - The base translation (moving everything far enough from the camera)
mat4 proj;
mat3 base_rotation;
mat4 base_translation;

// The center of the sphere to detect which lines should be draw as front or back.
vec4 reference_point;


// Random value in the range [a; b]
static float rand_ab(float a, float b)
{
	float v = rand() / (float)(RAND_MAX);
	return (1.f - v) * a + (b * v);
}

// Random value in the range [-1; 1]
static float rand_unit(void)
{
	return rand() / (float)(RAND_MAX) * 2.f - 1.f;
}

static void init(void)
{
	// Creating the stars.
	// Uncomment the next line for random star placement between runs.
	// srand(time(NULL));
	for (int i = 0; i < 200; ++i)
	{
		// Unit polar coordinate into xyz.
		// Not really unform, but it'll do.
		const float z = rand_ab(-1.0f, 1.0f);
		const float a = rand_ab(0.0f, C_TAU);
		const float r = sqrtf(1.0f - z * z);
		const float x = r * cosf(a);
		const float y = r * sinf(a);

		const float radius = rand_ab(SPHERE_R + 1.f, SPHERE_R + 40.f);

		stars[i] = (vec3){
		x * radius,
		y * radius,
		z * radius };
	}

	// The whole transform chain looks like this
	// Proj * Trans * Ry * Rx * Rz
	// Rz is varying in time.
	// As matrix multiplications are associative, we can factorize the formula into
	// (Proj * Trans * Ry * Rx) * Rz
	// From this, let's compute three matrices, for projection, translation
	// and the static part of the rotation.

	const mat3 rotx = m3_eulerx(C_PI * 0.1);
	const mat3 roty = m3_eulery(C_PI * 0.2);
	base_rotation = m3_mulm(roty, rotx);

	base_translation = m4_transl((vec3) { 0, 0, Z_OFFSET });

	proj = m4_persp_ndc(C_PI / 2.f, 400.f / 240.f, 0.1f, 100.f);

	const vec4 center = { 0, 0, Z_OFFSET, 1.f };
	const vec4 cp = m4_mulv(proj, center);
	reference_point = (vec4){ cp.x / cp.w, cp.y / cp.w, cp.z / cp.w };

	previous_projected_stars = &star_buffer_2;
	projected_stars = &star_buffer_1;

}

static float angle = 0;
static int update(void* userdata)
{
	PlaydateAPI* pd = userdata;

	pd->graphics->clear(kColorBlack);
	pd->graphics->setLineCapStyle(kLineCapStyleButt);


	const mat3 rotz = m3_eulerz(-angle);
	const mat3 rot = m3_mulm(base_rotation, rotz);
	const mat4 rot4 = m4_fromm3(rot);

	const mat4 trs = m4_mulm(base_translation, rot4);
	// Sure, (proj * translation * Ry * Rx) could be precomputed
	// during init(), but it runs fast enough like this.
	const mat4 pipe = m4_mulm(proj, trs);
	
	// Project the sphere's vertices
	for (int i = 0; i < 92; ++i)
	{
		// Here too, the sphere vertices could be pre-multiplied.
		// Those are left as "easy" optimizations if needed.
		const vec4 v = v4_fromv3(v3_muls(sphere_vertices[i], SPHERE_R));
		const vec4 vp = m4_mulv(pipe, v);

		const vec3 vj = { vp.x / vp.w, vp.y / vp.w, vp.z / vp.w };

		projected_sphere_vertices[i] = (vec4){
		vj.x * 200.f + 200.f,
		vj.y * 120.f + 120.f,
		vj.z,
		vp.w,
		};
	}

	// Project the stars
	for (int i = 0; i < 200; ++i)
	{
		const vec4 v = v4_fromv3(stars[i]);
		const vec4 vp = m4_mulv(pipe, v);

		const vec3 vj = { vp.x / vp.w, vp.y / vp.w, vp.z / vp.w };

		(*projected_stars)[i] = (vec4){
		vj.x * 200.f + 200.f,
		vj.y * 120.f + 120.f,
		vj.z,
		vp.w,
		};
	}

	// Drawing the sphere
	for (unsigned i = 0; i < 270; ++i)
	{
		const int i1 = sphere_edges[i][0];
		const int i2 = sphere_edges[i][1];
		const vec4 a = projected_sphere_vertices[i1];
		const vec4 b = projected_sphere_vertices[i2];

		// If the vertices are closer than the sphere center, draw the edge as a "front" edge, plain and thick.
		// Otherwise, draw is as a "back" edge, thin and dithered.
		// Directly inspired from the Pico-8 version.
		// A bonus from having a monochrome screen: no need to sort front and back edges
		// anymore to avoid wrong overdraw.
		LCDColor color = a.z < reference_point.z&& b.z < reference_point.z ? kColorWhite : (uintptr_t)(&dither_pattern);
		{
			const float v = 10.f - a.w / 5.f;
			const int thickness = (int)fmaxf(1.f, v);
			pd->graphics->drawLine(
				(int)(a.x),
				(int)(a.y),
				(int)(b.x),
				(int)(b.y),
				thickness, color);
		}

	}

	// Drawing the stars
	for (unsigned i = 0; i < 200; ++i)
	{
		const vec4 vs = (*projected_stars)[i];
		const vec4 vsp = (*previous_projected_stars)[i];

		// If the star is behind the camera's frustum (so, out of view), let's skip it.
		if (vs.w < -1)
		{
			continue;
		}

		const vec3 star = { vs.x, vs.y, vs.z };

		const vec3 pstar = { vsp.x, vsp.y, vsp.z };

		const float v = 10.f - vs.w / 5.f;
		const int thickness = (int)fmaxf(1.f, v);
		const int half_thickness = thickness / 2;

		if (fabsf(star.x - pstar.x) < 60 && fabsf(star.y - pstar.y) < 60) {
			pd->graphics->drawLine(
				(int)(star.x + half_thickness), (int)(star.y + half_thickness),
				(int)(pstar.x + half_thickness), (int)(pstar.y + half_thickness),
				thickness,
				(uintptr_t)(dither_pattern)
			);
		}

		pd->graphics->fillEllipse(
			(int)(star.x),
			(int)(star.y),
			thickness, thickness, 0, 0, kColorWhite);

	}

	angle += 1.f / 360.f * C_TAU;
	angle = fmodf(angle, C_TAU);

	// Buffer ping-ponging, to keep track of the last position
	// without copying from a buffer to another.
	if (previous_projected_stars == &star_buffer_1)
	{
		previous_projected_stars = &star_buffer_2;
		projected_stars = &star_buffer_1;
	}
	else
	{
		previous_projected_stars = &star_buffer_1;
		projected_stars = &star_buffer_2;
	}

	return 1;
}

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if (event == kEventInit)
	{
		pd->system->setUpdateCallback(update, pd);
		init();
	}

	return 0;
}

// Extra data

const uint8_t dither_pattern[16] = {
0xFF, 0x00,
0xFF, 0x00,
0xFF, 0x00,
0xFF, 0x00,

0xFF, 0x00,
0xFF, 0x00,
0xFF, 0x00,
0xFF, 0x00,
};
