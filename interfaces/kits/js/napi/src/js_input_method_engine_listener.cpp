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

namespace OHOS {
namespace MiscServices {
    void JsInputMethodEngineListener::RegisterListenerWithType(NativeEngine& engine,
                                                               std::string type, NativeValue* value)
    {
        // should do type check
        if (IfCallbackRegistered(type, value)) {
            IMSA_HILOGI("JsInputMethodEngineListener::RegisterListenerWithType callback already registered!");
            return;
        }
        std::unique_ptr<NativeReference> callbackRef;
        callbackRef.reset(engine.CreateReference(value, 1));

        AddCallback(type, value);
    }

    void JsInputMethodEngineListener::AddCallback(std::string type, NativeValue* jsListenerObject)
    {
        IMSA_HILOGI("JsInputMethodEngineListener::AddCallback is called");
        std::lock_guard<std::mutex> lock(mMutex);
        std::unique_ptr<NativeReference> callbackRef;
        callbackRef.reset(engine_->CreateReference(jsListenerObject, 1));
        jsCbMap_[type].push_back(std::move(callbackRef));
        IMSA_HILOGI("JsInputMethodEngineListener::AddCallback success");
        IMSA_HILOGI("jsCbMap_ size: %{public}d, and type[%{public}s] size: %{public}d!",
            static_cast<uint32_t>(jsCbMap_.size()), type.c_str(), static_cast<uint32_t>(jsCbMap_[type].size()));
        return;
    }

    void JsInputMethodEngineListener::UnregisterListenerWithType(std::string type, NativeValue* value)
    {
        IMSA_HILOGI("JsInputMethodEngineListener::UnregisterListenerWithType");
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

    bool JsInputMethodEngineListener::IfCallbackRegistered(std::string type, NativeValue* jsListenerObject)
    {
        IMSA_HILOGI("JsInputMethodEngineListener::IfCallbackRegistered");
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
        IMSA_HILOGI("JsInputMethodEngineListener::CallJsMethod");
        if (engine_ == nullptr) {
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

    bool JsInputMethodEngineListener::CallJsMethodReturnBool(std::string methodName,
                                                             NativeValue* const* argv, size_t argc)
    {
        IMSA_HILOGI("JsInputMethodEngineListener::CallJsMethodReturnBool");
        if (engine_ == nullptr) {
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
            if (AbilityRuntime::ConvertFromJsValue(*engine_, nativeValue, ret) && ret) {
                result = true;
            }
        }
        return result;
    }

    void JsInputMethodEngineListener::OnKeyboardStatus(bool isShow)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        IMSA_HILOGI("JsInputMethodEngineListener::OnKeyboardStatus");

        NativeValue* nativeValue = engine_->CreateObject();
        NativeObject* object = AbilityRuntime::ConvertNativeValueTo<NativeObject>(nativeValue);
        if (object == nullptr) {
            IMSA_HILOGI("Failed to convert rect to jsObject");
            return;
        }
        NativeValue* argv[] = {nativeValue};
        std::string methodName;
        if (isShow) {
            methodName = "keyboardShow";
        } else {
            methodName = "keyboardHide";
        }
        CallJsMethod(methodName, argv, AbilityRuntime::ArraySize(argv));
    }

    bool JsInputMethodEngineListener::OnKeyEvent(int32_t keyCode, int32_t keyStatus)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        IMSA_HILOGI("JsInputMethodEngineListener::OnKeyEvent");

        NativeValue* nativeValue = engine_->CreateObject();
        NativeObject* object = AbilityRuntime::ConvertNativeValueTo<NativeObject>(nativeValue);
        if (object == nullptr) {
            IMSA_HILOGI("Failed to convert rect to jsObject");
            return false;
        }
        NativeValue* argv[] = {nativeValue};
        std::string methodName;
        if (keyStatus == 2) {
            methodName = "keyDown";
        } else {
            methodName = "keyUp";
        }
        object->SetProperty("keyCode", AbilityRuntime::CreateJsValue(*engine_, static_cast<uint32_t>(keyCode)));
        object->SetProperty("keyAction", AbilityRuntime::CreateJsValue(*engine_, static_cast<uint32_t>(keyStatus)));
        return CallJsMethodReturnBool(methodName, argv, AbilityRuntime::ArraySize(argv));
    }

    void JsInputMethodEngineListener::OnCursorUpdate(int32_t positionX, int32_t positionY, int height)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        IMSA_HILOGI("JsInputMethodEngineListener::OnCursorUpdate");

        NativeValue* nativeXValue = engine_->CreateObject();
        NativeObject* objectX = AbilityRuntime::ConvertNativeValueTo<NativeObject>(nativeXValue);
        if (objectX == nullptr) {
            IMSA_HILOGI("Failed to convert rect to jsObject");
            return;
        }
        objectX->SetProperty("x", AbilityRuntime::CreateJsValue(*engine_, static_cast<uint32_t>(positionX)));

        NativeValue* nativeYValue = engine_->CreateObject();
        NativeObject* objectY = AbilityRuntime::ConvertNativeValueTo<NativeObject>(nativeYValue);
        if (objectY == nullptr) {
            IMSA_HILOGI("Failed to convert rect to jsObject");
            return;
        }
        objectY->SetProperty("y", AbilityRuntime::CreateJsValue(*engine_, static_cast<uint32_t>(positionY)));

        NativeValue* nativeHValue = engine_->CreateObject();
        NativeObject* objectH = AbilityRuntime::ConvertNativeValueTo<NativeObject>(nativeHValue);
        if (objectH == nullptr) {
            IMSA_HILOGI("Failed to convert rect to jsObject");
            return;
        }
        objectH->SetProperty("height", AbilityRuntime::CreateJsValue(*engine_, static_cast<uint32_t>(height)));

        NativeValue* argv[] = {nativeXValue, nativeYValue, nativeHValue};
        std::string methodName = "cursorContextChange";
        CallJsMethod(methodName, argv, AbilityRuntime::ArraySize(argv));
    }

    void JsInputMethodEngineListener::OnSelectionChange(int32_t oldBegin, int32_t oldEnd,
                                                        int32_t newBegin, int32_t newEnd)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        IMSA_HILOGI("JsInputMethodEngineListener::OnSelectionChange");

        NativeValue* nativeOBValue = engine_->CreateObject();
        NativeObject* objectOB = AbilityRuntime::ConvertNativeValueTo<NativeObject>(nativeOBValue);
        if (objectOB == nullptr) {
            IMSA_HILOGI("Failed to convert rect to jsObject");
            return;
        }
        objectOB->SetProperty("oldBegin", AbilityRuntime::CreateJsValue(*engine_, static_cast<uint32_t>(oldBegin)));

        NativeValue* nativeOEValue = engine_->CreateObject();
        NativeObject* objectOE = AbilityRuntime::ConvertNativeValueTo<NativeObject>(nativeOEValue);
        if (objectOE == nullptr) {
            IMSA_HILOGI("Failed to convert rect to jsObject");
            return;
        }
        objectOE->SetProperty("oldEnd", AbilityRuntime::CreateJsValue(*engine_, static_cast<uint32_t>(oldEnd)));

        NativeValue* nativeNBHValue = engine_->CreateObject();
        NativeObject* objectNB = AbilityRuntime::ConvertNativeValueTo<NativeObject>(nativeNBHValue);
        if (objectNB == nullptr) {
            IMSA_HILOGI("Failed to convert rect to jsObject");
            return;
        }
        objectNB->SetProperty("newBegin", AbilityRuntime::CreateJsValue(*engine_, static_cast<uint32_t>(newBegin)));

        NativeValue* nativeNEValue = engine_->CreateObject();
        NativeObject* objectNE = AbilityRuntime::ConvertNativeValueTo<NativeObject>(nativeNEValue);
        if (objectNE == nullptr) {
            IMSA_HILOGI("Failed to convert rect to jsObject");
            return;
        }
        objectNE->SetProperty("newEnd", AbilityRuntime::CreateJsValue(*engine_, static_cast<uint32_t>(newEnd)));

        NativeValue* argv[] = {nativeOEValue, nativeOEValue, nativeNBHValue, nativeNEValue};
        std::string methodName = "selectionChange";
        CallJsMethod(methodName, argv, AbilityRuntime::ArraySize(argv));
    }

    void JsInputMethodEngineListener::OnTextChange(std::string text)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        IMSA_HILOGI("JsInputMethodEngineListener::OnTextChange");

        NativeValue* nativeValue = engine_->CreateObject();
        NativeObject* object = AbilityRuntime::ConvertNativeValueTo<NativeObject>(nativeValue);
        if (object == nullptr) {
            IMSA_HILOGI("Failed to convert rect to jsObject");
            return;
        }
        object->SetProperty("text", AbilityRuntime::CreateJsValue(*engine_, text));

        NativeValue* argv[] = {nativeValue};
        std::string methodName = "textChange";
        CallJsMethod(methodName, argv, AbilityRuntime::ArraySize(argv));
    }

    void JsInputMethodEngineListener::OnInputStop(std::string imeId)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        IMSA_HILOGI("JsInputMethodEngineListener::OnInputStop");

        NativeValue* nativeValue = engine_->CreateObject();
        NativeObject* object = AbilityRuntime::ConvertNativeValueTo<NativeObject>(nativeValue);
        if (object == nullptr) {
            IMSA_HILOGI("Failed to convert rect to jsObject");
            return;
        }
        object->SetProperty("imeId", AbilityRuntime::CreateJsValue(*engine_, imeId));

        NativeValue* argv[] = {nativeValue};
        std::string methodName = "inputStop";
        CallJsMethod(methodName, argv, AbilityRuntime::ArraySize(argv));
    }
}
}