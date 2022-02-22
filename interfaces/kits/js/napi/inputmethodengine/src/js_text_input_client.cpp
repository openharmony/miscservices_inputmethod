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
#include "js_text_input_client.h"
#include "input_method_controller.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "event_handler.h"
#include "event_runner.h"
#include "string_ex.h"
#include "input_method_ability.h"
#include "js_input_method_engine_utils.h"
namespace OHOS {
namespace MiscServices {
    constexpr size_t ARGC_ZERO = 0;
    constexpr size_t ARGC_ONE = 1;

    void JsTextInputClient::Finalizer(NativeEngine* engine, void* data, void* hint)
    {
        IMSA_HILOGI("JsTextInputClient::Finalizer is called");
        std::unique_ptr<JsTextInputClient>(static_cast<JsTextInputClient*>(data));
    }

    NativeValue* JsTextInputClient::InsertText(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsTextInputClient* me = AbilityRuntime::CheckParamsAndGetThis<JsTextInputClient>(engine, info);
        return (me != nullptr) ? me->OnInsertText(*engine, *info) : nullptr;
    }

    NativeValue* JsTextInputClient::DeleteForward(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsTextInputClient* me = AbilityRuntime::CheckParamsAndGetThis<JsTextInputClient>(engine, info);
        return (me != nullptr) ? me->OnDeleteForward(*engine, *info) : nullptr;
    }

    NativeValue* JsTextInputClient::DeleteBackward(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsTextInputClient* me = AbilityRuntime::CheckParamsAndGetThis<JsTextInputClient>(engine, info);
        return (me != nullptr) ? me->OnDeleteBackward(*engine, *info) : nullptr;
    }

    NativeValue* JsTextInputClient::SendFunctionKey(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsTextInputClient* me = AbilityRuntime::CheckParamsAndGetThis<JsTextInputClient>(engine, info);
        return (me != nullptr) ? me->OnSendFunctionKey(*engine, *info) : nullptr;
    }

    NativeValue* JsTextInputClient::GetForward(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsTextInputClient* me = AbilityRuntime::CheckParamsAndGetThis<JsTextInputClient>(engine, info);
        return (me != nullptr) ? me->OnGetForward(*engine, *info) : nullptr;
    }

    NativeValue* JsTextInputClient::GetBackward(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsTextInputClient* me = AbilityRuntime::CheckParamsAndGetThis<JsTextInputClient>(engine, info);
        return (me != nullptr) ? me->OnGetBackward(*engine, *info) : nullptr;
    }

    NativeValue* JsTextInputClient::GetEditorAttribute(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsTextInputClient* me = AbilityRuntime::CheckParamsAndGetThis<JsTextInputClient>(engine, info);
        return (me != nullptr) ? me->OnGetEditorAttribute(*engine, *info) : nullptr;
    }

    NativeValue* JsTextInputClient::OnInsertText(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsTextInputClient::OnInsertText is called!");
        if (info.argc < ARGC_ONE) {
            IMSA_HILOGI("JsTextInputClient::OnInsertText has no params!");
            return engine.CreateUndefined();
        }

        NativeValue* nativeString = nullptr;
        nativeString = info.argv[ARGC_ZERO];

        std::string textString;
        if (!AbilityRuntime::ConvertFromJsValue(engine, nativeString, textString)) {
            IMSA_HILOGI("JsTextInputClient::OnInsertText Failed to convert parameter to string");
            return engine.CreateUndefined();
        }

        bool ret = InputMethodAbility::GetInstance()->InsertText(textString);

        NativeValue* result = AbilityRuntime::CreateJsValue(engine, ret);

        return result;
    }

    NativeValue* JsTextInputClient::OnDeleteForward(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsTextInputClient::OnDeleteForward is called!");
        if (info.argc < ARGC_ONE) {
            IMSA_HILOGI("JsTextInputClient::OnDeleteForward has no params!");
            return engine.CreateUndefined();
        }

        NativeValue* nativeString = nullptr;
        nativeString = info.argv[ARGC_ZERO];

        int32_t number;
        if (!AbilityRuntime::ConvertFromJsValue(engine, nativeString, number)) {
            IMSA_HILOGI("JsTextInputClient::OnDeleteForward Failed to convert parameter to string");
            return engine.CreateUndefined();
        }

        InputMethodAbility::GetInstance()->DeleteForward(number);

        NativeValue* result = AbilityRuntime::CreateJsValue(engine, true);

        return result;
    }

    NativeValue* JsTextInputClient::OnDeleteBackward(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsTextInputClient::OnDeleteBackward is called!");
        if (info.argc < ARGC_ONE) {
            IMSA_HILOGI("JsTextInputClient::OnDeleteBackward has no params!");
            return engine.CreateUndefined();
        }

        NativeValue* nativeString = nullptr;
        nativeString = info.argv[ARGC_ZERO];

        int32_t number;
        if (!AbilityRuntime::ConvertFromJsValue(engine, nativeString, number)) {
            IMSA_HILOGI("JsTextInputClient::OnDeleteBackward Failed to convert parameter to string");
            return engine.CreateUndefined();
        }

        InputMethodAbility::GetInstance()->DeleteBackward(number);

        NativeValue* result = AbilityRuntime::CreateJsValue(engine, true);

        return result;
    }

    NativeValue* JsTextInputClient::OnSendFunctionKey(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsTextInputClient::OnSendFunctionKey is called!");
        if (info.argc < ARGC_ONE) {
            IMSA_HILOGI("JsTextInputClient::OnSendFunctionKey has no params!");
            return engine.CreateUndefined();
        }

        NativeValue* nativeString = nullptr;
        nativeString = info.argv[ARGC_ZERO];

        int32_t number;
        if (!AbilityRuntime::ConvertFromJsValue(engine, nativeString, number)) {
            IMSA_HILOGI("JsTextInputClient::OnSendFunctionKey Failed to convert parameter to string");
            return engine.CreateUndefined();
        }

        InputMethodAbility::GetInstance()->SendFunctionKey(number);

        NativeValue* result = AbilityRuntime::CreateJsValue(engine, true);

        return result;
    }

    NativeValue* JsTextInputClient::OnGetForward(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsTextInputClient::OnGetForward is called!");
        if (info.argc != ARGC_ONE) {
            IMSA_HILOGI("JsTextInputClient::OnGetForward has not one params!");
            return engine.CreateUndefined();
        }

        NativeValue* nativeString = nullptr;
        nativeString = info.argv[ARGC_ZERO];

        int32_t number;
        if (!AbilityRuntime::ConvertFromJsValue(engine, nativeString, number)) {
            IMSA_HILOGI("JsTextInputClient::OnGetForward Failed to convert parameter to string");
            return engine.CreateUndefined();
        }

        std::string ret = Str16ToStr8(InputMethodAbility::GetInstance()->GetTextBeforeCursor(number));

        NativeValue* result = AbilityRuntime::CreateJsValue(engine, ret);

        return result;
    }

    NativeValue* JsTextInputClient::OnGetBackward(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsTextInputClient::OnGetBackward is called!");
        if (info.argc != ARGC_ONE) {
            IMSA_HILOGI("JsTextInputClient::OnGetBackward has not one params!");
            return engine.CreateUndefined();
        }

        NativeValue* nativeString = nullptr;
        nativeString = info.argv[ARGC_ZERO];

        int32_t number;
        if (!AbilityRuntime::ConvertFromJsValue(engine, nativeString, number)) {
            IMSA_HILOGI("JsTextInputClient::OnGetBackward Failed to convert parameter to string");
            return engine.CreateUndefined();
        }

        std::string ret = Str16ToStr8(InputMethodAbility::GetInstance()->GetTextAfterCursor(number));

        NativeValue* result = AbilityRuntime::CreateJsValue(engine, ret);

        return result;
    }

    NativeValue* JsTextInputClient::OnGetEditorAttribute(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsTextInputClient::OnGetEditorAttribute is called!");
        if (info.argc != ARGC_ZERO) {
            IMSA_HILOGI("JsTextInputClient::OnGetEditorAttribute has params!");
            return engine.CreateUndefined();
        }

        return CreateEditorAttribute(engine);
    }
}
}