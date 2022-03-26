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

#ifndef INTERFACE_KITS_JS_NAPI_INPUTMETHODENGINE_INCLUDE_JS_INPUT_METHOD_ENGINE_H
#define INTERFACE_KITS_JS_NAPI_INPUTMETHODENGINE_INCLUDE_JS_INPUT_METHOD_ENGINE_H

#include "js_runtime_utils.h"
#include "native_engine/native_engine.h"
#include "native_engine/native_value.h"
#include "js_input_method_engine_listener.h"

namespace OHOS {
    namespace MiscServices {
        class JsInputMethodEngine {
        public:
            JsInputMethodEngine(NativeEngine* engine);
            ~JsInputMethodEngine() = default;
            static void Finalizer(NativeEngine* engine, void* data, void* hint);
            static NativeValue* RegisterCallback(NativeEngine* engine, NativeCallbackInfo* info);
            static NativeValue* UnRegisterCallback(NativeEngine* engine, NativeCallbackInfo* info);

        private:
            sptr<JsInputMethodEngineListener> imeListener_;
            std::mutex mtx_;
            NativeValue* OnRegisterCallback(NativeEngine& engine, NativeCallbackInfo& info);
            NativeValue* OnUnRegisterCallback(NativeEngine& engine, NativeCallbackInfo& info);
            std::shared_ptr<AppExecFwk::EventHandler> GetMainHandler();
            std::shared_ptr<AppExecFwk::EventHandler> mainHandler_ = nullptr;
        };
    } // namespace MiscServices
} // namespace OHOS
#endif // INTERFACE_KITS_JS_NAPI_INPUTMETHODENGINE_INCLUDE_JS_INPUT_METHOD_ENGINE_H
