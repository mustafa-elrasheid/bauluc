#include "headers/program.hpp"

OffsetList::OffsetList(int size){
    this->offsets = (int**) malloc(sizeof(int*)*size);
    this->index = -1;
    this->length = size;
}

void OffsetList::resolve(int new_val){
    for(int x = 0; x < this->index+1; x++){
        *this->offsets[x] = new_val;
    }
}

void OffsetList::push(int* offset){
    if(this->index+1 > this->length)
        this->offsets = (int**)realloc(this->offsets, this->length*2);
    this->index++;
    this->offsets[index] = offset;
}

OffsetList::~OffsetList(){
    free(this->offsets);
}
