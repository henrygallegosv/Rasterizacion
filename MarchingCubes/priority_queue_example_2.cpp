#include <iostream>
using namespace std;

#include "mutable_priority_queue.h"
#include "Voxel.h"

template<typename Val>
class Compare {
public:
	const inline bool operator()( const Val a, const Val b ) const {
		return a < b;
	}
};


int main( int argc, char **argv ){

	Compare<int> c;
	mutable_priority_queue<float,Voxel,std::less<float>,std::less<Voxel>> A, B;
	mutable_priority_queue<float,Voxel,std::less<float>,std::less<Voxel>>::iterator i;

	Voxel *tmp1, *tmp2, *tmp3;
	tmp1 = new Voxel(1,1,1);	tmp1->distance = 0.0f;
	A.insert(*tmp1,  0.0f );
	tmp2 = new Voxel(2,2,2);	tmp1->distance = 2.0f;
	A.insert( *tmp2,  2.0f );
	tmp3 = new Voxel(3,3,3);	tmp3->distance = -1.0f;
	A.insert( *tmp3, -1.0f );

	std::cout << "A:" << std::endl;
	A.empty();
	Voxel vxl = A.front_value();
	for( i=A.begin(); i!=A.end(); i++ ){
		std::cout << "key: " << i->first << ", value: " << i->second.index << std::endl;
	}
	A.pop();

	tmp2->distance = -5.0f;
	A.update( *tmp2, -5.0f );
	std::cout << "A:" << std::endl;
	for( i=A.begin(); i!=A.end(); i++ ){
		std::cout << "key: " << i->first << ", value: " << i->second.index << std::endl;
	}

	i = A.begin();

	// Can't do this (by design), gives a compile error
	//A.erase(i);

	// Do this instead
	A.erase( i->second );

	// after performing an erase, the iterators are 
	// invalid, so this gives a runtime error:
	// i++;

	std::cout << "A:" << std::endl;
	for( i=A.begin(); i!=A.end(); i++ ){
		std::cout << "key: " << i->first << ", value: " << i->second.index << std::endl;
	}

	std::cout << "B:" << std::endl;
	for( i=B.begin(); i!=B.end(); i++ ){
		std::cout << "key: " << i->first << ", value: " << i->second.index << std::endl;
	}
	B = A;
	tmp2->distance = 5.0f;
	B.update( *tmp2, 5.0f );
	std::cout << "B:" << std::endl;
	for( i=B.begin(); i!=B.end(); i++ ){
		std::cout << "key: " << i->first << ", value: " << i->second.index << std::endl;
	}
	std::cout << "A:" << std::endl;
	for( i=A.begin(); i!=A.end(); i++ ){
		std::cout << "key: " << i->first << ", value: " << i->second.index << std::endl;
	}

	std::cin.get();
	return 0;
};
