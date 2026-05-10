#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

class Stack {
    static constexpr std::size_t MIN_CAPACITY = 8;
    static constexpr std::size_t GROW_FACTOR = 2;

    std::unique_ptr<std::unique_ptr<std::string>[]> items_;
    std::size_t size_;
    std::size_t capacity_;

    void resize(std::size_t new_capacity) {
        if (new_capacity < MIN_CAPACITY) {
            new_capacity = MIN_CAPACITY;
        }
        auto new_items = std::make_unique<std::unique_ptr<std::string>[]>(new_capacity);
        for (std::size_t i = 0; i < size_; ++i) {
            new_items[i] = std::move(items_[i]);
        }
        items_ = std::move(new_items);
        capacity_ = new_capacity;
    }

public:
    Stack()
        : items_(std::make_unique<std::unique_ptr<std::string>[]>(MIN_CAPACITY)),
          size_(0),
          capacity_(MIN_CAPACITY) {}

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void push(const std::string& str) {
        if (size_ == capacity_) {
            resize(capacity_ * GROW_FACTOR);
        }
        items_[size_++] = std::make_unique<std::string>(str);
    }

    std::string pop() {
        if (size_ == 0) {
            throw std::underflow_error("pop from empty stack");
        }
        std::string val = std::move(*items_[size_ - 1]);
        items_[--size_].reset();
        if (capacity_ > MIN_CAPACITY && size_ <= capacity_ / 4) {
            resize(capacity_ / GROW_FACTOR);
        }
        return val;
    }

    const std::string& peek() const {
        if (size_ == 0) {
            throw std::underflow_error("peek on empty stack");
        }
        return *items_[size_ - 1];
    }

    std::size_t size() const { return size_; }

    bool isEmpty() const { return size_ == 0; }
};

int main() {
    Stack s;

    try {
        s.pop();
        std::cout << "FAIL: expected exception on pop from empty\n";
    } catch (const std::underflow_error&) {
        std::cout << "PASS: pop from empty throws\n";
    }

    try {
        s.peek();
        std::cout << "FAIL: expected exception on peek from empty\n";
    } catch (const std::underflow_error&) {
        std::cout << "PASS: peek from empty throws\n";
    }

    s.push("hello");
    s.push("world");

    std::cout << (s.peek() == "world" ? "PASS" : "FAIL") << ": peek returns top\n";
    std::cout << (s.size() == 2 ? "PASS" : "FAIL") << ": size is 2\n";
    std::cout << (!s.isEmpty() ? "PASS" : "FAIL") << ": stack is not empty\n";

    std::string val = s.pop();
    std::cout << (val == "world" ? "PASS" : "FAIL") << ": pop returns 'world'\n";

    val = s.pop();
    std::cout << (val == "hello" ? "PASS" : "FAIL") << ": pop returns 'hello' (LIFO)\n";

    std::cout << (s.isEmpty() ? "PASS" : "FAIL") << ": stack is empty after all pops\n";

    for (int i = 0; i < 20; ++i) {
        s.push("item" + std::to_string(i));
    }
    std::cout << (s.size() == 20 ? "PASS" : "FAIL") << ": size is 20 after expansion\n";
    std::cout << (s.pop() == "item19" ? "PASS" : "FAIL") << ": pop after expansion returns correct item\n";

    return 0;
}
