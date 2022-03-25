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
#include "js_input_method_engine_utils.h"
#include "input_method_controller.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "event_handler.h"
#include "event_runner.h"
#include "string_ex.h"
#include "js_input_method_engine_listener.h"
#include "input_method_ability.h"
#include "global.h"
#include "js_input_method_engine.h"
#include "js_editor_attribute.h"
namespace OHOS {
    namespace MiscServices {
        using namespace AbilityRuntime;
        NativeValue* CreateInputMethodEngine(NativeEngine &engine)
        {
            IMSA_HILOGI("JsInputMethodEngineUtils::CreateInputMethodEngine is called");
            NativeValue *objValue = engine.CreateObject();
            NativeObject *object = ConvertNativeValueTo<NativeObject>(objValue);
            if (object == nullptr) {
                IMSA_HILOGI("JsInputMethodEngineUtils::CreateInputMethodEngine Failed to get object");
                return nullptr;
            }

            std::unique_ptr<JsInputMethodEngine> jsInputMethodEngine = std::make_unique<JsInputMethodEngine>(&engine);
            object->SetNativePointer(jsInputMethodEngine.release(), JsInputMethodEngine::Finalizer, nullptr);

            BindNativeFunction(engine, *object, "on", JsInputMethodEngine::RegisterCallback);
            BindNativeFunction(engine, *object, "off", JsInputMethodEngine::UnRegisterCallback);
            return objValue;
        }

        NativeValue* CreateKeyboardController(NativeEngine & engine)
        {
            IMSA_HILOGI("JsInputMethodEngineUtils::CreateKeyboardController is called");
            NativeValue *objValue = engine.CreateObject();
            NativeObject *object = ConvertNativeValueTo<NativeObject>(objValue);
            if (object == nullptr) {
                IMSA_HILOGI("CreateKeyboardController Failed to get object");
                return nullptr;
            }

            std::unique_ptr<JsKeyboardController> jsKeyboardController = std::make_unique<JsKeyboardController>();
            object->SetNativePointer(jsKeyboardController.release(), JsKeyboardController::Finalizer, nullptr);

            BindNativeFunction(engine, *object, "hideKeyboard", JsKeyboardController::HideKeyboardSelf);

            return objValue;
        }

        NativeValue* CreateTextInputClient(NativeEngine & engine)
        {
            IMSA_HILOGI("JsInputMethodEngineUtils::CreateTextInputClient is called");
            NativeValue *objValue = engine.CreateObject();
            NativeObject *object = ConvertNativeValueTo<NativeObject>(objValue);
            if (object == nullptr) {
                IMSA_HILOGI("CreateTextInputClient Failed to get object");
                return nullptr;
            }

            std::unique_ptr<JsTextInputClient> jsTextInputClient = std::make_unique<JsTextInputClient>();
            object->SetNativePointer(jsTextInputClient.release(), JsTextInputClient::Finalizer, nullptr);

            BindNativeFunction(engine, *object, "insertText", JsTextInputClient::InsertText);
            BindNativeFunction(engine, *object, "deleteForward", JsTextInputClient::DeleteForward);
            BindNativeFunction(engine, *object, "deleteBackward", JsTextInputClient::DeleteBackward);
            BindNativeFunction(engine, *object, "sendKeyFunction", JsTextInputClient::SendFunctionKey);
            BindNativeFunction(engine, *object, "getForward", JsTextInputClient::GetForward);
            BindNativeFunction(engine, *object, "getBackward", JsTextInputClient::GetBackward);
            BindNativeFunction(engine, *object, "getEditorAttribute", JsTextInputClient::GetEditorAttribute);
            return objValue;
        }

        NativeValue* CreateKeyboardDelegate(NativeEngine & engine)
        {
            IMSA_HILOGI("JsInputMethodEngineUtils::CreateKeyboardDelegate is called");
            NativeValue *objValue = engine.CreateObject();
            NativeObject *object = ConvertNativeValueTo<NativeObject>(objValue);
            if (object == nullptr) {
                IMSA_HILOGI("CreateKeyboardDelegate Failed to get object");
                return nullptr;
            }

            std::unique_ptr<JsKeyboardDelegate> jsKeyboardDelegate = std::make_unique<JsKeyboardDelegate>(&engine);
            object->SetNativePointer(jsKeyboardDelegate.release(), JsKeyboardDelegate::Finalizer, nullptr);

            BindNativeFunction(engine, *object, "on", JsKeyboardDelegate::RegisterCallback);
            BindNativeFunction(engine, *object, "off", JsKeyboardDelegate::UnRegisterCallback);
            return objValue;
        }

        NativeValue* CreateEditorAttribute(NativeEngine & engine)
        {
            IMSA_HILOGI("JsInputMethodEngineUtils::CreateEditorAttribute is called");
            NativeValue *objValue = engine.CreateObject();
            NativeObject *object = ConvertNativeValueTo<NativeObject>(objValue);
            if (object == nullptr) {
                IMSA_HILOGI("CreateEditorAttribute Failed to get object");
                return nullptr;
            }

            std::unique_ptr<JsEditorAttribute> jsEditorAttribute = std::make_unique<JsEditorAttribute>();
            object->SetNativePointer(jsEditorAttribute.release(), JsEditorAttribute::Finalizer, nullptr);

            object->SetProperty("enterKeyType", CreateJsValue(engine, InputMethodAbility::GetInstance()->GetEnterKeyType()));
            object->SetProperty("inputPattern", CreateJsValue(engine, InputMethodAbility::GetInstance()->GetInputPattern()));

            return objValue;
        }
    } // namespace MiscServices
} // namespace OHOS
