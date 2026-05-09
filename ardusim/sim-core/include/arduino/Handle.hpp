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
     * Construct a handle
     * @param ref Reference to the wrapped capability
     */
    explicit Handle(Capability& ref) : ref(ref) {}

    Capability& get() const { return ref; }

private:
    /**
     * Actual reference to the wrapped capability
     */
    Capability& ref;
};

#endif