//Vector Class
// Part of Tree Grammar Structure for creating procedural trees
// Hugh Smith 
// copyright 2015
// all rights reserved

#include "Vector3.h"

const GMATRIX GMATRIX::Identity(1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 1.0f );


V3::V3(){};

V3::V3(float a, float b, float c)
{
	x = a;
	y = b;
	z = c;
}

V3::V3( const GVECTOR3 &v )
{
	x = v.x;
	y = v.y;
	z = v.z;
}


float V3::Length( void ) const
{
    float magnitude = ( x * x ) + 
			          ( y * y ) +
			          ( z * z );
	return sqrtf ( magnitude);
}

V3& V3::Normalize( )
{
	float len = Length();
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
}


V2::V2(){};

V2::V2(float a, float b)
{
	x = a;
	y = b;
}

V2::V2( const GVECTOR2 &v )
{
	x = v.x;
	y = v.y;
}

float	DotProduct ( const V3& v1, const V3& v2 ) 
{
	
	
	return ( v1.x * v2.x ) + ( v1.y * v2.y ) + ( v1.z * v2.z );
}


V3	CrossProduct ( const V3& v1, const V3& v2 ) 
{	
	return V3 (( v1.y * v2.z ) - ( v1.z * v2.y ),
				   ( v1.z * v2.x ) - ( v1.x * v2.z ),
				   ( v1.x * v2.y ) - ( v1.y * v2.x ));
}
void B3::spin()
{
}

