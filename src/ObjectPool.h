#pragma once


#include <vector>

template<typename T>
class ObjectPool {
public:
    ObjectPool() : mPool(10) { }
    ObjectPool(int size) : mPool(size) { }
    ~ObjectPool() { }

    T *getUnusedObject() { 
        for (auto&& obj : mPool) {
            if (!obj.poolState.alive) {
                obj.poolState.alive = true;
                return &obj;
            }
        }
        return nullptr;
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
