// Copyright 2020 LabradorWithShades
// 70892548+LabradorWithShades@users.noreply.github.com

#ifndef INCLUDE_STACK_HPP_
#define INCLUDE_STACK_HPP_

#include <stdexcept>
#include <utility>

template <typename T>
class Stack {
 public:
  Stack() = default;
  Stack(const Stack&) = delete;
  Stack& operator=(const Stack&) = delete;

  Stack(Stack&& other) noexcept : mp_head(other.mp_head) {
    other.mp_head = nullptr;
  }

  Stack& operator=(Stack&& other) noexcept {
    if (this == &other) return *this;
    this->~Stack();
    mp_head = other.mp_head;
    other.mp_head = nullptr;
    return *this;
  }

  ~Stack() {
    while (mp_head != nullptr) {
      StackItem* p_newHead = mp_head->mp_prev;
      delete mp_head;
      mp_head = p_newHead;
    }
    mp_head = nullptr;
  }

  template <typename... Args>
  void push_emplace(Args&&... args) {
    mp_head = new StackItem(mp_head, std::forward<Args>(args)...);
  }

  void push(const T& value) {
    mp_head = new StackItem(mp_head, value);
  }

  void push(T&& value) {
    mp_head = new StackItem(mp_head, std::forward<T>(value));
  }

  const T& head() const {
    if (mp_head == nullptr)
      throw std::runtime_error(
          "Stack runtime error: tried to get head of empty stack");
    return mp_head->m_item;
  }

  T pop() {
    if (mp_head == nullptr)
      throw std::runtime_error(
          "Stack runtime error: tried to get head of empty stack");
    T tmp = std::move(mp_head->m_item);
    StackItem* p_newHead = mp_head->mp_prev;
    delete mp_head;
    mp_head = p_newHead;
    return tmp;
  }

 private:
  class StackItem {
    friend Stack;
    template <typename... Args>
    explicit StackItem(StackItem* prev, Args... args)
        : mp_prev(prev), m_item(std::forward<Args>(args)...) {}
    StackItem(StackItem* prev, const T& item) : mp_prev(prev), m_item(item) {}
    StackItem(StackItem* prev, T&& item)
        : mp_prev(prev), m_item(std::move(item)) {}
    StackItem* mp_prev;
    T m_item;
  };

  StackItem* mp_head{nullptr};
};

#endif  // INCLUDE_STACK_HPP_
