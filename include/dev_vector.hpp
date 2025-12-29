#pragma once //header guard
#include <cstddef>
#include <new> //operator new
#include <utility> //std::move
#include <type_traits> //std::is_nothrow_move_constructible_v
#include <memory>     // std::uninitialized_move, std::uninitialized_copy
#include <stdexcept>
#include <initializer_list>

namespace dev {

    template<typename T>
    class vector {
        private:
            T* m_data;
            std::size_t m_size;
            std::size_t m_capacity;

            static void relocate_into_uninitialized(T* dest, T* src, std::size_t count) { // static refers to belongs to class and only relies on the params passed, does not depend on object state
                if constexpr (std::is_nothrow_move_constructible_v<T>) { //constexpr -> at compile time decide which branch of the if exists and only use that in runtime; for example, if the first condition passes, the else is never compiled and it is not type checked.
                    std::uninitialized_move(src, src+count, dest);
                } else {
                    std::uninitialized_copy(src, src+count, dest);
                }
            }

        public:
            vector()
                : m_data(static_cast<T*>(operator new(sizeof(T)))), m_size(0), m_capacity(1) {}

            vector(const vector& other)
                : m_data(nullptr), m_size(0), m_capacity(0)
            {
                if(other.m_size == 0) return;

                m_capacity = other.m_capacity;
                m_data = static_cast<T*>(operator new(sizeof(T) * m_capacity));

                try {
                    std::uninitialized_copy(other.m_data, other.m_data + other.m_size, m_data);
                    m_size = other.m_size;
                } catch (...) {
                    operator delete(m_data);
                    m_data = nullptr;
                    m_capacity = 0;
                    throw;
                }
            }

            vector(vector&& other)
                :m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity)
            {
                other.m_data = nullptr;
                other.m_size = 0;
                other.m_capacity = 0;
            }

            ~vector() {
                if(m_data) {
                    for(std::size_t i =0; i < m_size;i++) {
                        std::destroy_at(m_data + i);
                    }
                    operator delete(m_data);
                }
            }

            bool empty() const {
                return m_size == 0;
            }

            std::size_t size() const {
                return m_size;
            }

            template<typename U>
            void push_back(U&& value) {
                if(m_size < m_capacity) {
                    std::construct_at(m_data+m_size, std::forward<U>(value));
                    ++m_size;
                    return;
                }

                std::size_t new_capacity = (m_capacity == 0) ? 1 : (m_capacity * 2);

                T* new_data = static_cast<T*>(operator new(sizeof(T) * new_capacity));

                std::size_t constructed = 0;

                try {
                    std::construct_at(new_data + m_size, std::forward<U>(value)); //forward helps with deciding whether to copy or move based on rvalue or lvalue

                    relocate_into_uninitialized(new_data, m_data, m_size);
                    constructed = m_size + 1;
                } catch (...) {
                    for(std::size_t i =0; i < constructed; ++i) {
                    std::destroy_at(new_data + i);
                    }
                    operator delete(new_data);
                    throw;
                }

                for(std::size_t i = 0; i < m_size; ++i) {
                    std::destroy_at(m_data + i);
                }

                operator delete(m_data);

                m_data = new_data;
                m_capacity = new_capacity;
                
                ++m_size;

            }

            T& operator[](std::size_t idx) {
                return m_data[idx];
            }

            const T& operator[](std::size_t idx) const {
                return m_data[idx];
            }

            void swap(vector& other) noexcept
            {
                std::swap(m_size, other.m_size);
                std::swap(m_capacity, other.m_capacity);
                std::swap(m_data, other.m_data);
            }

            vector& operator=(const vector& other) {
                vector temp(other);

                swap(temp);
                return *this;
            }

            vector& operator=(vector&& other) noexcept
            {
                if(this == &other) return *this;

                for(std::size_t i =0 ; i < m_size; ++i) {
                    std::destroy_at(m_data+i);
                }

                operator delete(m_data);

                m_data = other.m_data;
                m_size= other.m_size;
                m_capacity = other.m_capacity;

                other.m_data = nullptr;
                other.m_size = 0;
                other.m_capacity = 0;

                return *this;
            }

            T* begin() {return m_data;}

            const T* begin() const {return m_data;}

            T* end() {return m_data+m_size;}
            const T* end() const {return m_data+m_size;}

            T& front() {return m_data[0];}
            const T& front() const
            {
                return m_data[0];
            }

            T& back() {return m_data[m_size - 1];}
            const T& back() const
            {
                return m_data[m_size - 1];
            }

            T& at(std::size_t idx) {
                if(idx>=m_size) {
                    throw std::out_of_range("The requested index is out of range !");
                }
                return m_data[idx];
            }

            const T& at(std::size_t idx) const {
                if(idx>=m_size) {
                    throw std::out_of_range("The requested index is out of range !");
                }
                return m_data[idx];
            }

            std::size_t capacity() const {return m_capacity; }

            void pop_back() {
                std::destroy_at(m_data + m_size - 1);
                --m_size;
            }

            void reserve(std::size_t new_capacity)
            {
                if(new_capacity <= m_capacity) return;

                T* new_data = static_cast<T*>(operator new(sizeof(T) * new_capacity));

                try {
                    relocate_into_uninitialized(new_data, m_data, m_size);
                } catch(...) {
                    operator delete(new_data);
                    throw;
                }

                for(std::size_t i; i < m_size; ++i) {
                    std::destroy_at(m_data+i);
                }

                operator delete(m_data);

                m_data = new_data;
                m_capacity = new_capacity;
            }

            void resize(std::size_t new_size)
            {
                if(new_size == m_size) return;

                if(new_size < m_size) {
                    for(std::size_t i = new_size; i < m_size; ++i) {
                        std::destroy_at(m_data+i);
                    }
                    m_size = new_size;
                    return;
                }

                reserve(new_size);

                for(std::size_t i = m_size; i < new_size; ++i) {
                    std::construct_at(m_data+i);
                }

                m_size = new_size;
            }

            template<typename... Args>
            T& emplace_back(Args&&... args) {
                if(m_size < m_capacity) {
                    std::construct_at(m_data+m_size, std::forward<Args>(args)...);
                    ++m_size;
                    return back();
                }

                std::size_t new_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;

                T* new_data = static_cast<T*>(operator new(sizeof(T) * new_capacity));

                std::size_t old_size = m_size;
                std::size_t constructed = 0;

                try {
                    relocate_into_uninitialized(new_data, m_data, old_size);
                    constructed = old_size;

                    std::construct_at(new_data + old_size, std::forward<Args>(args)...);
                    constructed = old_size+1;
                } catch (...) {
                    for(std::size_t i = 0; i < constructed; ++i) {
                        std::destroy_at(new_data+i);
                    }

                    operator delete(new_data);
                    throw;
                }

                for(std::size_t i = 0; i < old_size; ++i) {
                    std::destroy_at(m_data+i);
                }

                operator delete(m_data);

                m_capacity = new_capacity;
                m_size = old_size + 1;
                m_data= new_data;

                return back();
            }

            vector(std::initializer_list<T> list)
                :m_data(nullptr), m_size(0), m_capacity(0)
            {
                std::size_t n = list.size();
                if(n == 0) return;

                m_data = static_cast<T*>(operator new(sizeof(T) * n));
                m_capacity = n;

                try {
                    std::uninitialized_copy(list.begin(), list.end(), m_data);
                    m_size = n;
                } catch (...) {
                    operator delete(m_data);
                    m_capacity = 0;
                    throw;
                }
            }

            vector(std::size_t n, const T& value)
                :m_data(nullptr), m_capacity(0), m_size(0)
            {
                if(n == 0) return;

                m_data = static_cast<T*>(operator new(sizeof(T) * n));
                m_capacity = n;

                try {
                    std::uninitialized_fill_n(m_data, n, value);
                    m_size = n;
                } catch (...) {
                    operator delete(m_data);
                    m_data = nullptr;
                    m_capacity = 0;
                    throw;
                }
            }

            bool operator==(const vector& other) const {
                if(m_size != other.m_size) return false;

                for(std::size_t i = 0; i < m_size; ++i) {
                    if(!(m_data[i] == other.m_data[i])) return false;
                }

                return true;
            }

            void clear() {
                for(std::size_t i = 0; i < m_size; ++i) {
                    std::destroy_at(m_data+i);
                }

                m_size = 0;   
            }

            void shrink_to_fit() {
                if(m_size == m_capacity) return;

                if(m_size == 0) {
                    operator delete(m_data);
                    m_capacity = 0;
                    m_size = 0;
                    m_data = nullptr;
                    return;
                }

                T* new_data = static_cast<T*>(operator new(sizeof(T) * m_size));

                try {
                    relocate_into_uninitialized(new_data, m_data, m_size);
                } catch(...) {
                    operator delete(new_data);
                    throw;
                }

                for(std::size_t i = 0; i < m_size; ++i) {
                    std::destroy_at(m_data+i);
                }

                operator delete(m_data);
                m_data = new_data;
                m_capacity = m_size;
            }
    };

}