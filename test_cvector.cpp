#include <iostream>
#include <algorithm>
#include <string>
import cvector;

using namespace containers;

void test_basic_operations() {
    std::cout << "=== Testing Basic Operations ===" << std::endl;
    
    // Test default constructor
    cvector<int> vec;
    std::cout << "Empty vector: size=" << vec.size() << ", capacity=" << vec.capacity() << std::endl;
    
    // Test size constructor
    cvector<int> vec2(5);
    std::cout << "Size constructor(5): size=" << vec2.size() << ", capacity=" << vec2.capacity() << std::endl;
    for (size_t i = 0; i < vec2.size(); ++i) {
        std::cout << "vec2[" << i << "] = " << vec2[i] << std::endl;
    }
    
    // Test push_back
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    std::cout << "After push_back: size=" << vec.size() << ", capacity=" << vec.capacity() << std::endl;
    
    // Test push_front
    vec.push_front(5);
    vec.push_front(1);
    std::cout << "After push_front: size=" << vec.size() << ", capacity=" << vec.capacity() << std::endl;
    
    // Print all elements
    std::cout << "Elements: ";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
    
    // Test front and back
    std::cout << "front() = " << vec.front() << ", back() = " << vec.back() << std::endl;
}

void test_iterators() {
    std::cout << "\n=== Testing Iterators ===" << std::endl;
    
    cvector<int> vec;
    for (int i = 1; i <= 5; ++i) {
        vec.push_back(i * 10);
    }
    
    // Test forward iteration
    std::cout << "Forward iteration: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Test range-based for loop
    std::cout << "Range-based for: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    // Test reverse iteration
    std::cout << "Reverse iteration: ";
    for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void test_iterator_stability() {
    std::cout << "\n=== Testing Iterator Stability ===" << std::endl;
    
    cvector<std::string> words;
    words.reserve(10);
    words.push_back("hello");
    words.push_back("world");
    words.push_back("test");
    
    auto it = words.begin() + 1;  // Points to "world"
    std::cout << "Iterator points to: '" << *it << "'" << std::endl;
    
    // Add elements at front - iterator should still point to same element
    words.push_front("first");
    std::cout << "After push_front('first'), iterator still points to: '" << *it << "'" << std::endl;
    
    // Add elements at back - iterator should still point to same element
    words.push_back("last");
    std::cout << "After push_back('last'), iterator still points to: '" << *it << "'" << std::endl;
    
    // Print all elements to verify
    std::cout << "All elements: ";
    for (const auto& word : words) {
        std::cout << "'" << word << "' ";
    }
    std::cout << std::endl;
}

void test_pop_operations() {
    std::cout << "\n=== Testing Pop Operations ===" << std::endl;
    
    cvector<int> vec;
    for (int i = 1; i <= 5; ++i) {
        vec.push_back(i);
    }
    
    std::cout << "Initial: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    vec.pop_front();
    std::cout << "After pop_front(): ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    vec.pop_back();
    std::cout << "After pop_back(): ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

void test_algorithms() {
    std::cout << "\n=== Testing Standard Algorithms ===" << std::endl;
    
    cvector<int> vec;
    for (int i : {5, 2, 8, 1, 9, 3}) {
        vec.push_back(i);
    }
    
    std::cout << "Before sort: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    // Test std::sort
    std::sort(vec.begin(), vec.end());
    std::cout << "After sort: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    // Test std::find
    auto it = std::find(vec.begin(), vec.end(), 5);
    if (it != vec.end()) {
        auto distance = std::distance(vec.begin(), it);
        std::cout << "Found 5 at position " << distance << std::endl;
    }
    
    // Test std::reverse
    std::reverse(vec.begin(), vec.end());
    std::cout << "After reverse: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main() {
    try {
        std::cout << "Testing cvector with C++23 modules!" << std::endl;
        
        test_basic_operations();
        test_iterators();
        test_iterator_stability();
        test_pop_operations();
        test_algorithms();
        
        std::cout << "\n=== All tests completed successfully! ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
