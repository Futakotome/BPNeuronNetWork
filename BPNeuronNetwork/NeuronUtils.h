#pragma once
#include<cstdlib>

inline double randomClamped() {
	return rand() % 1000 * 0.001 - 0.5;
}
inline double randomFloat() {
	return rand() / (RAND_MAX + 1.0);
}

template<typename T>
void _delete(T *x) {
	typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
	(void)sizeof(type_must_be_complete);
	delete[] x;
}
