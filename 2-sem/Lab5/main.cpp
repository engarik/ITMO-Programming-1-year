#include <iostream>
#include "CircularBuffer.hpp"

int main() {
    CircularBuffer<int> buffer(3);

    std::cout << "Add test:\n";
    buffer.addLast(10);
    std::cout << buffer.getFirst() << std::endl;
    buffer.addFirst(20);
    std::cout << buffer.getFirst() << std::endl;
    buffer.addFirst(30);
    std::cout << buffer.get(2) << std::endl;

    std::cout << "Remove test:\n";
    std::cout << buffer.getLast() << std::endl;
    buffer.removeLast();
    std::cout << buffer.getLast() << std::endl;
    std::cout << buffer.getFirst() << std::endl;
    buffer.removeFirst();
    std::cout << buffer.getFirst() << std::endl;


    std::cout << "Dynamic capacity test:\n";
    CircularBuffer<int> dynamicCapacity(10);
    for (int i = 0; i < 5; ++i) {
        dynamicCapacity.addLast(i + 1);
        std::cout << dynamicCapacity << std::endl;
    }

    std::cout << "Change capacity test:\n";
    dynamicCapacity.changeCapacity(4);
    std::cout << dynamicCapacity << std::endl;


    std::cout << "Iterator test:\n";
    std::reverse(dynamicCapacity.begin(), dynamicCapacity.end());
    std::cout << dynamicCapacity << std::endl;
    std::cout << dynamicCapacity.removeFirst();
    std::sort(dynamicCapacity.begin(), dynamicCapacity.end());
    std::cout << dynamicCapacity << std::endl;

    CircularBuffer<int> over(5);
    for(int i = 0; i < 10; ++i) {
        over.addFirst(i + 100);
        over.addFirst(i + 200);
        over.addLast(i + 2);
        std::cout << over << std::endl;
    }

    return 0;
}