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
#include "js_input_method_utils.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "event_handler.h"
#include "event_runner.h"
#include "string_ex.h"
#include "js_input_method_setting.h"
#include "js_input_method_controller.h"
namespace OHOS {
    namespace MiscServices {
        using namespace AbilityRuntime;
        NativeValue* CreateInputMethodSetting(NativeEngine& engine)
        {
            IMSA_HILOGI("JsInputMethodUtils::CreateInputMethodSetting is called");
            NativeValue *objValue = engine.CreateObject();
            NativeObject *object = ConvertNativeValueTo<NativeObject>(objValue);
            if (object == nullptr) {
                IMSA_HILOGI("JsInputMethodUtils::CreateInputMethodSetting Failed to get object");
                return nullptr;
            }

            std::unique_ptr<JsInputMethodSetting> jsInputMethodSetting = std::make_unique<JsInputMethodSetting>();
            object->SetNativePointer(jsInputMethodSetting.release(), JsInputMethodSetting::Finalizer, nullptr);

            BindNativeFunction(engine, *object, "displayOptionalInputMethod", JsInputMethodSetting::DisplayOptionalInputMethod);
            BindNativeFunction(engine, *object, "listInputMethod", JsInputMethodSetting::ListInputMethod);
            return objValue;
        }

        NativeValue* CreateInputMethodController(NativeEngine& engine)
        {
            IMSA_HILOGI("JsInputMethodUtils::CreateInputMethodController is called");
            NativeValue *objValue = engine.CreateObject();
            NativeObject *object = ConvertNativeValueTo<NativeObject>(objValue);
            if (object == nullptr) {
                IMSA_HILOGI("JsInputMethodUtils::CreateInputMethodController Failed to get object");
                return nullptr;
            }

            std::unique_ptr<JsInputMethodController> jsInputMethodController = std::make_unique<JsInputMethodController>();
            object->SetNativePointer(jsInputMethodController.release(), JsInputMethodController::Finalizer, nullptr);

            BindNativeFunction(engine, *object, "stopInput", JsInputMethodController::StopInput);
            return objValue;
        }
    } // namespace MiscServices
} // namespace OHOS
