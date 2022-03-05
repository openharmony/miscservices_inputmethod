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
#include "js_editor_attribute.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "event_handler.h"
#include "event_runner.h"
#include "string_ex.h"
#include "global.h"
#include "input_method_ability.h"
namespace OHOS {
namespace MiscServices {
    void JsEditorAttribute::Finalizer(NativeEngine* engine, void* data, void* hint)
    {
        IMSA_HILOGI("JsEditorAttribute::Finalizer is called");
        std::unique_ptr<JsEditorAttribute>(static_cast<JsEditorAttribute*>(data));
    }
}
}