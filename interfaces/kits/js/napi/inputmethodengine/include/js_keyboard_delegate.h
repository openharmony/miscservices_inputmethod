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

#ifndef OHOS_MISCSERVICES_JS_KEYBOARD_DELEGATE_H
#define OHOS_MISCSERVICES_JS_KEYBOARD_DELEGATE_H

#include "js_runtime_utils.h"
#include "native_engine/native_engine.h"
#include "native_engine/native_value.h"
#include "js_keyboard_delegate_listener.h"
#include "event_handler.h"
#include "event_runner.h"

namespace OHOS {
    namespace MiscServices {
        class JsKeyboardDelegate {
        public:
            JsKeyboardDelegate(NativeEngine* engine);
            ~JsKeyboardDelegate() = default;
            static void Finalizer(NativeEngine* engine, void* data, void* hint);
            static NativeValue* RegisterCallback(NativeEngine* engine, NativeCallbackInfo* info);
            static NativeValue* UnRegisterCallback(NativeEngine* engine, NativeCallbackInfo* info);
        private:
            sptr<JsKeyboardDelegateListener> kdListener_;
            std::mutex mtx_;
            NativeValue* OnRegisterCallback(NativeEngine& engine, NativeCallbackInfo& info);
            NativeValue* OnUnRegisterCallback(NativeEngine& engine, NativeCallbackInfo& info);
            std::shared_ptr<AppExecFwk::EventHandler> GetMainHandler();
            std::shared_ptr<AppExecFwk::EventHandler> mainHandler_ = nullptr;
        };
    }
}
#endif // OHOS_MISCSERVICES_JS_KEYBOARD_DELEGATE_H