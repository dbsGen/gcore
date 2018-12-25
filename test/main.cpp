#include <stdio.h>
#include "TestObject.h"
#include <core/String.h>
#include <core/Array.h>
#include <vector>
#include <core/Callback.h>
#include <core/math/Type.h>
#include <script/ruby/RubyScript.h>
#include <script/javascript/JSScript.h>

#include <gtest/gtest.h>

using namespace gcore;
using namespace gscript;

#define RUBY_PATH "/Users/gen2/Programs/grender/gcore/test/ruby"
#define JS_PATH "/Users/gen2/Programs/grender/gcore/test/javascript/run"

#define STR(S) #S

TEST(Class, ClassRelationships)
{
    EXPECT_EQ(TestObject::getClass()->getParent(), RefObject::getClass());
    EXPECT_TRUE(TestObject::getClass()->isTypeOf(Object::getClass()));
    EXPECT_TRUE(TestObject::getClass()->isSubclassOf(RefObject::getClass()));
    EXPECT_FALSE(TestObject::getClass()->isSubclassOf(TestObject::getClass()));
}

TEST(Object, NameOfTestObject)
{
    Ref<TestObject> obj(new TestObject());
    EXPECT_STREQ(obj->getClass()->getFullname().str(), STR(TestObject));
}

TEST(Object, SetterAndGetter)
{
    Ref<TestObject> obj(new TestObject());
    obj->callArgs("setIntValue", 1023);
    EXPECT_EQ((int)obj->call("getIntValue"), obj->getIntValue());
}

TEST(Object, Property)
{
    Ref<TestObject> obj(new TestObject());
    const Property *property = obj->getInstanceClass()->getProperty("int_value");
    property->set(obj, 1023);
    EXPECT_EQ((int)property->get(obj), 1023);
}

TEST(Variant, Transform)
{
    Variant f_variant(30.32);
    EXPECT_EQ((int)f_variant, 30);

    Variant v2_variant(Vector2f(2,3));
    Vector2f vec2 = v2_variant;
    Vector3f vec3 = v2_variant;
    EXPECT_EQ(vec2[0], 2);
    EXPECT_EQ(vec2[1], 3);

    EXPECT_EQ(vec3[0], 0);
    EXPECT_EQ(vec3[1], 0);
    EXPECT_EQ(vec3[2], 0);
}

TEST(Callback, Params)
{
    int target;
    RCallback cb = C([&](int l){
        EXPECT_EQ(l, target);
    });

    target = 30;        cb(30.32);
    target = 2883;      cb(2883, "1");

    Ref<TestObject> obj(new TestObject());
    const Class *clz;
    RCallback cb2 = C([&](Object *object){
        if (object) {
            EXPECT_EQ(object->getInstanceClass(), clz);
        }else {
            EXPECT_EQ(clz, nullptr);
        }
    });
    clz = nullptr;                  cb2(30.32);
    clz = nullptr;                  cb2(2883, obj);
    clz = _String::getClass();      cb2("nihao");
    clz = TestObject::getClass();   cb2(obj);

}

TEST(Callback, ParamsTransform)
{
    Vector2f test_v2;

    Vector2f v2(2,3);
    Vector3f v3(2,3,4);
    RCallback cb3 = C([&](Vector2f v){
        EXPECT_EQ(test_v2, v);
    });
    test_v2 = v2;           cb3(v2);
    test_v2 = Vector2f();   cb3(v3);
}

TEST(Size, ObjectMinnumSize)
{
    EXPECT_EQ(sizeof(StringName), 8);
    EXPECT_EQ(sizeof(Reference), 8);
    EXPECT_EQ(sizeof(Ref<TestObject>), 8);
    EXPECT_EQ(sizeof(Variant), 24);
}


TEST(Ruby, RunSimpleScript)
{
    RubyScript ruby;
    EXPECT_EQ((int)ruby.runScript("1+2"), 3);
    RArray arr = ruby.runScript("[1,'hello', 3]");

    EXPECT_EQ((int)arr[0], 1);
    EXPECT_STREQ((const char *)arr[1], "hello");
    EXPECT_EQ((int)arr[2], 3);
}

TEST(Ruby, RunEnvFile)
{
    Ref<TestObject> obj;

    RubyScript ruby;
    ruby.setup(RUBY_PATH);

    ruby.run(RUBY_PATH "/test.rb");

    obj = ruby.runScript("$to");

    EXPECT_EQ(obj->getIntValue(), 333);

    obj->setCallback(C([](const std::string &str){
        EXPECT_STREQ(str.c_str(), "InRuby");
    }));
    ruby.runScript("$to.call_cb");

    Reference objctA = ruby.runScript("$objct_a");
    EXPECT_EQ((int)objctA->applyArgs("get_number"), 2018);
}

int main(int argc, char* argv[]) {
    Ref<TestObject> obj(new TestObject());

    printf("ClassName %s\n", obj->getInstanceClass()->getFullname().str());

    printf("ClassName %s\n", obj->getInstanceClass()->getParent()->getFullname().str());

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

    JSScript::setup(JS_PATH);

    JSScript *js = new JSScript();
    js->runScript("1+2");

    delete js;

    JSScript::shutdown();

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}