#include <iostream>
#include <string>
#include <vector>

using namespace std;

class pagereplacement {
public:
    string name; //for pages names
    int  fault; // ely hwa m3rfsh y3mlo access
    int   hit;  //ely hwa 3mlo access
    int size; //7gm el 7aga
    vector<int> frames; //dynamic array l el frames (el frames gwaha kza page)

    pagereplacement(int framesCount) { //constructor for intializing kol 7aga
        size = framesCount;
        fault = 0;
        hit = 0;
    }

    virtual ~pagereplacement() { //destractor for the constructor
        cout << "Destructor called" << endl;
    }

    virtual void access(int page) = 0; //to access the pages
};

class fifo : public pagereplacement {
public:
    int ptr = 0;
    int size;
    int hit = 0;
    int fault = 0;
    fifo(int framesCount) : pagereplacement(framesCount) {    // inherits from the pagereplacement class

        size = framesCount;
        frames.resize(size, -1);
    }
    void access(int page) override {
        bool found = false;
        for (int i = 0; i < size; i++) {   //searching for the page in the frames
            if (frames[i] == page) {
                found = true;
                break;
            }
        }
        if (found) { //lw el page tl3t fe el frames yb2a hit w nzwd wa7ed 3la el hits
            hit++;
        }
        else {
            fault++; //lw el page msh fe el frames yb2a fault w nzwd wa7ed 3la el faults

            frames[ptr] = page;

            ptr = (ptr + 1) % size;   //pointer to the next frame to be replaced (circularly)
        }
    }
};

class LRU : public pagereplacement { //class l el LRU bya5od mn el page replacement
public:
    vector<int> lastUsed; //el last used array 3shan nshof ely mlosh lazma w nshelo 
    int timeStep; //3shan el time

    LRU(int framesCount) : pagereplacement(framesCount) {
        name = "LRU"; //el algorithm name
        timeStep = 0;
        for (int i = 0; i < size; i++) {
            frames.push_back(-1);
            lastUsed.push_back(-1);
        } //el for loop de 3shan lma na5od 7aga mno 
    }

    void access(int page) override { //el access function 3shan n access el pages
        timeStep++; //bnzwd 3la el time step wa7d 3shan n3rf enna fe anhy page
        for (int i = 0; i < size; i++) {
            int* ptr = &frames[i];
            if (*ptr == page) {
                hit++;
                lastUsed[i] = timeStep;
                return;
            }  //for loop 3shan lw ely bn3mlo access tl3 hit bnzwd wa7ed for hits 
        }
        fault++;
        int replaceId = 0; //hna lw 3ayzan n3ml replace item b item fe awl el memory
        for (int i = 0; i < size; i++) {
            if (frames[i] == -1) {
                replaceId = i;
                break;
            }
            if (lastUsed[i] < lastUsed[replaceId]) { //hna replace fe a5r el memory
                replaceId = i;
            }
        }
        int* mid = &frames[replaceId]; //hna replace fe nos el memory
        *mid = page;
        lastUsed[replaceId] = timeStep;
    }
};
class Clock : public pagereplacement { //class l el Clock bya5od mn el pagereplacement
public:
    vector<int> refBit; //array 3shan n3rf el page de leha second chance (1) wla nshlha (0)
    int index; //zay3a2rb el sa3a byshawr 3la el mkan ely wa2fen 3ndo fe el frames

    Clock(int framesCount) : pagereplacement(framesCount) {
        name = "Clock"; //el algorithm name
        index = 0; //bnbd2 mn awl mkan fe el memory (0)
        for (int i = 0; i < size; i++) {
            frames.push_back(-1);
            refBit.push_back(0);
        } //el for loop de 3shan n7ot -1 fe el frames el fadya w 0 fe el refBit
    }

    void access(int page) override { //el access function 3shan n access el pages
        for (int i = 0; i < size; i++) {
            int* ptr = &frames[i];
            if (*ptr == page) {
                hit++;
                refBit[i] = 1; //bndeha 1 (second chance) 3shan lsa mst5dmenha
                return;
            } //for loop 3shan lw ely bn3mlo access tl3 hit bnzwd wa7ed for hits
        }

        fault++; //lw ml2nahosh fe el frames yb2a fault w nzwd wa7ed
        for (;;) { //infinite loop btlef 3la el frames l7d ma tl2y mkan t3mlo replace
            int* bitPtr = &refBit[index];
            if (frames[index] == -1 || *bitPtr == 0) { //lw el mkan fady (-1) aw el bit bta3o 0 n3ml replace hna
                frames[index] = page; //n7ot el page el gdeda
                *bitPtr = 1; //n5ly el bit bta3ha 1
                index = (index + 1) % size; //n7rk el index 3la el mkan ely b3do (w % size 3shan ylef mn el awl tany)
                break;
            }
            else { //lw el bit kan 1 (3ndo second chance)
                *bitPtr = 0; //n5leh 0 3shan el mra el gaya 
                index = (index + 1) % size; //w nshof el mkan ely b3do
            }
        }
    }
};
