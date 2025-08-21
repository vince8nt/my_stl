#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

import cvector;

using namespace containers;

void test_basic_operations() {
    std::cout << "=== Testing Basic Operations ===\n";
    
    // Test default constructor
    cvector<int> vec;
    std::cout << "Empty vector: size=" << vec.size() << ", capacity=" << vec.capacity() << "\n";
    
    // Test size constructor
    cvector<int> vec2(5);
    std::cout << "Size constructor(5): size=" << vec2.size() << ", capacity=" << vec2.capacity() << "\n";
    for (size_t i = 0; i < vec2.size(); ++i) {
        std::cout << "vec2[" << i << "] = " << vec2[i] << "\n";
    }
    
    // Test push_back
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    std::cout << "After push_back: size=" << vec.size() << ", capacity=" << vec.capacity() << "\n";
    
    // Test push_front
    vec.push_front(5);
    vec.push_front(1);
    std::cout << "After push_front: size=" << vec.size() << ", capacity=" << vec.capacity() << "\n";
    
    // Print all elements
    std::cout << "Elements: ";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << "\n";
    
    // Test front and back
    std::cout << "front() = " << vec.front() << ", back() = " << vec.back() << "\n";
}

void test_iterators() {
    std::cout << "\n=== Testing Iterators ===\n";
    
    cvector<int> vec;
    for (int i = 1; i <= 5; ++i) {
        vec.push_back(i * 10);
    }
    
    // Test forward iteration
    std::cout << "Forward iteration: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";
    
    // Test range-based for loop
    std::cout << "Range-based for: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
    
    // Test reverse iteration
    std::cout << "Reverse iteration: ";
    for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";
}

void test_iterator_stability() {
    std::cout << "\n=== Testing Iterator Stability ===\n";
    
    cvector<std::string> words;
    words.reserve(10);
    words.push_back("hello");
    words.push_back("world");
    words.push_back("test");
    
    auto it = words.begin() + 1;  // Points to "world"
    std::cout << "Iterator points to: '" << *it << "'\n";
    
    // Add elements at front - iterator should still point to same element
    words.push_front("first");
    std::cout << "After push_front('first'), iterator still points to: '" << *it << "'\n";
    
    // Add elements at back - iterator should still point to same element
    words.push_back("last");
    std::cout << "After push_back('last'), iterator still points to: '" << *it << "'\n";
    
    // Print all elements to verify
    std::cout << "All elements: ";
    for (const auto& word : words) {
        std::cout << "'" << word << "' ";
    }
    std::cout << "\n";
}

void test_algorithms() {
    std::cout << "\n=== Testing Standard Algorithms ===\n";
    
    cvector<int> vec;
    for (int i : {5, 2, 8, 1, 9, 3}) {
        vec.push_back(i);
    }
    
    std::cout << "Before sort: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
    
    // Test std::sort
    std::sort(vec.begin(), vec.end());
    std::cout << "After sort: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
    
    // Test std::find
    auto it = std::find(vec.begin(), vec.end(), 5);
    if (it != vec.end()) {
        auto distance = std::distance(vec.begin(), it);
        std::cout << "Found 5 at position " << distance << "\n";
    }
    
    // Test std::reverse
    std::reverse(vec.begin(), vec.end());
    std::cout << "After reverse: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}

int main() {
    try {
        std::cout << "Testing cvector with C++23 modules!\n";
        
        test_basic_operations();
        test_iterators();
        test_iterator_stability();
        test_algorithms();
        
        std::cout << "\n=== All tests completed successfully! ===\n";
        
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
