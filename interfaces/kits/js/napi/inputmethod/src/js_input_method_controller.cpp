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
#include "js_input_method_controller.h"
#include "input_method_controller.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "event_handler.h"
#include "event_runner.h"
#include "string_ex.h"
namespace OHOS {
namespace MiscServices {
    constexpr size_t ARGC_ZERO = 0;

    void JsInputMethodController::Finalizer(NativeEngine* engine, void* data, void* hint)
    {
        IMSA_HILOGI("JsInputMethodController::Finalizer is called");
        std::unique_ptr<JsInputMethodController>(static_cast<JsInputMethodController*>(data));
    }

    NativeValue* JsInputMethodController::StopInput(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsInputMethodController* me = AbilityRuntime::CheckParamsAndGetThis<JsInputMethodController>(engine, info);
        return (me != nullptr) ? me->OnStopInput(*engine, *info) : nullptr;
    }

    NativeValue* JsInputMethodController::OnStopInput(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsInputMethodController::OnStopInput is called!");
        if (info.argc != ARGC_ZERO) {
            IMSA_HILOGI("JsInputMethodController::OnStopInput has params, return!");
            return engine.CreateUndefined();
        }

        InputMethodController::GetInstance()->HideCurrentInput();

        NativeValue* result = AbilityRuntime::CreateJsValue(engine, true);

        return result;
    }
}
}