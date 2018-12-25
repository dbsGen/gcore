//
// Created by Gen2 on 2018-12-25.
//

#include "JSScript.h"
#include <v8.h>
#include <libplatform/libplatform.h>


using namespace gscript;
using namespace gcore;


namespace gscript {

    struct JSContext {

        struct IsolateScope {
            v8::Isolate::Scope isolate_scope;
            v8::HandleScope handle_scope;

            IsolateScope(v8::Isolate *isolate) :
                    isolate_scope(isolate),
                    handle_scope(isolate) {

            }
        };

        struct ContextScope {
            v8::Context::Scope scope;

            ContextScope(v8::Local<v8::Context> context) : scope(context) {

            }
        };

        static std::unique_ptr<v8::Platform> platform;

        v8::Isolate *isolate;
        v8::Isolate::CreateParams create_params;
        IsolateScope *isolate_scope;

        v8::Local<v8::Context> context;
        ContextScope *context_scope;


        JSContext() :
                isolate(isolate)
        {
            create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
            isolate = v8::Isolate::New(create_params);

            isolate_scope = new IsolateScope(isolate);

            context = v8::Context::New(isolate);
            context_scope = new ContextScope(context);
        }
        ~JSContext() {
            delete context_scope;
            delete isolate_scope;
            isolate->Dispose();
        }
    };

    std::unique_ptr<v8::Platform>  JSContext::platform;
}

JSScript::JSScript() : Script("javascript") {
    js = new JSContext();
}



JSScript::~JSScript() {
    delete js;
}

void JSScript::setup(const char *path) {
    v8::V8::InitializeICUDefaultLocation(path);
    v8::V8::InitializeExternalStartupData(path);

    JSContext::platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(JSContext::platform.get());
    v8::V8::Initialize();
}

void JSScript::shutdown() {
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    JSContext::platform = nullptr;
}

gcore::Variant JSScript::runScript(const char *script) const {
    // Create a string containing the JavaScript source code.
    v8::Local<v8::String> source =
            v8::String::NewFromUtf8(js->isolate, script,
                                    v8::NewStringType::kNormal)
                    .ToLocalChecked();

    // Compile the source code.
    v8::Local<v8::Script> s =
            v8::Script::Compile(js->context, source).ToLocalChecked();

    // Run the script to get the result.
    v8::Local<v8::Value> result = s->Run(js->context).ToLocalChecked();

    // Convert the result to an UTF8 string and print it.
    v8::String::Utf8Value utf8(js->isolate, result);
    printf("%s\n", *utf8);
// Dispose the isolate and tear down V8.
    return 0;
}