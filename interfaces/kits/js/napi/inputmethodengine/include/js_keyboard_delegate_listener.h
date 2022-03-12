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
#ifndef FM_IMMS_PROJECT_JS_KEYBOARD_DELEGATE_LISTENER_H
#define FM_IMMS_PROJECT_JS_KEYBOARD_DELEGATE_LISTENER_H

#include <map>
#include <mutex>
#include <unordered_set>
#include <queue>
#include <condition_variable>
#include <refbase.h>
#include "native_engine/native_engine.h"
#include "native_engine/native_value.h"
#include "event_handler.h"
#include "event_runner.h"
namespace OHOS {
namespace MiscServices {
    class JsKeyboardDelegateListener : virtual public RefBase {
    public:
        explicit JsKeyboardDelegateListener(NativeEngine* engine) : engine_(engine) {}
        JsKeyboardDelegateListener(NativeEngine* engine, std::shared_ptr<AppExecFwk::EventHandler> &handler)
            : engine_(engine), mainHandler_(handler) {}
        virtual ~JsKeyboardDelegateListener() = default;
        void RegisterListenerWithType(NativeEngine& engine, std::string type, NativeValue* value);
        void UnregisterListenerWithType(std::string type, NativeValue* value);
        void UnregisterAllListenerWithType(std::string type);
        bool OnKeyEvent(int32_t keyCode, int32_t keyStatus);
        void OnCursorUpdate(int32_t positionX, int32_t positionY, int height);
        void OnSelectionChange(int32_t oldBegin, int32_t oldEnd, int32_t newBegin, int32_t newEnd);
        void OnTextChange(std::string text);
    private:
        void AddCallback(std::string type, NativeValue* jsListenerObject);
        void CallJsMethod(std::string methodName, NativeValue* const* argv = nullptr, size_t argc = 0);
        bool CallJsMethodReturnBool(std::string methodName, NativeValue* const* argv = nullptr, size_t argc = 0);
        void RemoveCallback(NativeValue* jsListenerObject);
        bool IfCallbackRegistered(std::string type, NativeValue* jsListenerObject);
        NativeEngine* engine_ = nullptr;
        std::mutex mMutex;
        std::map<std::string, std::vector<std::unique_ptr<NativeReference>>> jsCbMap_;
        std::shared_ptr<AppExecFwk::EventHandler> mainHandler_ = nullptr;
    };
}
}
#endif // FM_IMMS_PROJECT_JS_KEYBOARD_DELEGATE_LISTENER_H
