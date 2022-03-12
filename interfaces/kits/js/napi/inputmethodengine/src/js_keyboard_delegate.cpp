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
#include "js_keyboard_delegate.h"
#include "input_method_controller.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "event_handler.h"
#include "event_runner.h"
#include "string_ex.h"
#include "js_keyboard_delegate_listener.h"
#include "input_method_ability.h"
#include "global.h"
namespace OHOS {
namespace MiscServices {
    using namespace AbilityRuntime;
    constexpr size_t ARGC_TWO = 2;
    JsKeyboardDelegate::JsKeyboardDelegate(NativeEngine* engine)
    {
        IMSA_HILOGI("JsKeyboardDelegate::Constructor is called");
        auto mainHandler = GetMainHandler();
        kdListener_ = new JsKeyboardDelegateListener(engine, mainHandler);
        InputMethodAbility::GetInstance()->setKdListener(kdListener_);
    }

    void JsKeyboardDelegate::Finalizer(NativeEngine* engine, void* data, void* hint)
    {
        IMSA_HILOGI("JsKeyboardDelegate::Finalizer is called");
        std::unique_ptr<JsKeyboardDelegate>(static_cast<JsKeyboardDelegate*>(data));
    }

    std::shared_ptr<AppExecFwk::EventHandler> JsKeyboardDelegate::GetMainHandler()
    {
        if (!mainHandler_) {
            mainHandler_ =
            std::make_shared<AppExecFwk::EventHandler>(AppExecFwk::EventRunner::GetMainEventRunner());
        }
        return mainHandler_;
    }

    NativeValue* JsKeyboardDelegate::RegisterCallback(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsKeyboardDelegate* me = CheckParamsAndGetThis<JsKeyboardDelegate>(engine, info);
        return (me != nullptr) ? me->OnRegisterCallback(*engine, *info) : nullptr;
    }

    NativeValue* JsKeyboardDelegate::UnRegisterCallback(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsKeyboardDelegate* me = CheckParamsAndGetThis<JsKeyboardDelegate>(engine, info);
        return (me != nullptr) ? me->OnUnRegisterCallback(*engine, *info) : nullptr;
    }

    NativeValue* JsKeyboardDelegate::OnRegisterCallback(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsKeyboardDelegate::OnRegisterCallback is called!");
        if (info.argc != ARGC_TWO) {
            IMSA_HILOGI("JsKeyboardDelegate::OnRegisterCallback Params not match");
            return engine.CreateUndefined();
        }

        std::string cbType;
        if (!ConvertFromJsValue(engine, info.argv[0], cbType)) {
            IMSA_HILOGI("JsKeyboardDelegate::OnRegisterCallback Failed to convert parameter to callbackType");
            return engine.CreateUndefined();
        }

        NativeValue* value = info.argv[1];
        kdListener_->RegisterListenerWithType(engine, cbType, value);
        return engine.CreateUndefined();
    }

    NativeValue* JsKeyboardDelegate::OnUnRegisterCallback(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsKeyboardDelegate::OnUnRegisterCallback is called!");
        if (info.argc == 0) {
            IMSA_HILOGI("JsKeyboardDelegate::OnUnRegisterCallback Params not match");
            return engine.CreateUndefined();
        }

        std::string cbType;
        if (!ConvertFromJsValue(engine, info.argv[0], cbType)) {
            IMSA_HILOGI("JsKeyboardDelegate::OnUnRegisterCallback Failed to convert parameter to callbackType");
            return engine.CreateUndefined();
        }

        std::lock_guard<std::mutex> lock(mtx_);

        if (info.argc == 1) {
            kdListener_->UnregisterAllListenerWithType(cbType);
        } else {
            NativeValue* value = info.argv[1];
            if (!value->IsCallable()) {
                IMSA_HILOGI("JsKeyboardDelegate::OnUnregisterWindowManagerCallback info->argv[1] is not callable");
                return engine.CreateUndefined();
            }
            kdListener_->UnregisterListenerWithType(cbType, value);
        }
        return engine.CreateUndefined();
    }
}
}