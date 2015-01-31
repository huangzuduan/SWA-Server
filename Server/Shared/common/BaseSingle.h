#ifndef SWA_BASE_SINGLE_H_
#define SWA_BASE_SINGLE_H_

/*
 *
 *	Detail: Ä£°åµ¥Àý
 *   
 *  Created by hzd 2014-12-9
 *
 */
template< class T > 
class BaseSingle
{
public:
	static T* Instance()
	{
		if ( !mThis )
			mThis = new T;

		return mThis;
	}
	static T* mThis;
};

template< typename T >
T* BaseSingle< T >::mThis = 0;

#endif


