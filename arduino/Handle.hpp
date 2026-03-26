#ifndef HANDLE_H
#define HANDLE_H

/**
 * Handle to a piece of simulated hardware
 * @tparam Capability capability of hardware that the handle wraps
 */
template<typename Capability>
class Handle {
public:
    /**
     * Default ctor
     */
    Handle() : ptr(nullptr) {}

    /**
     * Construct a handle
     * @param ptr Pointer to hardware
     */
    explicit Handle(Capability* ptr) : ptr(ptr) {}

    // Operators
    Capability& operator*() const {
        return *ptr;
    }

    Capability* operator->() const {
        return ptr;
    }

private:
    /**
     * Pointer to hardware
     * This is kind of a code smell b/c raw pointers bad.
     * But, in this case it's necessary because we're not managing this memory
     * The simulator is.
     */
    Capability* ptr;
};

#endif