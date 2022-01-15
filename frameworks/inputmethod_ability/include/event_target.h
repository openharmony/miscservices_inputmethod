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

#ifndef INPUT_METHOD_NAPI_EVENT_TARGET_H
#define INPUT_METHOD_NAPI_EVENT_TARGET_H

#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "global.h"
#include "input_method_ability.h"

namespace OHOS {
namespace MiscServices {
    struct EventListener;
    class Event {
    public:
        virtual napi_value ToJsObject() = 0;
    };

    class NapiKeyEvent : public Event {
    public:
        NapiKeyEvent(napi_env env, int32_t key, int32_t status);
        ~NapiKeyEvent();
        napi_value ToJsObject() override;
    private:
        napi_env keyEev;
        int32_t key_;
        int32_t status_;
    };

    class NapiCursorChange : public Event {
    public:
        NapiCursorChange(napi_env env, int32_t px, int32_t py, int32_t height);
        ~NapiCursorChange();
        napi_value ToJsObject() override;
    private:
        napi_env env_;
        int32_t px_;
        int32_t py_;
        int32_t height_;
    };

    class NapiSelectionChange : public Event {
    public:
        NapiSelectionChange(napi_env env, int32_t oldBegin, int32_t oldEnd, int32_t newBegin, int32_t newEnd);
        ~NapiSelectionChange();
        napi_value ToJsObject() override;
    private:
        napi_env env_;
        int32_t oldBegin_;
        int32_t oldEnd_;
        int32_t newBegin_;
        int32_t newEnd_;
    };

    class NapiTextChange : public Event {
    public:
        NapiTextChange(napi_env env, std::u16string text);
        ~NapiTextChange();
        napi_value ToJsObject() override;
    private:
        napi_env env_;
        std::u16string text_;
    };

    class EventTarget : public RefBase {
    public:
        EventTarget(napi_env env, napi_value thisVar);
        virtual ~EventTarget();

        virtual void On(const char *type, napi_value handler);
        virtual void Once(const char *type, napi_value handler);
        virtual void Off(const char *type, napi_value handler);
        virtual void Off(const char *type);
        virtual void Emit(sptr<EventTarget> &eventTarget, const char *type, Event *event);
        virtual napi_env GetEnv();

    private:
        napi_env env_;
        napi_ref thisVarRef_;
        EventListener *first_;
        EventListener *last_;
    };
}
}
#endif // INPUT_METHOD_NAPI_EVENT_TARGET_H