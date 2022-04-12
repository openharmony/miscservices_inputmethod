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
#include "js_keyboard_delegate_listener.h"
#include "global.h"
#include "js_runtime_utils.h"
#include "js_input_method_engine_utils.h"

namespace OHOS {
namespace MiscServices {
    using namespace AbilityRuntime;
    void JsKeyboardDelegateListener::RegisterListenerWithType(NativeEngine& engine, std::string type, NativeValue* value)
    {
        // should do type check
        if (IfCallbackRegistered(type, value)) {
            IMSA_HILOGI("JsKeyboardDelegateListener::RegisterListenerWithType callback already registered!");
            return;
        }
        std::unique_ptr<NativeReference> callbackRef;
        callbackRef.reset(engine.CreateReference(value, 1));

        AddCallback(type, value);
    }

    void JsKeyboardDelegateListener::AddCallback(std::string type, NativeValue* jsListenerObject)
    {
        IMSA_HILOGI("JsKeyboardDelegateListener::AddCallback is called");
        std::lock_guard<std::mutex> lock(mMutex);
        std::unique_ptr<NativeReference> callbackRef;
        callbackRef.reset(engine_->CreateReference(jsListenerObject, 1));
        jsCbMap_[type].push_back(std::move(callbackRef));
        IMSA_HILOGI("JsKeyboardDelegateListener::AddCallback success");
        IMSA_HILOGI("jsCbMap_ size: %{public}d, and type[%{public}s] size: %{public}d!",
            static_cast<uint32_t>(jsCbMap_.size()), type.c_str(), static_cast<uint32_t>(jsCbMap_[type].size()));
        return;
    }
    void JsKeyboardDelegateListener::UnregisterAllListenerWithType(std::string type)
    {
        IMSA_HILOGI("JsKeyboardDelegateListener::UnregisterAllListenerWithType");
        // should do type check
        if (jsCbMap_.empty() || jsCbMap_.find(type) == jsCbMap_.end()) {
            IMSA_HILOGI("methodName %{public}s not registerted!", type.c_str());
            return;
        }
        for (auto it = jsCbMap_[type].begin(); it != jsCbMap_[type].end();) {
            jsCbMap_[type].erase(it);
        }
        // one type with multi jscallback, erase type when there is no callback in one type
        if (jsCbMap_[type].empty()) {
            jsCbMap_.erase(type);
        }
        return;
    }

    void JsKeyboardDelegateListener::UnregisterListenerWithType(std::string type, NativeValue* value)
    {
        IMSA_HILOGI("JsKeyboardDelegateListener::UnregisterListenerWithType");
        // should do type check
        if (jsCbMap_.empty() || jsCbMap_.find(type) == jsCbMap_.end()) {
            IMSA_HILOGI("methodName %{public}s not registerted!", type.c_str());
            return;
        }
        for (auto it = jsCbMap_[type].begin(); it != jsCbMap_[type].end();) {
            if (value->StrictEquals((*it)->Get())) {
                jsCbMap_[type].erase(it);
                break;
            }
        }
        // one type with multi jscallback, erase type when there is no callback in one type
        if (jsCbMap_[type].empty()) {
            jsCbMap_.erase(type);
        }
        return;
    }

    bool JsKeyboardDelegateListener::IfCallbackRegistered(std::string type, NativeValue* jsListenerObject)
    {
        IMSA_HILOGI("JsKeyboardDelegateListener::IfCallbackRegistered");
        if (jsCbMap_.empty() || jsCbMap_.find(type) == jsCbMap_.end()) {
            IMSA_HILOGI("methodName %{public}s not registertd!", type.c_str());
            return false;
        }

        for (auto iter = jsCbMap_[type].begin(); iter != jsCbMap_[type].end(); iter++) {
            if (jsListenerObject->StrictEquals((*iter)->Get())) {
                IMSA_HILOGI("JsKeyboardDelegateListener::IfCallbackRegistered callback already registered!");
                return true;
            }
        }
        return false;
    }

    void JsKeyboardDelegateListener::CallJsMethod(std::string methodName, NativeValue* const* argv, size_t argc)
    {
        IMSA_HILOGI("JsKeyboardDelegateListener::CallJsMethod");
        if (!engine_) {
            IMSA_HILOGI("engine_ nullptr");
            return;
        }

        if (jsCbMap_.empty() || jsCbMap_.find(methodName) == jsCbMap_.end()) {
            IMSA_HILOGI("methodName %{public}s not registertd!", methodName.c_str());
            return;
        }

        for (auto iter = jsCbMap_[methodName].begin(); iter != jsCbMap_[methodName].end(); iter++) {
            engine_->CallFunction(engine_->CreateUndefined(), (*iter)->Get(), argv, argc);
        }
    }

    bool JsKeyboardDelegateListener::CallJsMethodReturnBool(std::string methodName,
        NativeValue* const* argv, size_t argc)
    {
        IMSA_HILOGI("JsKeyboardDelegateListener::CallJsMethodReturnBool");
        if (!engine_) {
            IMSA_HILOGI("engine_ nullptr");
            return false;
        }

        if (jsCbMap_.empty() || jsCbMap_.find(methodName) == jsCbMap_.end()) {
            IMSA_HILOGI("methodName %{public}s not registertd!", methodName.c_str());
            return false;
        }

        bool result = false;
        for (auto iter = jsCbMap_[methodName].begin(); iter != jsCbMap_[methodName].end(); iter++) {
            NativeValue* nativeValue = engine_->CallFunction(engine_->CreateUndefined(), (*iter)->Get(), argv, argc);
            bool ret = false;
            if (ConvertFromJsValue(*engine_, nativeValue, ret) && ret) {
                result = true;
            }
        }
        return result;
    }

    bool JsKeyboardDelegateListener::OnKeyEvent(int32_t keyCode, int32_t keyStatus)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        IMSA_HILOGI("JsKeyboardDelegateListener::OnKeyEvent");

        auto result = false;
        auto task = [this, keyCode, keyStatus, &result] () {
            NativeValue* nativeValue = engine_->CreateObject();
            NativeObject* object = ConvertNativeValueTo<NativeObject>(nativeValue);
            if (!object) {
                IMSA_HILOGI("Failed to convert rect to jsObject");
                return;
            }
            NativeValue* argv[] = {nativeValue};
            std::string methodName;
            if (keyStatus == 2) {
                methodName = "keyDown";
            } else {
                methodName = "keyUp";
            }
            object->SetProperty("keyCode", CreateJsValue(*engine_, static_cast<uint32_t>(keyCode)));
            object->SetProperty("keyAction", CreateJsValue(*engine_, static_cast<uint32_t>(keyStatus)));
            result = CallJsMethodReturnBool(methodName, argv, ArraySize(argv));
        };

        mainHandler_->PostSyncTask(task);
        return result;
    }

    void JsKeyboardDelegateListener::OnCursorUpdate(int32_t positionX, int32_t positionY, int height)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        IMSA_HILOGI("JsKeyboardDelegateListener::OnCursorUpdate");

        auto task = [this, positionX, positionY, height] () {
            NativeValue* nativeXValue = CreateJsValue(*engine_, static_cast<uint32_t>(positionX));
            NativeValue* nativeYValue = CreateJsValue(*engine_, static_cast<uint32_t>(positionY));
            NativeValue* nativeHValue = CreateJsValue(*engine_, static_cast<uint32_t>(height));

            NativeValue* argv[] = {nativeXValue, nativeYValue, nativeHValue};
            std::string methodName = "cursorContextChange";
            CallJsMethod(methodName, argv, ArraySize(argv));
        };
        mainHandler_->PostTask(task);
    }

    void JsKeyboardDelegateListener::OnSelectionChange(int32_t oldBegin, int32_t oldEnd, int32_t newBegin, int32_t newEnd)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        IMSA_HILOGI("JsKeyboardDelegateListener::OnSelectionChange");

        auto task = [this, oldBegin, oldEnd, newBegin, newEnd] () {
            NativeValue* nativeOBValue = CreateJsValue(*engine_, static_cast<uint32_t>(oldBegin));
            NativeValue* nativeOEValue = CreateJsValue(*engine_, static_cast<uint32_t>(oldEnd));
            NativeValue* nativeNBHValue = CreateJsValue(*engine_, static_cast<uint32_t>(newBegin));
            NativeValue* nativeNEValue = CreateJsValue(*engine_, static_cast<uint32_t>(newEnd));

            NativeValue* argv[] = {nativeOBValue, nativeOEValue, nativeNBHValue, nativeNEValue};
            std::string methodName = "selectionChange";
            CallJsMethod(methodName, argv, ArraySize(argv));
        };

        mainHandler_->PostTask(task);
    }

    void JsKeyboardDelegateListener::OnTextChange(std::string text)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        IMSA_HILOGI("JsKeyboardDelegateListener::OnTextChange");
        auto task = [this, text] () {
            NativeValue* nativeValue = CreateJsValue(*engine_, text);

            NativeValue* argv[] = {nativeValue};
            std::string methodName = "textChange";
            CallJsMethod(methodName, argv, ArraySize(argv));
        };
        mainHandler_->PostTask(task);
    }
} // namespace MiscServices
} // namespace OHOS
