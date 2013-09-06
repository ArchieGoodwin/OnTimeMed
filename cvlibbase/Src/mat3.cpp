#include "mat3.h"
#include "cvlibmacros.h"
#include <math.h>
#include <float.h>

namespace CVLib
{


mat3::mat3()
{
}

mat3::mat3(const nv_scalar* array)
{
    memcpy(mat_array, array, sizeof(nv_scalar) * 9);
}

mat3::mat3(const mat3 & M)
{
    memcpy(mat_array, M.mat_array, sizeof(nv_scalar) * 9);
}

mat4::mat4()
{
}

mat4::mat4(const nv_scalar* array)
{
    memcpy(mat_array, array, sizeof(nv_scalar) * 16);
}

mat4::mat4(const mat4& M)
{
    memcpy(mat_array, M.mat_array, sizeof(nv_scalar) * 16);
}

vec3 & cross(vec3 & u, const vec3 & v, const vec3 & w)
{
    u.x = v.y*w.z - v.z*w.y;
    u.y = v.z*w.x - v.x*w.z;
    u.z = v.x*w.y - v.y*w.x;
    return u;
}

nv_scalar & dot(nv_scalar& u, const vec3& v, const vec3& w)
{
    u = v.x*w.x + v.y*w.y + v.z*w.z;
    return u;
}

nv_scalar dot(const vec3& v, const vec3& w)
{
    return v.x*w.x + v.y*w.y + v.z*w.z;
}

nv_scalar & dot(nv_scalar& u, const vec4& v, const vec4& w)
{
    u = v.x*w.x + v.y*w.y + v.z*w.z + v.w*w.w;
    return u;
}

nv_scalar dot(const vec4& v, const vec4& w)
{
    return v.x*w.x + v.y*w.y + v.z*w.z + v.w*w.w;
}

nv_scalar & dot(nv_scalar& u, const vec3& v, const vec4& w)
{
    u = v.x*w.x + v.y*w.y + v.z*w.z;
    return u;
}

nv_scalar dot(const vec3& v, const vec4& w)
{
    return v.x*w.x + v.y*w.y + v.z*w.z;
}

nv_scalar & dot(nv_scalar& u, const vec4& v, const vec3& w)
{
    u = v.x*w.x + v.y*w.y + v.z*w.z;
    return u;
}

nv_scalar dot(const vec4& v, const vec3& w)
{
    return v.x*w.x + v.y*w.y + v.z*w.z;
}

vec3 & reflect(vec3& r, const vec3& n, const vec3& l)
{
    nv_scalar n_dot_l;
    n_dot_l = nv_two * dot(n_dot_l,n,l);
    mult(r,l,-nv_one);
    madd(r,n,n_dot_l);
    return r;
}

vec3 & madd(vec3 & u, const vec3& v, const nv_scalar& lambda)
{
    u.x += v.x*lambda;
    u.y += v.y*lambda;
    u.z += v.z*lambda;
    return u;
}

vec3 & mult(vec3 & u, const vec3& v, const nv_scalar& lambda)
{
    u.x = v.x*lambda;
    u.y = v.y*lambda;
    u.z = v.z*lambda;
    return u;
}

vec3 & mult(vec3 & u, const vec3& v, const vec3& w)
{
    u.x = v.x*w.x;
    u.y = v.y*w.y;
    u.z = v.z*w.z;
    return u;
}

vec3 & sub(vec3 & u, const vec3& v, const vec3& w)
{
    u.x = v.x - w.x;
    u.y = v.y - w.y;
    u.z = v.z - w.z;
    return u;
}

vec3 & add(vec3 & u, const vec3& v, const vec3& w)
{
    u.x = v.x + w.x;
    u.y = v.y + w.y;
    u.z = v.z + w.z;
    return u;
}

vec3 & scale(vec3& u, const nv_scalar s)
{
    u.x *= s;
    u.y *= s;
    u.z *= s;
    return u;
}

vec4 & scale(vec4& u, const nv_scalar s)
{
    u.x *= s;
    u.y *= s;
    u.z *= s;
    u.w *= s;
    return u;
}

vec3 & mult(vec3& u, const mat3& M, const vec3& v)
{
    u.x = M.a00 * v.x + M.a01 * v.y + M.a02 * v.z;
    u.y = M.a10 * v.x + M.a11 * v.y + M.a12 * v.z;
    u.z = M.a20 * v.x + M.a21 * v.y + M.a22 * v.z;
    return u;
}

vec3 & mult(vec3& u, const vec3& v, const mat3& M)
{
    u.x = M.a00 * v.x + M.a10 * v.y + M.a20 * v.z;
    u.y = M.a01 * v.x + M.a11 * v.y + M.a21 * v.z;
    u.z = M.a02 * v.x + M.a12 * v.y + M.a22 * v.z;
    return u;
}

const vec3 operator*(const mat3& M, const vec3& v)
{
	vec3 u;
    u.x = M.a00 * v.x + M.a01 * v.y + M.a02 * v.z;
    u.y = M.a10 * v.x + M.a11 * v.y + M.a12 * v.z;
    u.z = M.a20 * v.x + M.a21 * v.y + M.a22 * v.z;
    return u;
}

const vec3 operator*(const vec3& v, const mat3& M)
{
	vec3 u;
    u.x = M.a00 * v.x + M.a10 * v.y + M.a20 * v.z;
    u.y = M.a01 * v.x + M.a11 * v.y + M.a21 * v.z;
    u.z = M.a02 * v.x + M.a12 * v.y + M.a22 * v.z;
    return u;
}

vec4 & mult(vec4& u, const mat4& M, const vec4& v)
{
    u.x = M.a00 * v.x + M.a01 * v.y + M.a02 * v.z + M.a03 * v.w;
    u.y = M.a10 * v.x + M.a11 * v.y + M.a12 * v.z + M.a13 * v.w;
    u.z = M.a20 * v.x + M.a21 * v.y + M.a22 * v.z + M.a23 * v.w;
    u.w = M.a30 * v.x + M.a31 * v.y + M.a32 * v.z + M.a33 * v.w;
    return u;
}

vec4 & mult(vec4& u, const vec4& v, const mat4& M)
{
    u.x = M.a00 * v.x + M.a10 * v.y + M.a20 * v.z + M.a30 * v.w;
    u.y = M.a01 * v.x + M.a11 * v.y + M.a21 * v.z + M.a31 * v.w;
    u.z = M.a02 * v.x + M.a12 * v.y + M.a22 * v.z + M.a32 * v.w;
    u.w = M.a03 * v.x + M.a13 * v.y + M.a23 * v.z + M.a33 * v.w;
    return u;
}

const vec4 operator*(const mat4& M, const vec4& v)
{
	vec4 u;
    u.x = M.a00 * v.x + M.a01 * v.y + M.a02 * v.z + M.a03 * v.w;
    u.y = M.a10 * v.x + M.a11 * v.y + M.a12 * v.z + M.a13 * v.w;
    u.z = M.a20 * v.x + M.a21 * v.y + M.a22 * v.z + M.a23 * v.w;
    u.w = M.a30 * v.x + M.a31 * v.y + M.a32 * v.z + M.a33 * v.w;
    return u;
}

const vec4 operator*(const vec4& v, const mat4& M)
{
	vec4 u;
    u.x = M.a00 * v.x + M.a10 * v.y + M.a20 * v.z + M.a30 * v.w;
    u.y = M.a01 * v.x + M.a11 * v.y + M.a21 * v.z + M.a31 * v.w;
    u.z = M.a02 * v.x + M.a12 * v.y + M.a22 * v.z + M.a32 * v.w;
    u.w = M.a03 * v.x + M.a13 * v.y + M.a23 * v.z + M.a33 * v.w;
    return u;
}

vec3 & mult_pos(vec3& u, const mat4& M, const vec3& v)
{
    nv_scalar oow = nv_one / (v.x * M.a30 + v.y * M.a31 + v.z * M.a32 + M.a33);
    u.x = (M.a00 * v.x + M.a01 * v.y + M.a02 * v.z + M.a03) * oow;
    u.y = (M.a10 * v.x + M.a11 * v.y + M.a12 * v.z + M.a13) * oow;
    u.z = (M.a20 * v.x + M.a21 * v.y + M.a22 * v.z + M.a23) * oow;
    return u;
}

vec3 & mult_pos(vec3& u, const vec3& v, const mat4& M)
{
    nv_scalar oow = nv_one / (v.x * M.a03 + v.y * M.a13 + v.z * M.a23 + M.a33);
    u.x = (M.a00 * v.x + M.a10 * v.y + M.a20 * v.z + M.a30) * oow;
    u.y = (M.a01 * v.x + M.a11 * v.y + M.a21 * v.z + M.a31) * oow;
    u.z = (M.a02 * v.x + M.a12 * v.y + M.a22 * v.z + M.a32) * oow;
    return u;
}

vec3 & mult_dir(vec3& u, const mat4& M, const vec3& v)
{
    u.x = M.a00 * v.x + M.a01 * v.y + M.a02 * v.z;
    u.y = M.a10 * v.x + M.a11 * v.y + M.a12 * v.z;
    u.z = M.a20 * v.x + M.a21 * v.y + M.a22 * v.z;
    return u;
}

vec3 & mult_dir(vec3& u, const vec3& v, const mat4& M)
{
    u.x = M.a00 * v.x + M.a10 * v.y + M.a20 * v.z;
    u.y = M.a01 * v.x + M.a11 * v.y + M.a21 * v.z;
    u.z = M.a02 * v.x + M.a12 * v.y + M.a22 * v.z;
    return u;
}

vec3 & mult(vec3& u, const mat4& M, const vec3& v)
{
    u.x = M.a00 * v.x + M.a01 * v.y + M.a02 * v.z + M.a03;
    u.y = M.a10 * v.x + M.a11 * v.y + M.a12 * v.z + M.a13;
    u.z = M.a20 * v.x + M.a21 * v.y + M.a22 * v.z + M.a23;
    return u;
}

vec3 & mult(vec3& u, const vec3& v, const mat4& M)
{
    u.x = M.a00 * v.x + M.a10 * v.y + M.a20 * v.z + M.a30;
    u.y = M.a01 * v.x + M.a11 * v.y + M.a21 * v.z + M.a31;
    u.z = M.a02 * v.x + M.a12 * v.y + M.a22 * v.z + M.a32;
    return u;
}

mat4 & add(mat4& A, const mat4& B)
{
    A.a00 += B.a00;
    A.a10 += B.a10;
    A.a20 += B.a20;
    A.a30 += B.a30;
    A.a01 += B.a01;
    A.a11 += B.a11;
    A.a21 += B.a21;
    A.a31 += B.a31;
    A.a02 += B.a02;
    A.a12 += B.a12;
    A.a22 += B.a22;
    A.a32 += B.a32;
    A.a03 += B.a03;
    A.a13 += B.a13;
    A.a23 += B.a23;
    A.a33 += B.a33;
    return A;
}

mat3 & add(mat3& A, const mat3& B)
{
    A.a00 += B.a00;
    A.a10 += B.a10;
    A.a20 += B.a20;
    A.a01 += B.a01;
    A.a11 += B.a11;
    A.a21 += B.a21;
    A.a02 += B.a02;
    A.a12 += B.a12;
    A.a22 += B.a22;
    return A;
}

// C = A * B

// C.a00 C.a01 C.a02 C.a03   A.a00 A.a01 A.a02 A.a03   B.a00 B.a01 B.a02 B.a03
//                                                                            
// C.a10 C.a11 C.a12 C.a13   A.a10 A.a11 A.a12 A.a13   B.a10 B.a11 B.a12 B.a13
//                                                                         
// C.a20 C.a21 C.a22 C.a23   A.a20 A.a21 A.a22 A.a23   B.a20 B.a21 B.a22 B.a23  
//                                                                            
// C.a30 C.a31 C.a32 C.a33 = A.a30 A.a31 A.a32 A.a33 * B.a30 B.a31 B.a32 B.a33

mat4 & mult(mat4& C, const mat4& A, const mat4& B)
{
    C.a00 = A.a00 * B.a00 + A.a01 * B.a10 + A.a02 * B.a20 + A.a03 * B.a30;
    C.a10 = A.a10 * B.a00 + A.a11 * B.a10 + A.a12 * B.a20 + A.a13 * B.a30;
    C.a20 = A.a20 * B.a00 + A.a21 * B.a10 + A.a22 * B.a20 + A.a23 * B.a30;
    C.a30 = A.a30 * B.a00 + A.a31 * B.a10 + A.a32 * B.a20 + A.a33 * B.a30;
    C.a01 = A.a00 * B.a01 + A.a01 * B.a11 + A.a02 * B.a21 + A.a03 * B.a31;
    C.a11 = A.a10 * B.a01 + A.a11 * B.a11 + A.a12 * B.a21 + A.a13 * B.a31;
    C.a21 = A.a20 * B.a01 + A.a21 * B.a11 + A.a22 * B.a21 + A.a23 * B.a31;
    C.a31 = A.a30 * B.a01 + A.a31 * B.a11 + A.a32 * B.a21 + A.a33 * B.a31;
    C.a02 = A.a00 * B.a02 + A.a01 * B.a12 + A.a02 * B.a22 + A.a03 * B.a32;
    C.a12 = A.a10 * B.a02 + A.a11 * B.a12 + A.a12 * B.a22 + A.a13 * B.a32;
    C.a22 = A.a20 * B.a02 + A.a21 * B.a12 + A.a22 * B.a22 + A.a23 * B.a32;
    C.a32 = A.a30 * B.a02 + A.a31 * B.a12 + A.a32 * B.a22 + A.a33 * B.a32;
    C.a03 = A.a00 * B.a03 + A.a01 * B.a13 + A.a02 * B.a23 + A.a03 * B.a33;
    C.a13 = A.a10 * B.a03 + A.a11 * B.a13 + A.a12 * B.a23 + A.a13 * B.a33;
    C.a23 = A.a20 * B.a03 + A.a21 * B.a13 + A.a22 * B.a23 + A.a23 * B.a33;
    C.a33 = A.a30 * B.a03 + A.a31 * B.a13 + A.a32 * B.a23 + A.a33 * B.a33;
    return C;
}

mat4 mat4::operator*(const mat4& B) const
{
    mat4 C;
    C.a00 = a00 * B.a00 + a01 * B.a10 + a02 * B.a20 + a03 * B.a30;
    C.a10 = a10 * B.a00 + a11 * B.a10 + a12 * B.a20 + a13 * B.a30;
    C.a20 = a20 * B.a00 + a21 * B.a10 + a22 * B.a20 + a23 * B.a30;
    C.a30 = a30 * B.a00 + a31 * B.a10 + a32 * B.a20 + a33 * B.a30;
    C.a01 = a00 * B.a01 + a01 * B.a11 + a02 * B.a21 + a03 * B.a31;
    C.a11 = a10 * B.a01 + a11 * B.a11 + a12 * B.a21 + a13 * B.a31;
    C.a21 = a20 * B.a01 + a21 * B.a11 + a22 * B.a21 + a23 * B.a31;
    C.a31 = a30 * B.a01 + a31 * B.a11 + a32 * B.a21 + a33 * B.a31;
    C.a02 = a00 * B.a02 + a01 * B.a12 + a02 * B.a22 + a03 * B.a32;
    C.a12 = a10 * B.a02 + a11 * B.a12 + a12 * B.a22 + a13 * B.a32;
    C.a22 = a20 * B.a02 + a21 * B.a12 + a22 * B.a22 + a23 * B.a32;
    C.a32 = a30 * B.a02 + a31 * B.a12 + a32 * B.a22 + a33 * B.a32;
    C.a03 = a00 * B.a03 + a01 * B.a13 + a02 * B.a23 + a03 * B.a33;
    C.a13 = a10 * B.a03 + a11 * B.a13 + a12 * B.a23 + a13 * B.a33;
    C.a23 = a20 * B.a03 + a21 * B.a13 + a22 * B.a23 + a23 * B.a33;
    C.a33 = a30 * B.a03 + a31 * B.a13 + a32 * B.a23 + a33 * B.a33;
    return C;
}

// C = A * B

// C.a00 C.a01 C.a02   A.a00 A.a01 A.a02   B.a00 B.a01 B.a02
//                                                          
// C.a10 C.a11 C.a12   A.a10 A.a11 A.a12   B.a10 B.a11 B.a12
//                                                          
// C.a20 C.a21 C.a22 = A.a20 A.a21 A.a22 * B.a20 B.a21 B.a22

mat3 & mult(mat3& C, const mat3& A, const mat3& B)
{
    C.a00 = A.a00 * B.a00 + A.a01 * B.a10 + A.a02 * B.a20;
    C.a10 = A.a10 * B.a00 + A.a11 * B.a10 + A.a12 * B.a20;
    C.a20 = A.a20 * B.a00 + A.a21 * B.a10 + A.a22 * B.a20;
    C.a01 = A.a00 * B.a01 + A.a01 * B.a11 + A.a02 * B.a21;
    C.a11 = A.a10 * B.a01 + A.a11 * B.a11 + A.a12 * B.a21;
    C.a21 = A.a20 * B.a01 + A.a21 * B.a11 + A.a22 * B.a21;
    C.a02 = A.a00 * B.a02 + A.a01 * B.a12 + A.a02 * B.a22;
    C.a12 = A.a10 * B.a02 + A.a11 * B.a12 + A.a12 * B.a22;
    C.a22 = A.a20 * B.a02 + A.a21 * B.a12 + A.a22 * B.a22;
    return C;
}


mat3 & transpose(mat3& A)
{
    nv_scalar tmp;
    tmp = A.a01;
    A.a01 = A.a10;
    A.a10 = tmp;

    tmp = A.a02;
    A.a02 = A.a20;
    A.a20 = tmp;

    tmp = A.a12;
    A.a12 = A.a21;
    A.a21 = tmp;
    return A;
}

mat4 & transpose(mat4& A)
{
    nv_scalar tmp;
    tmp = A.a01;
    A.a01 = A.a10;
    A.a10 = tmp;

    tmp = A.a02;
    A.a02 = A.a20;
    A.a20 = tmp;

    tmp = A.a03;
    A.a03 = A.a30;
    A.a30 = tmp;

    tmp = A.a12;
    A.a12 = A.a21;
    A.a21 = tmp;

    tmp = A.a13;
    A.a13 = A.a31;
    A.a31 = tmp;

    tmp = A.a23;
    A.a23 = A.a32;
    A.a32 = tmp;
    return A;
}

mat4 & transpose(mat4& B, const mat4& A)
{
    B.a00 = A.a00;
    B.a01 = A.a10;
    B.a02 = A.a20;
    B.a03 = A.a30;
    B.a10 = A.a01;
    B.a11 = A.a11;
    B.a12 = A.a21;
    B.a13 = A.a31;
    B.a20 = A.a02;
    B.a21 = A.a12;
    B.a22 = A.a22;
    B.a23 = A.a32;
    B.a30 = A.a03;
    B.a31 = A.a13;
    B.a32 = A.a23;
    B.a33 = A.a33;
    return B;
}

mat3 & transpose(mat3& B, const mat3& A)
{
    B.a00 = A.a00;
    B.a01 = A.a10;
    B.a02 = A.a20;
    B.a10 = A.a01;
    B.a11 = A.a11;
    B.a12 = A.a21;
    B.a20 = A.a02;
    B.a21 = A.a12;
    B.a22 = A.a22;
    return B;
}

/*
    calculate the determinent of a 2x2 matrix in the from

    | a1 a2 |
    | b1 b2 |

*/
nv_scalar det2x2(nv_scalar a1, nv_scalar a2, nv_scalar b1, nv_scalar b2)
{
    return a1 * b2 - b1 * a2;
}

/*
    calculate the determinent of a 3x3 matrix in the from

    | a1 a2 a3 |
    | b1 b2 b3 |
    | c1 c2 c3 |

*/
nv_scalar det3x3(nv_scalar a1, nv_scalar a2, nv_scalar a3, 
                         nv_scalar b1, nv_scalar b2, nv_scalar b3, 
                         nv_scalar c1, nv_scalar c2, nv_scalar c3)
{
    return a1 * det2x2(b2, b3, c2, c3) - b1 * det2x2(a2, a3, c2, c3) + c1 * det2x2(a2, a3, b2, b3);
}

mat4 & invert(mat4& B, const mat4& A)
{
    nv_scalar det,oodet;

    B.a00 =  det3x3(A.a11, A.a21, A.a31, A.a12, A.a22, A.a32, A.a13, A.a23, A.a33);
    B.a10 = -det3x3(A.a10, A.a20, A.a30, A.a12, A.a22, A.a32, A.a13, A.a23, A.a33);
    B.a20 =  det3x3(A.a10, A.a20, A.a30, A.a11, A.a21, A.a31, A.a13, A.a23, A.a33);
    B.a30 = -det3x3(A.a10, A.a20, A.a30, A.a11, A.a21, A.a31, A.a12, A.a22, A.a32);

    B.a01 = -det3x3(A.a01, A.a21, A.a31, A.a02, A.a22, A.a32, A.a03, A.a23, A.a33);
    B.a11 =  det3x3(A.a00, A.a20, A.a30, A.a02, A.a22, A.a32, A.a03, A.a23, A.a33);
    B.a21 = -det3x3(A.a00, A.a20, A.a30, A.a01, A.a21, A.a31, A.a03, A.a23, A.a33);
    B.a31 =  det3x3(A.a00, A.a20, A.a30, A.a01, A.a21, A.a31, A.a02, A.a22, A.a32);

    B.a02 =  det3x3(A.a01, A.a11, A.a31, A.a02, A.a12, A.a32, A.a03, A.a13, A.a33);
    B.a12 = -det3x3(A.a00, A.a10, A.a30, A.a02, A.a12, A.a32, A.a03, A.a13, A.a33);
    B.a22 =  det3x3(A.a00, A.a10, A.a30, A.a01, A.a11, A.a31, A.a03, A.a13, A.a33);
    B.a32 = -det3x3(A.a00, A.a10, A.a30, A.a01, A.a11, A.a31, A.a02, A.a12, A.a32);

    B.a03 = -det3x3(A.a01, A.a11, A.a21, A.a02, A.a12, A.a22, A.a03, A.a13, A.a23);
    B.a13 =  det3x3(A.a00, A.a10, A.a20, A.a02, A.a12, A.a22, A.a03, A.a13, A.a23);
    B.a23 = -det3x3(A.a00, A.a10, A.a20, A.a01, A.a11, A.a21, A.a03, A.a13, A.a23);
    B.a33 =  det3x3(A.a00, A.a10, A.a20, A.a01, A.a11, A.a21, A.a02, A.a12, A.a22);

    det = (A.a00 * B.a00) + (A.a01 * B.a10) + (A.a02 * B.a20) + (A.a03 * B.a30);

    oodet = nv_one / det;

    B.a00 *= oodet;
    B.a10 *= oodet;
    B.a20 *= oodet;
    B.a30 *= oodet;

    B.a01 *= oodet;
    B.a11 *= oodet;
    B.a21 *= oodet;
    B.a31 *= oodet;

    B.a02 *= oodet;
    B.a12 *= oodet;
    B.a22 *= oodet;
    B.a32 *= oodet;

    B.a03 *= oodet;
    B.a13 *= oodet;
    B.a23 *= oodet;
    B.a33 *= oodet;

    return B;
}

mat4 & invert_rot_trans(mat4& B, const mat4& A)
{
    B.a00 = A.a00;
    B.a10 = A.a01;
    B.a20 = A.a02;
    B.a30 = A.a30;
    B.a01 = A.a10;
    B.a11 = A.a11;
    B.a21 = A.a12;
    B.a31 = A.a31;
    B.a02 = A.a20;
    B.a12 = A.a21;
    B.a22 = A.a22;
    B.a32 = A.a32;
    B.a03 = - (A.a00 * A.a03 + A.a10 * A.a13 + A.a20 * A.a23);
    B.a13 = - (A.a01 * A.a03 + A.a11 * A.a13 + A.a21 * A.a23);
    B.a23 = - (A.a02 * A.a03 + A.a12 * A.a13 + A.a22 * A.a23);
    B.a33 = A.a33;
    return B;
}

nv_scalar det(const mat3& A)
{
    return det3x3(A.a00, A.a01, A.a02, 
                 A.a10, A.a11, A.a12, 
                 A.a20, A.a21, A.a22);
}

mat3 & invert(mat3& B, const mat3& A)
{
    nv_scalar det,oodet;

    B.a00 =  (A.a11 * A.a22 - A.a21 * A.a12);
    B.a10 = -(A.a10 * A.a22 - A.a20 * A.a12);
    B.a20 =  (A.a10 * A.a21 - A.a20 * A.a11);
    B.a01 = -(A.a01 * A.a22 - A.a21 * A.a02);
    B.a11 =  (A.a00 * A.a22 - A.a20 * A.a02);
    B.a21 = -(A.a00 * A.a21 - A.a20 * A.a01);
    B.a02 =  (A.a01 * A.a12 - A.a11 * A.a02);
    B.a12 = -(A.a00 * A.a12 - A.a10 * A.a02);
    B.a22 =  (A.a00 * A.a11 - A.a10 * A.a01);

    det = (A.a00 * B.a00) + (A.a01 * B.a10) + (A.a02 * B.a20);
    
    oodet = nv_one / det;

    B.a00 *= oodet; B.a01 *= oodet; B.a02 *= oodet;
    B.a10 *= oodet; B.a11 *= oodet; B.a12 *= oodet;
    B.a20 *= oodet; B.a21 *= oodet; B.a22 *= oodet;
    return B;
}

vec3 & normalize(vec3& u)
{
    nv_scalar norm = sqrtf(u.x * u.x + u.y * u.y + u.z * u.z);
    if (norm > nv_eps)
        norm = nv_one / norm;
    else
        norm = nv_zero;
    return scale(u,norm); 
}

vec4 & normalize(vec4& u)
{
    nv_scalar norm = sqrtf(u.x * u.x + u.y * u.y + u.z * u.z + u.w * u.w);
    if (norm > nv_eps)
        norm = nv_one / norm;
    else
        norm = nv_zero;
    return scale(u,norm); 
}

quat & normalize(quat & p)
{
    nv_scalar norm = sqrtf(p.x * p.x + p.y * p.y + p.z * p.z + p.w * p.w);
    if (norm > nv_eps)
        norm = nv_one / norm;
    else
        norm = nv_zero;
    p.x *= norm;
    p.y *= norm;
    p.z *= norm;
    p.w *= norm;
    return p; 
}

mat4 & look_at(mat4& M, const vec3& eye, const vec3& center, const vec3& up)
{
    vec3 x, y, z;

    // make rotation matrix

    // Z vector
    z.x = eye.x - center.x;
    z.y = eye.y - center.y;
    z.z = eye.z - center.z;
    normalize(z);

    // Y vector
    y.x = up.x;
    y.y = up.y;
    y.z = up.z;

    // X vector = Y cross Z
    cross(x,y,z);

    // Recompute Y = Z cross X
    cross(y,z,x);

    // cross product gives area of parallelogram, which is < 1.0 for
    // non-perpendicular unit-length vectors; so normalize x, y here
    normalize(x);
    normalize(y);

    M.a00 = x.x; M.a01 = x.y; M.a02 = x.z; M.a03 = -x.x * eye.x - x.y * eye.y - x.z*eye.z;
    M.a10 = y.x; M.a11 = y.y; M.a12 = y.z; M.a13 = -y.x * eye.x - y.y * eye.y - y.z*eye.z;
    M.a20 = z.x; M.a21 = z.y; M.a22 = z.z; M.a23 = -z.x * eye.x - z.y * eye.y - z.z*eye.z;
    M.a30 = nv_zero; M.a31 = nv_zero; M.a32 = nv_zero; M.a33 = nv_one;
    return M;
}

mat4 & frustum(mat4& M, const nv_scalar l, const nv_scalar r, const nv_scalar b, 
               const nv_scalar t, const nv_scalar n, const nv_scalar f)
{
    M.a00 = (nv_two*n) / (r-l);
    M.a10 = 0.0;
    M.a20 = 0.0;
    M.a30 = 0.0;

    M.a01 = 0.0;
    M.a11 = (nv_two*n) / (t-b);
    M.a21 = 0.0;
    M.a31 = 0.0;

    M.a02 = (r+l) / (r-l);
    M.a12 = (t+b) / (t-b);
    M.a22 = -(f+n) / (f-n);
    M.a32 = -nv_one;

    M.a03 = 0.0;
    M.a13 = 0.0;
    M.a23 = -(nv_two*f*n) / (f-n);
    M.a33 = 0.0;
    return M;
}

mat4 & perspective(mat4& M, const nv_scalar fovy, const nv_scalar aspect, const nv_scalar n, const nv_scalar f)
{
    nv_scalar xmin, xmax, ymin, ymax;

    ymax = n * tanf(fovy * nv_to_rad * nv_zero_5);
    ymin = -ymax;

    xmin = ymin * aspect;
    xmax = ymax * aspect;

    return frustum(M, xmin, xmax, ymin, ymax, n, f);
}

const quat quat::Identity(0, 0, 0, 1);

quat::quat(nv_scalar x, nv_scalar y, nv_scalar z, nv_scalar w) : x(x), y(y), z(z), w(w)
{
}

quat::quat(const quat& quat)
{
	x = quat.x;
	y = quat.y;
	z = quat.z;
	w = quat.w;
}

quat::quat(const vec3& axis, nv_scalar angle)
{
	nv_scalar len = axis.norm();
	if (len) {
		nv_scalar invLen = 1 / len;
		nv_scalar angle2 = angle / 2;
		nv_scalar scale = sinf(angle2) * invLen;
		x = scale * axis[0];
		y = scale * axis[1];
		z = scale * axis[2];
		w = cosf(angle2);
	}
}

quat::quat(const mat3& rot)
{
	FromMatrix(rot);
}

quat& quat::operator=(const quat& quat)
{
	x = quat.x;
	y = quat.y;
	z = quat.z;
	w = quat.w;
	return *this;
}

quat quat::Inverse()
{
	return quat(- x, - y, - z, w);
}

void quat::Normalize()
{
	nv_scalar len = sqrtf(x * x + y * y + z * z + w * w);
	if (len > 0) {
		nv_scalar invLen = 1 / len;
		x *= invLen;
		y *= invLen;
		z *= invLen;
		w *= invLen;
	}
}

void quat::FromMatrix(const mat3& mat)
{
	nv_scalar trace = mat(0, 0) + mat(1, 1) + mat(2, 2);
	if (trace > 0) {
		nv_scalar scale = sqrtf(trace + 1.0f);
		w = 0.5f * scale;
		scale = 0.5f / scale;
		x = scale * (mat(2, 1) - mat(1, 2));
		y = scale * (mat(0, 2) - mat(2, 0));
		z = scale * (mat(1, 0) - mat(0, 1));
	}
	else {
		static int next[] = { 1, 2, 0 };
		int i = 0;
		if (mat(1, 1) > mat(0, 0))
			i = 1;
		if (mat(2, 2) > mat(i, i))
			i = 2;
		int j = next[i];
		int k = next[j];
		nv_scalar scale = sqrtf(mat(i, i) - mat(j, j) - mat(k, k) + 1);
		nv_scalar* q[] = { &x, &y, &z };
		*q[i] = 0.5f * scale;
		scale = 0.5f / scale;
		w = scale * (mat(k, j) - mat(j, k));
		*q[j] = scale * (mat(j, i) + mat(i, j));
		*q[k] = scale * (mat(k, i) + mat(i, k));
	}
}

void quat::ToMatrix(mat3& mat) const
{
	nv_scalar x2 = x * 2;
	nv_scalar y2 = y * 2;
	nv_scalar z2 = z * 2;
	nv_scalar wx = x2 * w;
	nv_scalar wy = y2 * w;
	nv_scalar wz = z2 * w;
	nv_scalar xx = x2 * x;
	nv_scalar xy = y2 * x;
	nv_scalar xz = z2 * x;
	nv_scalar yy = y2 * y;
	nv_scalar yz = z2 * y;
	nv_scalar zz = z2 * z;
	mat(0, 0) = 1 - (yy + zz);
	mat(0, 1) = xy - wz;
	mat(0, 2) = xz + wy;
	mat(1, 0) = xy + wz;
	mat(1, 1) = 1 - (xx + zz);
	mat(1, 2) = yz - wx;
	mat(2, 0) = xz - wy;
	mat(2, 1) = yz + wx;
	mat(2, 2) = 1 - (xx + yy);
}

const quat operator*(const quat& p, const quat& q)
{
	return quat(
		p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y,
		p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z,
		p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x,
		p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z
	);
}

quat& quat::operator*=(const quat& quat)
{
	*this = *this * quat;
	return *this;
}

mat3 & quat_2_mat(mat3& M, const quat& q)
{
	q.ToMatrix(M);
    return M;
}

quat & mat_2_quat(quat& q, const mat3& M)
{
	q.FromMatrix(M);
    return q;
} 

quat & mat_2_quat(quat& q, const mat4& M)
{
	mat3 m;
	M.get_rot(m);
	q.FromMatrix(m);
    return q;
} 

/*
    Given an axis and angle, compute quaternion.
 */
quat & axis_to_quat(quat& q, const vec3& a, const nv_scalar phi)
{
    vec3 tmp(a.x, a.y, a.z);

    normalize(tmp);
	nv_scalar s = sinf(phi/nv_two);
    q.x = s * tmp.x;
    q.y = s * tmp.y;
    q.z = s * tmp.z;
    q.w = cosf(phi/nv_two);
    return q;
}

quat & conj(quat & p)
{
    p.x = -p.x;
    p.y = -p.y;
    p.z = -p.z;
    return p;
}

 quat & conj(quat& p, const quat& q)
{
    p.x = -q.x;
    p.y = -q.y;
    p.z = -q.z;
    p.w = q.w;
    return p;
}

 quat & add_quats(quat& p, const quat& q1, const quat& q2)
{
    quat t1, t2;

    t1 = q1;
    t1.x *= q2.w;
    t1.y *= q2.w;
    t1.z *= q2.w;

    t2 = q2;
    t2.x *= q1.w;
    t2.y *= q1.w;
    t2.z *= q1.w;

    p.x = (q2.y * q1.z) - (q2.z * q1.y) + t1.x + t2.x;
    p.y = (q2.z * q1.x) - (q2.x * q1.z) + t1.y + t2.y;
    p.z = (q2.x * q1.y) - (q2.y * q1.x) + t1.z + t2.z;
    p.w = q1.w * q2.w - (q1.x * q2.x + q1.y * q2.y + q1.z * q2.z);

    return p;
}

nv_scalar & dot(nv_scalar& s, const quat& q1, const quat& q2)
{
    s = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
    return s;
}

nv_scalar dot(const quat& q1, const quat& q2)
{
    return q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
}

#ifndef acosf
#define acosf acos
#endif

quat & slerp_quats(quat & p, nv_scalar s, const quat & q1, const quat & q2)
{
    nv_scalar cosine = dot(q1, q2);
	if (cosine < -1)
		cosine = -1;
	else if (cosine > 1)
		cosine = 1;
    nv_scalar angle = acosf(cosine);
    if (fabs(angle) < nv_eps) {
		p = q1;
        return p;
	}
    nv_scalar sine = sinf(angle);
    nv_scalar sineInv = 1.0f / sine;
    nv_scalar c1 = sinf((1.0f - s) * angle) * sineInv;
    nv_scalar c2 = sinf(s * angle) * sineInv;
	p.x = c1 * q1.x + c2 * q2.x;
	p.y = c1 * q1.y + c2 * q2.y;
	p.z = c1 * q1.z + c2 * q2.z;
	p.w = c1 * q1.w + c2 * q2.w;
    return p;
}

const int HALF_RAND = (RAND_MAX / 2);

 nv_scalar nv_random()
{
	return ((nv_scalar)(rand() - HALF_RAND) / (nv_scalar)HALF_RAND);
}

// v is normalized
// theta in radians
void mat3::set_rot(const nv_scalar& theta, const vec3& v) 
{
    nv_scalar ct = nv_scalar(cos(theta));
    nv_scalar st = nv_scalar(sin(theta));

    nv_scalar xx = v.x * v.x;
    nv_scalar yy = v.y * v.y;
    nv_scalar zz = v.z * v.z;
    nv_scalar xy = v.x * v.y;
    nv_scalar xz = v.x * v.z;
    nv_scalar yz = v.y * v.z;

    a00 = xx + ct*(1-xx);
    a01 = xy + ct*(-xy) + st*-v.z;
    a02 = xz + ct*(-xz) + st*v.y;

    a10 = xy + ct*(-xy) + st*v.z;
    a11 = yy + ct*(1-yy);
    a12 = yz + ct*(-yz) + st*-v.x;

    a20 = xz + ct*(-xz) + st*-v.y;
    a21 = yz + ct*(-yz) + st*v.x;
    a22 = zz + ct*(1-zz);
}

void mat3::set_rot(const vec3& u, const vec3& v)
{
    nv_scalar phi;
    nv_scalar h;
    nv_scalar lambda;
    vec3 w;

    cross(w,u,v);
    dot(phi,u,v);
    dot(lambda,w,w);
    if (lambda > nv_eps)
        h = (nv_one - phi) / lambda;
    else
        h = lambda;
    
    nv_scalar hxy = w.x * w.y * h;
    nv_scalar hxz = w.x * w.z * h;
    nv_scalar hyz = w.y * w.z * h;

    a00 = phi + w.x * w.x * h;
    a01 = hxy - w.z;
    a02 = hxz + w.y;

    a10 = hxy + w.z;
    a11 = phi + w.y * w.y * h;
    a12 = hyz - w.x;

    a20 = hxz - w.y;
    a21 = hyz + w.x;
    a22 = phi + w.z * w.z * h;
}

void mat4::set_rot(const quat& q)
{
	mat3 m;
	q.ToMatrix(m);
	set_rot(m);
}

// v is normalized
// theta in radians
void mat4::set_rot(const nv_scalar& theta, const vec3& v) 
{
    nv_scalar ct = nv_scalar(cos(theta));
    nv_scalar st = nv_scalar(sin(theta));

    nv_scalar xx = v.x * v.x;
    nv_scalar yy = v.y * v.y;
    nv_scalar zz = v.z * v.z;
    nv_scalar xy = v.x * v.y;
    nv_scalar xz = v.x * v.z;
    nv_scalar yz = v.y * v.z;

    a00 = xx + ct*(1-xx);
    a01 = xy + ct*(-xy) + st*-v.z;
    a02 = xz + ct*(-xz) + st*v.y;

    a10 = xy + ct*(-xy) + st*v.z;
    a11 = yy + ct*(1-yy);
    a12 = yz + ct*(-yz) + st*-v.x;

    a20 = xz + ct*(-xz) + st*-v.y;
    a21 = yz + ct*(-yz) + st*v.x;
    a22 = zz + ct*(1-zz);
}

void mat4::set_rot(const vec3& u, const vec3& v)
{
    nv_scalar phi;
    nv_scalar h;
    nv_scalar lambda;
    vec3 w;

    cross(w,u,v);
    dot(phi,u,v);
    dot(lambda,w,w);
    if (lambda > nv_eps)
        h = (nv_one - phi) / lambda;
    else
        h = lambda;
    
    nv_scalar hxy = w.x * w.y * h;
    nv_scalar hxz = w.x * w.z * h;
    nv_scalar hyz = w.y * w.z * h;

    a00 = phi + w.x * w.x * h;
    a01 = hxy - w.z;
    a02 = hxz + w.y;

    a10 = hxy + w.z;
    a11 = phi + w.y * w.y * h;
    a12 = hyz - w.x;

    a20 = hxz - w.y;
    a21 = hyz + w.x;
    a22 = phi + w.z * w.z * h;
}

void mat4::set_rot(const mat3& M)
{
    // copy the 3x3 rotation block
    a00 = M.a00; a10 = M.a10; a20 = M.a20;
    a01 = M.a01; a11 = M.a11; a21 = M.a21;
    a02 = M.a02; a12 = M.a12; a22 = M.a22;
}

void mat4::set_translation(const vec3& t)
{
    a03 = t.x;
    a13 = t.y;
    a23 = t.z;
}

vec3 & mat4::get_translation(vec3& t) const
{
    t.x = a03;
    t.y = a13;
    t.z = a23;
    return t;
}

mat3 & mat4::get_rot(mat3& M) const
{
    // assign the 3x3 rotation block
    M.a00 = a00; M.a10 = a10; M.a20 = a20;
    M.a01 = a01; M.a11 = a11; M.a21 = a21;
    M.a02 = a02; M.a12 = a12; M.a22 = a22;
    return M;
}

quat & mat4::get_rot(quat& q) const
{
	mat3 m;
	get_rot(m);
	q.FromMatrix(m);
    return q;
}

mat3& tangent_basis(mat3& basis, const vec3& v0, const vec3& v1, const vec3& v2, const vec2& t0, const vec2& t1, const vec2& t2, const vec3 & n)
{
    vec3 cp;
    vec3 e0(v1.x - v0.x, t1.s - t0.s, t1.t - t0.t);
    vec3 e1(v2.x - v0.x, t2.s - t0.s, t2.t - t0.t);

    cross(cp,e0,e1);
    if ( fabs(cp.x) > nv_eps)
    {
        basis.a00 = -cp.y / cp.x;        
        basis.a10 = -cp.z / cp.x;
    }

    e0.x = v1.y - v0.y;
    e1.x = v2.y - v0.y;

    cross(cp,e0,e1);
    if ( fabs(cp.x) > nv_eps)
    {
        basis.a01 = -cp.y / cp.x;        
        basis.a11 = -cp.z / cp.x;
    }

    e0.x = v1.z - v0.z;
    e1.x = v2.z - v0.z;

    cross(cp,e0,e1);
    if ( fabs(cp.x) > nv_eps)
    {
        basis.a02 = -cp.y / cp.x;        
        basis.a12 = -cp.z / cp.x;
    }

    // tangent...
    nv_scalar oonorm = nv_one / sqrtf(basis.a00 * basis.a00 + basis.a01 * basis.a01 + basis.a02 * basis.a02);
    basis.a00 *= oonorm;
    basis.a01 *= oonorm;
    basis.a02 *= oonorm;

    // binormal...
    oonorm = nv_one / sqrtf(basis.a10 * basis.a10 + basis.a11 * basis.a11 + basis.a12 * basis.a12);
    basis.a10 *= oonorm;
    basis.a11 *= oonorm;
    basis.a12 *= oonorm;

    // normal...
    // compute the cross product TxB
    basis.a20 = basis.a01*basis.a12 - basis.a02*basis.a11;
    basis.a21 = basis.a02*basis.a10 - basis.a00*basis.a12;
    basis.a22 = basis.a00*basis.a11 - basis.a01*basis.a10;

    oonorm = nv_one / sqrtf(basis.a20 * basis.a20 + basis.a21 * basis.a21 + basis.a22 * basis.a22);
    basis.a20 *= oonorm;
    basis.a21 *= oonorm;
    basis.a22 *= oonorm;

    // Gram-Schmidt orthogonalization process for B
    // compute the cross product B=NxT to obtain 
    // an orthogonal basis
    basis.a10 = basis.a21*basis.a02 - basis.a22*basis.a01;
    basis.a11 = basis.a22*basis.a00 - basis.a20*basis.a02;
    basis.a12 = basis.a20*basis.a01 - basis.a21*basis.a00;

    if (basis.a20 * n.x + basis.a21 * n.y + basis.a22 * n.z < nv_zero)
    {
        basis.a20 = -basis.a20;
        basis.a21 = -basis.a21;
        basis.a22 = -basis.a22;
    }
    return basis;
}

/*
 * Project an x,y pair onto a sphere of radius r OR a hyperbolic sheet
 * if we are away from the center of the sphere.
 */
 nv_scalar tb_project_to_sphere(nv_scalar r, nv_scalar x, nv_scalar y)
{
    nv_scalar d, t, z;

    d = sqrtf(x*x + y*y);
    if (d < r * 0.70710678118654752440) {    /* Inside sphere */
        z = sqrtf(r*r - d*d);
    } else {           /* On hyperbola */
        t = r / (nv_scalar)1.41421356237309504880;
        z = t*t / d;
    }
    return z;
}

/*
 * Ok, simulate a track-ball.  Project the points onto the virtual
 * trackball, then figure out the axis of rotation, which is the cross
 * product of P1 P2 and O P1 (O is the center of the ball, 0,0,0)
 * Note:  This is a deformed trackball-- is a trackball in the center,
 * but is deformed into a hyperbolic sheet of rotation away from the
 * center.  This particular function was chosen after trying out
 * several variations.
 *
 * It is assumed that the arguments to this routine are in the range
 * (-1.0 ... 1.0)
 */
quat & trackball(quat& q, vec2& pt1, vec2& pt2, nv_scalar trackballsize)
{
    vec3 a; // Axis of rotation
    float phi;  // how much to rotate about axis
    vec3 d;
    float t;

    if (pt1.x == pt2.x && pt1.y == pt2.y) 
    {
        // Zero rotation
        q = quat_id;
        return q;
    }

    // First, figure out z-coordinates for projection of P1 and P2 to
    // deformed sphere
    vec3 p1(pt1.x,pt1.y,tb_project_to_sphere(trackballsize,pt1.x,pt1.y));
    vec3 p2(pt2.x,pt2.y,tb_project_to_sphere(trackballsize,pt2.x,pt2.y));

    //  Now, we want the cross product of P1 and P2
    cross(a,p1,p2);

    //  Figure out how much to rotate around that axis.
    d.x = p1.x - p2.x;
    d.y = p1.y - p2.y;
    d.z = p1.z - p2.z;
    t = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z) / (trackballsize);

    // Avoid problems with out-of-control values...

    if (t > nv_one)
        t = nv_one;
    if (t < -nv_one) 
        t = -nv_one;
    phi = nv_two * nv_scalar(asin(t));
    axis_to_quat(q,a,phi);
    return q;
}

vec3& cube_map_normal(int i, int x, int y, int cubesize, vec3& v)
{
    nv_scalar s, t, sc, tc;
    s = (nv_scalar(x) + nv_zero_5) / nv_scalar(cubesize);
    t = (nv_scalar(y) + nv_zero_5) / nv_scalar(cubesize);
    sc = s * nv_two - nv_one;
    tc = t * nv_two - nv_one;

    switch (i) 
    {
        case 0:
            v.x = nv_one;
            v.y = -tc;
            v.z = -sc;
            break;
        case 1:
            v.x = -nv_one;
            v.y = -tc;
            v.z = sc;
            break;
        case 2:
            v.x = sc;
            v.y = nv_one;
            v.z = tc;
            break;
        case 3:
            v.x = sc;
            v.y = -nv_one;
            v.z = -tc;
            break;
        case 4:
            v.x = sc;
            v.y = -tc;
            v.z = nv_one;
            break;
        case 5:
            v.x = -sc;
            v.y = -tc;
            v.z = -nv_one;
            break;
    }
    normalize(v);
    return v;
}

// computes the area of a triangle
nv_scalar nv_area(const vec3& v1, const vec3& v2, const vec3& v3)
{
    vec3 cp_sum;
    vec3 cp;
    cross(cp_sum, v1, v2);
    cp_sum += cross(cp, v2, v3);
    cp_sum += cross(cp, v3, v1);
    return nv_norm(cp_sum) * nv_zero_5; 
}

// computes the perimeter of a triangle
nv_scalar nv_perimeter(const vec3& v1, const vec3& v2, const vec3& v3)
{
    nv_scalar perim;
    vec3 diff;
    sub(diff, v1, v2);
    perim = nv_norm(diff);
    sub(diff, v2, v3);
    perim += nv_norm(diff);
    sub(diff, v3, v1);
    perim += nv_norm(diff);
    return perim;
}

// compute the center and radius of the inscribed circle defined by the three vertices
nv_scalar nv_find_in_circle(vec3& center, const vec3& v1, const vec3& v2, const vec3& v3)
{
    nv_scalar area = nv_area(v1, v2, v3);
    // if the area is null
    if (area < nv_eps)
    {
        center = v1;
        return nv_zero;
    }

    nv_scalar oo_perim = nv_one / nv_perimeter(v1, v2, v3);

    vec3 diff;

    sub(diff, v2, v3);
    mult(center, v1, nv_norm(diff));

    sub(diff, v3, v1);
    madd(center, v2, nv_norm(diff));
    
    sub(diff, v1, v2);
    madd(center, v3, nv_norm(diff));

    center *= oo_perim;

    return nv_two * area * oo_perim;
}

// compute the center and radius of the circumscribed circle defined by the three vertices
// i.e. the osculating circle of the three vertices
nv_scalar nv_find_circ_circle( vec3& center, const vec3& v1, const vec3& v2, const vec3& v3)
{
    vec3 e0;
    vec3 e1;
    nv_scalar d1, d2, d3;
    nv_scalar c1, c2, c3, oo_c;

    sub(e0, v3, v1);
    sub(e1, v2, v1);
    dot(d1, e0, e1);

    sub(e0, v3, v2);
    sub(e1, v1, v2);
    dot(d2, e0, e1);

    sub(e0, v1, v3);
    sub(e1, v2, v3);
    dot(d3, e0, e1);

    c1 = d2 * d3;
    c2 = d3 * d1;
    c3 = d1 * d2;
    oo_c = nv_one / (c1 + c2 + c3);

    mult(center,v1,c2 + c3);
    madd(center,v2,c3 + c1);
    madd(center,v3,c1 + c2);
    center *= oo_c * nv_zero_5;
 
    return nv_zero_5 * sqrtf((d1 + d2) * (d2 + d3) * (d3 + d1) * oo_c);
}

 nv_scalar ffast_cos(const nv_scalar x)
{
    // assert:  0 <= fT <= PI/2
    // maximum absolute error = 1.1880e-03
    // speedup = 2.14

    nv_scalar x_sqr = x*x;
    nv_scalar res = nv_scalar(3.705e-02);
    res *= x_sqr;
    res -= nv_scalar(4.967e-01);
    res *= x_sqr;
    res += nv_one;
    return res;
}


 nv_scalar fast_cos(const nv_scalar x)
{
    // assert:  0 <= fT <= PI/2
    // maximum absolute error = 2.3082e-09
    // speedup = 1.47

    nv_scalar x_sqr = x*x;
    nv_scalar res = nv_scalar(-2.605e-07);
    res *= x_sqr;
    res += nv_scalar(2.47609e-05);
    res *= x_sqr;
    res -= nv_scalar(1.3888397e-03);
    res *= x_sqr;
    res += nv_scalar(4.16666418e-02);
    res *= x_sqr;
    res -= nv_scalar(4.999999963e-01);
    res *= x_sqr;
    res += nv_one;
    return res;
}

void nv_is_valid(const vec3& v)
{
#if CVLIB_OS==CVLIB_OS_APPLE
        assert(!isnan(v.x) && !isnan(v.y) && !isnan(v.z) );
#else
    assert(!_isnan(v.x) && !_isnan(v.y) && !_isnan(v.z) &&
        _finite(v.x) && _finite(v.y) && _finite(v.z));
#endif
}

void nv_is_valid(nv_scalar lambda)
{
#if CVLIB_OS==CVLIB_OS_APPLE
        assert(!isnan(lambda));
#else
        assert(!_isnan(lambda) && _finite(lambda));
#endif
}

////////////////////////////////////////////////////////////
// Really Ugly Add to got MatQ and MatR from a very old sdk....
//***************************************************************

void quat::Slerp(const quat & from, const quat & to, nv_scalar t)
{
	quat to1;
	nv_scalar omega, cosom, sinom, scale0, scale1;

	// calc cosine
	cosom = from[0] * to[0] + from[1] * to[1] + from[2] * to[2] + from[3] * to[3];

	// adjust signs (if necessary)
	if(cosom < 0.0)
	{
		cosom = -cosom;
		to1[0] = - to[0];
		to1[1] = - to[1];
		to1[2] = - to[2];
		to1[3] = - to[3];
	}
	else
	{
		to1[0] = to[0];
		to1[1] = to[1];
		to1[2] = to[2];
		to1[3] = to[3];
	}

	// calculate coefficients
	if((1.0 - cosom) > 1e-12)
	{
		// standard case (slerp)
		omega = acos(cosom);
		sinom = sin(omega);
		scale0 = (nv_scalar)sin((1.0 - t) * omega) / sinom;
		scale1 = (nv_scalar)sin(t * omega) / sinom;
	}
	else
	{        
		// "from" and "to" quaternions are very close 
		//  ... so we can do a linear interpolation
		scale0 = (nv_scalar)1.0 - t;
		scale1 = t;
	}

	// calculate final values
	x = scale0 * from[0] + scale1 * to1[0];
	y = scale0 * from[1] + scale1 * to1[1];
	z = scale0 * from[2] + scale1 * to1[2];
	w = scale0 * from[3] + scale1 * to1[3];
}



mat4 & get_col_matrix(quat &q, mat4 &destMatrix)
{

	nv_scalar u, _xs, _ys, _zs, _wx, _wy, _wz, _xx, _xy, _xz, _yy, _yz, _zz;
	u = (nv_scalar)2.0 /  (q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	
	_xs = q.x * u;
	_ys = q.y * u;
	_zs = q.z * u;
	_wx = q.w * _xs;
	_wy = q.w * _ys;
	_wz = q.w * _zs;
	_xx = q.x * _xs;
	_xy = q.x * _ys;
	_xz = q.x * _zs;
	_yy = q.y * _ys;
	_yz = q.y * _zs;
	_zz = q.z * _zs;
	
	/* set up 4x4 destMatrixrix */
	destMatrix(0, 0) = nv_scalar(1.0) - (_yy + _zz);
	destMatrix(1, 0) = _xy - _wz;
	destMatrix(2, 0) = _xz + _wy;
	destMatrix(3, 0) = 0.0;
	
	destMatrix(0, 1) = _xy + _wz;
	destMatrix(1, 1) = nv_scalar(1.0) - (_xx + _zz);
	destMatrix(2, 1) = _yz - _wx;
	destMatrix(3, 1) = 0.0;
	
	destMatrix(0, 2) = _xz - _wy;
	destMatrix(1, 2) = _yz + _wx;
	destMatrix(2, 2) = nv_scalar(1.0) - (_xx + _yy);
	destMatrix(3, 2) = 0.0;
	
	destMatrix(0, 3) = 0.0;
	destMatrix(1, 3) = 0.0;
	destMatrix(2, 3) = 0.0;
	destMatrix(3, 3) = 1.0;
	return destMatrix;
	
}


/*****************************************************************************
 *  
    SetQuatFromEulerDeg - converts 3 euler angles (in radians) to a quaternion
     
        angles are in degrees;  Assumes roll is rotation about X, pitch
        is rotation about Y, yaw is about Z.  Assumes order of 
        yaw, pitch, roll applied as follows:
            
            p' = roll( pitch( yaw(p) ) )

******************************************************************************/


quat &set_quat_from_euler_deg(quat &q, nv_scalar yaw, nv_scalar pitch, nv_scalar roll)
{

  nv_scalar cosYaw, sinYaw, cosPitch, sinPitch, cosRoll, sinRoll;
  nv_scalar half_roll, half_pitch, half_yaw;

  /* put angles into radians and divide by two, since all angles in formula
     *  are (angle/2)
   */

  half_yaw = yaw / nv_scalar(2.0);
  half_pitch = pitch / nv_scalar(2.0);
  half_roll = roll / nv_scalar(2.0);

//  static const double deg_to_rad = M_PI / 180.0;
  
  cosYaw = fast_cos(half_yaw * nv_to_rad);
  sinYaw = sin(half_yaw * nv_to_rad);

  cosPitch = fast_cos(half_pitch * nv_to_rad);
  sinPitch = sin(half_pitch * nv_to_rad);

  cosRoll = fast_cos(half_roll * nv_to_rad);
  sinRoll = sin(half_roll * nv_to_rad);

  q.x = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
  q.y = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
  q.z = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;

  q.w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
  
  return q;

}

void AngleAxis(const quat &q, nv_scalar angleaxis[4] )
{
	float scale, tw;
	tw = (float)acos(q.w) * 2.f;
	scale = (float)sin(tw * 0.5);
	if(scale == 0.0f){
    	angleaxis[0] = angleaxis[1] = angleaxis[2] = angleaxis[3] = 0;
		return;
	}
	angleaxis[0] = q.x / scale;
	angleaxis[1] = q.y / scale;
	angleaxis[2] = q.z / scale;
	angleaxis[3] = tw * nv_to_deg; //(tw * 180.f) / (float)M_PI;
}


void xform(quat & q, vec3 &v)
{
  vec3 uv, uuv;

  // uv.cross(vec3, v);
  uv = vec3(q.x,q.y,q.z) ^ v;
  // uuv.cross(vec3, uv);
  uuv = vec3(q.x,q.y,q.z) ^ uv;
  scale(uv, 2.0f * q.w);
  scale(uuv, 2.0f);
  v += uv;
  v += uuv;
}

void xform(const quat q, const vec3 &v, vec3 &xv)
{
  vec3 uv, uuv;
  // uv.cross(vec3, v);
  uv = vec3(q.x,q.y,q.z) ^ v;
  // uuv.cross(vec3, uv);
  uuv = vec3(q.x,q.y,q.z) ^ uv;


  
  scale(uv, 2.0f * q.w);
  scale(uuv, 2.0f);
  xv = v + uv;
  xv += uuv;
}


const vec3 operator* (const MatQ& m, const vec3& v)
{
	vec3 ret;
	// m.xformVect(v, ret);
	xform(m.q, v, ret);
	ret += m.d;
	return ret;
	// M * v
}


void mat4::set( const nv_scalar * mat )
{
    // XX = mat[0];
    // XY = mat[1];
    // XZ = mat[2];
    // XW = mat[3];
    // 
    // 
    // YX = mat[4];
    // YY = mat[5];
    // YZ = mat[6];
    // YW = mat[7];
    // 
    // 
    // ZX = mat[8];
    // ZY = mat[9];
    // ZZ = mat[10];
    // ZW = mat[11];
    // 
    // 
    // WX = mat[12];
    // WY = mat[13];
    // WZ = mat[14];
    // WW = mat[15];

	a00 = mat[0];
    a10 = mat[1];
    a20 = mat[2];
    a30 = mat[3];
    
    
    a01 = mat[4];
    a11 = mat[5];
    a21 = mat[6];
    a31 = mat[7];
    
    
    a02 = mat[8];
    a12 = mat[9];
    a22 = mat[10];
    a32 = mat[11];
    
    
    a03 = mat[12];
    a13 = mat[13];
    a23 = mat[14];
    a33 = mat[15];
}


void mat4::set( nv_scalar a0,  nv_scalar b0,  nv_scalar c0, nv_scalar d0,
               nv_scalar a1,  nv_scalar b1,  nv_scalar c1, nv_scalar d1,
               nv_scalar a2,  nv_scalar b2,  nv_scalar c2, nv_scalar d2,
               nv_scalar a3,  nv_scalar b3,  nv_scalar c3, nv_scalar d3)
{
    a00 = a0;
    a10 = b0;
    a20 = c0;
    a30 = d0;
    
    
    a01 = a1;
    a11 = b1;
    a21 = c1;
    a31 = d1;
    
    
    a02 = a2;
    a12 = b2;
    a22 = c2;
    a32 = d2;
    
    
    a03 = a3;
    a13 = b3;
    a23 = c3;
    a33 = d3;
}

void mat4::SetAxisRotation(const vec3 & axis, nv_scalar theta)
{
  vec3 k = axis;
  k.normalize();
  nv_scalar cost = cos(theta);
  nv_scalar sint = sin(theta);
  nv_scalar ctval = 1.0f - cost;
  set(k.x*k.x*ctval + cost,	  k.x*k.y*ctval - k.z*sint, k.x*k.z*ctval + k.y*sint, 0,
    k.x*k.y*ctval + k.z*sint,	k.y*k.y*ctval + cost,	    k.y*k.z*cost  - k.x*sint,	0,					
    k.x*k.z*ctval - k.y*sint, k.y*k.z*ctval + k.x*sint, k.z*k.z*ctval  + cost,    0,
    0,                                         0,               0,                1.0);
 
}


void quat::set(const mat3 &R)
{
  nv_scalar qs2, qx2, qy2, qz2;  // squared magniudes of quaternion components
  nv_scalar tmp;
  int n;

  // first compute squared magnitudes of quaternion components - at least one
  // will be greater than 0 since quaternion is unit magnitude

  qs2 = (float)(0.25 * (R.a00 + R.a11 + R.a22 + 1));
  qx2 = (float)(qs2 -   0.5 * (R.a11 + R.a22));
  qy2 = (float)(qs2 -   0.5 * (R.a22 + R.a00));
  qz2 = (float)(qs2 -   0.5 * (R.a00 + R.a11));

  
  // find maximum magnitude component
  n = (qs2 > qx2 ) ?
    ((qs2 > qy2) ? ((qs2 > qz2) ? 0 : 3) : ((qy2 > qz2) ? 2 : 3)) :
    ((qx2 > qy2) ? ((qx2 > qz2) ? 1 : 3) : ((qy2 > qz2) ? 2 : 3));

  // compute signed quaternion components using numerically stable method
  switch(n) 
  {
  case 0:
    w = sqrtf(qs2);
    tmp = 0.25f / w;
    x = (R.a12 - R.a21) * tmp;
    y = (R.a20 - R.a02) * tmp;
    z = (R.a01 - R.a10) * tmp;
    break;
  case 1:
    x = sqrtf(qx2);
    tmp = 0.25f / x;
    w = (R.a12 - R.a21) * tmp;
    y = (R.a10 + R.a01) * tmp;
    z = (R.a20 + R.a02) * tmp;
    break;
  case 2:
    y = sqrtf(qy2);
    tmp = 0.25f / y;
    w = (R.a20 - R.a02) * tmp;
    z = (R.a21 + R.a12) * tmp;
    x = (R.a01 + R.a10) * tmp;
    break;
  case 3:
    z = sqrtf(qz2);
    tmp = 0.25f / z;
    w = (R.a01 - R.a10) * tmp;
    x = (R.a02 + R.a20) * tmp;
    y = (R.a12 + R.a21) * tmp;
    break;
  }
  // for consistency, force positive scalar component [ (s; v) = (-s; -v) ]
  if (w < 0) 
  {
    w = -w;
    x = -x;
    y = -y;
    z = -z;
  }
  // ulNormalize(i,j,k,s);
  Normalize();

  
}



/******************************************************************************
ulMatR.cpp

Comments:

	3 x 3 Rotation + Translation vector 

******************************************************************************/
  


void ulInvertMatrix(const MatR & a, MatR & b)
{
  // invert the rotation part by transposing it
  // b.R.XX = a.R.XX;
  // b.R.XY = a.R.YX;
  // b.R.XZ = a.R.ZX;
  // b.R.YX = a.R.XY;
  // b.R.YY = a.R.YY;
  // b.R.YZ = a.R.ZY;
  // b.R.ZX = a.R.XZ;
  // b.R.ZY = a.R.YZ;
  // b.R.ZZ = a.R.ZZ;

  b.R.a00 = a.R.a00;
  b.R.a01 = a.R.a10;
  b.R.a02 = a.R.a20;
  b.R.a10 = a.R.a01;
  b.R.a11 = a.R.a11;
  b.R.a12 = a.R.a21;
  b.R.a20 = a.R.a02;
  b.R.a21 = a.R.a12;
  b.R.a22 = a.R.a22;

  
  // new displacement vector given by:  d' = -(R^-1) * d 
  b.d.x = - (a.R.a00 * a.d.x + b.R.a10 * a.d.y + b.R.a20 * a.d.z);
  b.d.y = - (a.R.a01 * a.d.x + b.R.a11 * a.d.y + b.R.a21 * a.d.z);
  b.d.z = - (a.R.a02 * a.d.x + b.R.a12 * a.d.y + b.R.a22 * a.d.z);
}


void MatR::GetInverse(MatR &M) const
{
  ulInvertMatrix(*this, M);
}


void MatR::SetInverse(const MatR &M) 
{
  ulInvertMatrix(M, *this);
}

void MatR::Invert()
{
  nv_scalar tmp, odx, ody;

  // invert the rotation part by transposing it
  tmp  = R.a10;
  R.a10 = R.a01;
  R.a01 = tmp;

  tmp  = R.a21;
  R.a21 = R.a12;
  R.a12 = tmp;

  tmp  = R.a02;
  R.a02 = R.a20;
  R.a20 = tmp;

  // new displacement vector given by:  d' = -(R^T) * d 
  odx = d.x; ody = d.y;
  d.x = - (R.a00 * odx + R.a10 * ody + R.a20 * d.z);
  d.y = - (R.a01 * odx + R.a11 * ody + R.a21 * d.z);
  d.z = - (R.a02 * odx + R.a12 * ody + R.a22 * d.z);
}



// void MatR::set(const MatQ &T)
// {
  // R.set(T.q); 
  // d = T.d;
// }
// 
// 
// void MatR::set(const Quat & q, const vec3 & v)
// {
  // R.set(q); 
  // d = v;
// }



void MatR::xformVect(const vec3 &v, vec3 &xv) const
{
  //R.xform(v, xv);
  xv = R * v;
}

  
void MatR::xformVect(vec3 &v) const
{
  //R.xform(v);
  v = R * v;
}

  // 
// void MatR::xformPoint(const vec3 &p, vec3 &xp) const
// {
  // R.xform(p, xp);
  // //xp.add(d);
  // xp += d;
// }


// void MatR::xformPoint(vec3 &p) const
// {
  // R.xform(p);
  // //p.add(d);
  // p += d;
// }

// 
// void MatR::invXformVect(const vec3 &v, vec3 &xv) const
// {
  // R.invXform(v, xv);
// }

  
// void MatR::invXformVect(vec3 &v) const
// {
  // R.invXform(v);
// }

  
// void MatR::invXformPoint(const vec3 &p, vec3 &xp) const
// {
  // //xp.sub(p, d);
  // xp = p - d;
  // R.invXform(xp);
// }


// void MatR::invXformPoint(vec3 &p) const
// {
  // //p.sub(d);
  // p -= d;
  // R.invXform(p);
// }




void MatR::mult(const MatR &M, const MatR &N)
{
  
  
  
  // multiply rotation matrices
  R.a00 = M.R.a00 * N.R.a00 + M.R.a10 * N.R.a01 + M.R.a20 * N.R.a02;
  R.a10 = M.R.a00 * N.R.a10 + M.R.a10 * N.R.a11 + M.R.a20 * N.R.a12;
  R.a20 = M.R.a00 * N.R.a20 + M.R.a10 * N.R.a21 + M.R.a20 * N.R.a22;
  R.a01 = M.R.a01 * N.R.a00 + M.R.a11 * N.R.a01 + M.R.a21 * N.R.a02;
  R.a11 = M.R.a01 * N.R.a10 + M.R.a11 * N.R.a11 + M.R.a21 * N.R.a12;
  R.a21 = M.R.a01 * N.R.a20 + M.R.a11 * N.R.a21 + M.R.a21 * N.R.a22;
  R.a02 = M.R.a02 * N.R.a00 + M.R.a12 * N.R.a01 + M.R.a22 * N.R.a02;
  R.a12 = M.R.a02 * N.R.a10 + M.R.a12 * N.R.a11 + M.R.a22 * N.R.a12;
  R.a22 = M.R.a02 * N.R.a20 + M.R.a12 * N.R.a21 + M.R.a22 * N.R.a22;

  // d = M.R * N.d + M.d
  d.x = M.R.a00 * N.d.x + M.R.a10 * N.d.y + M.R.a20 * N.d.z + M.d.x;
  d.y = M.R.a01 * N.d.x + M.R.a11 * N.d.y + M.R.a21 * N.d.z + M.d.y;
  d.z = M.R.a02 * N.d.x + M.R.a12 * N.d.y + M.R.a22 * N.d.z + M.d.z;
}

MatR operator * (const MatR& a, const MatR& b) 
{
  MatR c;

  c.mult(a, b);
      
  return c;
}


  
void MatR::mult(const MatR &M, bool post)
{

  nv_scalar oxy, oyz, ozx, oyx, ozy, oxz, odx, ody;

  if (post == false)
  {
    // multiply rotation matrices
    oxy = R.a10; oyx = R.a01; oyz = R.a21; ozy = R.a12; ozx = R.a02; oxz = R.a20;
    
    R.a10 = M.R.a00 * oxy  + M.R.a10 * R.a11 + M.R.a20 * ozy;
    R.a20 = M.R.a00 * oxz  + M.R.a10 * oyz  + M.R.a20 * R.a22;
    R.a01 = M.R.a01 * R.a00 + M.R.a11 * oyx  + M.R.a21 * ozx;
    R.a21 = M.R.a01 * oxz  + M.R.a11 * oyz  + M.R.a21 * R.a22;
    R.a02 = M.R.a02 * R.a00 + M.R.a12 * oyx  + M.R.a22 * ozx;
    R.a12 = M.R.a02 * oxy  + M.R.a12 * R.a11 + M.R.a22 * ozy;
    
    R.a00 = M.R.a00 * R.a00 + M.R.a10 * oyx  + M.R.a20 * ozx;
    R.a11 = M.R.a01 * oxy  + M.R.a11 * R.a11 + M.R.a21 * ozy;
    R.a22 = M.R.a02 * oxz  + M.R.a12 * oyz  + M.R.a22 * R.a22;
    
    // d = M.R * d + M.d
    odx = d.x; ody = d.y;
    d.x = M.R.a00 * odx + M.R.a10 * ody + M.R.a20 * d.z + M.d.x;
    d.y = M.R.a01 * odx + M.R.a11 * ody + M.R.a21 * d.z + M.d.y;
    d.z = M.R.a02 * odx + M.R.a12 * ody + M.R.a22 * d.z + M.d.z;
  }
  else
  {

  //real oxy, oyz, ozx, oyx, ozy, oxz;
    vec3 v;
    
    //d = R * M.d + d
    d.x += R.a00 * M.d.x + R.a10 * M.d.y + R.a20 * M.d.z;
    d.y += R.a01 * M.d.x + R.a11 * M.d.y + R.a21 * M.d.z;
    d.z += R.a02 * M.d.x + R.a12 * M.d.y + R.a22 * M.d.z;
    
    // multiply rotation matrices
    oxy = R.a10; oyx = R.a01; oyz = R.a21; ozy = R.a12; ozx = R.a02; oxz = R.a20;
    R.a10 = R.a00 * M.R.a10 + oxy  * M.R.a11 + oxz  * M.R.a12;
    R.a20 = R.a00 * M.R.a20 + oxy  * M.R.a21 + oxz  * M.R.a22;
    R.a01 = oyx  * M.R.a00 + R.a11 * M.R.a01 + oyz  * M.R.a02;
    R.a21 = oyx  * M.R.a20 + R.a11 * M.R.a21 + oyz  * M.R.a22;
    R.a02 = ozx  * M.R.a00 + ozy  * M.R.a01 + R.a22 * M.R.a02;
    R.a12 = ozx  * M.R.a10 + ozy  * M.R.a11 + R.a22 * M.R.a12;
    
    R.a00 = R.a00 * M.R.a00 + oxy  * M.R.a01 + oxz  * M.R.a02;
    R.a11 = oyx  * M.R.a10 + R.a11 * M.R.a11 + oyz  * M.R.a12;
    R.a22 = ozx  * M.R.a20 + ozy  * M.R.a21 + R.a22 * M.R.a22;
  }
}


// 
// int MatR::CalcRotationMatrixFromNU(const vec3 &vpn, const vec3 &vup)
// {
	// d.set(0,0,0);
	// return R.CalcRotationMatrixFromNU(vpn, vup);
// }
// 
// int MatR::CalcRotationMatrixFromNU(const vec3 & vpn, const vec3 & vup, const vec3 & p)
// {
	// int res = R.CalcRotationMatrixFromNU(vpn, vup);
	// 
	// d[UL_N] = -(R(X, UL_N) * p.x + R(Y, UL_N) * p.y + R(Z, UL_N) * p.z);
	// d[UL_U] = -(R(X, UL_U) * p.x + R(Y, UL_U) * p.y + R(Z, UL_U) * p.z);
	// d[UL_R] = -(R(X, UL_R) * p.x + R(Y, UL_R) * p.y + R(Z, UL_R) * p.z);
	// 
	// return res;
// }



// int MatR::CalcRotationMatrixFromRN(const vec3 &vr, const vec3 &vpn)
// {
	// d.set(0,0,0);
	// return R.CalcRotationMatrixFromRN(vr, vpn);
// }

// int MatR::CalcRotationMatrixFromRN(const vec3 & vr, const vec3 & vpn, const vec3 & p)
// {
	// int res = R.CalcRotationMatrixFromRN(vr, vpn);
	// 
	// d[UL_N] = -(R(X, UL_N) * p.x + R(Y, UL_N) * p.y + R(Z, UL_N) * p.z);
	// d[UL_U] = -(R(X, UL_U) * p.x + R(Y, UL_U) * p.y + R(Z, UL_U) * p.z);
	// d[UL_R] = -(R(X, UL_R) * p.x + R(Y, UL_R) * p.y + R(Z, UL_R) * p.z);
	// 
	// return res;
// }

// int MatR::CalcRotationMatrixFromUR(const vec3 &vup, const vec3 &vr)
// {
	// d.set(0,0,0);
	// return R.CalcRotationMatrixFromUR(vup, vr);
// }

// int MatR::CalcRotationMatrixFromUR(const vec3 &vup, const vec3 &vr, const vec3 & p)
// {
	// int res = R.CalcRotationMatrixFromUR(vup, vr);
	// 
	// d[UL_N] = -(R(X, UL_N) * p.x + R(Y, UL_N) * p.y + R(Z, UL_N) * p.z);
	// d[UL_U] = -(R(X, UL_U) * p.x + R(Y, UL_U) * p.y + R(Z, UL_U) * p.z);
	// d[UL_R] = -(R(X, UL_R) * p.x + R(Y, UL_R) * p.y + R(Z, UL_R) * p.z);
	// 
	// return res;
	// 
// }



// ostream& MatR::print(ostream &os) const
// {
  // return os << R << d << endl;
// }

void MatR::SetTranslate(const vec3 & v)
{
  d = v;
}

void MatR::SetTranslate(nv_scalar x, nv_scalar y, nv_scalar z)
{
  d.x = x;
  d.y = y;
  d.z = z;
}

void MatR::AddTranslate(const vec3 & v)
{
  d += v;
}

void MatR::AddTranslate(nv_scalar x, nv_scalar y, nv_scalar z)
{
  d.x += x;
  d.y += y;
  d.z += z;
}



// nv_scalar MatR::xformAxis(const vec3 &v, int axis) const
// {
  // return R(X, axis) * v.x + R(Y, axis) * v.y + R(Z, axis) * v.z;
// }


// void MatR::SetIdentity()
// {
  // R.SetIdentity();
  // d.set(0,0,0);
// }



// Mat4 MatR::GetMat4()
// {
	// Mat4 m;
	// vec3 vr;
	// vec3 vpn;
	// vec3 vup;
	// 
	// vup = R.GetViewUp();
	// vpn = R.GetViewNormal();
	// vr = R.GetViewRight();
	// 
	// m(X, UL_R) = vr[X];
	// m(Y, UL_R) = vr[Y];
	// m(Z, UL_R) = vr[Z];
	// m(W, UL_R) = -(vr DOT d);
	// 
	// m(X, UL_U) = vup[X];
	// m(Y, UL_U) = vup[Y];
	// m(Z, UL_U) = vup[Z];
	// m(W, UL_U) = -(vup DOT d);
	// 
	// m(X, UL_N) = vpn[X];
	// m(Y, UL_N) = vpn[Y];
	// m(Z, UL_N) = vpn[Z];
	// m(W, UL_N) = -(vpn DOT d);
	// 
	// m(X, W) = 0;
	// m(Y, W) = 0;
	// m(Z, W) = 0;
	// m(W, W) = 1;
	// 
	// return m;
// }




#define XX row[0].x
#define XY row[0].y
#define XZ row[0].z
#define XW row[0].w

#define yx row[1].x
#define yy row[1].y
#define yz row[1].z
#define yw row[1].w

#define zx row[2].x
#define zy row[2].y
#define zz row[2].z
#define zw row[2].w

#define wx row[3].x
#define wy row[3].y
#define wz row[3].z
#define ww row[3].w


/******************************************************************************
ulMatQ.cpp

******************************************************************************/

void MatQ::set(const mat4 &T)
{
	MatR matr;
	mat3 tempmat3;
	T.get_rot(tempmat3);
	transpose(tempmat3);
	vec3 tempvec3;
	T.get_translation(tempvec3);
	matr.SetRotation(tempmat3);
	matr.SetTranslate(tempvec3);
	set(matr);
}  


// void MatQ::GetInverse(MatQ &T) const
// {
  // T.q.s = -q.s;
  // T.q.i =  q.i;
  // T.q.j =  q.j;
  // T.q.k =  q.k;
// 
  // T.q.xform(d, T.d);
  // T.d.negate(T.d);
// }
// 
MatQ MatQ::GetInverse() const
{
	MatQ T;
	T.q.w = -q.w;
	T.q.x =  q.x;
	T.q.y =  q.y;
	T.q.z =  q.z;
	
	// T.q.xform(d, T.d);
	xform(T.q, d, T.d);
	// T.d.negate(T.d);
	T.d = -T.d;
	return T;
}
// 
// void MatQ::Invert()
// {
  // q.s = -q.s;
  // q.xform(d);
  // d.negate();
// }
// 
// 
// 
// void MatQ::mult(const MatQ &T, const MatQ &U)
// {
  // q.mult(T.q, U.q);
  // T.q.xform(U.d, d);
  // d.add(d, T.d);
// }
// 
// 
// void MatQ::mult(const MatQ &T, bool post)
// {
  // if (post == false)
  // {
    // q.mult(T.q);
    // T.q.xform(d);
    // d.add(T.d);
  // }
  // else
  // {
    // vec3 v;
    // 
    // q.xform(T.d, v);
    // d.add(v);
    // q.mult(T.q, true);
  // }
// }
// 
// void MatQ::SetInverse(const MatQ &T)
// {
  // q.s = -T.q.s;
  // q.i =  T.q.i;
  // q.j =  T.q.j;
  // q.k =  T.q.k;
  // q.xform(T.d, d);
  // d.negate(d);
// }
// 





void MatQ::xformVect(const vec3 &v, vec3 &xv) const
{
	xform(q, v, xv);
}
// 
// 
// void MatQ::xformVect(vec3 &v) const
// {
  // q.xform(v);
// }
// 
// 
// void MatQ::xformPoint(const vec3 &p, vec3 &xp) const
// {
  // q.xform(p, xp);
  // xp.add(d);
// }
// 
// 
// void MatQ::xformPoint(vec3 &p) const
// {
  // q.xform(p);
  // p.add(d);
// }
// 
// 
// void MatQ::invXformVect(const vec3 &v, vec3 &xv) const
// {
  // q.invXform(v, xv);
// }
// 
// 
// void MatQ::invXformVect(vec3 &v) const
// {
  // q.invXform(v);
// }
// 
// 
// void MatQ::invXformPoint(const vec3 &p, vec3 &xp) const
// {
  // xp.sub(p, d);
  // q.invXform(xp);
// }
// 
// 
// void MatQ::invXformPoint(vec3 &p) const
// {
  // p.sub(d);
  // q.invXform(p);
// }
// 
// 
// 
// 
// 
// 
// ostream& MatQ::print(ostream &os) const
// {
  // return os << q << d;
// }
// 
// 
   // 

void MatQ::SetIdentity()
{
 	// q.SetIdentity();
	q = quat_id;
	// d.set(0,0,0);
	d = vec3_null;
}

#define XX row[0].x
#define XY row[0].y
#define XZ row[0].z
#define XW row[0].w

#define yx row[1].x
#define yy row[1].y
#define yz row[1].z
#define yw row[1].w

#define zx row[2].x
#define zy row[2].y
#define zz row[2].z
#define zw row[2].w

#define wx row[3].x
#define wy row[3].y
#define wz row[3].z
#define ww row[3].w


// void MatQ::premult(const MatQ &T)
// {
  // q.premult(T.q);
  // T.q.xform(d);
  // d.add(T.d);
// }
// 
// 
// void MatQ::postmult(const MatQ &T)
// {
  // vec3 v;
// 
  // q.xform(T.d, v);
  // d.add(v);
  // q.postmult(T.q);
// }
// 
// 
// 
// void MatQ::invert(const MatQ &T)
// {
  // q.s = -T.q.s;
  // q.i =  T.q.i;
  // q.j =  T.q.j;
  // q.k =  T.q.k;
  // q.xform(T.d, d);
  // d.negate(d);
// }
// Mat4 MatQ::GetMat4()
// { 
  // Mat4 mat;
  // mat.SetIdentity();
  // q.GetColMatrix(mat);
  // mat.Translate(d);
  // return mat;
// }
// 


MatQ operator * (const MatQ& a, const MatQ& b) 
{
	MatQ ret;
	ret.q = a.q * b.q;
	// a.q.xform(b.d, ret.d);
	xform(a.q, b.d, ret.d);
	ret.d += a.d;
	return ret;
}

}