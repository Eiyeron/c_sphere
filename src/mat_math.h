#ifndef MAT_MATH_H
#define MAT_MATH_H

// Extra bits and bobs

#define C_PI 3.14159265358979323846f
#define C_TAU (2.f * C_PI)

// Types

typedef struct vec3
{
	float x;
	float y;
	float z;
} vec3;

typedef struct vec4
{
	float x;
	float y;
	float z;
	float w;
} vec4;

typedef struct mat3
{
	vec3 c[3];
} mat3;

typedef struct mat4
{
	vec4 c[4];
} mat4;

// Functions
// - s suffix : scalar version,	applies the operation to the whole vector/matrix, component-wise
// - v suffix : operation between matrix and a vector.
// Unless specified, most arithmetic operations are component-wise.

vec3 v3_add(vec3 a, vec3 b);

vec3 v3_adds(vec3 a, float b);

vec3 v3_sub(vec3 a, vec3 b);

vec3 v3_subs(vec3 a, float b);

vec3 v3_mul(vec3 a, vec3 b);

vec3 v3_muls(vec3 a, float b);

vec3 v3_div(vec3 a, vec3 b);

vec3 v3_divs(vec3 a, float b);


// mat3 operations

mat3 m3_add(mat3 a, mat3 b);

mat3 m3_adds(mat3 a, float b);

mat3 m3_sub(mat3 a, mat3 b);

mat3 m3_subs(mat3 a, float b);

mat3 m3_muls(mat3 a, float b);

mat3 m3_mulm(mat3 a, mat3 b);

// mat3 * vec3
vec3 m3_mulv(mat3 a, vec3 b);

// Rotation matrix around global x axis.
mat3 m3_eulerx(float x);

// Rotation matrix around global y axis.
mat3 m3_eulery(float y);

// Rotation matrix around global z axis.
mat3 m3_eulerz(float z);

// vec4 operations

// vec4(vec3, 1);
// Useful for homogenous coordinates operations
vec4 v4_fromv3(vec3 v);

// vec4(vec3, 0);
vec4 v4_fromv3z(vec3 v);

vec4 v4_add(vec4 a, vec4 b);

vec4 v4_adds(vec4 a, float b);

vec4 v4_div(vec4 a, vec4 b);

vec4 v4_divs(vec4 a, float b);

vec4 v4_mul(vec4 a, vec4 b);

vec4 v4_muls(vec4 a, float b);


// mat4 operations


mat4 m4_zero(void);

// Promotes into a mat4, with mat4[3][3] at 1;
mat4 m4_fromm3(mat3 m3);

// Identity matrix
mat4 m4_iden(void);

// Translation matrix
mat4 m4_transl(vec3 xyz);

// Transpose matrix
mat4 m4_transp(mat4 a);

// mat4 * vec4
vec4 m4_mulv(mat4 a, vec4 b);

mat4 m4_mulm(mat4 a, mat4 b);

// Right-hand Perspective matrix (into NDX coordinates)
// A reminder: as it's based on OpenGL conventions, the z component
// for your point to project should be negative to work.
mat4 m4_persp_ndc(float fov_y, float ratio, float near, float far);


#endif // MAT_MATH_H