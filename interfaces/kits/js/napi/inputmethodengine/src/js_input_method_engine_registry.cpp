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

#include "js_input_method_engine_registry.h"
#include <map>
#include "js_input_method_engine_listener.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "string_ex.h"
#include "input_method_utils.h"
#include "input_method_ability.h"
#include "js_input_method_engine_utils.h"

namespace OHOS {
namespace MiscServices {
    namespace {
        constexpr size_t ARGC_ZERO = 0;
        constexpr size_t ARGC_ONE = 1;
    }
    class JsInputMethodEngineRegistry {
    public:
        explicit JsInputMethodEngineRegistry(NativeEngine* engine)
        {
            IMSA_HILOGI("JsInputMethodEngineRegistry::Constructor is called");
        }

        ~JsInputMethodEngineRegistry() = default;

        static void Finalizer(NativeEngine* engine, void* data, void* hint)
        {
            IMSA_HILOGI("JsInputMethodEngineRegistry::Finalizer is called");
            std::unique_ptr<JsInputMethodEngineRegistry>(static_cast<JsInputMethodEngineRegistry*>(data));
        }

        static NativeValue* GetInputMethodEngine(NativeEngine* engine, NativeCallbackInfo* info)
        {
            JsInputMethodEngineRegistry* me = AbilityRuntime::CheckParamsAndGetThis<JsInputMethodEngineRegistry>(engine, info);
            return (me != nullptr) ? me->OnGetInputMethodEngine(*engine, *info) : nullptr;
        }

        static NativeValue* GetKeyboardDelegate(NativeEngine* engine, NativeCallbackInfo* info)
        {
            JsInputMethodEngineRegistry* me = AbilityRuntime::CheckParamsAndGetThis<JsInputMethodEngineRegistry>(engine, info);
            return (me != nullptr) ? me->OnGetKeyboardDelegate(*engine, *info) : nullptr;
        }

        static NativeValue* MoveCursor(NativeEngine* engine, NativeCallbackInfo* info)
        {
            JsInputMethodEngineRegistry* me = AbilityRuntime::CheckParamsAndGetThis<JsInputMethodEngineRegistry>(engine, info);
            return (me != nullptr) ? me->OnMoveCursor(*engine, *info) : nullptr;
        }
    private:
        NativeValue* OnGetInputMethodEngine(NativeEngine& engine, NativeCallbackInfo& info)
        {
            IMSA_HILOGI("JsInputMethodEngineRegistry::OnGetInputMethodEngine is called!");
            if (info.argc > ARGC_ONE) {
                IMSA_HILOGI("JsInputMethodEngineRegistry::OnGetInputMethodEngine Params not match");
                return engine.CreateUndefined();
            }

            return CreateInputMethodEngine(engine);
        }

        NativeValue* OnGetKeyboardDelegate(NativeEngine& engine, NativeCallbackInfo& info)
        {
            IMSA_HILOGI("JsInputMethodEngineRegistry::OnGetKeyboardDelegate is called!");
            if (info.argc > ARGC_ONE) {
                IMSA_HILOGI("JsInputMethodEngineRegistry::OnGetKeyboardDelegate Params not match");
                return engine.CreateUndefined();
            }

            return CreateKeyboardDelegate(engine);
        }

        NativeValue* OnMoveCursor(NativeEngine& engine, NativeCallbackInfo& info)
        {
            IMSA_HILOGI("JsInputMethodEngineRegistry::OnMoveCursor is called!");
            if (info.argc < ARGC_ONE) {
                IMSA_HILOGI("JsInputMethodEngineRegistry::OnMoveCursor has no params!");
                return engine.CreateUndefined();
            }

            NativeValue* nativeString = nullptr;
            nativeString = info.argv[ARGC_ZERO];

            int32_t number;
            if (!AbilityRuntime::ConvertFromJsValue(engine, nativeString, number)) {
                IMSA_HILOGI("JsInputMethodEngineRegistry::OnMoveCursor Failed to convert parameter to string");
                return engine.CreateUndefined();
            }

            InputMethodAbility::GetInstance()->MoveCursor(number);

            NativeValue* result = AbilityRuntime::CreateJsValue(engine, true);

            return result;
        }
    };

    NativeValue* JsInputMethodEngineRegistryInit(NativeEngine* engine, NativeValue* exportObj)
    {
        IMSA_HILOGI("JsInputMethodEngineRegistryInit() is called!");

        if (engine == nullptr || exportObj == nullptr) {
            IMSA_HILOGI("engine or exportObj null");
            return nullptr;
        }

        NativeObject* object = AbilityRuntime::ConvertNativeValueTo<NativeObject>(exportObj);
        if (object == nullptr) {
            IMSA_HILOGI("object null");
            return nullptr;
        }

        std::unique_ptr<JsInputMethodEngineRegistry> jsInputMethodEngineRegistry = std::make_unique<JsInputMethodEngineRegistry>(engine);
        object->SetNativePointer(jsInputMethodEngineRegistry.release(), JsInputMethodEngineRegistry::Finalizer, nullptr);

        AbilityRuntime::BindNativeFunction(*engine, *object, "getInputMethodEngine", JsInputMethodEngineRegistry::GetInputMethodEngine);
        AbilityRuntime::BindNativeFunction(*engine, *object, "createKeyboardDelegate", JsInputMethodEngineRegistry::GetKeyboardDelegate);

        object->SetProperty("ENTER_KEY_TYPE_UNSPECIFIED", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(EnterKeyType::UNSPECIFIED)));
        object->SetProperty("ENTER_KEY_TYPE_GO", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(EnterKeyType::GO)));
        object->SetProperty("ENTER_KEY_TYPE_SEARCH", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(EnterKeyType::SEARCH)));
        object->SetProperty("ENTER_KEY_TYPE_SEND", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(EnterKeyType::SEND)));
        object->SetProperty("ENTER_KEY_TYPE_NEXT", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(EnterKeyType::NEXT)));
        object->SetProperty("ENTER_KEY_TYPE_DONE", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(EnterKeyType::DONE)));
        object->SetProperty("ENTER_KEY_TYPE_PREVIOUS", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(EnterKeyType::PREVIOUS)));

        object->SetProperty("PATTERN_NULL", AbilityRuntime::CreateJsValue(*engine, static_cast<int32_t>(TextInputType::NONE)));
        object->SetProperty("PATTERN_TEXT", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(TextInputType::TEXT)));
        object->SetProperty("PATTERN_NUMBER", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(TextInputType::NUMBER)));
        object->SetProperty("PATTERN_PHONE", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(TextInputType::PHONE)));
        object->SetProperty("PATTERN_DATETIME", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(TextInputType::DATETIME)));
        object->SetProperty("PATTERN_EMAIL", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(TextInputType::EMAIL_ADDRESS)));
        object->SetProperty("PATTERN_URI", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(TextInputType::URL)));
        object->SetProperty("PATTERN_PASSWORD", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(TextInputType::VISIBLE_PASSWORD)));


        object->SetProperty("FLAG_SELECTING", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(2)));
        object->SetProperty("FLAG_SINGLE_LINE", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(1)));

        object->SetProperty("DISPLAY_MODE_PART", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(0)));
        object->SetProperty("DISPLAY_MODE_FULL", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(1)));

        object->SetProperty("OPTION_ASCII", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(20)));
        object->SetProperty("OPTION_NONE", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(0)));
        object->SetProperty("OPTION_AUTO_CAP_CHARACTERS", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(2)));
        object->SetProperty("OPTION_AUTO_CAP_SENTENCES", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(8)));
        object->SetProperty("OPTION_AUTO_WORDS", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(4)));
        object->SetProperty("OPTION_MULTI_LINE", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(1)));
        object->SetProperty("OPTION_NO_FULLSCREEN", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(10)));



        AbilityRuntime::BindNativeFunction(*engine, *object, "MoveCursor", JsInputMethodEngineRegistry::MoveCursor);

        object->SetProperty("FUNCTION_KEY_CONFIRM", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(1)));
        object->SetProperty("CURSOR_UP", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(1)));
        object->SetProperty("CURSOR_DOWN", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(2)));
        object->SetProperty("CURSOR_LEFT", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(3)));
        object->SetProperty("CURSOR_RIGHT", AbilityRuntime::CreateJsValue(*engine, static_cast<uint32_t>(4)));
        return engine->CreateUndefined();
    }
}
}