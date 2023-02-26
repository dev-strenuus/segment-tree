#include <iostream>
#include <random>
#include <cassert>
#include "solver.h"
using namespace std;

// This program tests that fast and slow solvers return same answers to same queries on same arrays.

template <class T>
void full_assert(int length, QueryUpdateSolver<T>* s1, QueryUpdateSolver<T>* s2){
    for (int i=0; i<length; i++)
        for (int j=i; j<length; j++){
            assert (s1->query(i, j) == s2->query(i, j));
        }
}
 
int main(){

    int N = 100;
    int U = 200;
    int v[100];

    generate(v, v+N, rand);

    for (string o: {"max", "min"}) {
        cout << "Running tests for operator <" << o << "> ..." << endl;
        QueryUpdateSolver<int>* fastSolver = QueryUpdateSolver<int>::create("fast", o, v, N);
        QueryUpdateSolver<int>* slowSolver = QueryUpdateSolver<int>::create("slow", o, v, N);
        
        full_assert(N, slowSolver, fastSolver);

        for(int u=0; u<U; u++){
            int value = rand();
            int pos = rand() % N;
            slowSolver->update(pos, value);
            fastSolver->update(pos, value);

            full_assert(N, slowSolver, fastSolver);
        }
        cout << "... tests passed!" <<endl;
        delete fastSolver;
        delete slowSolver;
    }
    cout << "All tests passed!" <<endl;
    return 0;
}