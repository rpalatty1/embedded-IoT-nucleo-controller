#include <iostream>
#include <queue>
#include <string>
#include "mbed.h"
#include "uop_msb.h"
using namespace uop_msb;
using namespace std;

#define FIFO_SIZE 20

/*class FIFOBuffer 
{
private:
    //int buffer [32];
    //int write_index=0;

    //this array holds items in queue
    string queue[FIFO_SIZE];
    //shows front and back of queue
    int front, rear;

public:
    //pointers
    FIFOBuffer() : front(-1), rear(-1) {}
    //checks if queue full
    bool isFull() 
    {
        return (rear + 1) % FIFO_SIZE == front; 
    //checks if queue empty
    bool isEmpty() 
    {
        return front == -1;
    }
    //to add items to FIFO buffer
    void add_to_buffer(string item)
    {   
        lock();
        if (isFull()) 
        {
            //prints when queue is full
            printf("FIFO full"); 
            return;
        }
        //increments pointer from back
        rear = (rear + 1) % FIFO_SIZE; 
        //stores at back
        queue[rear] = item; 
        if (front == -1) 
        {
            //front to back when queue empty
            front = rear; 
        }

        unlock();
    };
    //to remove items from FIFO buffer
    void remove_from_buffer()
    {
          if (isEmpty()) 
          {
            //prints when queue empty
            printf("FIFO empty"); 
            return "";
         }
        //takes item from front 
        string item = queue[front]; 
        if (front == rear) 
        {
            //resets if only 1 item in queue
            front = rear = -1;
        } 
        else 
        {   //increments pointer
            front = (front + 1) % FIFO_SIZE; 
        }
        return item;
    }

    //virtual void increment_index()=0;
    void lock();
    void add();
    void isFull();
    void remove();
    void isEmpty();
    void unlock();

    //virtual void add_data()=0;

};


class myFIFO : public FIFOBuffer
{
    private:
    Mutex mutex;
    public:
    void fifofunc()
    {
        printf("fifo function\r\n");
    }
    void lock()
    {
        mutex.lock();
        printf("");
    }
     void unlock()
    {
        mutex.unlock();
        printf("");
    }
     void add_to_buffer()
    {
        printf("");
    }

    //virtual increment_index=0;
};*/

Mutex mutex;

 template <typename T>
class FIFOBuffer {
public:
    FIFOBuffer(int capacity) : capacity_(capacity) {}

    void push(const T& item) {
        mutex.lock();
        if (buffer_.size() >= capacity_) {
            //buffer is full, wait for space to become available
            cout << "Critical Failure. Restarting" << endl;
            wait_us(1000);
            system_reset();
        }
        buffer_.push(item);
        mutex.unlock();
    }

    bool isFull() const {
        mutex.lock();
        return buffer_.size() >= capacity_;
    }

    T pop() {
        mutex.lock();
        if (buffer_.empty()) {
            // Buffer is empty, wait for data to become available
            cout << "Buffer is empty" << endl;
        }
        T item = buffer_.front();
        buffer_.pop();
        mutex.unlock();
        return item;
    }

    bool isEmpty() const {
        mutex.lock();
        return buffer_.empty();
    }

private:
    queue<T> buffer_;
    int capacity_;
};