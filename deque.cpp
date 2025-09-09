template <typename T>
class deque {
    private:
        // nodes have a capacity of 4-256 elements
        // nodes maintain an index to be able to reference their positionin the circular buffer
        // we use an index instead of a pointer since the circular buffer can be resized
        constexpr static const size_t node_size_ = std::max(4, 256 / sizeof(T));
        struct node {
            size_t base_index; // the (potentially offsetted) index of this node's first element
            array<T, node_size_> data;
        };
        
        // circular vector of node pointers
        // the true index of an element is it's placement within the list of all nodes
        // these indices will change when inserting/removing nodes from the front of the cvector
        // node_index_offset_ maintains the offset for node.index
        // node_index_offset_ relies on unsigned integer overflow to wrap around
        cvector<node*> data_;
        size_t node_index_offset_;

        // the true index of the front element within the list of nodes
        // divide by node_size_ to get the node index
        // use modulo to get the element index within the node
        size_t front_offset_;

        // the number of elements in the deque
        size_t size_;

    public:
    // circular buffer of nodes
        // front offset and back offset are the front and back indices of elements within the nodes
        // data_ can contain unused nodes at the beginning and end (to be reused later)
        deque() : node_index_offset_(0), front_offset_(0), size_(0) {}

        ~deque() {
            clear();
            shrink_to_fit();
        }

        // clear the deque, but don't free the nodes
        void clear() {
            if constexpr (!std::is_trivially_destructible_v<T>) {
                for (size_t i = front_offset_; i < front_offset_ + size_; i++) {
                    data_[i / node_size_][i % node_size_].~T();
                }
            }
            front_offset_ = 0;
            size_ = 0;
            // don't reset node_index_offset_
        }

        // shrink the deque by removing unused nodes from the front and back
        void shrink_to_fit() {
            if (size_ == 0)
                front_offset_ = 0;
            while (front_offset_ >= node_size_) {
                delete data_.front();
                data_.pop_front();
                front_offset_ -= node_size_;
                node_index_offset_ -= node_size_;
            }
            while (front_offset_ + size_ > data_.size() * node_size_) {
                delete data_.back();
                data_.pop_back();
                front_offset_ += node_size_;
                node_index_offset_ += node_size_;
            }
            data_.shrink_to_fit();
        }


        // push to back, only allocate new nodes if necessary
        void push_back(const T& value) {
            back_offset_++;
            size_t data_index = back_offset_ / node_size_;
            uint8_t node_offset = back_offset_ % node_size_;
            if (data_index >= data_.size()) { // no node allocated for this index
                if (front_offset_ >= node_size_) { // unused node at the front can be used
                    auto node_ptr = data_.front();
                    data_.pop_front();
                    data_.push_back(node_ptr);
                    front_offset_ -= node_size_;
                    back_offset_ -= node_size_;
                    data_index--;
                    data_[data_index] = node_ptr;
                } else { // no unused node at the front, so we need to allocate a new node
                    data_.push_back(new node());
                }
            }
            data_[data_index][node_offset] = value;
            size_++;
        }

        // push to front, only allocate new nodes if necessary
        void push_front(const T& value) {
            if (front_offset_ == 0) { // no node allocated for this index
                if (back_offset_ / node_size_ < data_.size() - 1) { // unused node at the back can be used
                    auto node_ptr = data_.back();
                    data_.pop_back();
                    data_.push_front(node_ptr);
                } else { // no unused node at the back, so we need to allocate a new node
                    data_.push_front(new node());
                }
                back_offset_ += node_size_;
                front_offset_ = node_size_ - 1;
            }
            else {
                front_offset--;
            }
            data_[front_offset_ / node_size_][front_offset_ % node_size_] = value;
            size_++;
        }
        
        // pop from back, but don't free the node
        void pop_back() {
            if (size_ > 0) {
                if constexpr (!std::is_trivially_destructible_v<T>) {
                    data_[back_offset_ / node_size_][back_offset_ % node_size_].~T();
                }
                back_offset_--;
                size_--;
            }
        }

        // pop from front, but don't free the node
        void pop_front() {
            if (size_ > 0) {
                if constexpr (!std::is_trivially_destructible_v<T>) {
                    data_[front_offset_ / node_size_][front_offset_ % node_size_].~T();
                }
                front_offset_++;
                size_--;
            }
        }

        // get the front element
        T& front() {
            return data_[front_offset_ / node_size_][front_offset_ % node_size_];
        }
        
        // get the back element
        T& back() {
            return data_[(back_offset_) / node_size_][(back_offset_) % node_size_];
        }
        
        // get the element at the given index
        T& operator[](size_t index) {
            index += front_offset_;
            return data_[index / node_size_][index % node_size_];
        }
        
        
};



