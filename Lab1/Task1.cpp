//#include <omp.h>

#include <iostream>
#include <iomanip>
#include <bitset>
#include <chrono>

#include <cstdlib>
#include <ctime>
#define OUTPUT
#define PARALLEL
#define M 10000
#define N 10000
#define N_h N/2

int matrix[M][N];
int matrix_computed[M][N/2] = {0};

int mul(int a, int b){
    return a*b;
}

int sum(int a, int b){
    return a+b;
}

long long bits(int a){
    return std::bitset<sizeof(decltype(matrix[0][0]))*8>(a).count();
}

long long sevens(int a){
    long long count = 0;

    while(a > 0){
        if(a % 10 == 7){
            count++;
        }
        a /= 10;
    }
    
    return count;
}

void print_matrix(){
    #ifdef OUTPUT
    std::cout << std::setw(3) << "";

    for(int i = 0; i < M; i++){
        std::cout << std::setw(5) << std::setfill('_') << i;
    }
    std::cout << std::endl;

    std::cout << std::setfill(' ');
    
    for(int i = 0; i < M; i++){
        std::cout << std::setw(3) << i << "|";
        for(int j = 0; j < N; j++){
             std::cout << std::setw(5) << matrix[i][j];
        }
        std::cout << std::endl;
    }
    #endif

}

void init(){
    std::srand(unsigned(std::time(0)));
    
#ifdef PARALLEL
    #pragma omp parallel for
#endif
    for(int i = 0; i < M; i++){
#ifdef PARALLEL
    #pragma omp parallel for
#endif
        for(int j = 0; j < N; j++){
             matrix[i][j] = std::rand()%M+1;
        }
    }
}

long long compute(int (*op)(int,int), long long (*amount)(int)){
    long long result = 0;
#ifdef PARALLEL
    #pragma omp parallel for
#endif
    for(int i = 0; i < M; i++){
        // Без этой штуки будет быстрее
#ifdef PARALLEL
    #pragma omp parallel for
#endif

        for(int j = 0; j < N_h; j++){
             int j_m = j*2;
             matrix_computed[i][j] = op(matrix[i][j_m], matrix[i][j_m+1]); 
             #pragma omp atomic
             result += amount(matrix_computed[i][j]);
        }
    }
    return result;
}

int main(){
    init();
    print_matrix();
    long long result;
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now();

    result = compute(mul,bits);

    end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count();
    std::cout << elapsed << " microseconds" << std::endl;
    std::cout << "result: " << result << std::endl;

    

}
