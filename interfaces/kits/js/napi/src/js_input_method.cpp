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

#include "js_input_method.h"
#include <map>
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "string_ex.h"
#include "input_method_utils.h"
#include "event_handler.h"
#include "event_runner.h"
#include "input_method_controller.h"

namespace OHOS {
namespace MiscServices {
    namespace {
        constexpr size_t ARGC_ZERO = 0;
    }
    class JsInputMethod {
    public:
        explicit JsInputMethod(NativeEngine* engine)
        {
            IMSA_HILOGI("JsInputMethod::Constructor is called");
        }

        ~JsInputMethod() = default;

        static void Finalizer(NativeEngine* engine, void* data, void* hint)
        {
            IMSA_HILOGI("JsInputMethod::Finalizer is called");
            std::unique_ptr<JsInputMethod>(static_cast<JsInputMethod*>(data));
        }

        static NativeValue* GetInputMethodSetting(NativeEngine* engine, NativeCallbackInfo* info)
        {
            IMSA_HILOGI("JsInputMethod::GetInputMethodSetting is called");
            JsInputMethod* me = AbilityRuntime::CheckParamsAndGetThis<JsInputMethod>(engine, info);
            return (me != nullptr) ? me->OnGetInputMethodSetting(*engine, *info) : nullptr;
        }

        static NativeValue* DisplayOptionalInputMethod(NativeEngine* engine, NativeCallbackInfo* info)
        {
            IMSA_HILOGI("JsInputMethod::DisplayOptionalInputMethod is called");
            if (info->argc != ARGC_ZERO) {
                IMSA_HILOGI("JsInputMethod::OnDisplayOptionalInputMethod has params, return!");
                return engine->CreateUndefined();
            }

            InputMethodController::GetInstance()->DisplayOptionalInputMethod();

            return engine->CreateUndefined();
        }

        static NativeValue* ListInputMethod(NativeEngine* engine, NativeCallbackInfo* info)
        {
            IMSA_HILOGI("JsInputMethod::ListInputMethod is called");
            if (info->argc != ARGC_ZERO) {
                IMSA_HILOGI("JsInputMethod::ListInputMethod has params, return!");
                return engine->CreateUndefined();
            }

            std::vector<InputMethodProperty*> properties = InputMethodController::GetInstance()->ListInputMethod();
            if (properties.size() == 0) {
                IMSA_HILOGI("JsInputMethod::ListInputMethod has no ime");
                return engine->CreateUndefined();
            }
            NativeValue* arrayValue = engine->CreateArray(properties.size());
            NativeArray* array = AbilityRuntime::ConvertNativeValueTo<NativeArray>(arrayValue);
            uint32_t index = 0;
            for (const auto &info : properties) {
                NativeValue* objValue = engine->CreateObject();
                NativeObject* object = AbilityRuntime::ConvertNativeValueTo<NativeObject>(objValue);

                IMSA_HILOGI("JsInputMethod::ListInputMethod %{public}s/%{public}s", Str16ToStr8(info->mPackageName).c_str(), Str16ToStr8(info->mAbilityName).c_str());

                object->SetProperty("packageName", AbilityRuntime::CreateJsValue(*engine, Str16ToStr8(info->mPackageName)));
                object->SetProperty("methodId", AbilityRuntime::CreateJsValue(*engine, Str16ToStr8(info->mAbilityName)));
                array->SetElement(index++, objValue);
            }

            return arrayValue;
        }
    private:
        NativeValue* OnGetInputMethodSetting(NativeEngine& engine, NativeCallbackInfo& info)
        {
            IMSA_HILOGI("JsInputMethod::OnGetInputMethodSetting is called!");

            NativeValue* objValue = engine.CreateObject();
            NativeObject* object = AbilityRuntime::ConvertNativeValueTo<NativeObject>(objValue);

            AbilityRuntime::BindNativeFunction(engine, *object, "displayOptionalInputMethod",
                JsInputMethod::DisplayOptionalInputMethod);
            AbilityRuntime::BindNativeFunction(engine, *object, "listInputMethod", JsInputMethod::ListInputMethod);

            return objValue;
        }
    };

    NativeValue* JsInputMethodInit(NativeEngine* engine, NativeValue* exportObj)
    {
        IMSA_HILOGI("JsInputMethodInit() is called!");

        if (engine == nullptr || exportObj == nullptr) {
            IMSA_HILOGI("engine or exportObj null");
            return nullptr;
        }

        NativeObject* object = AbilityRuntime::ConvertNativeValueTo<NativeObject>(exportObj);
        if (object == nullptr) {
            IMSA_HILOGI("object null");
            return nullptr;
        }

        std::unique_ptr<JsInputMethod> jsInputMethod = std::make_unique<JsInputMethod>(engine);
        object->SetNativePointer(jsInputMethod.release(), JsInputMethod::Finalizer, nullptr);

        // create inputMethod
        AbilityRuntime::BindNativeFunction(*engine, *object, "getInputMethodSetting",
            JsInputMethod::GetInputMethodSetting);

        return engine->CreateUndefined();
    }
}
}