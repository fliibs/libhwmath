// ChildRef.h
#ifndef CHILD_REF_H
#define CHILD_REF_H

#include "ref.h"

class ChildRef : public Ref {
public:
    // 构造函数中添加新函数到 functionTable
    ChildRef() {
        // 在子类中增加新的函数
        FunctionPointer newFunc = std::bind(&ChildRef::function4, this);
        addFunction("function4", newFunc);
    }

    void function4() {
        std::cout << "Function 4\n";
    }
};

#endif  // CHILD_REF_H
