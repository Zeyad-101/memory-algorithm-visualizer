#include <iostream>
#include <vector>
#include <string>
using namespace std;

class memoryManger {

public:

    struct Block {
        string name;
        int size;
        bool isFree;
        // bool isReachable;
    };

    vector<Block> memory;

    void coalesce() {

        for (int i = 0; i + 1 < memory.size(); ) {
            if (memory[i].isFree && memory[i + 1].isFree) {
                memory[i].size += memory[i + 1].size;
                memory.erase(memory.begin() + i + 1);
            }
            else
                i++;
        }

    }

    void compact() {

        vector<Block> temp;
        int freeSize = 0;

        for (int i = 0; i < memory.size(); i++) {
            if (memory[i].isFree == false)
                temp.push_back(memory[i]);
            else
                freeSize += memory[i].size;
        }

        if (freeSize > 0)
            temp.push_back({ "", freeSize, true, false });

        memory.clear();

        for (int i = 0; i < temp.size(); i++) {
            memory.push_back(temp[i]);
        }

    }

    void freeMemory(string processName) {

        for (int i = 0; i < memory.size(); i++) {
            if (memory[i].name == processName && memory[i].isFree == false)
            {
                memory[i].isFree = true;
                memory[i].name = "";
            }
        }
        coalesce();

    }

    /*
        void resetMarks() {

            for (int i = 0; i < memory.size(); i++) {
                memory[i].isReachable = false;
            }

        }

        void mark(string processName) {

            for (int i = 0; i < memory.size(); i++) {
                if (memory[i].name == processName && memory[i].isFree == false)
                    memory[i].isReachable = true;
            }

        }

        void garbageCollection() {

            for (int i = 0; i < memory.size(); i++) {
                if (memory[i].isFree == false && memory[i].isReachable == false) {
                    memory[i].isFree = true;
                    memory[i].name = "";
                }
            }
            coalesce();

        }
    */

};

