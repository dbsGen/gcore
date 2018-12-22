#include <stdio.h>
#include "TestObject.h"
#include <core/String.h>
#include <core/Array.h>
#include <vector>
#include <core/Callback.h>
#include <core/math/Type.h>

using namespace gcore;

int main() {
    Ref<TestObject> obj(new TestObject());

    printf("ClassName %s\n", obj->getInstanceClass()->getFullname().str());

    printf("ClassName %s\n", obj->getInstanceClass()->getParent()->getFullname().str());

    obj->callArgs("setIntValue", 1023);
    printf("int value is %d -> %d\n", (int)obj->call("getIntValue"), obj->getIntValue());

#define PrintSize(CLASS) printf(#CLASS " size %d\n", sizeof(CLASS))
    PrintSize(StringName);

    PrintSize(Vector3f);

    RCallback cb = C([](int l){
        printf("output %d\n", l);
    });

    cb(30.32);
    cb(2883, "1");
    cb("nihao");

    RCallback cb2 = C([](Object *object){
        if (object) {
            printf("output %s\n", object->getInstanceClass()->getFullname().str());
        }else {
            printf("Object is NULL\n");
        }
    });

    cb2(30.32);
    cb2(2883, obj);
    cb2("nihao");
    cb2(obj);

    Vector2f v2(2,3);
    Vector3f v3(2,3,4);
    RCallback cb3 = C([](Vector2f v){
        printf("v(%f,%f)\n",v.x(), v.y());
    });
    cb3(v2);
    cb3(v3);

    return 0;
}