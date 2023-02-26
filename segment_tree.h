#include <cstddef>

template <class T>
class Node {
    private:
        T value;
        int left;
        int mid;
        int right;
        Node<T> *left_child = NULL;
        Node<T> *right_child = NULL;
        const T& (*reduce)(const T&, const T&);
    public:
        /**
         * @brief Construct a new Node of the segment tree (https://en.wikipedia.org/wiki/Segment_tree).
         * This node holds the aggregated `value` of the values between positions `left` and `right` of the array.
         * It also holds a pointer to two children nodes that respectively hold the aggregation between values inside
         * - `[left, mid]`
         * - `(mid, right]`
         * where `mid` is the floor mean of `left` and `right`.
         * 
         * @param left leftmost index covered by this node
         * @param right rightmost index covered by this node
         * @param reduce aggregation operator
         */
        Node(int left, int right, const T& (*reduce)(const T&, const T&)) {
            this->left = left;
            this->right = right;
            this->reduce = reduce;
            this->mid = (left+right) / 2;
            if (left < right){
                left_child = new Node<T>(left, mid, reduce);
                right_child = new Node<T>(mid+1, right, reduce);
            }
        }
        ~Node() {
            delete left_child;
            delete right_child;
        }
        /**
         * @brief Update node and its children with new value inserted in position `pos` of array
         * 
         * @param pos position of new value
         * @param value new value
         * @return T new aggregated node value
         */
        T update(int pos, T value) {
            if (left_child != NULL && right_child != NULL){
                if (pos <= mid) {
                    left_child->update(pos, value);
                }
                else {
                    right_child->update(pos, value);
                }
                this->value = reduce(left_child->query(), right_child->query());
            }
            else
                this->value = value;
            return this->value;
        }

        /**
         * @brief Return aggregation of values between position indices `left` and `right`
         * 
         * @param left left index position
         * @param right right index position
         * @return T aggregated position
         */
        T query(int left, int right) {
            if (left == this->left && right == this->right)
                return value;
            if (right <= mid) {
                return left_child->query(left, right);
            }
            if (left > mid) {
                return right_child->query(left, right);
            }
            return reduce(left_child->query(left, mid), right_child->query(mid+1, right));
        }

        /**
         * @brief Get aggregated value of this node
         * 
         * @return aggregated value of this node
         */
        T query() {
            return value;
        }
};