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
    
    class RubyScript : public gcore::Script {
        std::string context_root;
        mrb_state *mrb;
        
        void _setup(const char *root) const;
        
    protected:
        virtual gcore::ScriptClass *makeClass() const;
    public:
        RubyScript();
        ~RubyScript();

        gcore::ScriptInstance  *newBuff(const std::string &cls_name, gcore::Object *target, const gcore::Variant **params, int count) const;
        RubyInstance    *newBuff(struct RClass *cls, gcore::Object *target, const gcore::Variant **params, int count) const;
        RubyClass       *reg_class(struct RClass *cls, const gcore::StringName &name);
        
        void reset();
        void setup(const char *root);
        void addEnvPath(const char *path);
        gcore::Variant run(const char *filepath) const;
        gcore::Variant runScript(const char *script) const;
        gcore::Variant call(const gcore::StringName &name, const gcore::Variant **params, int count) const;
        _FORCE_INLINE_ mrb_state *getMRB() {
            return mrb;
        }
    };
    
    CLASS_BEGIN_N(RubyClass, gcore::ScriptClass)
        struct RClass *script_class;
        friend class RubyScript;
        
    protected:
        virtual gcore::ScriptInstance *makeInstance() const;
    public:
        _FORCE_INLINE_ struct RClass *getScriptClass() const {
            return script_class;
        }
    
    public:
        
        virtual gcore::Variant apply(const gcore::StringName &name, const gcore::Variant **params, int count) const;
    CLASS_END
    
    CLASS_BEGIN_N(RubyInstance, gcore::ScriptInstance)
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
        
        virtual gcore::Variant apply(const gcore::StringName &name, const gcore::Variant **params, int count);
    CLASS_END
    
    CLASS_BEGIN_N(RubyNativeObject, gcore::NativeObject)

        mrb_state *mrb;

        _FORCE_INLINE_ struct RObject *getInstance() {
            return static_cast<struct RObject *>(getNative());
        }

    public:
        RubyNativeObject(){}
        RubyNativeObject(mrb_state *mrb, struct RObject *native);

        virtual void setNative(void *native);
        ~RubyNativeObject();

        virtual void apply(const gcore::StringName &name, gcore::Variant *result = NULL, const gcore::Variant **params = NULL, int count = 0);

    CLASS_END
}

#endif /* RubyScript_hpp */

