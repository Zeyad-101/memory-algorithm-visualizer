#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

struct Block {
    int size;
	int pid; // store the process ID, -1 for first process
};

vector<Block> mem;
int nextPid = 1; // ID of the next incoming process

// Find suitable memory hole: 1 = First Fit, 2 = Best Fit, 3 = Worst Fit
int findHole(int req, int algo) {
    int c = -1; // -1 none found yet
	int size = mem.size();
    for (int i = 0; i < size; i++) {
        if (mem[i].pid != -1 || mem[i].size < req) continue;

        // Select the hole based on the chosen algorithm criteria
        if (c == -1 || (algo == 2 && mem[i].size < mem[c].size)
            || (algo == 3 && mem[i].size > mem[c].size)) {
            c = i;
        }
        if (algo == 1) break; // First fit stops immediately at the first matching hole
    }
    return c; // -1 no suitable hole exists
}

// Allocate memory for a new process with a given size and algorithm
void allocate(int req, int algo) {
    int i = findHole(req, algo);
    if (i == -1) {
        cout << "Allocation failed!\n";
        return;
    }
    int left = mem[i].size - req; // Remaining leftover space after allocation
    cout << "P" << nextPid << " allocated in hole of size " << mem[i].size
        << " (leftover " << left << ")\n";

    // Update the current block to be allocated to the new process and increment nextPid
    mem[i] = { req, nextPid++ };

    // Insert a new free hole block right after the allocated block
    if (left > 0) {
        mem.insert(mem.begin() + i + 1, { left, -1 });
    }
}

// Release memory for a specific process and coalesce adjacent free holes
void release(int pid) {
    bool found = false;
    // Mark the block belonging to the given process as a free hole (pid = -1)
    for (auto& b : mem) {
        if (b.pid == pid) {
            b.pid = -1;
            found = true;
        }
    }
    if (!found) {
        cout << "Process P" << pid << " not found!\n";
        return;
    }

    // Merge any consecutive free holes to reduce external fragmentation
    for (size_t i = 0; i + 1 < mem.size(); ) {
        if (mem[i].pid == -1 && mem[i + 1].pid == -1) {
            mem[i].size += mem[i + 1].size; // Merge sizes together
            mem.erase(mem.begin() + i + 1); // Remove the redundant second block
        }
        else {
            i++;
        }
    }
    cout << "Process P" << pid << " released.\n";
}

// Display the current memory state and calculate external fragmentation
void print() {
    int addr = 0, freeMem = 0, largest = 0;
    for (auto& b : mem) {
        cout << "[" << addr << "-" << addr + b.size - 1 << "] size " << b.size
            << (b.pid == -1 ? "  HOLE" : "  P" + to_string(b.pid)) << endl;
        addr += b.size;

        // Track total available free memory and the size of the largest hole
        if (b.pid == -1) {
            freeMem += b.size;
            if (b.size > largest) { // Replaces max() from <algorithm>
                largest = b.size;
            }
        }
    }
    // Calculate external fragmentation percentage based on available holes
    double frag = freeMem ? (1.0 - (double)largest / freeMem) * 100 : 0;
    cout << "Free: " << freeMem << "  Largest hole: " << largest
        << "  Fragmentation: " << (int)round(frag) << "%\n";
}