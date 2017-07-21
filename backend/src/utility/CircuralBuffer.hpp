#pragma once
#include <mutex>
#include <numeric>
#include <vector>

namespace utility {

class CircuralBuffer {
public:
	CircuralBuffer( size_t capacity ) :
			values( capacity, 0 ) {
	}

	unsigned getAverage( void ) const {
		Lock lock( mtx );
		return std::accumulate( values.begin(), values.end(), 0 ) / values.size();
	}

	void addSample( unsigned value ) {
		Lock lock( mtx );
		pos = ( pos + 1 ) % values.size();
		values[ pos ] = value;
	}

private:
	using Lock = std::lock_guard<std::mutex>;

	std::vector<unsigned> values;
	size_t pos;

	mutable std::mutex mtx;
};

} // namespace utility
