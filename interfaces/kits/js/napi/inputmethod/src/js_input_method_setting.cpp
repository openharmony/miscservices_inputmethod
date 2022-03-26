/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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
#include "js_input_method_setting.h"
#include "input_method_controller.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "event_handler.h"
#include "event_runner.h"
#include "string_ex.h"
namespace OHOS {
namespace MiscServices {
    using namespace AbilityRuntime;
    constexpr size_t ARGC_ZERO = 0;

    void JsInputMethodSetting::Finalizer(NativeEngine* engine, void* data, void* hint)
    {
        IMSA_HILOGI("JsInputMethodSetting::Finalizer is called");
        std::unique_ptr<JsInputMethodSetting>(static_cast<JsInputMethodSetting*>(data));
    }

    NativeValue* JsInputMethodSetting::DisplayOptionalInputMethod(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsInputMethodSetting* me = CheckParamsAndGetThis<JsInputMethodSetting>(engine, info);
        return (me != nullptr) ? me->OnDisplayOptionalInputMethod(*engine, *info) : nullptr;
    }

    NativeValue* JsInputMethodSetting::ListInputMethod(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsInputMethodSetting* me = CheckParamsAndGetThis<JsInputMethodSetting>(engine, info);
        return (me != nullptr) ? me->OnListInputMethod(*engine, *info) : nullptr;
    }

    NativeValue* JsInputMethodSetting::OnDisplayOptionalInputMethod(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsInputMethodSetting::OnDisplayOptionalInputMethod is called!");
        if (info.argc != ARGC_ZERO) {
            IMSA_HILOGI("JsInputMethodSetting::OnDisplayOptionalInputMethod has params, return!");
            return engine.CreateUndefined();
        }

        InputMethodController::GetInstance()->DisplayOptionalInputMethod();

        return engine.CreateUndefined();
    }

    NativeValue* JsInputMethodSetting::OnListInputMethod(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsInputMethodSetting::ListInputMethod is called");
        if (info.argc != ARGC_ZERO) {
            IMSA_HILOGI("JsInputMethodSetting::ListInputMethod has params, return!");
            return engine.CreateUndefined();
        }

        std::vector<InputMethodProperty*> properties = InputMethodController::GetInstance()->ListInputMethod();
        if (!properties.size()) {
            IMSA_HILOGI("JsInputMethodSetting::ListInputMethod has no ime");
            return engine.CreateUndefined();
        }
        NativeValue* arrayValue = engine.CreateArray(properties.size());
        NativeArray* array = ConvertNativeValueTo<NativeArray>(arrayValue);
        uint32_t index = 0;
        for (const auto &info : properties) {
            NativeValue* objValue = engine.CreateObject();
            NativeObject* object = ConvertNativeValueTo<NativeObject>(objValue);

            IMSA_HILOGI("JsInputMethodSetting::ListInputMethod %{public}s/%{public}s", Str16ToStr8(info->mPackageName).c_str(), Str16ToStr8(info->mAbilityName).c_str());

            object->SetProperty("packageName", CreateJsValue(engine, Str16ToStr8(info->mPackageName)));
            object->SetProperty("methodId", CreateJsValue(engine, Str16ToStr8(info->mAbilityName)));
            array->SetElement(index++, objValue);
        }

        return arrayValue;
    }
} // namespace MiscServices
} // namespace OHOS
