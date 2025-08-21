module;

// Traditional includes in global module fragment
#include <type_traits>
#include <memory>
#include <cstdlib>
#include <iterator>
#include <bit>
#include <new>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <vector>

export module cvector;

export namespace containers {

// circular vector
// capacity is always a power of 2 (or 0)
template <typename T>
class cvector {
    private:
        T* data_;
        size_t size_;
        size_t capacity_;
        size_t head_;

        // grow capacity to new_capacity
        // assume new_capacity is a power of 2 and is greater than current capacity
        inline void grow_capacity(size_t new_capacity) {
            if constexpr (std::is_trivially_copyable_v<T>) {
                grow_capacity_trivial(new_capacity);
            } else {
                grow_capacity_non_trivial(new_capacity);
            }
        }

        // For trivially copyable types - can use realloc/memcpy
        inline void grow_capacity_trivial(size_t new_capacity) {
            if (head_ + size_ <= capacity_) {
                // use realloc since data doesn't wrap around
                data_ = (T*)realloc(data_, new_capacity * sizeof(T));
            } else {
                // use memcpy since data wraps around
                T* new_data = (T*)malloc(new_capacity * sizeof(T));
                memcpy(new_data, data_ + head_, (capacity_ - head_) * sizeof(T));
                memcpy(new_data + (capacity_ - head_), data_, (head_ + size_ - capacity_) * sizeof(T));
                free(data_);
                data_ = new_data;
                head_ = 0;
            }
            capacity_ = new_capacity;
        }

        // For non-trivially copyable types - use proper construction/destruction
        inline void grow_capacity_non_trivial(size_t new_capacity) {
            // Use malloc for MinGW compatibility (aligned_alloc not always available)
            T* new_data = static_cast<T*>(std::malloc(new_capacity * sizeof(T)));
            
            // Move/copy construct all existing elements to new location
            for (size_t i = 0; i < size_; ++i) {
                size_t old_index = (head_ + i) & (capacity_ - 1);
                if constexpr (std::is_move_constructible_v<T>) {
                    new (new_data + i) T(std::move(data_[old_index]));
                } else {
                    new (new_data + i) T(data_[old_index]);
                }
            }
            
            // Destroy all objects in old memory
            if constexpr (!std::is_trivially_destructible_v<T>) {
                for (size_t i = 0; i < size_; ++i) {
                    size_t old_index = (head_ + i) & (capacity_ - 1);
                    data_[old_index].~T();
                }
            }
            
            std::free(data_);
            data_ = new_data;
            head_ = 0;
            capacity_ = new_capacity;
        }

    public:
        cvector() : data_(nullptr), size_(0), capacity_(0), head_(0) {}
        
        cvector(size_t initial_size) : data_(nullptr), size_(0), capacity_(0), head_(0) {
            if (initial_size > 0) {
                capacity_ = std::bit_ceil(initial_size);
                data_ = static_cast<T*>(std::malloc(capacity_ * sizeof(T)));
                
                if (!data_) {
                    throw std::bad_alloc();
                }
                
                // Default construct initial_size elements
                try {
                    while (size_ < initial_size) {
                        new (data_ + size_) T{};  // Value initialization
                        ++size_;
                    }
                } catch (...) {
                    // Clean up any already-constructed objects
                    for (size_t i = 0; i < size_; ++i) {
                        data_[i].~T();
                    }
                    std::free(data_);
                    data_ = nullptr;
                    size_ = 0;
                    capacity_ = 0;
                    throw;
                }
            }
        }

        ~cvector() {
            clear();
            std::free(data_);
        }

        void reserve(size_t new_capacity) {
            if (new_capacity > capacity_) {
                grow_capacity(std::bit_ceil(new_capacity));
            }
        }

        void push_back(const T& value) {
            if (size_ >= capacity_) {
                size_t new_capacity = capacity_ ? capacity_ * 2 : 1;
                grow_capacity(new_capacity);
            }
            
            if constexpr (std::is_trivially_constructible_v<T>) {
                data_[(head_ + size_) & (capacity_ - 1)] = value;
            } else {
                new (data_ + ((head_ + size_) & (capacity_ - 1))) T(value);
            }
            size_++;
        }

        void push_front(const T& value) {
            if (size_ >= capacity_) {
                size_t new_capacity = capacity_ ? capacity_ * 2 : 1;
                grow_capacity(new_capacity);
            }
            head_ = (head_ - 1) & (capacity_ - 1);
            
            if constexpr (std::is_trivially_constructible_v<T>) {
                data_[head_] = value;
            } else {
                new (data_ + head_) T(value);
            }
            size_++;
        }

        void pop_back() {
            if (size_ == 0) {
                throw std::out_of_range("cvector::pop_back: size is 0");
            }
            if constexpr (!std::is_trivially_destructible_v<T>) {
                data_[(head_ + size_ - 1) & (capacity_ - 1)].~T();
            }
            size_--;
        }

        void pop_front() {
            if (size_ == 0) {
                throw std::out_of_range("cvector::pop_front: size is 0");
            }
            if constexpr (!std::is_trivially_destructible_v<T>) {
                data_[head_].~T();
            }
            head_ = (head_ + 1) & (capacity_ - 1);
            size_--;
        }

        T& operator[](size_t index) {
            return data_[(head_ + index) & (capacity_ - 1)];
        }
        const T& operator[](size_t index) const {
            return data_[(head_ + index) & (capacity_ - 1)];
        }

        T& front() {
            return data_[head_];
        }
        const T& front() const {
            return data_[head_];
        }

        T& back() {
            return data_[(head_ + size_ - 1) & (capacity_ - 1)];
        }
        const T& back() const {
            return data_[(head_ + size_ - 1) & (capacity_ - 1)];
        }

        size_t size() const {
            return size_;
        }
        size_t capacity() const {
            return capacity_;
        }
        bool empty() const {
            return size_ == 0;
        }
        
        void clear() {
            // Destroy all objects for non-trivially destructible types
            if constexpr (!std::is_trivially_destructible_v<T>) {
                for (size_t i = 0; i < size_; ++i) {
                    size_t index = (head_ + i) & (capacity_ - 1);
                    data_[index].~T();
                }
            }
            size_ = 0;
            head_ = 0;  // Reset head for cleaner state
        }

        // Forward declaration
        class iterator;
        class const_iterator;
        
        // Friend declarations
        friend class iterator;
        friend class const_iterator;

        // Iterator implementation
        class iterator {
        private:
            cvector* container_;
            size_t physical_index_;  // Physical position in the buffer (remains stable)
            
        public:
            // Iterator traits
            using iterator_category = std::random_access_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;
            
            iterator(cvector* container, size_t physical_index) 
                : container_(container), physical_index_(physical_index) {}
            
            // Convert physical index to logical position (for distance calculations)
            difference_type logical_position() const {
                // Calculate logical distance from head
                if (physical_index_ >= container_->head_) {
                    return static_cast<difference_type>(physical_index_ - container_->head_);
                } else {
                    // Wrapped around case
                    return static_cast<difference_type>(container_->capacity_ - container_->head_ + physical_index_);
                }
            }
            
            // Dereference
            reference operator*() const { 
                return container_->data_[physical_index_]; 
            }
            pointer operator->() const { 
                return &container_->data_[physical_index_]; 
            }
            
            // Increment/Decrement (physical, with wrapping)
            iterator& operator++() { 
                physical_index_ = (physical_index_ + 1) & (container_->capacity_ - 1); 
                return *this; 
            }
            iterator operator++(int) { 
                iterator tmp = *this; 
                physical_index_ = (physical_index_ + 1) & (container_->capacity_ - 1); 
                return tmp; 
            }
            iterator& operator--() { 
                physical_index_ = (physical_index_ - 1) & (container_->capacity_ - 1); 
                return *this; 
            }
            iterator operator--(int) { 
                iterator tmp = *this; 
                physical_index_ = (physical_index_ - 1) & (container_->capacity_ - 1); 
                return tmp; 
            }
            
            // Random access
            iterator& operator+=(difference_type n) { 
                physical_index_ = (physical_index_ + n) & (container_->capacity_ - 1);
                return *this; 
            }
            iterator& operator-=(difference_type n) { 
                physical_index_ = (physical_index_ - n) & (container_->capacity_ - 1);
                return *this; 
            }
            iterator operator+(difference_type n) const { 
                return iterator(container_, (physical_index_ + n) & (container_->capacity_ - 1)); 
            }
            iterator operator-(difference_type n) const { 
                return iterator(container_, (physical_index_ - n) & (container_->capacity_ - 1)); 
            }
            
            // Comparison
            bool operator==(const iterator& other) const { 
                return container_ == other.container_ && physical_index_ == other.physical_index_; 
            }
            bool operator!=(const iterator& other) const { return !(*this == other); }
            
            // Ordering comparisons based on logical position
            bool operator<(const iterator& other) const { 
                return logical_position() < other.logical_position(); 
            }
            bool operator<=(const iterator& other) const { 
                return logical_position() <= other.logical_position(); 
            }
            bool operator>(const iterator& other) const { 
                return logical_position() > other.logical_position(); 
            }
            bool operator>=(const iterator& other) const { 
                return logical_position() >= other.logical_position(); 
            }
            
            difference_type operator-(const iterator& other) const { 
                return logical_position() - other.logical_position(); 
            }
            
            // Array access
            reference operator[](difference_type n) const {
                size_t target_physical = (physical_index_ + n) & (container_->capacity_ - 1);
                return container_->data_[target_physical];
            }
            
            // Friend access for const_iterator
            friend class const_iterator;
        };
        
        class const_iterator {
        private:
            const cvector* container_;
            size_t physical_index_;
            
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = const T*;
            using reference = const T&;
            
            const_iterator(const cvector* container, size_t physical_index) 
                : container_(container), physical_index_(physical_index) {}
                
            // Convert from non-const iterator
            const_iterator(const iterator& it) 
                : container_(it.container_), physical_index_(it.physical_index_) {}
            
            difference_type logical_position() const {
                if (physical_index_ >= container_->head_) {
                    return static_cast<difference_type>(physical_index_ - container_->head_);
                } else {
                    return static_cast<difference_type>(container_->capacity_ - container_->head_ + physical_index_);
                }
            }
            
            reference operator*() const { return container_->data_[physical_index_]; }
            pointer operator->() const { return &container_->data_[physical_index_]; }
            
            const_iterator& operator++() { 
                physical_index_ = (physical_index_ + 1) & (container_->capacity_ - 1); 
                return *this; 
            }
            const_iterator operator++(int) { 
                const_iterator tmp = *this; 
                physical_index_ = (physical_index_ + 1) & (container_->capacity_ - 1); 
                return tmp; 
            }
            const_iterator& operator--() { 
                physical_index_ = (physical_index_ - 1) & (container_->capacity_ - 1); 
                return *this; 
            }
            const_iterator operator--(int) { 
                const_iterator tmp = *this; 
                physical_index_ = (physical_index_ - 1) & (container_->capacity_ - 1); 
                return tmp; 
            }
            
            const_iterator& operator+=(difference_type n) { 
                physical_index_ = (physical_index_ + n) & (container_->capacity_ - 1);
                return *this; 
            }
            const_iterator& operator-=(difference_type n) { 
                physical_index_ = (physical_index_ - n) & (container_->capacity_ - 1);
                return *this; 
            }
            const_iterator operator+(difference_type n) const { 
                return const_iterator(container_, (physical_index_ + n) & (container_->capacity_ - 1)); 
            }
            const_iterator operator-(difference_type n) const { 
                return const_iterator(container_, (physical_index_ - n) & (container_->capacity_ - 1)); 
            }
            
            bool operator==(const const_iterator& other) const { 
                return container_ == other.container_ && physical_index_ == other.physical_index_; 
            }
            bool operator!=(const const_iterator& other) const { return !(*this == other); }
            bool operator<(const const_iterator& other) const { 
                return logical_position() < other.logical_position(); 
            }
            bool operator<=(const const_iterator& other) const { 
                return logical_position() <= other.logical_position(); 
            }
            bool operator>(const const_iterator& other) const { 
                return logical_position() > other.logical_position(); 
            }
            bool operator>=(const const_iterator& other) const { 
                return logical_position() >= other.logical_position(); 
            }
            
            difference_type operator-(const const_iterator& other) const { 
                return logical_position() - other.logical_position(); 
            }
            
            reference operator[](difference_type n) const {
                size_t target_physical = (physical_index_ + n) & (container_->capacity_ - 1);
                return container_->data_[target_physical];
            }
        };

        // Iterator methods
        iterator begin() { return iterator(this, head_); }
        const_iterator begin() const { return const_iterator(this, head_); }
        const_iterator cbegin() const { return const_iterator(this, head_); }
        
        iterator end() { return iterator(this, (head_ + size_) & (capacity_ - 1)); }
        const_iterator end() const { return const_iterator(this, (head_ + size_) & (capacity_ - 1)); }
        const_iterator cend() const { return const_iterator(this, (head_ + size_) & (capacity_ - 1)); }
        
        // Reverse iterators
        std::reverse_iterator<iterator> rbegin() { return std::reverse_iterator<iterator>(end()); }
        std::reverse_iterator<const_iterator> rbegin() const { return std::reverse_iterator<const_iterator>(end()); }
        std::reverse_iterator<const_iterator> crbegin() const { return std::reverse_iterator<const_iterator>(end()); }
        
        std::reverse_iterator<iterator> rend() { return std::reverse_iterator<iterator>(begin()); }
        std::reverse_iterator<const_iterator> rend() const { return std::reverse_iterator<const_iterator>(begin()); }
        std::reverse_iterator<const_iterator> crend() const { return std::reverse_iterator<const_iterator>(begin()); }
};

} // namespace containers
