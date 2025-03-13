#pragma once


#include <vector>
#include <functional>

template<typename T>
class ObjectPool {
public:
    ObjectPool() : mPool(10), mActiveCount(0) { }
    ObjectPool(int size) : mPool(size), mActiveCount(0) { }
    ~ObjectPool() { }

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

    int size() { return mPool.size(); }
    int getActiveCount() { return mActiveCount; }
    //T *extend(int amount) { }
private:
    std::vector<T> mPool;
    int mActiveCount;
};

struct PoolState {
    PoolState() : alive(false) {}
    PoolState(bool state) : alive(state) {}
    bool alive;
};
