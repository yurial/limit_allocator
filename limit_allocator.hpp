#pragma once

#include <memory>

template <class T, class Alloc = std::allocator<T>>
class limit_allocator
{
public:
using value_type = typename Alloc::value_type;
using pointer = typename Alloc::pointer;
using const_pointer = typename Alloc::const_pointer;
using size_type = typename Alloc::size_type;

template <class X>
using basic_allocator = typename std::allocator_traits<Alloc>::template rebind_alloc<X>;
template <class X>
using rebind_alloc = limit_allocator<X, basic_allocator<X>>;
template <class X, class A>
friend class limit_allocator;

template <class X>
struct rebind
    {
    using other = rebind_alloc<X>;
    };

            limit_allocator(size_t soft_limit, size_t hard_limit, const Alloc& alloc = Alloc());
            template <class X>
            limit_allocator(const rebind_alloc<X>& other);

pointer     allocate(size_t n);
void        deallocate(pointer p, size_t n) noexcept;
size_type   max_size() const noexcept;

size_t  get_usage() const noexcept;
size_t  get_soft_limit() const noexcept;
size_t  get_hard_limit() const noexcept;

protected:
std::shared_ptr<limit_allocator<void,basic_allocator<char>>>   m_impl;
}; // class limit_allocator

template <class Alloc>
class limit_allocator<void, Alloc>
{
public:
template <class X>
using basic_allocator = typename std::allocator_traits<Alloc>::template rebind_alloc<X>;
template <class X>
using rebind_alloc = limit_allocator<X, basic_allocator<X>>;
template <class X>
using value_type = typename basic_allocator<X>::value_type;
template <class X>
using pointer = typename basic_allocator<X>::pointer;
template <class X>
using const_pointer = typename basic_allocator<X>::const_pointer;
template <class X>
using size_type = typename basic_allocator<X>::size_type;

template <class X>
struct rebind
    {
    using other = rebind_alloc<X>;
    };

        limit_allocator(size_t soft_limit, size_t hard_limit, const Alloc& alloc = Alloc());

template <class X>
pointer<X> allocate(size_t n);
template <class X>
void deallocate(pointer<X> p, size_t n) noexcept;
template <class X>
size_type<X> max_size() const noexcept;

size_t  get_usage() const noexcept;
size_t  get_soft_limit() const noexcept;
size_t  get_hard_limit() const noexcept;
protected:
Alloc   m_alloc;
size_t  m_soft_limit = 0;
size_t  m_hard_limit = 0;
size_t  m_used = 0;
}; // class limit_allocator<void>

template <class Alloc>
limit_allocator<void, Alloc>::limit_allocator(size_t soft_limit, size_t hard_limit, const Alloc& alloc):
        m_alloc(alloc),
        m_soft_limit(soft_limit),
        m_hard_limit(hard_limit)
    {
    }

template <class Alloc>
size_t limit_allocator<void,Alloc>::get_usage() const noexcept
    {
    return m_used;
    }

template <class Alloc>
size_t limit_allocator<void,Alloc>::get_soft_limit() const noexcept
    {
    return m_soft_limit;
    }

template <class Alloc>
size_t limit_allocator<void,Alloc>::get_hard_limit() const noexcept
    {
    return m_hard_limit;
    }


template <class Alloc>
template <class X>
typename limit_allocator<void,Alloc>::template pointer<X> limit_allocator<void, Alloc>::allocate(size_t n)
    {
    const size_t size = sizeof(value_type<X>) * n;
    const size_t limit = get_hard_limit();
    if (limit > 0 && m_used + size > limit)
        throw std::bad_alloc();
    m_used += size;
    return basic_allocator<X>(m_alloc).allocate(n);
    }

template <class Alloc>
template <class X>
void limit_allocator<void,Alloc>::deallocate(typename limit_allocator<void, Alloc>::template pointer<X> p, size_t n) noexcept
    {
    m_used -= sizeof(value_type<X>) * n;
    basic_allocator<X>(m_alloc).deallocate(p, n);
    }

template <class Alloc>
template <class X>
typename limit_allocator<void,Alloc>::template size_type<X> limit_allocator<void,Alloc>::max_size() const noexcept
    {
    return basic_allocator<X>(m_alloc).max_size();
    }

template <class T, class Alloc>
limit_allocator<T,Alloc>::limit_allocator(size_t soft_limit, size_t hard_limit, const Alloc& alloc)
    {
    m_impl = std::make_shared<limit_allocator<void,basic_allocator<char>>>(soft_limit, hard_limit, basic_allocator<char>(alloc));
    }

template <class T, class Alloc>
template <class X>
limit_allocator<T,Alloc>::limit_allocator(const rebind_alloc<X>& other):
        m_impl(other.m_impl)
    {
    }

template <class T, class Alloc>
typename limit_allocator<T,Alloc>::pointer limit_allocator<T,Alloc>::allocate(size_t n)
    {
    return m_impl->template allocate<T>(n);
    }

template <class T, class Alloc>
void limit_allocator<T,Alloc>::deallocate(pointer p, size_t n) noexcept
    {
    m_impl->template deallocate<T>(p, n);
    }

template <class T, class Alloc>
typename limit_allocator<T,Alloc>::size_type limit_allocator<T,Alloc>::max_size() const noexcept
    {
    return m_impl->template max_size<T>();
    }

template <class T, class Alloc>
size_t limit_allocator<T,Alloc>::get_usage() const noexcept
    {
    return m_impl->get_usage();
    }

template <class T, class Alloc>
size_t limit_allocator<T,Alloc>::get_soft_limit() const noexcept
    {
    return m_impl->get_soft_limit();
    }

template <class T, class Alloc>
size_t limit_allocator<T,Alloc>::get_hard_limit() const noexcept
    {
    return m_impl->get_hard_limit();
    }

