#include <algorithm>
#include <string>
#include "segment_tree.h"
using namespace std;

template <class T>
class QueryUpdateSolver{
    public:
        virtual ~QueryUpdateSolver<T>(){};
        /**
         * @brief Create a solver based on the type and the aggregation operation.
         * The solver receives updates on an array and it is responsible to
         * solve aggregation queries perfomed on intervals of the most recent snapshot of the array.
         * 
         * @param type type of solver (fast or slow)
         * @param op operation (min or max)
         * @param numbers arrays of values
         * @param length lenght of array
         * @return QueryUpdateSolver<T>* solver
         */
        static QueryUpdateSolver<T>* create(string type, string op, T* numbers, int length);

        /**
         * @brief Returned aggregation on interval `[left, right]`
         * 
         * @param left leftmost interval index
         * @param right rightmost interval index
         * @return T aggregated result
         */
        virtual T query(int left, int right) = 0;

        /**
         * @brief Inform about on array update
         * 
         * @param pos position of update
         * @param value new value
         */
        virtual void update(int pos, T value) = 0;
};

template <class T>
class SlowQueryUpdateSolver: public QueryUpdateSolver<T> {
    private:
        T* numbers;
        T* (*reduce)(T*, T*);
    public:
    /**
     * @brief This solver updates a copy of the array and answer queries by iterating on intervals.
     * 
     * @param numbers arrays of values
     * @param length length of array
     * @param reduce reduce function
     */
        SlowQueryUpdateSolver (T* numbers, int length, T* (*reduce)(T*, T*)){
            this->numbers = new T[length];
            for(int i=0; i<length; i++)
                this->numbers[i] = numbers[i];
            this->reduce = reduce;
        }
        ~SlowQueryUpdateSolver() {
            delete[] numbers;
        }
        T query(int left, int right){
            return *reduce(numbers+left, numbers+right+1);
        }
        void update(int pos, T value){
            numbers[pos] = value;
        }
};

template <class T>
class FastQueryUpdateSolver: public QueryUpdateSolver<T> {
    private:
        const T& (*reduce)(const T&, const T&);
        Node<T> *root = NULL;
    public:
        /**
         * @brief This solver perform efficient updates and queries with a segment tree.
         * 
         * @param numbers arrays of values
         * @param length length of array
         * @param reduce reduce function 
         */
        FastQueryUpdateSolver (T* numbers, int length, const T& (*reduce)(const T&, const T&)) {
            root = new Node<T>(0, length-1, reduce);
            for (int i=0; i<length; i++)
                root->update(i, numbers[i]);
        }
        ~FastQueryUpdateSolver() {
            delete root;
        }
        T query(int left, int right){
            return root->query(left, right);
        }
        void update(int pos, T value){
            root->update(pos, value);
        }
};

template <class T>
QueryUpdateSolver<T>* QueryUpdateSolver<T>::create(string type, string op, T* numbers, int length){
    if (type == "slow"){
        T* (*reduce)(T*, T*);

        if (op == "max")
            reduce = max_element;
        else if (op == "min")
            reduce = min_element;
        else
            throw invalid_argument("op non recognized");
            
        return new SlowQueryUpdateSolver<T>(numbers, length, reduce);
    }
    else if (type == "fast") {
        const T& (*reduce)(const T&, const T&);

        if (op == "max")
            reduce = max;
        else if (op == "min")
            reduce = min;
        else
            throw invalid_argument("op non recognized");
        
        return new FastQueryUpdateSolver<T>(numbers, length, reduce);    
    }
    throw invalid_argument("type non recognized");
}