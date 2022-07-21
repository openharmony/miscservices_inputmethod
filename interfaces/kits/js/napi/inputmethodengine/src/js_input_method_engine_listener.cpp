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
#include "js_input_method_engine_listener.h"
#include "global.h"
#include "js_runtime_utils.h"
#include "js_input_method_engine_utils.h"

namespace OHOS {
namespace MiscServices {
    using namespace AbilityRuntime;
    void JsInputMethodEngineListener::RegisterListenerWithType(NativeEngine& engine,
                                                               std::string type, NativeValue* value)
    {
        // should do type check
        if (IfCallbackRegistered(type, value)) {
            IMSA_HILOGI("JsInputMethodEngineListener::RegisterListenerWithType callback already registered!");
            return;
        }
        AddCallback(type, value);
    }

    void JsInputMethodEngineListener::AddCallback(std::string type, NativeValue* jsListenerObject)
    {
        IMSA_HILOGI("JsInputMethodEngineListener::AddCallback is called : %{public}s", type.c_str());
        std::lock_guard<std::mutex> lock(mMutex);
        std::shared_ptr<NativeReference> callbackRef(engine_->CreateReference(jsListenerObject, 1));
        if (callbackRef == nullptr) {
            IMSA_HILOGI("JsInputMethodEngineListener::AddCallback fail, callbackRef is nullptr");
            return;
        }
        std::lock_guard<std::recursive_mutex> lk(mapMutex);
        jsCbMap_[type].push_back(callbackRef);
        IMSA_HILOGI("AddCallback success! jsCbMap_ size: %{public}d, and type[%{public}s] size: %{public}d",
            static_cast<uint32_t>(jsCbMap_.size()), type.c_str(), static_cast<uint32_t>(jsCbMap_[type].size()));
    }

    void JsInputMethodEngineListener::UnregisterAllListenerWithType(std::string type)
    {
        IMSA_HILOGI("JsInputMethodEngineListener::UnregisterAllListenerWithType : %{public}s", type.c_str());
        // should do type check
        std::lock_guard<std::recursive_mutex> lk(mapMutex);
        if (jsCbMap_.empty() || jsCbMap_.find(type) == jsCbMap_.end()) {
            IMSA_HILOGI("methodName %{public}s not registerted!", type.c_str());
            return;
        }
        jsCbMap_.erase(type);
    }

    void JsInputMethodEngineListener::UnregisterListenerWithType(std::string type, NativeValue* value)
    {
        IMSA_HILOGI("JsInputMethodEngineListener::UnregisterListenerWithType : %{public}s.", type.c_str());
        // should do type check
        std::lock_guard<std::recursive_mutex> lk(mapMutex);
        if (jsCbMap_.empty() || jsCbMap_.find(type) == jsCbMap_.end()) {
            IMSA_HILOGI("methodName %{public}s not registered!", type.c_str());
            return;
        }
        for (auto it = jsCbMap_[type].begin(); it != jsCbMap_[type].end();) {
            if (value->StrictEquals((*it)->Get())) {
                jsCbMap_[type].erase(it);
                break;
            }
        }
        if (jsCbMap_[type].empty()) {
            jsCbMap_.erase(type);
        }
    }

    bool JsInputMethodEngineListener::IfCallbackRegistered(std::string type, NativeValue* jsListenerObject)
    {
        IMSA_HILOGI("JsInputMethodEngineListener::IfCallbackRegistered");
        std::lock_guard<std::recursive_mutex> lk(mapMutex);
        if (jsCbMap_.empty() || jsCbMap_.find(type) == jsCbMap_.end()) {
            IMSA_HILOGI("methodName %{public}s not registertd!", type.c_str());
            return false;
        }
        for (auto iter = jsCbMap_[type].begin(); iter != jsCbMap_[type].end(); iter++) {
            if (jsListenerObject->StrictEquals((*iter)->Get())) {
                IMSA_HILOGI("JsInputMethodEngineListener::IfCallbackRegistered callback already registered!");
                return true;
            }
        }
        return false;
    }

    void JsInputMethodEngineListener::CallJsMethod(std::string methodName, NativeValue* const* argv, size_t argc)
    {
        IMSA_HILOGI("JsInputMethodEngineListener::CallJsMethod : %{public}s", methodName.c_str());
        if (!engine_) {
            IMSA_HILOGI("engine_ nullptr");
            return;
        }
        std::lock_guard<std::recursive_mutex> lk(mapMutex);
        if (jsCbMap_.empty() || jsCbMap_.find(methodName) == jsCbMap_.end()) {
            IMSA_HILOGI("methodName %{public}s not registertd!", methodName.c_str());
            return;
        }
        for (auto iter : jsCbMap_[methodName]) {
            if (iter == nullptr) {
                IMSA_HILOGE("JsInputMethodEngineListener::CallJsMethod iter is null");
                continue;
            }
            engine_->CallFunction(engine_->CreateUndefined(), iter->Get(), argv, argc);
        }
    }

    bool JsInputMethodEngineListener::CallJsMethodReturnBool(std::string methodName,
                                                             NativeValue* const* argv, size_t argc)
    {
        IMSA_HILOGI("JsInputMethodEngineListener::CallJsMethodReturnBool");
        if (!engine_) {
            IMSA_HILOGI("engine_ nullptr");
            return false;
        }
        std::lock_guard<std::recursive_mutex> lk(mapMutex);
        if (jsCbMap_.empty() || jsCbMap_.find(methodName) == jsCbMap_.end()) {
            IMSA_HILOGI("methodName %{public}s not registered!", methodName.c_str());
            return false;
        }

        bool result = false;
        for (auto iter : jsCbMap_[methodName]) {
            NativeValue *nativeValue =
                engine_->CallFunction(engine_->CreateUndefined(), iter->Get(), argv, argc);
            bool ret = false;
            if (ConvertFromJsValue(*engine_, nativeValue, ret) && ret) {
                result = true;
            }
        }
        return result;
    }

    void JsInputMethodEngineListener::OnKeyboardStatus(bool isShow)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        IMSA_HILOGI("JsInputMethodEngineListener::OnKeyboardStatus");

        auto task = [this, isShow] () {
            NativeValue* nativeValue = engine_->CreateObject();
            NativeObject* object = ConvertNativeValueTo < NativeObject >(nativeValue);
            if (!object) {
                IMSA_HILOGI("Failed to convert rect to jsObject");
                return;
            }
            NativeValue* argv[] = { nativeValue };
            std::string methodName;
            if (isShow) {
                methodName = "keyboardShow";
            } else {
                methodName = "keyboardHide";
            }
            CallJsMethod(methodName, argv, ArraySize(argv));
        };
        mainHandler_->PostTask(task);
    }

    void JsInputMethodEngineListener::OnInputStart()
    {
        std::lock_guard<std::mutex> lock(mMutex);
        IMSA_HILOGI("JsInputMethodEngineListener::OnInputStart");
        auto task = [this] () {
            NativeValue *nativeValuekb = CreateKeyboardController(*engine_);
            NativeValue *nativeValuetx = CreateTextInputClient(*engine_);
            NativeValue* argv[] = {nativeValuekb, nativeValuetx};
            std::string methodName = "inputStart";
            CallJsMethod(methodName, argv, ArraySize(argv));
        };
        mainHandler_->PostTask(task);
    }

    void JsInputMethodEngineListener::OnInputStop(std::string imeId)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        IMSA_HILOGI("JsInputMethodEngineListener::OnInputStop");

        auto task = [this, imeId] () {
            NativeValue* nativeValue = CreateJsValue(*engine_, imeId);

            NativeValue* argv[] = { nativeValue };
            std::string methodName = "inputStop";
            CallJsMethod(methodName, argv, ArraySize(argv));
        };
        mainHandler_->PostTask(task);
    }

    void JsInputMethodEngineListener::OnSetCallingWindow(uint32_t windowId)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        IMSA_HILOGI("JsInputMethodEngineListener::OnSetCallingWindow");

        auto task = [this, windowId] () {
            NativeValue* nativeValue = CreateJsValue(*engine_, windowId);
            NativeValue* argv[] = { nativeValue };
            std::string methodName = "setCallingWindow";
            CallJsMethod(methodName, argv, ArraySize(argv));
        };
        mainHandler_->PostTask(task);
    }
} // namespace MiscServices
} // namespace OHOS
