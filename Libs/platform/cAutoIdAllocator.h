#pragma once
//idÉú³ÉÆ÷

#include <list>
#include <assert.h>
#ifdef _WIN32
#undef max
#endif
//id·ÖÅäÆ÷
template <typename T = uint32_t>
class cAutoIdAllocator
{
    static_assert(std::is_unsigned<T>::value, "!is_unsigned");
public:
    cAutoIdAllocator() : m_tMax(std::numeric_limits<T>::max() - 1) {}
    cAutoIdAllocator(T tMax) : m_tMax(tMax) { assert(tMax < std::numeric_limits<T>::max()); }
    ~cAutoIdAllocator() {}
    bool Init(T tMax) 
    {
        if (m_tAutoId != 0)
            return false;
        m_tMax = tMax; 
        return true; 
    }
public:
    T alloc()
    {
        if (!m_lstFree.empty())
        {
            auto id = m_lstFree.front();
            m_lstFree.pop_front();
            return id;
        }
        if (m_tAutoId > m_tMax) return m_tAutoId;
        return ++m_tAutoId;
    }
    void free(T id)
    {
        m_lstFree.push_back(id);
    }
    bool check(T id) const
    {
        return id && id <= m_tMax;
    }
    bool isFull() const 
    {
        return m_lstFree.empty() && m_tAutoId >= m_tMax;
    }
    size_t size()
    {
        return (m_tAutoId >= m_lstFree.size() ? (m_tAutoId - m_lstFree.size()) : 0);
    }
private:
    T m_tMax;
    T m_tAutoId{};
    std::list<T> m_lstFree;
};
