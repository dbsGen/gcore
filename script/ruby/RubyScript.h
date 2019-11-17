//
//  RubyScript.hpp
//  hirender_iOS
//
//  Created by Gen on 16/9/28.
//  Copyright © 2016年 gen. All rights reserved.
//

#ifndef RubyScript_hpp
#define RubyScript_hpp

#include <core/script/NativeObject.h>
#include <core/script/Script.h>
#include <core/script/ScriptClass.h>
#include <core/script/ScriptInstance.h>

#include "../script_define.h"

struct mrb_state;
struct RClass;
struct RObject;
struct mrb_value;

namespace gscript {
    class RubyClass;
    class RubyInstance;
    
    class RubyScript : public gc::Script {
        std::string context_root;
        mrb_state *mrb;

        void _setup(const char *root) const;

        static mrb_value callFunction(struct mrb_state *mrb, mrb_value);

        gc::Variant _apply(const mrb_value &value, const gc::StringName &name, const gc::Variant **params, int count);

        friend class RubyClass;
        friend class RubyInstance;
        
    protected:
        virtual gc::ScriptClass *makeClass() const;
        void defineFunction(const gc::StringName &name, const gc::RCallback &function);
    public:
        RubyScript();
        ~RubyScript();

        gc::ScriptInstance  *newBuff(const std::string &cls_name, gc::Object *target, const gc::Variant **params, int count) const;
        RubyInstance    *newBuff(struct RClass *cls, gc::Object *target, const gc::Variant **params, int count) const;
        RubyClass       *reg_class(struct RClass *cls, const gc::StringName &name);
        
        void reset();
        void setup(const char *root);
        void addEnvPath(const char *path);
        gc::Variant run(const char *filepath) const;
        gc::Variant runScript(const char *script) const;

        gc::Variant apply(const gc::StringName &name, const gc::Variant **params, int count);
        _FORCE_INLINE_ mrb_state *getMRB() {
            return mrb;
        }
    };
    
    CLASS_BEGIN_N(RubyClass, gc::ScriptClass)
        struct RClass *script_class;
        friend class RubyScript;
        
    protected:
        virtual gc::ScriptInstance *makeInstance() const;
    public:
        _FORCE_INLINE_ struct RClass *getScriptClass() const {
            return script_class;
        }
    
    public:
        
        virtual gc::Variant apply(const gc::StringName &name, const gc::Variant **params, int count) const;
    CLASS_END
    
    CLASS_BEGIN_N(RubyInstance, gc::ScriptInstance)
        struct RObject *script_instance;
        struct RClass *script_class;
    
        friend class RubyScript;
        
    public:
        _FORCE_INLINE_ struct RObject *getScriptInstance() {
            return script_instance;
        }
        _FORCE_INLINE_ void setScriptInstance(struct RObject *si) {
            script_instance = si;
        }
        
        virtual gc::Variant apply(const gc::StringName &name, const gc::Variant **params, int count);
    CLASS_END
    
    CLASS_BEGIN_N(RubyNativeObject, gc::NativeObject)

        mrb_state *mrb;
    public:

        virtual void missMethod(const gc::StringName &name, gc::Variant *result, const gc::Variant **params, int count);

        RubyNativeObject() {};
        RubyNativeObject(void *native, mrb_state *mrb);
        virtual void setNative(void *native);
        ~RubyNativeObject();
    CLASS_END
}

#endif /* RubyScript_hpp */

