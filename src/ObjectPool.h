#pragma once


#include <fmt/format.h>
#include <limits>
#include <vector>
#include <functional>

template<typename T>
class ObjectPool {
public:
    ObjectPool() : mPool(8), mActiveCount(0) { }
    ObjectPool(std::size_t n) : mPool(n), mActiveCount(0) { }
    ~ObjectPool() { }

    // Return a pointer to next available object in pool,
    // or nullptr if full.
    T *aquireObject() {
        for (auto&& obj : mPool) {
            if (!obj.poolState.alive) {
                obj.poolState.alive = true;
                mActiveCount++;
                return &obj;
            }
        }
        return nullptr;
    }

    void releaseObject(T &obj) {
        obj.poolState.alive = false;
        mActiveCount--;
    }

    void releaseAll() {
        for (auto&& obj : mPool) {
            obj.poolState.alive = false;
            mActiveCount = 0;
        }
    }

    void apply(std::function<void(T&)> fn) {
        for (auto&& obj : mPool) {
            if (!obj.poolState.alive) continue;
            fn(obj);
        }
    }

    std::size_t size() { return mPool.size(); }
    std::size_t sizeActive() { return mActiveCount; }
    std::size_t sizeInactive() { return mPool.size() - mActiveCount; }

    // NOTE: Resize and friends reallocate container,
    //       invalidating all ptrs/refs.

    // Resize exactly to n. Expands and shrinks container.
    void resize(std::size_t n) {
        //fmt::print("resizing obj pool from: {}"
        //           " to: {}. Active objects: {}\n",
        //            mPool.size(), n, mActiveCount);
        mPool.resize(n);
    }

    // Resize to n. Expands only.
    void resizeAtLeast(std::size_t n) {
        if (n > mPool.size())
            resize(n);
    }

    // Make available at least n inactive objects.
    void reserveAtLeast(std::size_t n) {
        std::size_t avail = sizeInactive();
        if (avail < n) {
            std::size_t expand = n - avail;
            resizeAtLeast(size() + expand);
        }
    }

private:
    std::vector<T> mPool;
    std::size_t mActiveCount;
};

struct PoolState {
    PoolState() : alive(false) {}
    PoolState(bool state) : alive(state) {}
    bool alive;
};
