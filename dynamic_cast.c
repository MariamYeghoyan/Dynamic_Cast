#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Base Base;
typedef struct Derived Derived;
typedef struct TypeInfo TypeInfo;
typedef struct VTable VTable;

void foo_base(Base* const b) {
    printf("Base's foo\n");
}

void foo_derived(Derived* const d) {
    printf("Derived's foo\n");
}

struct TypeInfo {
    char* class_name;
    char* base_name;
};

TypeInfo Basetypeinfo = {"Base", NULL};
TypeInfo Derivedtypeinfo = {"Derived", "Base"};

struct VTable {
    TypeInfo* typeinfo;
    void (*foo) (void*);
};

VTable basevtble = {&Basetypeinfo, (void (*)(void*))&foo_base};
VTable derivedvtble = {&Derivedtypeinfo, (void (*)(void*))&foo_derived};

struct Base {
    VTable* vtable;
};

struct Derived {
    Base base; 
};

int is_type_compatible(const TypeInfo* source, const TypeInfo* target) {
    return (strcmp(source->base_name, target->class_name) == 0);
}

void* DynamicCast(void* ptr, TypeInfo* desttype) {
    VTable** vtable_ptr = (VTable**)ptr; //convert ptr rt Vptr**
    if (*vtable_ptr == NULL) {
        return NULL;
    }
    if (is_type_compatible((*vtable_ptr)->typeinfo, desttype)) {  // Corrected access
        return ptr;
    }
    else {
        return NULL;
    }
}

int main() {
    Derived derived;
    derived.base.vtable = &derivedvtble;  

    Base* basePtr = (Base*)DynamicCast(&derived.base.vtable, &Basetypeinfo); 

    if (basePtr != NULL) {
        printf("Downcast successful\n");
    }
    else {
        printf("Downcast failed!\n");
    }

    return 0;
}