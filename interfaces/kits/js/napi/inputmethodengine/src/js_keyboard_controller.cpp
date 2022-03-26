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
#include "js_keyboard_controller.h"
#include "input_method_controller.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "event_handler.h"
#include "event_runner.h"
#include "string_ex.h"
#include "input_method_ability.h"
namespace OHOS {
namespace MiscServices {
    using namespace AbilityRuntime;
    constexpr size_t ARGC_ZERO = 0;
    void JsKeyboardController::Finalizer(NativeEngine* engine, void* data, void* hint)
    {
        IMSA_HILOGI("JsKeyboardController::Finalizer is called");
        std::unique_ptr<JsKeyboardController>(static_cast<JsKeyboardController*>(data));
    }

    NativeValue* JsKeyboardController::HideKeyboardSelf(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsKeyboardController* me = CheckParamsAndGetThis<JsKeyboardController>(engine, info);
        return (me) ? me->OnHideKeyboardSelf(*engine, *info) : nullptr;
    }

    NativeValue* JsKeyboardController::OnHideKeyboardSelf(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsKeyboardController::OnHideKeyboardSelf is called!");
        if (info.argc != ARGC_ZERO) {
            IMSA_HILOGI("JsKeyboardController::OnHideKeyboardSelf has params, return!");
            return engine.CreateUndefined();
        }

        InputMethodAbility::GetInstance()->HideKeyboardSelf();

        return engine.CreateUndefined();
    }
} // namespace MiscServices
} // namespace OHOS
