#ifndef MJI_FIXED_VECTOR_HPP_INCLUDED_
#define MJI_FIXED_VECTOR_HPP_INCLUDED_

#if !(defined(__cplusplus) && __cplusplus >= 201703L)
    #error "C++17 or later is required for mji::fixed_vector."
#endif

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <type_traits>

namespace mji {

template <typename T, typename Allocator = std::allocator<T>>
class fixed_vector final {
public:
    // types

    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = alloc_traits::pointer;  // TODO: should be T*?
    using const_pointer = alloc_traits::const_pointer;
    using iterator = fixed_vector_iterator<T, false>;
    using const_iterator = const fixed_vector_iterator<T, true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = const std::reverse_iterator<const_iterator>;

    // constructors

    // 1
    constexpr fixed_vector() noexcept(noexcept(Allocator()))
    : fixed_vector(PrivateCtorTag(), 0)
    {}

    // 2
    constexpr explicit fixed_vector(const Allocator& alloc) noexcept
    : fixed_vector(PrivateCtorTag(), 0, alloc)
    {}

    // 3
    explicit fixed_vector(size_type count, const T& value, const Allocator& alloc = Allocator())
    : fixed_vector(PrivateCtorTag(), count, alloc)
    {
        pointer pT = data_;
        auto idx = 0;
        for (; idx < size_; ++idx, ++pT) {
            alloc_traits::construct(allocator_, pT, value);
        }
    }

    constexpr explicit fixed_vector(size_type count, const Allocator& alloc = Allocator())
    : fixed_vector(PrivateCtorTag(), count, alloc)
    {
        pointer pT = data_;
        auto idx = 0;
        for (; idx < size_; ++idx, ++pT) {
            alloc_traits::construct(allocator_, pT);
        }
    }

    template<class InputIt>
    constexpr fixed_vector(size_type count, InputIt first, InputIt last, const Allocator& alloc = Allocator())
    {
        // TODO: restrict iterator type overload resolution LegacyInputIterator
        // TODO: implement
    }

    // copy ctor
    constexpr fixed_vector(const fixed_vector& other)
    // TODO: implement
    {}

    constexpr fixed_vector(const fixed_vector& other, const Allocator& alloc)
    // TODO: implement
    {}

    // move ctor
    constexpr fixed_vector(fixed_vector&& other) noexcept
    // TODO: implement
    {}

    constexpr fixed_vector(fixed_vector&& other, const Allocator& alloc)
    // TODO: implement
    {}

    constexpr fixed_vector(std::initializer_list<T> init, const Allocator& alloc = Allocator())
    : fixed_vector(PrivateCtorTag(), init.size(), alloc)
    {
        // TODO: implement
    }

    // TODO: destructor

    // TODO: assignment operators
    // https://stackoverflow.com/questions/27471053/example-usage-of-propagate-on-container-move-assignment

    // methods

    constexpr T* data() noexcept {
        return static_cast<T*>(data_);
    }

    constexpr size_type size() const noexcept {
        return size_;
    }

    constexpr reference operator[](size_type pos) {
        assert(pos < size_);
        return data_[pos];
    }

    constexpr reference at(size_type pos) {
        if (pos >= size_) {
            throw std::out_of_range("mji::fixed_vector: at() index out of range");
        }
        return data_[pos];
    }

    constexpr reference front() {
        assert(size_ > 0);
        return data_[0];
    }

    constexpr reference back() {
        assert(size_ > 0);
        return data_[size_ - 1];
    }

    constexpr T* data() noexcept {
        return data_;
    }

    constexpr allocator_type get_allocator() const noexcept {
        return allocator_;
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return size_ == 0;
    }

    constexpr iterator begin() noexcept {
        return iterator(data_);
    }

    constexpr const_iterator cbegin() const noexcept {
        return const_iterator(data_);
    }

    constexpr iterator end() noexcept {
        return iterator(data_ + size_);
    }

    constexpr const_iterator cend() const noexcept {
        return const_iterator(data_ + size_);
    }

    constexpr reverse_iterator rbegin() noexcept {
        return reverse_iterator(this->end());
    }

    constexpr const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(this->cend());
    }

    constexpr reverse_iterator rend() noexcept {
        return reverse_iterator(this->begin());
    }

    constexpr const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(this->cbegin());
    }

    constexpr void fill(const_reference value) {
        for (auto& element : *this) {
            element = value;
        }
    }

    // const method copies

    constexpr const T* data() const noexcept {
        return static_cast<const T*>(data_);
    }

    constexpr const_reference operator[](size_type pos) const {
        assert(pos < size_);
        return data_[pos];
    }

    constexpr const_reference at(size_type pos) const {
        if (pos >= size_) {
            throw std::out_of_range("mji::fixed_vector: at() index out of range");
        }
        return data_[pos];
    }

    constexpr const_reference front() const {
        assert(size_ > 0);
        return data_[0];
    }

    constexpr const_reference back() const {
        assert(size_ > 0);
        return data_[size_ - 1];
    }

    constexpr const T* data() const noexcept {
        return data_;
    }

    constexpr const_iterator begin() const noexcept {
        return const_iterator(data_);
    }

    constexpr const_iterator end() const noexcept {
        return const_iterator(data_ + size_);
    }

    constexpr const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(this->cend());
    }

    constexpr const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(this->cbegin());
    }

private:
    // types

    using alloc_traits = std::allocator_traits<Allocator>;
    // tag type used for private constructor overload
    struct PrivateCtorTag {};

    // variables

    Allocator allocator_;
    pointer data_;
    std::size_t size_;

    // shared constructor
    fixed_vector(PrivateCtorTag, size_type count, const Allocator& alloc = Allocator())
    : size_(count), allocator_(alloc), data_(alloc_traits::allocate(allocator_, size_))
    {}

    // classes

    // TODO: make this class const correct
    class fixed_vector_iterator<bool is_citer> final {
    public:
        using value_type = T;
        using difference_type = fixed_vector<T, Allocator>::difference_type;
        using reference = std::conditional<is_citer, fixed_vector<T, Allocator>::const_reference, fixed_vector<T, Allocator>::reference>::type;
        using pointer = std::conditional<is_citer, fixed_vector<T, Allocator>::const_pointer, fixed_vector<T, Allocator>::pointer>::type;
        using iterator_category = std::random_access_iterator_tag;

        constexpr fixed_vector_iterator()
        : location_(nullptr)
        {}

        constexpr fixed_vector_iterator(pointer location)
        : location_(location)
        {}

        constexpr fixed_vector_iterator(const typeof_this& other)
        : location_(other.location_)
        {}

        // operators

        constexpr typeof_this& operator=(const typeof_this& other) {
            location_ = other.location_;
        }

        reference operator*() {
            assert(location_ != nullptr);
            return *location;
        }

        pointer operator->() {
            assert(location_ != nullptr);
            return location_;
        }

        // prefix
        typeof_this& operator++() {
            this->location_++;
            return *this;
        }

        // postfix
        typeof_this& operator++(int) {
            auto old = *this;
            ++*this;
            return old;
        }

        friend bool operator==(const typeof_this& iter_a, const typeof_this& iter_b) {
            return iter_a.location_ == iter_b.location_;
        }

        bool operator=!(const typeof_this& iter_a, const typeof_this& iter_b) {
            return iter_a.location_ != iter_b.location_;
        }

        friend typeof_this operator+(const typeof_this& iter, difference_type offset) {
            auto result = iter;
            result.location_ += offset;
            return result;
        }

        typeof_this operator+(difference_type offset, const typeof_this& iter) {
            return iter + offset;
        }

        typeof_this operator-(const typeof_this& iter, difference_type offset) {
            return iter + (-offset);
        }

        friend difference_type operator-(const typeof_this& iter_a, const typeof_this& iter_b) {
            return iter_a.location_ - iter_b.location_;
        }

        friend typeof_this& operator+=(difference_type offset) {
            this->location_ += offset;
            return *this;
        }

        // prefix
        typeof_this& operator--() {
            this->location_ -= 1;
            return *this;
        }

        // postfix
        typeof_this& operator--(int) {
            auto old = *this;
            this->location_ -= 1;
            return old;
        }

        friend bool operator<(const typeof_this& iter_a, const typeof_this& iter_b) {
            return iter_a.location_ < iter_b.location_;
        }

        friend bool operator<=(const typeof_this& iter_a, const typeof_this& iter_b) {
            return iter_a.location_ <= iter_b.location_;
        }

        friend bool operator>(const typeof_this& iter_a, const typeof_this& iter_b) {
            return iter_a.location_ > iter_b.location_;
        }

        friend bool operator>=(const typeof_this& iter_a, const typeof_this& iter_b) {
            return iter_a.location_ >= iter_b.location_;
        }

        reference operator[](difference_type offset) {
            return *(location_ + offset);
        }

    private:
        using typeof_this = fixed_vector<T, Allocator>::fixed_vector_iterator<is_const>;

        pointer location_;
    };
};

}

#endif // MJI_FIXED_VECTOR_HPP_INCLUDED_
