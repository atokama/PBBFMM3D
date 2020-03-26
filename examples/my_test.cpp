#include <iostream>
#include <fftw3.h>

int main() {
	int size = 10;
	double* d = fftw_alloc_real(size);
	for (int i = 0; i != size; ++i) {
		std::cout << "i: " << d[i] << std::endl;
	}
	fftw_free(d);
	return 0;
}
