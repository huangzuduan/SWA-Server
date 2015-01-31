#ifndef SWA_MATHBASE_H_
#define SWA_MATHBASE_H_

#include "BaseDefine.h"

#include <math.h>

namespace SWA
{
	const float32 PI = 3.14159265359f;

	const float32 PI2 = 3.14159265359f * 2.0f;

	//! Constant for reciprocal of PI.
	const float32 RECIPROCAL_PI	= 1.0f/PI;

	//! Constant for half of PI.
	const float32 HALF_PI = PI/2.0f;

	const float64 PI64 = 3.1415926535897932384626433832795028841971693993751;

	//! Constant for 64bit reciprocal of PI.
	const float64 RECIPROCAL_PI64 = 1.0/PI64;

	//! 32bit Constant for converting from degrees to radians
	const float32 DEGTORAD = PI / 180.0f;

	//! 32bit constant for converting from radians to degrees (formally known as GRAD_PI)
	const float32 RADTODEG = 180.0f / PI;

	//! 64bit constant for converting from degrees to radians (formally known as GRAD_PI2)
	const float64 DEGTORAD64 = PI64 / 180.0;

 	enum MathQuadSplitMode
 	{
 		// Diagonal goes from top-left to bottom-right
 		Math_TopLeftToBottomRight,	
 
 		// Diagonal goes from bottom-left to top-right
 		Math_BottomLeftToTopRight
 	};
 
 
 	enum MathPixelFormat
 	{
 		// Each pixel is 3 chars. RGB in that order.
 		MPF_RGB,
 		// Each pixel is 4 chars. RGBA in that order.
 		MPF_RGBA
 	};

	class MathVector3;

	struct MathBase 
	{
		static map< int32 , float32 > mCosMap;
		static map< int32 , float32 > mSinMap;

		static void		Init()
		{
			for ( int32 i = -360 ; i <= 360 ; i++ )
			{
				mCosMap[i] = (float32)cos( DEGTORAD64 * i );
				mSinMap[i] = (float32)sin( DEGTORAD64 * i );
			}
		}

		static float32		Sin( float64 degrees )
		{
			return mSinMap[ (int32)degrees ];
		}

		static float32		Cos( float64 degrees )
		{
			return mCosMap[ (int32)degrees ];
		}

		static int32		GetRandomInt( int32 min , int32 max )
		{
			if ( max == min )
			{
				return max;
			}

			return rand() % ( max - min ) + min;
		}

		static float32		GetRandomFloat( float32 lowBound , float32 highBound )
		{
			if( lowBound >= highBound ) // bad input
				return lowBound;

			// get random float in [0, 1] interval
			float f = ( rand() % 10000 ) * 0.0001f; 

			// return float in [lowBound, highBound] interval. 
			return ( f * (highBound - lowBound) ) + lowBound; 
		}

		static ulong		FtoDw( float32 f )
		{
			return *((ulong*)&f);
		}

//		static void		GetRandomVector(
//			MathVector3* out ,
//			MathVector3* min ,
//			MathVector3* max );
	};

	//! position 
	struct MathPos
	{
		float32 x , y;

		MathPos(): 
		x(0) , y(0)
		{}

		MathPos( float32 x , float32 y )
		{
			this->x = x;
			this->y = y;
		}

		MathPos& operator+=(const MathPos& other) { x+=other.x; y+=other.y; return *this; }
		MathPos& operator-=(const MathPos& other) { x-=other.x; y-=other.y; return *this; }

	};

	//! vector three
	class MathVector3
	{
	public:

		MathVector3():
		x( 0.0f ) , y( 0.0f ) , z( 0.0f )
		{}

		MathVector3( float32 fx, float32 fy, float32 fz ):
		x(fx), y(fy), z(fz)
		{}

		float32 x , y , z;

		MathVector3 operator+(const MathVector3& other) const { return MathVector3(x + other.x, y + other.y, z + other.z); }
		MathVector3& operator+=(const MathVector3& other) { x+=other.x; y+=other.y; z+=other.z; return *this; }
		MathVector3 operator+(const float32 val) const { return MathVector3(x + val, y + val, z + val); }
		MathVector3& operator+=(const float32 val) { x+=val; y+=val; z+=val; return *this; }

		MathVector3 operator-(const MathVector3& other) const { return MathVector3(x - other.x, y - other.y, z - other.z); }
		MathVector3& operator-=(const MathVector3& other) { x-=other.x; y-=other.y; z-=other.z; return *this; }
		MathVector3 operator-(const float32 val) const { return MathVector3(x - val, y - val, z - val); }
		MathVector3& operator-=(const float32 val) { x-=val; y-=val; z-=val; return *this; }

		MathVector3 operator*(const MathVector3& other) const { return MathVector3(x * other.x, y * other.y, z * other.z); }
		MathVector3& operator*=(const MathVector3& other) { x*=other.x; y*=other.y; z*=other.z; return *this; }
		MathVector3 operator*(const float32 v) const { return MathVector3(x * v, y * v, z * v); }
		MathVector3& operator*=(const float32 v) { x*=v; y*=v; z*=v; return *this; }

		MathVector3 operator/(const MathVector3& other) const { return MathVector3(x / other.x, y / other.y, z / other.z); }
		MathVector3& operator/=(const MathVector3& other) { x/=other.x; y/=other.y; z/=other.z; return *this; }
		MathVector3 operator/(const float32 v) const { float32 i=1.0f/v; return MathVector3(x * i, y * i, z * i); }
		MathVector3& operator/=(const float32 v) { float32 i=1.0f/v; x*=i; y*=i; z*=i; return *this; }

		void	Set( float32 fx, float32 fy, float32 fz )
		{
		  x = fx;
		  y = fy;
		  z = fz;
		}

		void	RotateXYBy( float64 degrees, const MathVector3& center )
		{
		  float32 cs = MathBase::Cos( degrees );
		  float32 sn = MathBase::Sin( degrees );

		  x -= center.x;
		  y -= center.y;

		  Set( (float32)(x * cs - y * sn) , (float32)(x * sn + y * cs) , (float32)z );

		  x += center.x;
		  y += center.y;
		}

		void	RotateYZBy( float64 degrees, const MathVector3& center )
		{
		  float32 cs = MathBase::Cos( degrees );
		  float32 sn = MathBase::Sin( degrees );

		  z -= center.z;
		  y -= center.y;

		  Set( (float32)x , (float32)(y * cs - z * sn) , (float32)(y * sn + z * cs) );

		  z += center.z;
		  y += center.y;
		}

		void	RotateXZBy( float64 degrees, const MathVector3& center )
		{
		  float64 cs = MathBase::Cos( degrees );
		  float64 sn = MathBase::Sin( degrees );

		  x -= center.x;
		  z -= center.z;

		  Set( (float32)(x * cs - z * sn) , (float32)y , (float32)(x * sn + z * cs) );

		  x += center.x;
		  z += center.z;
		}

		MathVector3 CrossProduct( const MathVector3& p ) const
		{
		  return MathVector3( y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x );
		}

		MathVector3& Normalize()
		{
		  float32 length = (float32)( x*x + y*y + z*z );
		  if ( Equals( length , 0.0f ) )
			  return *this;

		  length = 1.f / sqrtf ( length );
		  x = x * length;
		  y = y * length;
		  z = z * length;

		  return *this;
		}

		bool Equals( const float32 a, const float32 b, const float32 tolerance = 0.000001f )
		{
		return (a + tolerance >= b) && (a - tolerance <= b);
		}

// 		bool Equals( const MathVector3& other , const float32 tolerance = 0.000001f ) const
// 		{
// 		return Equals( x , other.x , tolerance ) &&
// 			Equals( y , other.y , tolerance ) &&
// 			Equals( z, other.z , tolerance );
// 		}


	};


	//! Rectangle
	struct MathRect
	{
		MathRect(): 
		top(0) , bottom(0) , left(0) , right(0)
		{}

		MathRect( float32 left , float32 top , float32 right , float32 bottom )
		{
			this->left = left;
			this->top = top;
			this->right = right;
			this->bottom = bottom;
		}

		float32 top , bottom , left , right;

		MathRect operator+(const MathRect& other) const { return MathRect( left + other.left, top + other.top, right + other.right, bottom + other.bottom ); }
		MathRect& operator+=(const MathRect& other) { top+=other.top; bottom+=other.bottom; left+=other.left; right+=other.right; return *this; }


		void			Clear()
		{
			top = 0.0f;
			bottom = 0.0f;
			left = 0.0f;
			right = 0.0f;
		}


		float32			Width() const
		{
			return right - left;
		}


		float32			Height() const
		{
			return bottom - top;
		}


		MathRect		GetIntersection( const MathRect& rect ) const
		{
			//! check for total exclusion
			if ((right > rect.left) &&
				(left < rect.right) &&
				(bottom > rect.top) &&
				(top < rect.bottom))
			{
				MathRect temp;

				//! fill in temp with the intersection
				temp.left = (left > rect.left) ? left : rect.left;
				temp.right = (right < rect.right) ? right : rect.right;
				temp.top = (top > rect.top) ? top : rect.top;
				temp.bottom = (bottom < rect.bottom) ? bottom : rect.bottom;

				return temp;
			}
			else
			{
				return MathRect( 0.0f , 0.0f , 0.0f , 0.0f );
			}

		}

		void			Offset( float32 x , float32 y )
		{
			this->left += x;
			this->right += x;
			this->top += y;
			this->bottom += y;
		}

		bool			IsPointIn( float32 x , float32 y )
		{
			return ( x >= left && y >= top && x <= right && y <= bottom );
		}

		bool			IsPointIn( MathPos& pos )
		{
			return ( pos.x >= left && pos.y >= top && pos.x <= right && pos.y <= bottom );
		}

	};


	//! Ray
	struct MathRay
	{
		MathRay()
		{

		}

		MathVector3 origin;
		MathVector3 direction;
	};

	//! Mouse
	struct MathMouse
	{
		MathMouse():
		Left( false ) , Right( false ) , Middle( false ) ,
		Wheel( 0 )
	{

	}

		bool			Left , Right , Middle;
		
		int32			Wheel;

		MathPos			Position;
	};




};



#endif



