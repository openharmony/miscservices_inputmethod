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

#ifndef INTERFACE_KITS_JS_NAPI_INPUTMETHODENGINE_INCLUDE_JS_KEYBOARD_CONTROLLER_H
#define INTERFACE_KITS_JS_NAPI_INPUTMETHODENGINE_INCLUDE_JS_KEYBOARD_CONTROLLER_H
#include "native_engine/native_engine.h"
#include "native_engine/native_value.h"
#include "js_runtime_utils.h"
namespace OHOS {
    namespace MiscServices {
        class JsKeyboardController {
        public:
            JsKeyboardController() = default;
            ~JsKeyboardController() = default;
            static void Finalizer(NativeEngine* engine, void* data, void* hint);
            static NativeValue* HideKeyboardSelf(NativeEngine* engine, NativeCallbackInfo* info);

        private:
            NativeValue* OnHideKeyboardSelf(NativeEngine& engine, NativeCallbackInfo& info);
        };
    } // namespace MiscServices
} // namespace OHOS
#endif // INTERFACE_KITS_JS_NAPI_INPUTMETHODENGINE_INCLUDE_JS_KEYBOARD_CONTROLLER_H
