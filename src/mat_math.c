#include "mat_math.h"

#include <math.h>

// vec3 operations

vec3 v3_add(vec3 a, vec3 b)
{
	return (vec3) {
		a.x + b.x,
			a.y + b.y,
			a.z + b.z,
	};
}

vec3 v3_adds(vec3 a, float b)
{
	return (vec3) {
		a.x + b,
			a.y + b,
			a.z + b,
	};
}

vec3 v3_sub(vec3 a, vec3 b)
{
	return (vec3) {
		a.x - b.x,
			a.y - b.y,
			a.z - b.z,
	};
}

vec3 v3_subs(vec3 a, float b)
{
	return (vec3) {
		a.x - b,
			a.y - b,
			a.z - b,
	};
}

vec3 v3_mul(vec3 a, vec3 b)
{
	return (vec3) {
		a.x* b.x,
			a.y* b.y,
			a.z* b.z,
	};
}

vec3 v3_muls(vec3 a, float b)
{
	return (vec3) {
		a.x* b,
			a.y* b,
			a.z* b,
	};
}

vec3 v3_div(vec3 a, vec3 b)
{
	return (vec3) {
		a.x / b.x,
			a.y / b.y,
			a.z / b.z,
	};
}

vec3 v3_divs(vec3 a, float b)
{
	return (vec3) {
		a.x / b,
			a.y / b,
			a.z / b,
	};
}


// mat3 operations

mat3 m3_add(mat3 a, mat3 b)
{
	mat3 m;
	m.c[0] = v3_add(a.c[0], b.c[0]);
	m.c[1] = v3_add(a.c[1], b.c[1]);
	m.c[2] = v3_add(a.c[2], b.c[2]);
	return m;
}

mat3 m3_adds(mat3 a, float b)
{
	mat3 m;
	m.c[0] = v3_adds(a.c[0], b);
	m.c[1] = v3_adds(a.c[1], b);
	m.c[2] = v3_adds(a.c[2], b);
	return m;
}

mat3 m3_sub(mat3 a, mat3 b)
{
	mat3 m;
	m.c[0] = v3_sub(a.c[0], b.c[0]);
	m.c[1] = v3_sub(a.c[1], b.c[1]);
	m.c[2] = v3_sub(a.c[2], b.c[2]);
	return m;
}

mat3 m3_subs(mat3 a, float b)
{
	mat3 m;
	m.c[0] = v3_subs(a.c[0], b);
	m.c[1] = v3_subs(a.c[1], b);
	m.c[2] = v3_subs(a.c[2], b);
	return m;
}

mat3 m3_muls(mat3 a, float b)
{
	mat3 m;
	m.c[0] = v3_muls(a.c[0], b);
	m.c[1] = v3_muls(a.c[1], b);
	m.c[2] = v3_muls(a.c[2], b);
	return m;
}

mat3 m3_mulm(mat3 a, mat3 b)
{
	const float a0x = a.c[0].x;
	const float a0y = a.c[0].y;
	const float a0z = a.c[0].z;
	const float a1x = a.c[1].x;
	const float a1y = a.c[1].y;
	const float a1z = a.c[1].z;
	const float a2x = a.c[2].x;
	const float a2y = a.c[2].y;
	const float a2z = a.c[2].z;

	const float b0x = b.c[0].x;
	const float b0y = b.c[0].y;
	const float b0z = b.c[0].z;
	const float b1x = b.c[1].x;
	const float b1y = b.c[1].y;
	const float b1z = b.c[1].z;
	const float b2x = b.c[2].x;
	const float b2y = b.c[2].y;
	const float b2z = b.c[2].z;

	mat3 c;

	c.c[0] = (vec3){
	a0x * b0x + a1x * b0y + a2x * b0z,
	a0y * b0x + a1y * b0y + a2y * b0z,
	a0z * b0x + a1z * b0y + a2z * b0z };
	c.c[1] = (vec3){
	a0x * b1x + a1x * b1y + a2x * b1z,
	a0y * b1x + a1y * b1y + a2y * b1z,
	a0z * b1x + a1z * b1y + a2z * b1z };
	c.c[2] = (vec3){
	a0x * b2x + a1x * b2y + a2x * b2z,
	a0y * b2x + a1y * b2y + a2y * b2z,
	a0z * b2x + a1z * b2y + a2z * b2z };
	return c;
}

vec3 m3_mulv(mat3 a, vec3 b)
{
	return (vec3) {
		a.c[0].x* b.x + a.c[1].x * b.y + a.c[2].x * b.z,
			a.c[0].y* b.x + a.c[1].y * b.y + a.c[2].y * b.z,
			a.c[0].z* b.x + a.c[1].z * b.y + a.c[2].z * b.z,
	};
}

mat3 m3_eulerx(float x)
{
	const float cos_x = cosf(x);
	const float sin_x = sinf(x);

	return (mat3) {
		(vec3) {
		1, 0, 0
	},
			(vec3) {
			0, cos_x, sin_x
		},
			(vec3) {
			0, -sin_x, cos_x
		},
	};
}

mat3 m3_eulery(float y)
{
	const float cos_y = cosf(y);
	const float sin_y = sinf(y);

	return (mat3) {
		(vec3) {
		cos_y, 0, -sin_y
	},
			(vec3) {
			0, 1, 0
		},
			(vec3) {
			sin_y, 0, cos_y
		},
	};
}

mat3 m3_eulerz(float z)
{
	const float cos_z = cosf(z);
	const float sin_z = sinf(z);

	return (mat3) {
		(vec3) {
		cos_z, sin_z, 0
	},
			(vec3) {
			-sin_z, cos_z, 0
		},
			(vec3) {
			0, 0, 1
		},
	};
}

// vec4 operations


vec4 v4_fromv3(vec3 v)
{
	return (vec4) {
		v.x, v.y, v.z, 1
	};
}

vec4 v4_fromv3z(vec3 v)
{
	return (vec4) {
		v.x, v.y, v.z, 0
	};
}

vec4 v4_add(vec4 a, vec4 b)
{
	return (vec4) {
		a.x + b.x,
			a.y + b.y,
			a.z + b.z,
			a.w + b.w,
	};
}

vec4 v4_adds(vec4 a, float b)
{
	return (vec4) {
		a.x + b,
			a.y + b,
			a.z + b,
			a.w + b,
	};
}

vec4 v4_div(vec4 a, vec4 b)
{
	return (vec4) {
		a.x / b.x,
			a.y / b.y,
			a.z / b.z,
			a.w / b.w,
	};
}

vec4 v4_divs(vec4 a, float b)
{
	return (vec4) {
		a.x / b,
			a.y / b,
			a.z / b,
			a.w / b,
	};
}

vec4 v4_mul(vec4 a, vec4 b)
{
	return (vec4) {
		a.x* b.x,
			a.y* b.y,
			a.z* b.z,
			a.w* b.w,
	};
}

vec4 v4_muls(vec4 a, float b)
{
	return (vec4) {
		a.x* b,
			a.y* b,
			a.z* b,
			a.w* b,
	};
}


// mat4 operations


mat4 m4_zero(void)
{
	return (mat4) {
		(vec4) {
		0, 0, 0, 0
	},
			(vec4) {
			0, 0, 0, 0
		},
			(vec4) {
			0, 0, 0, 0
		},
				(vec4) {
				0, 0, 0, 0
			}
	};
}

mat4 m4_fromm3(mat3 m3) {
	mat4 m = m4_zero();
	m.c[0] = v4_fromv3z(m3.c[0]);
	m.c[1] = v4_fromv3z(m3.c[1]);
	m.c[2] = v4_fromv3z(m3.c[2]);
	m.c[3].w = 1;

	return m;
}

mat4 m4_iden(void)
{
	return (mat4) {
		(vec4) {
		1, 0, 0, 0
	},
			(vec4) {
			0, 1, 0, 0
		},
			(vec4) {
			0, 0, 1, 0
		},
				(vec4) {
				0, 0, 0, 1
			}
	};
}


mat4 m4_transl(vec3 xyz)
{
	mat4 m = m4_iden();
	m.c[3] = v4_fromv3(xyz);

	return m;
}

mat4 m4_transp(mat4 a)
{
	mat4 m;
	m.c[0] = (vec4){ a.c[0].x, a.c[1].x, a.c[2].x, a.c[3].x };
	m.c[1] = (vec4){ a.c[0].y, a.c[1].y, a.c[2].y, a.c[3].y };
	m.c[2] = (vec4){ a.c[0].z, a.c[1].z, a.c[2].z, a.c[3].z };
	m.c[3] = (vec4){ a.c[0].w, a.c[1].w, a.c[2].w, a.c[3].w };
	return m;
}

mat4 m4_persp_ndc(float fov_y, float ratio, float near, float far)
{
	float half_tan = tanf(fov_y / 2.f);

	mat4 m = m4_zero();
	m.c[0].x = 1.f / (ratio * half_tan);
	m.c[1].y = 1.f / (half_tan);

	m.c[2].w = -1.f;
	// -1 -> 1 depth
	m.c[2].z = -(far + near) / (far - near);
	m.c[3].z = -(2.f * far * near) / (far - near);

	return m;
}

vec4 m4_mulv(mat4 a, vec4 b)
{
	vec4 v = v4_muls(a.c[0], b.x);
	v = v4_add(v, v4_muls(a.c[1], b.y));
	v = v4_add(v, v4_muls(a.c[2], b.z));
	v = v4_add(v, v4_muls(a.c[3], b.w));
	return v;
}


mat4 m4_mulm(mat4 a, mat4 b)
{
	const float a0x = a.c[0].x;
	const float a0y = a.c[0].y;
	const float a0z = a.c[0].z;
	const float a0w = a.c[0].w;

	const float a1x = a.c[1].x;
	const float a1y = a.c[1].y;
	const float a1z = a.c[1].z;
	const float a1w = a.c[1].w;

	const float a2x = a.c[2].x;
	const float a2y = a.c[2].y;
	const float a2z = a.c[2].z;
	const float a2w = a.c[2].w;

	const float a3x = a.c[3].x;
	const float a3y = a.c[3].y;
	const float a3z = a.c[3].z;
	const float a3w = a.c[3].w;

	const float b0x = b.c[0].x;
	const float b0y = b.c[0].y;
	const float b0z = b.c[0].z;
	const float b0w = b.c[0].w;

	const float b1x = b.c[1].x;
	const float b1y = b.c[1].y;
	const float b1z = b.c[1].z;
	const float b1w = b.c[1].w;

	const float b2x = b.c[2].x;
	const float b2y = b.c[2].y;
	const float b2z = b.c[2].z;
	const float b2w = b.c[2].w;

	const float b3x = b.c[3].x;
	const float b3y = b.c[3].y;
	const float b3z = b.c[3].z;
	const float b3w = b.c[3].w;

	mat4 c;

	c.c[0] = (vec4){
	a0x * b0x + a1x * b0y + a2x * b0z + a3x * b0w,
	a0y * b0x + a1y * b0y + a2y * b0z + a3y * b0w,
	a0z * b0x + a1z * b0y + a2z * b0z + a3z * b0w,
	a0w * b0x + a1w * b0y + a2w * b0z + a3w * b0w };
	c.c[1] = (vec4){
	a0x * b1x + a1x * b1y + a2x * b1z + a3x * b1w,
	a0y * b1x + a1y * b1y + a2y * b1z + a3y * b1w,
	a0z * b1x + a1z * b1y + a2z * b1z + a3z * b1w,
	a0w * b1x + a1w * b1y + a2w * b1z + a3w * b1w };
	c.c[2] = (vec4){
	a0x * b2x + a1x * b2y + a2x * b2z + a3x * b2w,
	a0y * b2x + a1y * b2y + a2y * b2z + a3y * b2w,
	a0z * b2x + a1z * b2y + a2z * b2z + a3z * b2w,
	a0w * b2x + a1w * b2y + a2w * b2z + a3w * b2w };
	c.c[3] = (vec4){
	a0x * b3x + a1x * b3y + a2x * b3z + a3x * b3w,
	a0y * b3x + a1y * b3y + a2y * b3z + a3y * b3w,
	a0z * b3x + a1z * b3y + a2z * b3z + a3z * b3w,
	a0w * b3x + a1w * b3y + a2w * b3z + a3w * b3w };

	return c;
}
