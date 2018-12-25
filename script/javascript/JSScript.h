//
// Created by Gen2 on 2018-12-25.
//

#ifndef GRENDER_TEST_JSSCRIPT_H
#define GRENDER_TEST_JSSCRIPT_H


#include <core/script/NativeObject.h>
#include <core/script/Script.h>
#include <core/script/ScriptClass.h>
#include <core/script/ScriptInstance.h>

#include "../script_define.h"

namespace gscript {

    struct JSContext;

    class JSScript : public gcore::Script {
        JSContext *js;

    public:
        JSScript();
        ~JSScript();

        static void setup(const char *path);
        static void shutdown();

        virtual gcore::ScriptClass *makeClass() const {
        }
        virtual gcore::ScriptInstance *newBuff(const std::string &cls_name, gcore::Object *target, const gcore::Variant **params, int count) const {
        }

        gcore::Variant runScript(const char *script) const;
    };
}


#endif //GRENDER_TEST_JSSCRIPT_H
