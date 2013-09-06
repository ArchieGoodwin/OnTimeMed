/*!
 * \file    Mat3.h
 * \ingroup base
 * \brief   ÃÔµÛ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author  ±®½èË§
 * \last update date	2010/12/09
 */

#ifndef _MAT3_H_
#define _MAT3_H_

#include "cvlibstructs.h"
#include "cvlibmalloc.h"

namespace CVLib 
{

#define nv_scalar float
#define nv_zero			      nv_scalar(0)
#define nv_zero_5             nv_scalar(0.5)
#define nv_one			      nv_scalar(1.0)
#define nv_two			      nv_scalar(2)
#define nv_half_pi            nv_scalar(3.14159265358979323846264338327950288419716939937510582 * 0.5)
#define nv_quarter_pi         nv_scalar(3.14159265358979323846264338327950288419716939937510582 * 0.25)
#define nv_pi			      nv_scalar(3.14159265358979323846264338327950288419716939937510582)
#define nv_two_pi			  nv_scalar(3.14159265358979323846264338327950288419716939937510582 * 2.0)
#define nv_oo_pi			  nv_one / nv_pi
#define nv_oo_two_pi	      nv_one / nv_two_pi
#define nv_oo_255   	      nv_one / nv_scalar(255)
#define nv_oo_128   	      nv_one / nv_scalar(128)
#define nv_to_rad             nv_pi / nv_scalar(180)
#define nv_to_deg             nv_scalar(180) / nv_pi
#define nv_eps		          nv_scalar(10e-6)
#define nv_double_eps	      nv_scalar(10e-6) * nv_two
#define nv_big_eps            nv_scalar(10e-6)
#define nv_small_eps          nv_scalar(10e-2)


class quat;

class CVLIB_DECLSPEC mat3
{
public:
   mat3();
    mat3(const nv_scalar * array);
    mat3(const mat3 & M);
    mat3( const nv_scalar& f0,  const nv_scalar& f1,  const nv_scalar& f2,  
          const nv_scalar& f3,  const nv_scalar& f4,  const nv_scalar& f5,  
          const nv_scalar& f6,  const nv_scalar& f7,  const nv_scalar& f8 )
  		  : a00( f0 ), a10( f1 ), a20( f2 ), 
            a01( f3 ), a11( f4 ), a21( f5 ),
  		    a02( f6 ), a12( f7 ), a22( f8) { }

    const vec3 col(const int i) const
    {
        return vec3(&mat_array[i * 3]);
    }

    const vec3 operator[](int i) const
    {
        return vec3(mat_array[i], mat_array[i + 3], mat_array[i + 6]);
    }

    const nv_scalar& operator()(const int& i, const int& j) const
    {
        return mat_array[ j * 3 + i ];
    }

    nv_scalar& operator()(const int& i, const int& j)
    {
        return  mat_array[ j * 3 + i ];
    }

    void set_row(int i, const vec3 & v)
    {
        mat_array[i] = v.x;
        mat_array[i + 3] = v.y;
        mat_array[i + 6] = v.z;
    }

	void set_col(int i, const vec3 & v)
	{
        mat_array[i * 3] = v.x;
        mat_array[i * 3 + 1] = v.y;
        mat_array[i * 3 + 2] = v.z;
	}

    void set_rot(const nv_scalar & theta, const vec3 & v);
    void set_rot(const vec3 & u, const vec3 & v);

    union {
        struct {
            nv_scalar a00, a10, a20;        // standard names for components
            nv_scalar a01, a11, a21;        // standard names for components
            nv_scalar a02, a12, a22;        // standard names for components
        };
        nv_scalar mat_array[9];     // array access
    };
};

const vec3 operator*(const mat3&, const vec3&);
const vec3 operator*(const vec3&, const mat3&);

class CVLIB_DECLSPEC mat4
{
public:
    mat4();
    mat4(const nv_scalar * array);
    mat4(const mat4 & M);

    mat4( const nv_scalar& f0,  const nv_scalar& f1,  const nv_scalar& f2,  const nv_scalar& f3,
  		  const nv_scalar& f4,  const nv_scalar& f5,  const nv_scalar& f6,  const nv_scalar& f7,
  		  const nv_scalar& f8,  const nv_scalar& f9,  const nv_scalar& f10, const nv_scalar& f11,
  		  const nv_scalar& f12, const nv_scalar& f13, const nv_scalar& f14, const nv_scalar& f15 )
  		  : a00( f0 ), a10( f1 ), a20( f2 ), a30( f3 ),
  		    a01( f4 ), a11( f5 ), a21( f6 ), a31( f7 ),
  		    a02( f8 ), a12( f9 ), a22( f10), a32( f11),
			a03( f12), a13( f13), a23( f14), a33( f15) { }
 
			
	// ugly adds for expression toolkit
	void set( const nv_scalar * mat );
	void set( nv_scalar a0,  nv_scalar b0,  nv_scalar c0, nv_scalar d0,
               nv_scalar a1,  nv_scalar b1,  nv_scalar c1, nv_scalar d1,
               nv_scalar a2,  nv_scalar b2,  nv_scalar c2, nv_scalar d2,
               nv_scalar a3,  nv_scalar b3,  nv_scalar c3, nv_scalar d3);
	void SetAxisRotation(const vec3 & axis, nv_scalar theta);
			
    const vec4 col(const int i) const
    {
        return vec4(&mat_array[i * 4]);
    }
    
    const vec4 operator[](const int& i) const
    {
        return vec4(mat_array[i], mat_array[i + 4], mat_array[i + 8], mat_array[i + 12]);
    }
   
    const nv_scalar& operator()(const int& i, const int& j) const
    {
        return mat_array[ j * 4 + i ];
    }

    nv_scalar& operator()(const int& i, const int& j)
    {
        return  mat_array[ j * 4 + i ];
    }

    void set_col(int i, const vec4 & v)
    {
        mat_array[i * 4] = v.x;
        mat_array[i * 4 + 1] = v.y;
        mat_array[i * 4 + 2] = v.z;
        mat_array[i * 4 + 3] = v.w;
    }

    void set_row(int i, const vec4 & v)
    {
        mat_array[i] = v.x;
        mat_array[i + 4] = v.y;
        mat_array[i + 8] = v.z;
        mat_array[i + 12] = v.w;
    }

    mat3 & get_rot(mat3 & M) const;
    quat & get_rot(quat & q) const;
    void set_rot(const quat & q);
    void set_rot(const mat3 & M);
    void set_rot(const nv_scalar & theta, const vec3 & v);
    void set_rot(const vec3 & u, const vec3 & v);

    void set_translation(const vec3 & t);
    vec3 & get_translation(vec3 & t) const;

	mat4 operator*(const mat4&) const;

    union {
        struct {
            nv_scalar a00, a10, a20, a30;   // standard names for components
            nv_scalar a01, a11, a21, a31;   // standard names for components
            nv_scalar a02, a12, a22, a32;   // standard names for components
            nv_scalar a03, a13, a23, a33;   // standard names for components
        };
        struct {
            nv_scalar _11, _12, _13, _14;   // standard names for components
            nv_scalar _21, _22, _23, _24;   // standard names for components
            nv_scalar _31, _32, _33, _34;   // standard names for components
            nv_scalar _41, _42, _43, _44;   // standard names for components
        };
        struct {
            struct {
                nv_scalar b00, b10, b20, p; // standard names for components
                nv_scalar b01, b11, b21, q; // standard names for components
                nv_scalar b02, b12, b22, r; // standard names for components
                nv_scalar x, y, z, w;       // standard names for components
            };
        };
        nv_scalar mat_array[16];     // array access
    };
};

const vec4 operator*(const mat4&, const vec4&);
const vec4 operator*(const vec4&, const mat4&);

// quaternion
class CVLIB_DECLSPEC quat {
public:
	quat(nv_scalar x = 0, nv_scalar y = 0, nv_scalar z = 0, nv_scalar w = 1);
	quat(const quat& quat);
	quat(const vec3& axis, nv_scalar angle);
	quat(const mat3& rot);
	quat& operator=(const quat& quat);
	quat operator-()
	{
		return quat(-x, -y, -z, -w);
	}
	quat Inverse();
	void Normalize();
	void FromMatrix(const mat3& mat);
	void ToMatrix(mat3& mat) const;
	//ugly add for expression toolkit
	void set(const mat3 &R);
	void Slerp(const quat & from, const quat & to, nv_scalar t);
	//
	quat& operator*=(const quat& quat);
	static const quat Identity;
	nv_scalar& operator[](int i) { return comp[i]; }
	const nv_scalar operator[](int i) const { return comp[i]; }
	union {
		struct {
			nv_scalar x, y, z, w;
		};
		nv_scalar comp[4];
	};
};
const quat operator*(const quat&, const quat&);
extern quat & conj(quat & p, const quat & q);
extern quat & add_quats(quat & p, const quat & q1, const quat & q2);
extern nv_scalar dot(const quat & p, const quat & q);
extern quat & dot(nv_scalar s, const quat & p, const quat & q);
extern quat & slerp_quats(quat & p, nv_scalar s, const quat & q1, const quat & q2);
extern quat & axis_to_quat(quat & q, const vec3 & a, const nv_scalar phi);
extern mat3 & quat_2_mat(mat3 &M, const quat &q );
extern quat & mat_2_quat(quat &q,const mat3 &M);

// constant algebraic values
const nv_scalar array16_id[] =        { nv_one, nv_zero, nv_zero, nv_zero,
                                        nv_zero, nv_one, nv_zero, nv_zero,
                                        nv_zero, nv_zero, nv_one, nv_zero,
                                        nv_zero, nv_zero, nv_zero, nv_one};

const nv_scalar array16_null[] =      { nv_zero, nv_zero, nv_zero, nv_zero,
                                        nv_zero, nv_zero, nv_zero, nv_zero,
                                        nv_zero, nv_zero, nv_zero, nv_zero,
                                        nv_zero, nv_zero, nv_zero, nv_zero};

const nv_scalar array16_scale_bias[] = { nv_zero_5, nv_zero,   nv_zero,   nv_zero,
                                         nv_zero,   nv_zero_5, nv_zero,   nv_zero,
                                         nv_zero,   nv_zero,   nv_zero_5, nv_zero,
                                         nv_zero_5, nv_zero_5, nv_zero_5, nv_one};

const nv_scalar array9_id[] =         { nv_one, nv_zero, nv_zero,
                                        nv_zero, nv_one, nv_zero,
                                        nv_zero, nv_zero, nv_one};


const vec2      vec2_null(nv_zero,nv_zero);
const vec4      vec4_one(nv_one,nv_one,nv_one,nv_one);
const vec3      vec3_one(nv_one,nv_one,nv_one);
const vec3      vec3_null(nv_zero,nv_zero,nv_zero);
const vec3      vec3_x(nv_one,nv_zero,nv_zero);
const vec3      vec3_y(nv_zero,nv_one,nv_zero);
const vec3      vec3_z(nv_zero,nv_zero,nv_one);
const vec3      vec3_neg_x(-nv_one,nv_zero,nv_zero);
const vec3      vec3_neg_y(nv_zero,-nv_one,nv_zero);
const vec3      vec3_neg_z(nv_zero,nv_zero,-nv_one);
const vec4      vec4_null(nv_zero,nv_zero,nv_zero,nv_zero);
const vec4      vec4_x(nv_one,nv_zero,nv_zero,nv_zero);
const vec4      vec4_neg_x(-nv_one,nv_zero,nv_zero,nv_zero);
const vec4      vec4_y(nv_zero,nv_one,nv_zero,nv_zero);
const vec4      vec4_neg_y(nv_zero,-nv_one,nv_zero,nv_zero);
const vec4      vec4_z(nv_zero,nv_zero,nv_one,nv_zero);
const vec4      vec4_neg_z(nv_zero,nv_zero,-nv_one,nv_zero);
const vec4      vec4_w(nv_zero,nv_zero,nv_zero,nv_one);
const vec4      vec4_neg_w(nv_zero,nv_zero,nv_zero,-nv_one);
const quat      quat_id(nv_zero,nv_zero,nv_zero,nv_one);
const mat4      mat4_id(array16_id);
const mat3      mat3_id(array9_id);
const mat4      mat4_null(array16_null);
const mat4      mat4_scale_bias(array16_scale_bias);

// normalizes a vector and return a reference of itself
extern vec3 & normalize(vec3 & u);
extern vec4 & normalize(vec4 & u);

// Computes the squared magnitude
inline nv_scalar nv_sq_norm(const vec3 & n)
{ return n.x * n.x + n.y * n.y + n.z * n.z; }

inline nv_scalar nv_sq_norm(const vec4 & n)
{ return n.x * n.x + n.y * n.y + n.z * n.z + n.w * n.w; }

// Computes the magnitude
inline nv_scalar nv_norm(const vec3 & n)
{ return sqrtf(nv_sq_norm(n)); }

inline nv_scalar nv_norm(const vec4 & n)
{ return sqrtf(nv_sq_norm(n)); }

// computes the cross product ( v cross w) and stores the result in u
// i.e.     u = v cross w
extern vec3 & cross(vec3 & u, const vec3 & v, const vec3 & w);

// computes the dot product ( v dot w) and stores the result in u
// i.e.     u = v dot w
extern nv_scalar & dot(nv_scalar & u, const vec3 & v, const vec3 & w);
extern nv_scalar dot(const vec3 & v, const vec3 & w);
extern nv_scalar & dot(nv_scalar & u, const vec4 & v, const vec4 & w);
extern nv_scalar dot(const vec4 & v, const vec4 & w);
extern nv_scalar & dot(nv_scalar & u, const vec3 & v, const vec4 & w);
extern nv_scalar dot(const vec3 & v, const vec4 & w);
extern nv_scalar & dot(nv_scalar & u, const vec4 & v, const vec3 & w);
extern nv_scalar dot(const vec4 & v, const vec3 & w);

// compute the reflected vector R of L w.r.t N - vectors need to be 
// normalized
//
//                R     N     L
//                  _       _
//                 |\   ^   /|
//                   \  |  /
//                    \ | /
//                     \|/
//                      +
extern vec3 & reflect(vec3 & r, const vec3 & n, const vec3 & l);

// Computes u = v * lambda + u
extern vec3 & madd(vec3 & u, const vec3 & v, const nv_scalar & lambda);
// Computes u = v * lambda
extern vec3 & mult(vec3 & u, const vec3 & v, const nv_scalar & lambda);
// Computes u = v * w
extern vec3 & mult(vec3 & u, const vec3 & v, const vec3 & w);
// Computes u = v + w
extern vec3 & add(vec3 & u, const vec3 & v, const vec3 & w);
// Computes u = v - w
extern vec3 & sub(vec3 & u, const vec3 & v, const vec3 & w);

// Computes u = u * s
extern vec3 & scale(vec3 & u, const nv_scalar s);
extern vec4 & scale(vec4 & u, const nv_scalar s);

// Computes u = M * v
extern vec3 & mult(vec3 & u, const mat3 & M, const vec3 & v);
extern vec4 & mult(vec4 & u, const mat4 & M, const vec4 & v);

// Computes u = v * M
extern vec3 & mult(vec3 & u, const vec3 & v, const mat3 & M);
extern vec4 & mult(vec4 & u, const vec4 & v, const mat4 & M);

// Computes u = M(4x4) * v and divides by w
extern vec3 & mult_pos(vec3 & u, const mat4 & M, const vec3 & v);
// Computes u = M(4x4) * v
extern vec3 & mult_dir(vec3 & u, const mat4 & M, const vec3 & v);
// Computes u = M(4x4) * v and does not divide by w (assumed to be 1)
extern vec3 & mult(vec3& u, const mat4& M, const vec3& v);

// Computes u = v * M(4x4) and divides by w
extern vec3 & mult_pos(vec3 & u, const vec3 & v, const mat4 & M);
// Computes u = v * M(4x4)
extern vec3 & mult_dir(vec3 & u, const vec3 & v, const mat4 & M);
// Computes u = v * M(4x4) and does not divide by w (assumed to be 1)
extern vec3 & mult(vec3& u, const vec3& v, const mat4& M);

// Computes A += B
extern mat4 & add(mat4 & A, const mat4 & B);
extern mat3 & add(mat3 & A, const mat3 & B);

// Computes C = A * B
extern mat4 & mult(mat4 & C, const mat4 & A, const mat4 & B);
extern mat3 & mult(mat3 & C, const mat3 & A, const mat3 & B);

// Computes B = Transpose(A)
//       T
//  B = A
extern mat3 & transpose(mat3 & B, const mat3 & A);
extern mat4 & transpose(mat4 & B, const mat4 & A);
extern mat3 & transpose(mat3 & B);
extern mat4 & transpose(mat4 & B);

// Computes B = inverse(A)
//       -1
//  B = A
extern mat4 & invert(mat4 & B, const mat4 & A);
extern mat3 & invert(mat3 & B, const mat3 & A);

// Computes B = inverse(A)
//                                       T  T
//                   (R t)             (R -R t)
// assuming that A = (0 1) so that B = (0    1)
//  B = A
extern mat4 & invert_rot_trans(mat4 & B, const mat4 & A);

extern mat4 & look_at(mat4 & M, const vec3 & eye, const vec3 & center, const vec3 & up);
extern mat4 & frustum(mat4 & M, const nv_scalar l, const nv_scalar r, const nv_scalar b, 
               const nv_scalar t, const nv_scalar n, const nv_scalar f);

extern mat4 & perspective(mat4 & M, const nv_scalar fovy, const nv_scalar aspect, const nv_scalar n, const nv_scalar f);

// quaternion
extern quat & normalize(quat & p);
extern quat & conj(quat & p);
extern quat & conj(quat & p, const quat & q);
extern quat & add_quats(quat & p, const quat & q1, const quat & q2);
extern quat & axis_to_quat(quat & q, const vec3 & a, const nv_scalar phi);
extern mat3 & quat_2_mat(mat3 &M, const quat &q );
extern quat & mat_2_quat(quat &q,const mat3 &M);
extern quat & mat_2_quat(quat &q,const mat4 &M);

// surface properties
extern mat3 & tangent_basis(mat3 & basis,const vec3 & v0,const vec3 & v1,const vec3 & v2,const vec2 & t0,const vec2 & t1,const vec2 & t2, const vec3 & n);

// linear interpolation
inline nv_scalar lerp(nv_scalar t, nv_scalar a, nv_scalar b)
{ return a * (nv_one - t) + t * b; }

inline vec3 & lerp(vec3 & w, const nv_scalar & t, const vec3 & u, const vec3 & v)
{ w.x = lerp(t, u.x, v.x); w.y = lerp(t, u.y, v.y); w.z = lerp(t, u.z, v.z); return w; }

// utilities
inline nv_scalar nv_min(const nv_scalar & lambda, const nv_scalar & n)
{ return (lambda < n ) ? lambda : n; }

inline nv_scalar nv_max(const nv_scalar & lambda, const nv_scalar & n)
{ return (lambda > n ) ? lambda : n; }

inline nv_scalar nv_clamp(nv_scalar u, const nv_scalar min, const nv_scalar max)
{ u = (u < min) ? min : u; u = (u > max) ? max : u; return u; }

extern nv_scalar nv_random();

extern quat & trackball(quat & q, vec2 & pt1, vec2 & pt2, nv_scalar trackballsize);

extern vec3 & cube_map_normal(int i, int x, int y, int cubesize, vec3 & v);

// geometry
// computes the area of a triangle
extern nv_scalar nv_area(const vec3 & v1, const vec3 & v2, const vec3 &v3);
// computes the perimeter of a triangle
extern nv_scalar nv_perimeter(const vec3 & v1, const vec3 & v2, const vec3 &v3);
// find the inscribed circle
extern nv_scalar nv_find_in_circle( vec3 & center, const vec3 & v1, const vec3 & v2, const vec3 &v3);
// find the circumscribed circle
extern nv_scalar nv_find_circ_circle( vec3 & center, const vec3 & v1, const vec3 & v2, const vec3 &v3);

// fast cosine functions
extern nv_scalar fast_cos(const nv_scalar x);
extern nv_scalar ffast_cos(const nv_scalar x);

// determinant
nv_scalar det(const mat3 & A);

extern void nv_is_valid(const vec3& v);
extern void nv_is_valid(nv_scalar lambda);


/////////////////////////////////////////////////////
// Really Ugly Add to got MatQ and MatR from a very old sdk....


extern mat4 & get_col_matrix(quat &q, mat4 &dest_matrix);
extern quat & set_quat_from_euler_deg(quat &q, nv_scalar yaw, nv_scalar pitch, nv_scalar roll);
extern void AngleAxis(const quat &q, nv_scalar angleaxis[4] );
extern void xform(quat & q, vec3 &v);
extern void xform(const quat q, const vec3 &v, vec3 &xv);


//forward declaration
class MatQ;

const vec3 operator* (const MatQ& m, const vec3& v);


class MatR 
{
    
    friend class MatQ;
    
public:
    
    mat3 R;
    vec3 d;
    
public:
    
    // constructors //////////////////////////////////////////////////////////////
    
    MatR()                                {}
    MatR(const mat3 &R_, const vec3 &d_) {set(R_, d_);}
    MatR(const MatQ &T)                    {set(T);}
    
    
    // setters / accessors / translators /////////////////////////////////////////
    
    void set(const mat3 &R_, const vec3 &d_) {R = R_; d = d_;}
    void set(const MatQ &T);
    void set(const quat &q, const vec3 & v);
    void set(const mat4 &T);
    
    const mat3  &rot()   const {return R;}
    const vec3 &trans() const {return d;}
    
    mat3  &rot()         {return R;}
    vec3 &trans()       {return d;}
    
    
    // vec3 GetViewUp() { return R.GetViewUp() ;}
    // void GetViewUp(vec3 & v) { v = R.GetViewUp();}
    // 
    // vec3 GetViewNormal() {return R.GetViewNormal();}
    // void GetViewNormal(vec3 &v) {v = R.GetViewNormal();}
    // 
    // vec3 GetViewRight() { return R.GetViewRight();}
    // void GetViewRight(vec3 & v) { v = R.GetViewRight();}
    // 
    
    void SetRotation(const mat3& a) { R = a;}
    
    
    friend MatR operator * (const MatR& a, const MatR& b);        // m1 * m2
    
    void SetIdentity();
    void Invert();                       
    void GetInverse(MatR &M) const;                 // M^-1     
    void SetInverse(const MatR &M);                 // M^-1      
    mat4 GetMat4();                 // Cat
    
    void mult(const MatR &M, const MatR &N); 
    void mult(const MatR &M, bool post = false);
    //void postmult(const MatR &M);
    
    
    // Transforming vec3s ///////////////////////////////////////////////////////
    
    
    void xformVect(const vec3 &v, vec3 &xv) const;   
    void xformVect(vec3 &v) const;                 
    void xformPoint(const vec3 &p, vec3 &xp) const;  
    void xformPoint(vec3 &p) const;                
    
    
    void invXformVect(const vec3 &v, vec3 &xv) const;
    void invXformVect(vec3 &v) const;                 
    
    void invXformPoint(const vec3 &p, vec3 &xp) const;
    void invXformPoint(vec3 &p) const;                 
    
    
    
    int CalcRotationMatrixFromPoints(const vec3 & p1, const vec3 & p2, const vec3 & p3);
    
    int CalcRotationMatrixFromRN(const vec3 & vr, const vec3 & vpn);
    int CalcRotationMatrixFromRN(const vec3 & vr, const vec3 & vpn, const vec3 &p1);
    
    int CalcRotationMatrixFromNU(const vec3 & vpn, const vec3 & vup);
    int CalcRotationMatrixFromNU(const vec3 & vpn, const vec3 & vup, const vec3 &p1);
    
    int CalcRotationMatrixFromUR(const vec3 & vup, const vec3 & vr);
    int CalcRotationMatrixFromUR(const vec3 & vup, const vec3 & vr, const vec3 &p1);
    
    
    int CalcRotationMatrixFromAllVectors(vec3 & vpn, vec3 &vup, vec3 &vr, vec3 &pos);
    
    void SetTranslate(const vec3 & v);
    void SetTranslate(const nv_scalar x, const nv_scalar y, const nv_scalar z);
    void AddTranslate(const vec3 & v);
    void AddTranslate(const nv_scalar x, const nv_scalar y, const nv_scalar z);
    
    // ostream& print(ostream &os) const;
    nv_scalar xformAxis(const vec3 &v, int axis) const;
};


// inline ostream &operator<<(ostream &os, const MatR &X)  {return X.print(os);}


class MatQ 
{
    
    friend class MatR;
    
    //private:
public:
    
    quat q;     // rotation component
    vec3 d;    // translation component
    
public:
    
    // constructors //////////////////////////////////////////////////////////////
    
    
    MatQ() {}
    MatQ(const quat &q_, const vec3 &d_) {set(q_, d_);}
    //MatQ(const MatR &X) {set(X);}
    
    // setters / accessors / translators /////////////////////////////////////////
    
    void set(const quat &q_, const vec3 &d_) {q = q_; d = d_;}
    void set(const MatR &X) {q.set(X.R); d = X.d;}
	//void set(const MatR &X) {mat_2_quat(q, X.R); d = X.d;}
    void set(const mat4 &T);

    const quat  &rot()   const {return q;}
    const vec3 &trans() const {return d;}
    const vec3 &GetTranslation() const {return d;}
    quat  &rot()         {return q;}
    vec3 &trans()       {return d;}
    
    friend MatQ operator * (const MatQ& a, const MatQ& b);      // m1 * m2
   
    //ostream& print(ostream &os) const;
    
    void mult(const MatQ &T, const MatQ &U);    // T * U     
    void mult(const MatQ &T, bool post = false);               // T * this  
    void postmult(const MatQ &T);              // this * T 
    void premult ( const MatQ &q );    

    void SetIdentity();
    void Invert();                            // this^-1
    void InvertArbitrary();                            // this^-1
    void InvertAffine();                            // this^-1
    void GetInverse(MatQ &T) const;                // T^-1
    MatQ GetInverse() const;                // T^-1
    void GetInverseArbitrary(MatQ &T) const;                // T^-1
    void GetInverseAffine(MatQ &T) const;                // T^-1
    void SetInverse(const MatQ &T);
    void SetInverseArbitrary(const MatQ &T);
    void invert(const MatQ &M);                 // M^-1      [!]    
    
    // Transforming vec3s ///////////////////////////////////////////////////////
    
    // MatQs can transform elements of R^3 either as vectors or as
    // points.  Multiple operands need not be distinct.
    
    void xformVect(const vec3 &v, vec3 &xv) const; 
    
    nv_scalar xformAxis(const vec3 &v, int axis) const;
    
    void xformVect(vec3 &v) const;       // this * (v 0) => v
    void xformPoint(const vec3 &p, vec3 &xp) const; // this * (p 1) => xp
    void xformPoint(vec3 &p) const;      // this * (p 1) => p
    
    // These are exactly like the above methods, except the inverse
    // transform this^-1 is used.
    void invXformVect(const vec3 &v, vec3 &xv) const;
    void invXformVect(vec3 &v) const;                 
    void invXformPoint(const vec3 &p, vec3 &xp) const;
    void invXformPoint(vec3 &p) const;                 
    
    //ostream& print(ostream &os) const;
    
    void SetTranslate(nv_scalar x, nv_scalar y, nv_scalar z){ d.x = x; d.y = y; d.z = z;}
    void SetTranslate(const vec3 & v) { d =v;}
    void SetRotation(const mat3 & m);
    mat4 GetMat4();

    int CalcRotationMatrixFromPoints(const vec3 & p1, const vec3 & p2, const vec3 & p3);
    
    int CalcRotationMatrixFromRN(vec3 vr, vec3 vpn);
    int CalcRotationMatrixFromRN(vec3 vr, vec3 vpn, const vec3 &p1);
    
    int CalcRotationMatrixFromNU(vec3 vpn, vec3 vup);
    int CalcRotationMatrixFromNU(vec3 vpn, vec3 vup, const vec3 &p1);
    
    int CalcRotationMatrixFromUR(vec3 vup, vec3 vr);
    int CalcRotationMatrixFromUR(vec3 vup, vec3 vr, const vec3 &p1);
    
    
    void CalcRotationMatrixFromAllVectors(const vec3 & vpn, const vec3 &vup, const vec3 &vr, const vec3 &pos);
        
};

}

#endif //_MAT3_H_
