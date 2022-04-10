#pragma once

#include <cstdint>

#include <limits>

class HandleBase {
public:
    using HandleId = uint32_t;

    static constexpr const HandleId invalidId = HandleId{ std::numeric_limits<HandleId>::max() };

    constexpr HandleBase() noexcept: object(invalidId) {}

    // whether this Handle is initialized
    explicit operator bool() const noexcept { return object != invalidId; }

    // clear the handle, this doesn't free associated resources
    void clear() noexcept { object = invalidId; }

    // compare handles
    bool operator==(const HandleBase& rhs) const noexcept { return object == rhs.object; }
    bool operator!=(const HandleBase& rhs) const noexcept { return object != rhs.object; }
    bool operator<(const HandleBase& rhs) const noexcept { return object < rhs.object; }
    bool operator<=(const HandleBase& rhs) const noexcept { return object <= rhs.object; }
    bool operator>(const HandleBase& rhs) const noexcept { return object > rhs.object; }
    bool operator>=(const HandleBase& rhs) const noexcept { return object >= rhs.object; }

    // get this handle's handleId
    HandleId getId() const noexcept { return object; }

    // initialize a handle, for internal use only.
    //explicit HandleBase(HandleId id) noexcept : object(id) 
    //{
    //   assert_invariant(object != nullid); // usually means an uninitialized handle is used
    //}

protected:
    HandleBase(HandleBase const& rhs) noexcept = default;
    HandleBase& operator=(HandleBase const& rhs) noexcept = default;

private:
    HandleId object;
};

template<typename T>
class Handle : public HandleBase
{
public:
    Handle() noexcept = default;
    Handle(const Handle& rhs) noexcept = default;
    Hanlde& operator=(const Handle& rhs) noexcept = default;
    explicit Handle(HandleId id) noexcept : HandleBase(id) {}

    template<typename B, typename = std::enable_if_t<std::is_base_of<T, B>::value> >
    Handle(const Handle<B>& base) noexcept : HandleBase(base) {}

private:

};

