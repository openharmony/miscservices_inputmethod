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

        static NativeValue *SwitchInputMethod(NativeEngine *engine, NativeCallbackInfo *info)
        {
            IMSA_HILOGI("JsInputMethodRegistry::SwitchInputMethod is called");
            JsInputMethodRegistry *me = CheckParamsAndGetThis<JsInputMethodRegistry>(engine, info);
            return (me != nullptr) ? me->OnSwitchInputMethod(*engine, *info) : nullptr;
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

        NativeValue *OnSwitchInputMethod(NativeEngine &engine, NativeCallbackInfo &info)
        {
            IMSA_HILOGI("JsInputMethodRegistry::OnSwitchInputMethod is called!");
            if (info.argc != 1) {
                IMSA_HILOGE("JsInputMethodRegistry::OnSwitchInputMethod Params not match");
                return engine.CreateUndefined();
            }

            InputMethodProperty *target = new InputMethodProperty();
            NativeObject *object = ConvertNativeValueTo<NativeObject>(info.argv[0]);
            if (object == nullptr) {
                IMSA_HILOGE("JsInputMethodRegistry::OnSwitchInputMethod Failed to get object");
                return engine.CreateUndefined();
            }

            if (!GetInputMethodPropertyFromJs(engine, object, *target)) {
                return engine.CreateUndefined();
            }

            bool isSwitchSuccess = false;
            if (!InputMethodController::GetInstance()->SwitchInputMethod(target)) {
                isSwitchSuccess = true;
            } else {
                IMSA_HILOGE("JsInputMethodRegistry::OnSwitchInputMethod isSwitchSuccess is false !");
            }

            NativeValue *result = CreateJsValue(engine, isSwitchSuccess);

            return result;
        }

        bool GetInputMethodPropertyFromJs(NativeEngine &engine, NativeObject *propertyObject, 
            InputMethodProperty &target)
        {
            IMSA_HILOGI("JsInputMethodRegistry::GetInputMethodPropertyFromJs is called!");
            NativeValue *packageName = propertyObject->GetProperty("packageName");
            std::string packageNameUtf8;
            if (!ConvertFromJsValue(engine, packageName, packageNameUtf8)) {
                IMSA_HILOGE("JsInputMethodRegistry::GetInputMethodPropertyFromJs Failed to convert parameter to "
                            "PackageName");
                return false;
            }
            target.mPackageName =
                std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(packageNameUtf8);

            NativeValue *imeId = propertyObject->GetProperty("methodId");
            std::string imeIdUtf8;
            if (!ConvertFromJsValue(engine, imeId, imeIdUtf8)) {
                IMSA_HILOGE("JsInputMethodRegistry::GetInputMethodPropertyFromJs Failed to convert parameter to ImeId");
                return false;
            }
            target.mImeId = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(imeIdUtf8);

            return true;
        }
    };

    NativeValue* JsInputMethodRegistryInit(NativeEngine* engine, NativeValue* exportObj)
    {
        IMSA_HILOGI("JsInputMethodRegistryInit() is called!");

        if (!engine || !exportObj) {
            IMSA_HILOGI("engine or exportObj null");
            return nullptr;
        }

        NativeObject* object = ConvertNativeValueTo<NativeObject>(exportObj);
        if (!object) {
            IMSA_HILOGI("object null");
            return nullptr;
        }

        std::unique_ptr<JsInputMethodRegistry> jsInputMethodRegistry = std::make_unique<JsInputMethodRegistry>(engine);
        object->SetNativePointer(jsInputMethodRegistry.release(), JsInputMethodRegistry::Finalizer, nullptr);

        BindNativeFunction(*engine, *object, "getInputMethodSetting", JsInputMethodRegistry::GetInputMethodSetting);
        BindNativeFunction(*engine, *object, "getInputMethodController", JsInputMethodRegistry::GetInputMethodController);
        BindNativeFunction(*engine, *object, "switchInputMethod", JsInputMethodRegistry::SwitchInputMethod);

        object->SetProperty("MAX_TYPE_NUM", CreateJsValue(*engine, static_cast<uint32_t>(MAX_TYPE_NUM)));

        return engine->CreateUndefined();
    }
} // namespace MiscServices
} // namespace OHOS
