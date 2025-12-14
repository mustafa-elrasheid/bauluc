#include "headers/program.hpp"

Label::Label(const char* identif, int _offset){
    this->identifier = identif;
    this->offset = _offset;
    this->unresolved_offsets = new OffsetList();
}

LabelList::LabelList(int size, bool allow_hoisting){
    this->index = -1;
    this->length = size;
    this->labels = (Label**)malloc(sizeof(Label*)*size);
    this->allow_hoisting = allow_hoisting;
}

void LabelList::push(Label* label){
    if(allow_hoisting){
        for(int x = 0; x < this->index+1; x++){
            Label* _label = this->labels[x];
            if(strcmp(label->identifier,_label->identifier) == 0){
                _label->offset = label->offset;
                return;
            }
        }
    }
    this->index += 1;
    this->labels[index] = label;

}

void LabelList::resolve(){
    for(int x = 0; x < this->index+1; x++){
        this->labels[x]->unresolved_offsets->resolve(this->labels[x]->offset);
    }
}

// TODO
Label::~Label(){

}

void LabelList::find(const char* identifier, int* offset){
    for(int x = 0; x < this->index+1; x++){
        Label* label = this->labels[x];
        if(strcmp(identifier,label->identifier) == 0){
            if(label->offset != -1){
                *offset = label->offset;
            }
            else {
                if(!allow_hoisting)
                    error("Unknown Identifier","Identifier: \"%s\" was not found.\n",identifier);
                label->unresolved_offsets->push(offset);
                *offset = -1;
            }
            return;
        }
    }
    if(!allow_hoisting)
        error("Unknown Identifier","Identifier: \"%s\" was not found.\n",identifier);
    this->push(new Label(identifier, -1));
    this->labels[index]->unresolved_offsets->push(offset);
    *offset = -1;
}

int LabelList::peak(){
    if(index == -1) return 0;
    return this->labels[index]->offset;
}

void LabelList::empty(){
    for(int x = 0; x < this->index+1; x++)
        delete this->labels[x];
    this->index = -1;
}

LabelList::~LabelList(){
    for(int x = 0; x < this->index+1; x++)
        delete this->labels[x];
    free(labels);
}

int Max(int x,int y){
	if(x > y) return x;
	return y;
}
