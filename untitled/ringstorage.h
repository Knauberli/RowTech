#ifndef RINGSTOAGE_H
#define RINGSTOAGE_H
#include <mutex>

template <typename T>
class RingStorage
{
    T *storage_;
    std::mutex *lock;
    unsigned int head = 0;
    unsigned int tail = 0;
    unsigned int max_size = 0;
    bool full_ = false;
    bool empty_ = true;
public:
    RingStorage(unsigned int size) {
        // Create new Stoage on the Heap
        this->storage_ = new T[size];
        this->max_size = size;

        // create a Mutex for Multithrerads safety
        this->lock = new std::mutex();
    }
    ~RingStorage(){
        delete this->storage_;
        delete this->lock;
    }
    unsigned int size(){
        return max_size;
    }

    bool empty(){
        return (!full_ && (tail == head));
    }
    bool full(){
        return full_;
    }
    void reset(){
        head = tail;
        full_ = false;
    }
    T get_Data(){
        T val;
        this->lock->lock();
        if(empty_)
            return T();
        val = this->storage_[tail];
        full_ = false; // if we read the buffer a value is remove so we got new space
        tail = (tail + 1) % max_size;
        this->lock->unlock();
        return val;
    }
    T at(unsigned int idx){
        T ret;
        this->lock->lock();
        ret = this->storage_[(tail + idx) % max_size];
        this->lock->unlock();
        return ret;
    }
    void put_data(T val){
        this->lock->lock();

        this->storage_[head] = val;
        // set head and tail to new values
        if(this->full_)
            tail = (tail+1) % max_size;
        head = (head +1 ) % max_size;
        // if head and tail are on the same index the array is full
        this->full_ = head == tail;
        this->lock->unlock();
    }
    const T operator[](unsigned int idx){
        return this->storage_[(tail+idx) % max_size];
    }
};
#endif // RINGSTOAGE_H
