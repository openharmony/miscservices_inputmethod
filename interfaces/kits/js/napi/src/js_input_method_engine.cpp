/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#include "js_input_method_engine.h"
#include "js_input_method_engine_listener.h"
#include "input_method_ability.h"
#include <map>
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "string_ex.h"
#include "input_method_utils.h"

namespace OHOS {
namespace MiscServices {
    namespace {
        constexpr size_t ARGC_ZERO = 0;
        constexpr size_t ARGC_ONE = 1;
        constexpr size_t ARGC_TWO = 2;
    }
class JsInputMethodEngine{
public:
    sptr<JsInputMethodEngineListener> imeListener_;
    explicit JsInputMethodEngine(NativeEngine* engine) {
        IMSA_HILOGI("JsInputMethodEngine::Constructor is called");
        imeListener_ = new JsInputMethodEngineListener(engine);
        InputMethodAbility::GetInstance()->setImeListener(imeListener_);
    }

    ~JsInputMethodEngine() = default;

    static void Finalizer(NativeEngine* engine, void* data, void* hint)
    {
        IMSA_HILOGI("JsInputMethodEngine::Finalizer is called");
        std::unique_ptr<JsInputMethodEngine>(static_cast<JsInputMethodEngine*>(data));
    }

    static NativeValue* InsertText(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsInputMethodEngine* me = AbilityRuntime::CheckParamsAndGetThis<JsInputMethodEngine>(engine, info);
        return (me != nullptr) ? me->OnInsertText(*engine, *info) : nullptr;
    }

    static NativeValue* DeleteForward(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsInputMethodEngine* me = AbilityRuntime::CheckParamsAndGetThis<JsInputMethodEngine>(engine, info);
        return (me != nullptr) ? me->OnDeleteForward(*engine, *info) : nullptr;
    }

    static NativeValue* DeleteBackward(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsInputMethodEngine* me = AbilityRuntime::CheckParamsAndGetThis<JsInputMethodEngine>(engine, info);
        return (me != nullptr) ? me->OnDeleteBackward(*engine, *info) : nullptr;
    }

    static NativeValue* MoveCursor(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsInputMethodEngine* me = AbilityRuntime::CheckParamsAndGetThis<JsInputMethodEngine>(engine, info);
        return (me != nullptr) ? me->OnMoveCursor(*engine, *info) : nullptr;
    }

    static NativeValue* HideKeyboardSelf(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsInputMethodEngine* me = AbilityRuntime::CheckParamsAndGetThis<JsInputMethodEngine>(engine, info);
        return (me != nullptr) ? me->OnHideKeyboardSelf(*engine, *info) : nullptr;
    }

    static NativeValue* GetTextBeforeCursor(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsInputMethodEngine* me = AbilityRuntime::CheckParamsAndGetThis<JsInputMethodEngine>(engine, info);
        return (me != nullptr) ? me->OnGetTextBeforeCursor(*engine, *info) : nullptr;
    }

    static NativeValue* GetTextAfterCursor(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsInputMethodEngine* me = AbilityRuntime::CheckParamsAndGetThis<JsInputMethodEngine>(engine, info);
        return (me != nullptr) ? me->OnGetTextAfterCursor(*engine, *info) : nullptr;
    }

    static NativeValue* SendFunctionKey(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsInputMethodEngine* me = AbilityRuntime::CheckParamsAndGetThis<JsInputMethodEngine>(engine, info);
        return (me != nullptr) ? me->OnSendFunctionKey(*engine, *info) : nullptr;
    }

    static NativeValue* RegisterCallback(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsInputMethodEngine* me = AbilityRuntime::CheckParamsAndGetThis<JsInputMethodEngine>(engine, info);
        return (me != nullptr) ? me->OnRegisterCallback(*engine, *info) : nullptr;
    }

    static NativeValue* UnRegisterCallback(NativeEngine* engine, NativeCallbackInfo* info)
    {
        JsInputMethodEngine* me = AbilityRuntime::CheckParamsAndGetThis<JsInputMethodEngine>(engine, info);
        return (me != nullptr) ? me->OnUnRegisterCallback(*engine, *info) : nullptr;
    }

private:
    std::mutex mtx_;

    NativeValue* OnInsertText(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsInputMethodEngine::OnInsertText is called!");
        if (info.argc < ARGC_ONE) {
            IMSA_HILOGI("JsInputMethodEngine::OnInsertText has no params!");
            return engine.CreateUndefined();
        }

        NativeValue* nativeString = nullptr;
        nativeString = info.argv[ARGC_ZERO];

        std::string textString;
        if(!AbilityRuntime::ConvertFromJsValue(engine, nativeString, textString)) {
            IMSA_HILOGI("JsInputMethodEngine::OnInsertText Failed to convert parameter to string");
            return engine.CreateUndefined();
        }

        bool ret = InputMethodAbility::GetInstance()->InsertText(textString);

        NativeValue* result = AbilityRuntime::CreateJsValue(engine, ret);

        return result;
    }

    NativeValue* OnDeleteForward(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsInputMethodEngine::OnDeleteForward is called!");
        if (info.argc < ARGC_ONE) {
            IMSA_HILOGI("JsInputMethodEngine::OnDeleteForward has no params!");
            return engine.CreateUndefined();
        }

        NativeValue* nativeString = nullptr;
        nativeString = info.argv[ARGC_ZERO];

        int32_t number;
        if(!AbilityRuntime::ConvertFromJsValue(engine, nativeString, number)) {
            IMSA_HILOGI("JsInputMethodEngine::OnDeleteForward Failed to convert parameter to string");
            return engine.CreateUndefined();
        }

        InputMethodAbility::GetInstance()->DeleteForward(number);

        NativeValue* result = AbilityRuntime::CreateJsValue(engine, true);

        return result;
    }

    NativeValue* OnDeleteBackward(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsInputMethodEngine::OnDeleteBackward is called!");
        if (info.argc < ARGC_ONE) {
            IMSA_HILOGI("JsInputMethodEngine::OnDeleteBackward has no params!");
            return engine.CreateUndefined();
        }

        NativeValue* nativeString = nullptr;
        nativeString = info.argv[ARGC_ZERO];

        int32_t number;
        if(!AbilityRuntime::ConvertFromJsValue(engine, nativeString, number)) {
            IMSA_HILOGI("JsInputMethodEngine::OnDeleteBackward Failed to convert parameter to string");
            return engine.CreateUndefined();
        }

        InputMethodAbility::GetInstance()->DeleteBackward(number);

        NativeValue* result = AbilityRuntime::CreateJsValue(engine, true);

        return result;
    }

    NativeValue* OnMoveCursor(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsInputMethodEngine::OnMoveCursor is called!");
        if (info.argc < ARGC_ONE) {
            IMSA_HILOGI("JsInputMethodEngine::OnMoveCursor has no params!");
            return engine.CreateUndefined();
        }

        NativeValue* nativeString = nullptr;
        nativeString = info.argv[ARGC_ZERO];

        int32_t number;
        if(!AbilityRuntime::ConvertFromJsValue(engine, nativeString, number)) {
            IMSA_HILOGI("JsInputMethodEngine::OnMoveCursor Failed to convert parameter to string");
            return engine.CreateUndefined();
        }

        InputMethodAbility::GetInstance()->MoveCursor(number);

        NativeValue* result = AbilityRuntime::CreateJsValue(engine, true);

        return result;
    }

    NativeValue* OnHideKeyboardSelf(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsInputMethodEngine::OnHideKeyboardSelf is called!");
        if (info.argc != ARGC_ZERO) {
            IMSA_HILOGI("JsInputMethodEngine::OnHideKeyboardSelf has params, return!");
            return engine.CreateUndefined();
        }

        InputMethodAbility::GetInstance()->HideKeyboardSelf();

        return engine.CreateUndefined();
    }

    NativeValue* OnGetTextBeforeCursor(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsInputMethodEngine::OnGetTextBeforeCursor is called!");
        if (info.argc != ARGC_ZERO) {
            IMSA_HILOGI("JsInputMethodEngine::OnGetTextBeforeCursor has params, return!");
            return engine.CreateUndefined();
        }

        std::string ret = Str16ToStr8(InputMethodAbility::GetInstance()->GetTextBeforeCursor());

        NativeValue* result = AbilityRuntime::CreateJsValue(engine, ret);

        return result;
    }

    NativeValue* OnGetTextAfterCursor(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsInputMethodEngine::OnGetTextAfterCursor is called!");
        if (info.argc != ARGC_ZERO) {
            IMSA_HILOGI("JsInputMethodEngine::OnGetTextAfterCursor has params, return!");
            return engine.CreateUndefined();
        }

        std::string ret = Str16ToStr8(InputMethodAbility::GetInstance()->GetTextAfterCursor());

        NativeValue* result = AbilityRuntime::CreateJsValue(engine, ret);

        return result;
    }

    NativeValue* OnSendFunctionKey(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsInputMethodEngine::OnSendFunctionKey is called!");
        if (info.argc < ARGC_ONE) {
            IMSA_HILOGI("JsInputMethodEngine::OnSendFunctionKey has no params!");
            return engine.CreateUndefined();
        }

        NativeValue* nativeString = nullptr;
        nativeString = info.argv[ARGC_ZERO];

        int32_t number;
        if(!AbilityRuntime::ConvertFromJsValue(engine, nativeString, number)) {
            IMSA_HILOGI("JsInputMethodEngine::OnSendFunctionKey Failed to convert parameter to string");
            return engine.CreateUndefined();
        }

        InputMethodAbility::GetInstance()->SendFunctionKey(number);

        return engine.CreateUndefined();
    }

    NativeValue* OnRegisterCallback(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsInputMethodEngine::OnRegisterCallback is called!");
        if (info.argc != ARGC_TWO) {
            IMSA_HILOGI("JsInputMethodEngine::OnRegisterCallback Params not match");
            return engine.CreateUndefined();
        }

        std::string cbType;
        if (!AbilityRuntime::ConvertFromJsValue(engine, info.argv[0], cbType)) {
            IMSA_HILOGI("JsInputMethodEngine::OnRegisterCallback Failed to convert parameter to callbackType");
            return engine.CreateUndefined();
        }

        NativeValue* value = info.argv[1];
        imeListener_->RegisterListenerWithType(engine, cbType, value);
        return engine.CreateUndefined();
    }

    NativeValue* OnUnRegisterCallback(NativeEngine& engine, NativeCallbackInfo& info)
    {
        IMSA_HILOGI("JsInputMethodEngine::OnUnRegisterCallback is called!");
        if (info.argc == 0) {
            IMSA_HILOGI("JsInputMethodEngine::OnUnRegisterCallback Params not match");
            return engine.CreateUndefined();
        }

        std::string cbType;
        if (!AbilityRuntime::ConvertFromJsValue(engine, info.argv[0], cbType)) {
            IMSA_HILOGI("JsInputMethodEngine::OnUnRegisterCallback Failed to convert parameter to callbackType");
            return engine.CreateUndefined();
        }

        std::lock_guard<std::mutex> lock(mtx_);
        NativeValue* value = info.argv[1];
        if (!value->IsCallable()) {
            IMSA_HILOGI("JsInputMethodEngine::OnUnregisterWindowManagerCallback info->argv[1] is not callable");
            return engine.CreateUndefined();
        }
        imeListener_->UnregisterListenerWithType(cbType, value);
        return engine.CreateUndefined();
    }
};

NativeValue* JsInputMethodEngineInit(NativeEngine* engine, NativeValue* exportObj)
{
    IMSA_HILOGI("JsInputMethodEngineInit() is called!");

    if (engine == nullptr || exportObj == nullptr) {
        IMSA_HILOGI("engine or exportObj null");
        return nullptr;
    }

    NativeObject* object = AbilityRuntime::ConvertNativeValueTo<NativeObject>(exportObj);
    if (object == nullptr) {
        IMSA_HILOGI("object null");
        return nullptr;
    }

    std::unique_ptr<JsInputMethodEngine> jsIMEngine = std::make_unique<JsInputMethodEngine>(engine);
    object->SetNativePointer(jsIMEngine.release(), JsInputMethodEngine::Finalizer, nullptr);

    AbilityRuntime::BindNativeFunction(*engine, *object, "InsertText", JsInputMethodEngine::InsertText);
    AbilityRuntime::BindNativeFunction(*engine, *object, "DeleteForward", JsInputMethodEngine::DeleteForward);
    AbilityRuntime::BindNativeFunction(*engine, *object, "DeleteBackward", JsInputMethodEngine::DeleteBackward);
    AbilityRuntime::BindNativeFunction(*engine, *object, "MoveCursor", JsInputMethodEngine::MoveCursor);
    AbilityRuntime::BindNativeFunction(*engine, *object, "HideKeyboardSelf", JsInputMethodEngine::HideKeyboardSelf);
    AbilityRuntime::BindNativeFunction(*engine, *object, "GetTextBeforeCursor", JsInputMethodEngine::GetTextBeforeCursor);
    AbilityRuntime::BindNativeFunction(*engine, *object, "GetTextAfterCursor", JsInputMethodEngine::GetTextAfterCursor);
    AbilityRuntime::BindNativeFunction(*engine, *object, "SendFunctionKey", JsInputMethodEngine::SendFunctionKey);

    AbilityRuntime::BindNativeFunction(*engine, *object, "on", JsInputMethodEngine::RegisterCallback);
    AbilityRuntime::BindNativeFunction(*engine, *object, "off", JsInputMethodEngine::UnRegisterCallback);

    object->SetProperty("FUNCTION_KEY_CONFIRM", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(1)));
    object->SetProperty("CURSOR_UP", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(1)));
    object->SetProperty("CURSOR_DOWN", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(2)));
    object->SetProperty("CURSOR_LEFT", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(3)));
    object->SetProperty("CURSOR_RIGHT", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(4)));
    return engine->CreateUndefined();
}
}
}