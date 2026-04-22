#include <omp.h>
#include <stdio.h>

int main(){
	#pragma omp parallel for
    for (size_t i = 0; i < 100; i++){
        printf("%d\n", omp_get_thread_num());
    }
	return 0;
}
