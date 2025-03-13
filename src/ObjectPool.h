#pragma once


#include <vector>
#include <functional>

template<typename T>
class ObjectPool {
public:
    ObjectPool() : mPool(10) { }
    ObjectPool(int size) : mPool(size) { }
    ~ObjectPool() { }

    T *aquireObject() { 
        for (auto&& obj : mPool) {
            if (!obj.poolState.alive) {
                obj.poolState.alive = true;
                return &obj;
            }
        }
        return nullptr;
    }

    void releaseObject(T &obj) {
        obj.poolState.alive = false;
    }

    void apply(std::function<void(T&)> fn) {
        for (auto&& obj : mPool) {
            if (!obj.poolState.alive) continue;
            fn(obj);
        }
    }
    //T *extend(int amount) { }
private:
    std::vector<T> mPool;
};

struct PoolState {
    PoolState() : alive(false) {}
    PoolState(bool state) : alive(state) {}
    bool alive;
};
