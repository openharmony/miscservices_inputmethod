/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "js_input_method_registry.h"
#include <map>
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "string_ex.h"
#include "event_handler.h"
#include "event_runner.h"
#include "input_method_controller.h"

namespace OHOS {
namespace MiscServices {
    using namespace AbilityRuntime;
    namespace {
        constexpr size_t ARGC_ZERO = 0;
        constexpr int32_t MAX_TYPE_NUM = 128;
    }
    class JsInputMethodRegistry {
    public:
        explicit JsInputMethodRegistry(NativeEngine* engine)
        {
            IMSA_HILOGI("JsInputMethodRegistry::Constructor is called");
        }

        ~JsInputMethodRegistry() = default;

        static void Finalizer(NativeEngine* engine, void* data, void* hint)
        {
            IMSA_HILOGI("JsInputMethodRegistry::Finalizer is called");
            std::unique_ptr<JsInputMethodRegistry>(static_cast<JsInputMethodRegistry*>(data));
        }

        static NativeValue* GetInputMethodSetting(NativeEngine* engine, NativeCallbackInfo* info)
        {
            IMSA_HILOGI("JsInputMethodRegistry::GetInputMethodSetting is called");
            JsInputMethodRegistry* me = CheckParamsAndGetThis<JsInputMethodRegistry>(engine, info);
            return (me != nullptr) ? me->OnGetInputMethodSetting(*engine, *info) : nullptr;
        }

        static NativeValue* GetInputMethodController(NativeEngine* engine, NativeCallbackInfo* info)
        {
            IMSA_HILOGI("JsInputMethodRegistry::GetInputMethodController is called");
            JsInputMethodRegistry* me = CheckParamsAndGetThis<JsInputMethodRegistry>(engine, info);
            return (me != nullptr) ? me->OnGetInputMethodController(*engine, *info) : nullptr;
        }

    private:
        NativeValue* OnGetInputMethodSetting(NativeEngine& engine, NativeCallbackInfo& info)
        {
            IMSA_HILOGI("JsInputMethodRegistry::OnGetInputMethodSetting is called!");
            if (info.argc != ARGC_ZERO) {
                IMSA_HILOGI("JsInputMethodRegistry::OnGetInputMethodSetting Params not match");
                return engine.CreateUndefined();
            }

            return CreateInputMethodSetting(engine);
        }

        NativeValue* OnGetInputMethodController(NativeEngine& engine, NativeCallbackInfo& info)
        {
            IMSA_HILOGI("JsInputMethodRegistry::OnGetInputMethodController is called!");
            if (info.argc != ARGC_ZERO) {
                IMSA_HILOGI("JsInputMethodRegistry::OnGetInputMethodController Params not match");
                return engine.CreateUndefined();
            }

            return CreateInputMethodController(engine);
        }
    };

    NativeValue* JsInputMethodRegistryInit(NativeEngine* engine, NativeValue* exportObj)
    {
        IMSA_HILOGI("JsInputMethodRegistryInit() is called!");

        if (engine == nullptr || exportObj == nullptr) {
            IMSA_HILOGI("engine or exportObj null");
            return nullptr;
        }

        NativeObject* object = ConvertNativeValueTo<NativeObject>(exportObj);
        if (object == nullptr) {
            IMSA_HILOGI("object null");
            return nullptr;
        }

        std::unique_ptr<JsInputMethodRegistry> jsInputMethodRegistry = std::make_unique<JsInputMethodRegistry>(engine);
        object->SetNativePointer(jsInputMethodRegistry.release(), JsInputMethodRegistry::Finalizer, nullptr);

        BindNativeFunction(*engine, *object, "getInputMethodSetting", JsInputMethodRegistry::GetInputMethodSetting);
        BindNativeFunction(*engine, *object, "getInputMethodController", JsInputMethodRegistry::GetInputMethodController);

        object->SetProperty("MAX_TYPE_NUM", CreateJsValue(*engine, static_cast<uint32_t>(MAX_TYPE_NUM)));

        return engine->CreateUndefined();
    }
}
}