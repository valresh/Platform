#pragma once

#include <assert.h>


class ref_counted
{
public:
    virtual unsigned add_ref() const = 0;
    virtual unsigned release() const = 0;

protected:
    virtual ~ref_counted() {}

private:
    friend unsigned Add_ref(ref_counted const* p)
    {
        return p->add_ref();
    }
    friend unsigned Release(ref_counted const* p)
    {
        return p->release();
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////

template<class itf>
class TIntrusivePtrImpl : public itf
{
public:
    typedef itf interface_type;

public: // ref_counted
    unsigned add_ref() const
    {
        return ++m_references;
    }
    unsigned release() const
    {
        if (!--m_references)
        {
            const_cast<TIntrusivePtrImpl*>(this)->PreDelete();
            delete this;
            return 0;
        }
        return m_references;
    }

protected:
    TIntrusivePtrImpl() : m_references(0)
    {
    }

    ~TIntrusivePtrImpl()
    {
    }

    virtual void PreDelete()
    {
    }

private:
    mutable unsigned m_references;
};

template<class T> class TIntrusivePtr
{
private:

    typedef TIntrusivePtr this_type;

public:

    typedef T element_type;

    TIntrusivePtr() : p_(0)
    {
    }

    TIntrusivePtr(T* p, bool add_ref = true) : p_(p)
    {
        if (p_ != 0 && add_ref)
            Add_ref(p_);
    }

    TIntrusivePtr(TIntrusivePtr const& rhs) : p_(rhs.p_)
    {
        if (p_ != 0)
            Add_ref(p_);
    }

    ~TIntrusivePtr()
    {
        if (p_ != 0)
            Release(p_);
    }

    TIntrusivePtr& operator=(TIntrusivePtr const& rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    TIntrusivePtr& operator=(T* rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    T* get() const
    {
        return p_;
    }

    T& operator*() const
    {
        return *p_;
    }

    T* operator->() const
    {
        assert(p_);
        return p_;
    }

    operator bool() const
    {
        return p_ != 0;
    }

    // operator! is a Borland-specific workaround
    bool operator! () const
    {
        return p_ == 0;
    }

    void swap(TIntrusivePtr& rhs)
    {
        T* tmp = p_;
        p_ = rhs.p_;
        rhs.p_ = tmp;
    }

private:

    T* p_;
};

template<class T, class U> inline bool operator==(TIntrusivePtr<T> const& a, TIntrusivePtr<U> const& b)
{
    return a.get() == b.get();
}

template<class T, class U> inline bool operator!=(TIntrusivePtr<T> const& a, TIntrusivePtr<U> const& b)
{
    return a.get() != b.get();
}

template<class T, class U> inline bool operator==(TIntrusivePtr<T> const& a, U* b)
{
    return a.get() == b;
}

template<class T, class U> inline bool operator!=(TIntrusivePtr<T> const& a, U* b)
{
    return a.get() != b;
}

template<class T, class U> inline bool operator==(T* a, TIntrusivePtr<U> const& b)
{
    return a == b.get();
}

template<class T, class U> inline bool operator!=(T* a, TIntrusivePtr<U> const& b)
{
    return a != b.get();
}

template<class T> void swap(TIntrusivePtr<T>& lhs, TIntrusivePtr<T>& rhs)
{
    lhs.swap(rhs);
}
