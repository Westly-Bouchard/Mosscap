#ifndef HANDLE_H
#define HANDLE_H

template<typename Capability>
class Handle {
public:
    Handle() : ptr(nullptr) {}
    explicit Handle(Capability* ptr) : ptr(ptr) {}

    Capability& operator*() const {
        return *ptr;
    }
    Capability* operator->() const {
        return ptr;
    }

private:
    Capability* ptr;
};

#endif