import std;
import cvector;

using namespace containers;

void test_basic_operations() {
    std::println("=== Testing Basic Operations ===");
    
    // Test default constructor
    cvector<int> vec;
    std::println("Empty vector: size={}, capacity={}", vec.size(), vec.capacity());
    
    // Test size constructor
    cvector<int> vec2(5);
    std::println("Size constructor(5): size={}, capacity={}", vec2.size(), vec2.capacity());
    for (size_t i = 0; i < vec2.size(); ++i) {
        std::println("vec2[{}] = {}", i, vec2[i]);
    }
    
    // Test push_back
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    std::println("After push_back: size={}, capacity={}", vec.size(), vec.capacity());
    
    // Test push_front
    vec.push_front(5);
    vec.push_front(1);
    std::println("After push_front: size={}, capacity={}", vec.size(), vec.capacity());
    
    // Print all elements
    std::print("Elements: ");
    for (size_t i = 0; i < vec.size(); ++i) {
        std::print("{} ", vec[i]);
    }
    std::println("");
    
    // Test front and back
    std::println("front() = {}, back() = {}", vec.front(), vec.back());
}

void test_iterators() {
    std::println("\n=== Testing Iterators ===");
    
    cvector<int> vec;
    for (int i = 1; i <= 5; ++i) {
        vec.push_back(i * 10);
    }
    
    // Test forward iteration
    std::print("Forward iteration: ");
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::print("{} ", *it);
    }
    std::println("");
    
    // Test range-based for loop
    std::print("Range-based for: ");
    for (const auto& elem : vec) {
        std::print("{} ", elem);
    }
    std::println("");
    
    // Test reverse iteration
    std::print("Reverse iteration: ");
    for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
        std::print("{} ", *it);
    }
    std::println("");
}

void test_iterator_stability() {
    std::println("\n=== Testing Iterator Stability ===");
    
    cvector<std::string> words;
    words.reserve(10);
    words.push_back("hello");
    words.push_back("world");
    words.push_back("test");
    
    auto it = words.begin() + 1;  // Points to "world"
    std::println("Iterator points to: '{}'", *it);
    
    // Add elements at front - iterator should still point to same element
    words.push_front("first");
    std::println("After push_front('first'), iterator still points to: '{}'", *it);
    
    // Add elements at back - iterator should still point to same element
    words.push_back("last");
    std::println("After push_back('last'), iterator still points to: '{}'", *it);
    
    // Print all elements to verify
    std::print("All elements: ");
    for (const auto& word : words) {
        std::print("'{}' ", word);
    }
    std::println("");
}

void test_pop_operations() {
    std::println("\n=== Testing Pop Operations ===");
    
    cvector<int> vec;
    for (int i = 1; i <= 5; ++i) {
        vec.push_back(i);
    }
    
    std::print("Initial: ");
    for (const auto& elem : vec) {
        std::print("{} ", elem);
    }
    std::println("");
    
    vec.pop_front();
    std::print("After pop_front(): ");
    for (const auto& elem : vec) {
        std::print("{} ", elem);
    }
    std::println("");
    
    vec.pop_back();
    std::print("After pop_back(): ");
    for (const auto& elem : vec) {
        std::print("{} ", elem);
    }
    std::println("");
}

void test_algorithms() {
    std::println("\n=== Testing Standard Algorithms ===");
    
    cvector<int> vec;
    for (int i : {5, 2, 8, 1, 9, 3}) {
        vec.push_back(i);
    }
    
    std::print("Before sort: ");
    for (const auto& elem : vec) {
        std::print("{} ", elem);
    }
    std::println("");
    
    // Test std::sort
    std::sort(vec.begin(), vec.end());
    std::print("After sort: ");
    for (const auto& elem : vec) {
        std::print("{} ", elem);
    }
    std::println("");
    
    // Test std::find
    auto it = std::find(vec.begin(), vec.end(), 5);
    if (it != vec.end()) {
        auto distance = std::distance(vec.begin(), it);
        std::println("Found 5 at position {}", distance);
    }
    
    // Test std::reverse
    std::reverse(vec.begin(), vec.end());
    std::print("After reverse: ");
    for (const auto& elem : vec) {
        std::print("{} ", elem);
    }
    std::println("");
}

int main() {
    try {
        std::println("Testing cvector with C++23 modules!");
        
        test_basic_operations();
        test_iterators();
        test_iterator_stability();
        test_pop_operations();
        test_algorithms();
        
        std::println("\n=== All tests completed successfully! ===");
        
    } catch (const std::exception& e) {
        std::println("Error: {}", e.what());
        return 1;
    }
    
    return 0;
}
