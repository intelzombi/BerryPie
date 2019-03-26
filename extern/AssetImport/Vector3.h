//Vector Class
// Part of Tree Grammar Structure for creating procedural trees

#include <cmath>
#include <Windows.h>
#ifndef _Vector3
#define _Vector3

const float _eps = 0.0005f;

typedef struct _GVCOLOR {
    float r;
    float g;
    float b;
    float a;
}GVCOLOR;

typedef struct _GVECTOR {
    float x;
    float y;
    float z;
}GVECTOR;

typedef struct GVECTOR2 {
public:
    float x,y;
    GVECTOR2() {};
    GVECTOR2(const float *f){
        x = *f;
    };
    GVECTOR2(const GVECTOR2& gv){
        x = gv.x;
        y = gv.y;
    };
    GVECTOR2(float _x, float _y){
        x =_x;
        y =_y;

    }; 
    operator float* (){
        return &x;
    };
    operator const float* () const{
        return &x;
    };

    GVECTOR2& operator += (const GVECTOR2& gv){
        
        this->x += gv.x;
        this->y += gv.y;

        return *this;
    };
    GVECTOR2& operator -= (const GVECTOR2& gv){
        
        this->x -= gv.x;
        this->y -= gv.y;

        return *this;
    };
    GVECTOR2& operator *= (float _f){
        
        this->x *= _f;
        this->y *= _f;

        return *this;
    };
    GVECTOR2& operator /= (float _f){
        
        this->x /= _f;
        this->y /= _f;

        return *this;
    };
    GVECTOR2  operator + () const{
        
        GVECTOR2 gv;
        
        gv.x = this->x;
        gv.y = this->y;
        
        return gv;
    };
    GVECTOR2  operator - () const{
        
        GVECTOR2 gv;
        
        gv.x = -this->x;
        gv.y = -this->y;
        
        return gv;
    };


    GVECTOR2 operator + (const GVECTOR2& _gv) const{
        
        GVECTOR2 gv;
        
        gv.x = this->x + _gv.x;
        gv.y = this->y + _gv.y;

        return gv;
    };
    GVECTOR2 operator - (const GVECTOR2& _gv) const{
        
        GVECTOR2 gv;
        
        gv.x = this->x - _gv.x;
        gv.y = this->y - _gv.y;

        return gv;
    };
    GVECTOR2 operator * (float _f) const{
        
        GVECTOR2 gv;
        
        gv.x = this->x * _f;
        gv.y = this->y * _f;

        return gv;
    };
    GVECTOR2 operator / (float _f) const{
        
        GVECTOR2 gv;
        
        gv.x = this->x / _f;
        gv.y = this->y / _f;

        return gv;
    };
    
    friend GVECTOR2 operator * (float _f, const struct GVECTOR2& sgv){
        
        GVECTOR2 gv;
        
        gv.x = _f * sgv.x;
        gv.y = _f * sgv.y;

        return gv;
    };

    bool operator == (const GVECTOR2& _gv) const{
        if( ((this->x + _eps) >= _gv.x) &&
            ((this->x - _eps) <= _gv.x) &&
            ((this->y + _eps) >= _gv.y) &&
            ((this->y - _eps) <= _gv.y) 
          ){
            return true;
        }else{
            return false;
        }

    };
    bool operator != (const GVECTOR2& _gv) const{
        return !(*this == _gv);
    };


} GVECTOR2, *PGVECTOR2;



typedef struct GVECTOR3 : public GVECTOR
{
public:
    GVECTOR3(){};
    GVECTOR3(const float *f){
        x = *f;
    };
    GVECTOR3(const GVECTOR& gv){
        x = gv.x;
        y = gv.y;
        z = gv.z;
    };
    GVECTOR3(float _x, float _y, float _z){
        x =_x;
        y =_y;
        z =_z;
    };

    operator float* (){
        return &x;
    };
    operator const float* () const{
        return &x;
    };

    GVECTOR3& operator += (const GVECTOR3& gv){
        
        this->x += gv.x;
        this->y += gv.y;
        this->z += gv.z;

        return *this;
    };
    GVECTOR3& operator -= (const GVECTOR3& gv){
        
        this->x -= gv.x;
        this->y -= gv.y;
        this->z -= gv.z;

        return *this;
    };
    GVECTOR3& operator *= (float _f){
        
        this->x *= _f;
        this->y *= _f;
        this->z *= _f;

        return *this;
    };
    GVECTOR3& operator /= (float _f){
        
        this->x /= _f;
        this->y /= _f;
        this->z /= _f;

        return *this;
    };
    GVECTOR3  operator + () const{
        
        GVECTOR3 gv;
        
        gv.x = this->x;
        gv.y = this->y;
        gv.z = this->z;
        
        return gv;
    };
    GVECTOR3  operator - () const{
        
        GVECTOR3 gv;
        
        gv.x = -this->x;
        gv.y = -this->y;
        gv.z = -this->z;
        
        return gv;
    };


    GVECTOR3 operator + (const GVECTOR3& _gv) const{
        
        GVECTOR gv;
        
        gv.x = this->x + _gv.x;
        gv.y = this->y + _gv.y;
        gv.z = this->z + _gv.z;

        return gv;
    };
    GVECTOR3 operator - (const GVECTOR3& _gv) const{
        
        GVECTOR gv;
        
        gv.x = this->x - _gv.x;
        gv.y = this->y - _gv.y;
        gv.z = this->z - _gv.z;

        return gv;
    };
    GVECTOR3 operator * (float _f) const{
        
        GVECTOR gv;
        
        gv.x = this->x * _f;
        gv.y = this->y * _f;
        gv.z = this->z * _f;

        return gv;
    };
    GVECTOR3 operator / (float _f) const{
        
        GVECTOR gv;
        
        gv.x = this->x / _f;
        gv.y = this->y / _f;
        gv.z = this->z / _f;

        return gv;
    };
    
    friend GVECTOR3 operator * (float _f, const struct GVECTOR3& sgv){
        
        GVECTOR gv;
        
        gv.x = _f * sgv.x;
        gv.y = _f * sgv.y;
        gv.z = _f * sgv.z;

        return gv;
    };

    bool operator == (const GVECTOR3& _gv) const{
        if( ((this->x + _eps) >= _gv.x) &&
            ((this->x - _eps) <= _gv.x) &&
            ((this->y + _eps) >= _gv.y) &&
            ((this->y - _eps) <= _gv.y) &&
            ((this->z + _eps) >= _gv.z) &&
            ((this->z - _eps) <= _gv.z) 
          ){
            return true;
        }else{
            return false;
        }

    };
    bool operator != (const GVECTOR3& _gv) const{
        return !(*this == _gv);
    };

    GVECTOR3& Normalize()
    {
        float magnitude = ( x * x ) + 
			              ( y * y ) +
			              ( z * z );
	    
	float len = sqrtf ( magnitude);
	if ( len > 0.0f ) 
	{
		len = 1.0f / len;
		x *= len;
		y *= len;
		z *= len;
	}
	else
	{
	}
	return *this;
    };

    
    friend static float	
    GDotProduct ( const GVECTOR3& v1, const GVECTOR3& v2 ) 
    {
	
	
	    return ( v1.x * v2.x ) + ( v1.y * v2.y ) + ( v1.z * v2.z );
    }

    
    friend static GVECTOR3	
    GCrossProduct ( const GVECTOR3& v1, const GVECTOR3& v2 ) 
    {	
	    return GVECTOR3 (( v1.y * v2.z ) - ( v1.z * v2.y ),
				       ( v1.z * v2.x ) - ( v1.x * v2.z ),
				       ( v1.x * v2.y ) - ( v1.y * v2.x ));
    }


} GVECTOR3, *PGVECTOR3;


class	V3	: public GVECTOR3
{
public:

	V3();

	V3(float a, float b, float c);
	V3( const GVECTOR3 &v );

	float Length( void ) const;

   	V3& Normalize( );
    V3 operator+( const V3& Vector ) const
    {
        V3 v(x + Vector.x, 
             y + Vector.y, 
             z + Vector.z );
        return v;
    }

    V3 operator-( const V3& Vector ) const
    {
        V3 v(x - Vector.x, 
             y - Vector.y, 
             z - Vector.z);
        return v;
    }

};

class	V2	: public GVECTOR2
{
public:

	V2();

	V2(float a, float b);
	V2( const GVECTOR2 &v );

};

class B3
{
public:
    B3(){};
    B3(V3 i,V3 j, V3 k)
    { 
        iAxis = i;
        jAxis = j;
        kAxis = k;
    };
    void normalize()
    {
        iAxis.Normalize();
        jAxis.Normalize();
        kAxis.Normalize();
    };
    void spin();
    V3 iAxis;
    V3 jAxis;
    V3 kAxis;

};


float	DotProduct ( const V3& v1, const V3& v2 );
V3	CrossProduct ( const V3& v1, const V3& v2 );


typedef unsigned long GCOLOR;
#define GCOLOR_ARGB(a,r,g,b) \
    ((GCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define GCOLOR_RGBA(r,g,b,a) GCOLOR_ARGB(a,r,g,b)
#define GCOLOR_XRGB(r,g,b)   GCOLOR_ARGB(0xff,r,g,b)


typedef struct GMATRIX {
    union {
        struct {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;

        };
        float m[4][4];
    };

public:
    GMATRIX() 
    {
        _11 = 0.0f;
        _12 = 0.0f;
        _13 = 0.0f;
        _14 = 0.0f;
        _21 = 0.0f;
        _22 = 0.0f;
        _23 = 0.0f;
        _24 = 0.0f;
        _31 = 0.0f;
        _32 = 0.0f;
        _33 = 0.0f;
        _34 = 0.0f;
        _41 = 0.0f;
        _42 = 0.0f;
        _43 = 0.0f;
        _44 = 0.0f;

    };
    GMATRIX( float _f11, float _f12, float _f13, float _f14,
                float _f21, float _f22, float _f23, float _f24,
                float _f31, float _f32, float _f33, float _f34,
                float _f41, float _f42, float _f43, float _f44 )
    {
        _11 = _f11;
        _12 = _f12;
        _13 = _f13;
        _14 = _f14;
        _21 = _f21;
        _22 = _f22;
        _23 = _f23;
        _24 = _f24;
        _31 = _f31;
        _32 = _f32;
        _33 = _f33;
        _34 = _f34;
        _41 = _f41;
        _42 = _f42;
        _43 = _f43;
        _44 = _f44;

    };
    GMATRIX( const float * _f)
    {
        GMATRIX( *_f, *_f, *_f, *_f,
                 *_f, *_f, *_f, *_f,
                 *_f, *_f, *_f, *_f,
                 *_f, *_f, *_f, *_f);

    };
    GMATRIX( const GMATRIX& gm)
    {
        _11 = gm._11;
        _12 = gm._12;
        _13 = gm._13;
        _14 = gm._14;
        _21 = gm._21;
        _22 = gm._22;
        _23 = gm._23;
        _24 = gm._24;
        _31 = gm._31;
        _32 = gm._32;
        _33 = gm._33;
        _34 = gm._34;
        _41 = gm._41;
        _42 = gm._42;
        _43 = gm._43;
        _44 = gm._44;
      
    };


    // access grants
    float& operator () ( unsigned int Row, unsigned int Col )
    {
        return m[Row][Col];
    };
    float  operator () ( unsigned int Row, unsigned int Col ) const
    {
        float _f;
        _f = m[Row][Col];
        return _f;
    };

    // casting operators
    // Go study and rewrite when understood
    operator float* ()
    {
        return &m[0][0];
    };
    // Go study and rewrite when understood
    operator const float* () const
    {
        return &m[0][0];
    };

    // assignment operators
    GMATRIX& operator *= ( const GMATRIX& gm)
    {
        GMATRIX tmp;
        tmp._11 = this->_11 * gm._11 +
                  this->_12 * gm._21 +
                  this->_13 * gm._31 +
                  this->_14 * gm._41;


        tmp._12 = this->_11 * gm._12 +
                  this->_12 * gm._22 +
                  this->_13 * gm._32 +
                  this->_14 * gm._42;

        tmp._13 = this->_11 * gm._13 + 
                  this->_12 * gm._23 +
                  this->_13 * gm._33 +
                  this->_14 * gm._43;

        tmp._14 = this->_11 * gm._14 +
                  this->_12 * gm._24 +
                  this->_13 * gm._34 +
                  this->_14 * gm._44;

        tmp._21 = this->_21 * gm._11 +
                  this->_22 * gm._21 +
                  this->_23 * gm._31 +
                  this->_24 * gm._41;


        tmp._22 = this->_21 * gm._12 +
                  this->_22 * gm._22 +
                  this->_23 * gm._32 +
                  this->_24 * gm._42;

        tmp._23 = this->_21 * gm._13 + 
                  this->_22 * gm._23 +
                  this->_23 * gm._33 +
                  this->_24 * gm._43;

        tmp._24 = this->_21 * gm._14 +
                  this->_22 * gm._24 +
                  this->_23 * gm._34 +
                  this->_24 * gm._44;



        tmp._31 = this->_31 * gm._11 +
                  this->_32 * gm._21 +
                  this->_33 * gm._31 +
                  this->_34 * gm._41;


        tmp._32 = this->_31 * gm._12 +
                  this->_32 * gm._22 +
                  this->_33 * gm._32 +
                  this->_34 * gm._42;

        tmp._33 = this->_31 * gm._13 + 
                  this->_32 * gm._23 +
                  this->_33 * gm._33 +
                  this->_34 * gm._43;

        tmp._34 = this->_31 * gm._14 +
                  this->_32 * gm._24 +
                  this->_33 * gm._34 +
                  this->_34 * gm._44;


        tmp._41 = this->_41 * gm._11 +
                  this->_42 * gm._21 +
                  this->_43 * gm._31 +
                  this->_44 * gm._41;


        tmp._42 = this->_41 * gm._12 +
                  this->_42 * gm._22 +
                  this->_43 * gm._32 +
                  this->_44 * gm._42;

        tmp._43 = this->_41 * gm._13 + 
                  this->_42 * gm._23 +
                  this->_43 * gm._33 +
                  this->_44 * gm._43;

        tmp._44 = this->_41 * gm._14 +
                  this->_42 * gm._24 +
                  this->_43 * gm._34 +
                  this->_44 * gm._44;




        
        
        this->_11 = tmp._11;
        this->_12 = tmp._12;
        this->_13 = tmp._13;
        this->_14 = tmp._14;
        this->_21 = tmp._21;
        this->_22 = tmp._22;
        this->_23 = tmp._23;
        this->_24 = tmp._24;
        this->_31 = tmp._31;
        this->_32 = tmp._32;
        this->_33 = tmp._33;
        this->_34 = tmp._34;
        this->_41 = tmp._41;
        this->_42 = tmp._42;
        this->_43 = tmp._43;
        this->_44 = tmp._44;

        return *this;

    };
    GMATRIX& operator += ( const GMATRIX& gm)
    {
        this->_11 += gm._11;
        this->_12 += gm._12;
        this->_13 += gm._13;
        this->_14 += gm._14;
        this->_21 += gm._21;
        this->_22 += gm._22;
        this->_23 += gm._23;
        this->_24 += gm._24;
        this->_31 += gm._31;
        this->_32 += gm._32;
        this->_33 += gm._33;
        this->_34 += gm._34;
        this->_41 += gm._41;
        this->_42 += gm._42;
        this->_43 += gm._43;
        this->_44 += gm._44;

        return *this;

    };
    GMATRIX& operator -= ( const GMATRIX& gm)
    {
        this->_11 -= gm._11;
        this->_12 -= gm._12;
        this->_13 -= gm._13;
        this->_14 -= gm._14;
        this->_21 -= gm._21;
        this->_22 -= gm._22;
        this->_23 -= gm._23;
        this->_24 -= gm._24;
        this->_31 -= gm._31;
        this->_32 -= gm._32;
        this->_33 -= gm._33;
        this->_34 -= gm._34;
        this->_41 -= gm._41;
        this->_42 -= gm._42;
        this->_43 -= gm._43;
        this->_44 -= gm._44;

        return *this;

    };
    GMATRIX& operator *= ( float _f)
    {
        this->_11 *= _f;
        this->_12 *= _f;
        this->_13 *= _f;
        this->_14 *= _f;
        this->_21 *= _f;
        this->_22 *= _f;
        this->_23 *= _f;
        this->_24 *= _f;
        this->_31 *= _f;
        this->_32 *= _f;
        this->_33 *= _f;
        this->_34 *= _f;
        this->_41 *= _f;
        this->_42 *= _f;
        this->_43 *= _f;
        this->_44 *= _f;

        return *this;

    };
    GMATRIX& operator /= ( float _f)
    {
        this->_11 /= _f;
        this->_12 /= _f;
        this->_13 /= _f;
        this->_14 /= _f;
        this->_21 /= _f;
        this->_22 /= _f;
        this->_23 /= _f;
        this->_24 /= _f;
        this->_31 /= _f;
        this->_32 /= _f;
        this->_33 /= _f;
        this->_34 /= _f;
        this->_41 /= _f;
        this->_42 /= _f;
        this->_43 /= _f;
        this->_44 /= _f;

        return *this;

    };

    // unary operators
    GMATRIX operator + () const
    {
        GMATRIX gm;
        
        gm._11 = this->_11;
        gm._21 = this->_12;
        gm._31 = this->_13;
        gm._41 = this->_14;
        gm._12 = this->_21;
        gm._22 = this->_22;
        gm._32 = this->_23;
        gm._42 = this->_24;
        gm._13 = this->_31;
        gm._23 = this->_32;
        gm._33 = this->_33;
        gm._43 = this->_34;
        gm._14 = this->_41;
        gm._24 = this->_42;
        gm._34 = this->_43;
        gm._44 = this->_44;

        
        return gm;

    };
    GMATRIX operator - () const
    {
        GMATRIX gm;
        
        gm._11 = -this->_11;
        gm._21 = -this->_12;
        gm._31 = -this->_13;
        gm._41 = -this->_14;
        gm._12 = -this->_21;
        gm._22 = -this->_22;
        gm._32 = -this->_23;
        gm._42 = -this->_24;
        gm._13 = -this->_31;
        gm._23 = -this->_32;
        gm._33 = -this->_33;
        gm._43 = -this->_34;
        gm._14 = -this->_41;
        gm._24 = -this->_42;
        gm._34 = -this->_43;
        gm._44 = -this->_44;

        
        return gm;

    };

    // binary operators
    GMATRIX operator * ( const GMATRIX& gm) const
    {
        GMATRIX tmp;
        tmp._11 = this->_11 * gm._11 +
                  this->_12 * gm._21 +
                  this->_13 * gm._31 +
                  this->_14 * gm._41;


        tmp._12 = this->_11 * gm._12 +
                  this->_12 * gm._22 +
                  this->_13 * gm._32 +
                  this->_14 * gm._42;

        tmp._13 = this->_11 * gm._13 + 
                  this->_12 * gm._23 +
                  this->_13 * gm._33 +
                  this->_14 * gm._43;

        tmp._14 = this->_11 * gm._14 +
                  this->_12 * gm._24 +
                  this->_13 * gm._34 +
                  this->_14 * gm._44;

        tmp._21 = this->_21 * gm._11 +
                  this->_22 * gm._21 +
                  this->_23 * gm._31 +
                  this->_24 * gm._41;


        tmp._22 = this->_21 * gm._12 +
                  this->_22 * gm._22 +
                  this->_23 * gm._32 +
                  this->_24 * gm._42;

        tmp._23 = this->_21 * gm._13 + 
                  this->_22 * gm._23 +
                  this->_23 * gm._33 +
                  this->_24 * gm._43;

        tmp._24 = this->_21 * gm._14 +
                  this->_22 * gm._24 +
                  this->_23 * gm._34 +
                  this->_24 * gm._44;



        tmp._31 = this->_31 * gm._11 +
                  this->_32 * gm._21 +
                  this->_33 * gm._31 +
                  this->_34 * gm._41;


        tmp._32 = this->_31 * gm._12 +
                  this->_32 * gm._22 +
                  this->_33 * gm._32 +
                  this->_34 * gm._42;

        tmp._33 = this->_31 * gm._13 + 
                  this->_32 * gm._23 +
                  this->_33 * gm._33 +
                  this->_34 * gm._43;

        tmp._34 = this->_31 * gm._14 +
                  this->_32 * gm._24 +
                  this->_33 * gm._34 +
                  this->_34 * gm._44;


        tmp._41 = this->_41 * gm._11 +
                  this->_42 * gm._21 +
                  this->_43 * gm._31 +
                  this->_44 * gm._41;


        tmp._42 = this->_41 * gm._12 +
                  this->_42 * gm._22 +
                  this->_43 * gm._32 +
                  this->_44 * gm._42;

        tmp._43 = this->_41 * gm._13 + 
                  this->_42 * gm._23 +
                  this->_43 * gm._33 +
                  this->_44 * gm._43;

        tmp._44 = this->_41 * gm._14 +
                  this->_42 * gm._24 +
                  this->_43 * gm._34 +
                  this->_44 * gm._44;
        
        return tmp;

    };
    GMATRIX operator + ( const GMATRIX& gm) const
    {
        GMATRIX gr;

        gr._11 = this->_11 + gm._11;
        gr._21 = this->_12 + gm._12;
        gr._31 = this->_13 + gm._13;
        gr._41 = this->_14 + gm._14;
        gr._12 = this->_21 + gm._21;
        gr._22 = this->_22 + gm._22;
        gr._32 = this->_23 + gm._23;
        gr._42 = this->_24 + gm._24;
        gr._13 = this->_31 + gm._31;
        gr._23 = this->_32 + gm._32;
        gr._33 = this->_33 + gm._33;
        gr._43 = this->_34 + gm._34;
        gr._14 = this->_41 + gm._41;
        gr._24 = this->_42 + gm._42;
        gr._34 = this->_43 + gm._43;
        gr._44 = this->_44 + gm._44;

        return gr;
    };
    GMATRIX operator - ( const GMATRIX& gm) const
    {
        GMATRIX gr;

        gr._11 = this->_11 - gm._11;
        gr._21 = this->_12 - gm._12;
        gr._31 = this->_13 - gm._13;
        gr._41 = this->_14 - gm._14;
        gr._12 = this->_21 - gm._21;
        gr._22 = this->_22 - gm._22;
        gr._32 = this->_23 - gm._23;
        gr._42 = this->_24 - gm._24;
        gr._13 = this->_31 - gm._31;
        gr._23 = this->_32 - gm._32;
        gr._33 = this->_33 - gm._33;
        gr._43 = this->_34 - gm._34;
        gr._14 = this->_41 - gm._41;
        gr._24 = this->_42 - gm._42;
        gr._34 = this->_43 - gm._43;
        gr._44 = this->_44 - gm._44;

        return gr;
    };
    GMATRIX operator * ( float _f) const
    {
        GMATRIX gr;

        gr._11 = this->_11 * _f;
        gr._21 = this->_12 * _f;
        gr._31 = this->_13 * _f;
        gr._41 = this->_14 * _f;
        gr._12 = this->_21 * _f;
        gr._22 = this->_22 * _f;
        gr._32 = this->_23 * _f;
        gr._42 = this->_24 * _f;
        gr._13 = this->_31 * _f;
        gr._23 = this->_32 * _f;
        gr._33 = this->_33 * _f;
        gr._43 = this->_34 * _f;
        gr._14 = this->_41 * _f;
        gr._24 = this->_42 * _f;
        gr._34 = this->_43 * _f;
        gr._44 = this->_44 * _f;

        return gr;
    };
    GMATRIX operator / ( float _f) const
    {
        GMATRIX gr;

        gr._11 = this->_11 / _f;
        gr._21 = this->_12 / _f;
        gr._31 = this->_13 / _f;
        gr._41 = this->_14 / _f;
        gr._12 = this->_21 / _f;
        gr._22 = this->_22 / _f;
        gr._32 = this->_23 / _f;
        gr._42 = this->_24 / _f;
        gr._13 = this->_31 / _f;
        gr._23 = this->_32 / _f;
        gr._33 = this->_33 / _f;
        gr._43 = this->_34 / _f;
        gr._14 = this->_41 / _f;
        gr._24 = this->_42 / _f;
        gr._34 = this->_43 / _f;
        gr._44 = this->_44 / _f;

        return gr;
    };



    friend static GMATRIX * 
    GMatrixIdentity(GMATRIX *gm)
    {
        gm->m[0][1] = gm->m[0][2] = gm->m[0][3] =
        gm->m[1][0] = gm->m[1][2] = gm->m[1][3] =
        gm->m[2][0] = gm->m[2][1] = gm->m[2][3] =
        gm->m[3][0] = gm->m[3][1] = gm->m[3][2] = 0.0f;

        gm->m[0][0] = gm->m[1][1] = gm->m[2][2] = gm->m[3][3] = 1.0f;
        return gm;
    };

    friend GMATRIX operator * ( float _f, const GMATRIX& gm)
    {
        GMATRIX gr;

        gr._11 = _f * gm._11;
        gr._21 = _f * gm._21;
        gr._31 = _f * gm._31;
        gr._41 = _f * gm._41;
        gr._12 = _f * gm._12;
        gr._22 = _f * gm._22;
        gr._32 = _f * gm._32;
        gr._42 = _f * gm._42;
        gr._13 = _f * gm._13;
        gr._23 = _f * gm._23;
        gr._33 = _f * gm._33;
        gr._43 = _f * gm._43;
        gr._14 = _f * gm._14;
        gr._24 = _f * gm._24;
        gr._34 = _f * gm._34;
        gr._44 = _f * gm._44;

        return gr;

    };

    bool operator == ( const GMATRIX& _gm) const
    {
        if( ((this->_11 + _eps) >= _gm._11) &&
            ((this->_11 - _eps) <= _gm._11) &&
            ((this->_21 + _eps) >= _gm._21) &&
            ((this->_21 - _eps) <= _gm._21) &&
            ((this->_31 + _eps) >= _gm._31) &&
            ((this->_31 - _eps) <= _gm._31) &&
            ((this->_41 + _eps) >= _gm._41) &&
            ((this->_41 - _eps) <= _gm._41) &&
            ((this->_12 + _eps) >= _gm._21) &&
            ((this->_12 - _eps) <= _gm._21) &&
            ((this->_22 + _eps) >= _gm._22) &&
            ((this->_22 - _eps) <= _gm._22) &&
            ((this->_32 + _eps) >= _gm._32) &&
            ((this->_32 - _eps) <= _gm._32) &&
            ((this->_42 + _eps) >= _gm._42) &&
            ((this->_42 - _eps) <= _gm._42) &&

            ((this->_13 + _eps) >= _gm._13) &&
            ((this->_13 - _eps) <= _gm._13) &&
            ((this->_23 + _eps) >= _gm._23) &&
            ((this->_23 - _eps) <= _gm._23) &&
            ((this->_33 + _eps) >= _gm._33) &&
            ((this->_33 - _eps) <= _gm._33) &&
            ((this->_43 + _eps) >= _gm._43) &&
            ((this->_43 - _eps) <= _gm._43) &&
            ((this->_14 + _eps) >= _gm._24) &&
            ((this->_14 - _eps) <= _gm._24) &&
            ((this->_24 + _eps) >= _gm._24) &&
            ((this->_24 - _eps) <= _gm._24) &&
            ((this->_34 + _eps) >= _gm._34) &&
            ((this->_34 - _eps) <= _gm._34) &&
            ((this->_44 + _eps) >= _gm._44) &&
            ((this->_44 - _eps) <= _gm._44) 

          ){
            return true;
        }else{
            return false;
        }

    };
    bool operator != ( const GMATRIX& _gm) const
    {
        return !(*this == _gm); 
    };
	friend static GMATRIX* 
	GMatrixPerspectiveFovRH(GMATRIX *pout, float fovy, float aspect, float zn, float zf)
	{
		GMatrixIdentity(pout);
		pout->m[0][0] = 1.0f / (aspect * tan(fovy/2.0f));
		pout->m[1][1] = 1.0f / tan(fovy/2.0f);
		pout->m[2][2] = zf / (zn - zf);
		pout->m[2][3] = -1.0f;
		pout->m[3][2] = (zf * zn) / (zn - zf);
		pout->m[3][3] = 0.0f;
		return pout;
	}

    friend static GMATRIX* 
    GMatrixPerspectiveFovLH(GMATRIX *pout, float fovy, float aspect, float zn, float zf)
    {
        GMatrixIdentity(pout);
        pout->m[0][0] = 1.0f / (aspect * tan(fovy/2.0f));
        pout->m[1][1] = 1.0f / tan(fovy/2.0f);
        pout->m[2][2] = zf / (zf - zn);
        pout->m[2][3] = 1.0f;
        pout->m[3][2] = (zf * zn) / (zn - zf);
        pout->m[3][3] = 0.0f;
        return pout;
    };

    friend static GMATRIX 
    GProjectionMatrix(const float near_plane, // Distance to near clipping 
                                              // plane
                      const float far_plane,  // Distance to far clipping 
                                              // plane
                      const float fov_horiz,  // Horizontal field of view 
                                              // angle, in radians
                      const float fov_vert)   // Vertical field of view 
                                              // angle, in radians
    {
        float    h, w, Q;

        w = (float)1/tan(fov_horiz*0.5);  // 1/tan(x) == cot(x)
        h = (float)1/tan(fov_vert*0.5);   // 1/tan(x) == cot(x)
        Q = far_plane/(far_plane - near_plane);

        GMATRIX ret;
        ZeroMemory(&ret, sizeof(ret));

        ret(0, 0) = w;
        ret(1, 1) = h;
        ret(2, 2) = Q;
        ret(3, 2) = -Q*near_plane;
        ret(2, 3) = 1;
        return ret;
    };   // End of ProjectionMatrix


    friend static GMATRIX
    GScale (float sx, float sy, float sz) 
    
    {
       GMATRIX scale(
        sx,    0.0f,   0.0f,  0.0f,
        0.0f,  sy,     0.0f,  0.0f,
        0.0f,  0.0f,   sz,    0.0f,
        0.0f,  0.0f,   0.0f,  1.0f
       );
       return scale;
     };

    friend static GMATRIX
    GScale (float s) 
    
    {
       GMATRIX scale(
        s,    0.0f,   0.0f,  0.0f,
        0.0f,  s,     0.0f,  0.0f,
        0.0f,  0.0f,   s,    0.0f,
        0.0f,  0.0f,   0.0f,  1.0f
       );
       return scale;
     };

    friend static GMATRIX
    GTranslate(const float dx, const float dy, const float dz) {
        GMATRIX ret;

        GMatrixIdentity(&ret);
        ret(3, 0) = dx;
        ret(3, 1) = dy;
        ret(3, 2) = dz;
        return ret;
    };    // End of Translate

    friend static GMATRIX *
    GRotateX(GMATRIX *pOut, float angle )
    {

        float sin_, cos_;
        sin_ = sin(angle);
        cos_ = cos(angle);
        //sincosf(angle, &sin, &cos);  // Determine sin and cos of angle

        pOut->_11 = 1.0f; pOut->_12 =  0.0f;    pOut->_13 = 0.0f;  pOut->_14 = 0.0f;
        pOut->_21 = 0.0f; pOut->_22 =  cos_;    pOut->_23 = sin_;  pOut->_24 = 0.0f;
        pOut->_31 = 0.0f; pOut->_32 = -sin_;    pOut->_33 = cos_;  pOut->_34 = 0.0f;
        pOut->_41 = 0.0f; pOut->_42 =  0.0f;    pOut->_43 = 0.0f;  pOut->_44 = 1.0f;

        return pOut;
    };

    friend static GMATRIX *
    GRotateY(GMATRIX *pOut, float angle )
    {

        float sin_, cos_;
        sin_ = sin(angle);
        cos_ = cos(angle);
        //sincosf(angle, &sin, &cos);  // Determine sin and cos of angle

        pOut->_11 = cos_;  pOut->_12 =  0.0f;   pOut->_13 = -sin_; pOut->_14 = 0.0f;
        pOut->_21 = 0.0f;  pOut->_22 =  1.0f;   pOut->_23 = 0.0f;  pOut->_24 = 0.0f;
        pOut->_31 = sin_;  pOut->_32 =  0.0f;   pOut->_33 = cos_;  pOut->_34 = 0.0f;
        pOut->_41 = 0.0f;  pOut->_42 =  0.0f;   pOut->_43 = 0.0f;  pOut->_44 = 1.0f;

        return pOut;
    };

    friend static GMATRIX *
    GRotateZ(GMATRIX *pOut, float angle )
    {

        float sin_, cos_;
        sin_ = sinf(angle);
        cos_ = cosf(angle);
        //sincosf(angle, &sin, &cos);  // Determine sin and cos of angle

        pOut->_11 = cos_;  pOut->_12 =  sin_;   pOut->_13 = 0.0f; pOut->_14 = 0.0f;
        pOut->_21 = -sin_; pOut->_22 =  cos_;   pOut->_23 = 0.0f; pOut->_24 = 0.0f;
        pOut->_31 = 0.0f;  pOut->_32 =  0.0f;   pOut->_33 = 1.0f; pOut->_34 = 0.0f;
        pOut->_41 = 0.0f;  pOut->_42 =  0.0f;   pOut->_43 = 0.0f; pOut->_44 = 1.0f;

        return pOut;
    };
 
    friend static GVECTOR3 * 
    GVec3TransformNormal(GVECTOR3 *pout, const GVECTOR3 *pv, const GMATRIX *pm)
    {
        pout->x = pm->m[0][0] * pv->x + pm->m[1][0] * pv->y + pm->m[2][0] * pv->z;
        pout->y = pm->m[0][1] * pv->x + pm->m[1][1] * pv->y + pm->m[2][1] * pv->z;
        pout->z = pm->m[0][2] * pv->x + pm->m[1][2] * pv->y + pm->m[2][2] * pv->z;
        return pout;

    };
    friend static GMATRIX* 
    GMatrixRotationAxis(GMATRIX *pout, const GVECTOR3 *pv, float angle)
    {
        GVECTOR3 v(*pv);

        v.Normalize();
        GMatrixIdentity(pout);
        pout->m[0][0] = (1.0f - cos(angle)) * v.x * v.x + cos(angle);
        pout->m[1][0] = (1.0f - cos(angle)) * v.x * v.y - sin(angle) * v.z;
        pout->m[2][0] = (1.0f - cos(angle)) * v.x * v.z + sin(angle) * v.y;
        pout->m[0][1] = (1.0f - cos(angle)) * v.y * v.x + sin(angle) * v.z;
        pout->m[1][1] = (1.0f - cos(angle)) * v.y * v.y + cos(angle);
        pout->m[2][1] = (1.0f - cos(angle)) * v.y * v.z - sin(angle) * v.x;
        pout->m[0][2] = (1.0f - cos(angle)) * v.z * v.x - sin(angle) * v.y;
        pout->m[1][2] = (1.0f - cos(angle)) * v.z * v.y + sin(angle) * v.x;
        pout->m[2][2] = (1.0f - cos(angle)) * v.z * v.z + cos(angle);
        return pout;
    };
} GMATRIX;




#endif
