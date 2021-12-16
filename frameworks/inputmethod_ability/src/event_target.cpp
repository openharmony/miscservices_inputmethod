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
#include <uv.h>
#include "event_target.h"

#include "securec.h"

#include "utils/log.h"
#include "input_method_ability.h"
namespace OHOS {
namespace MiscServices {
    struct EventListener {
        char type[LISTENER_TYPTE_MAX_LENGTH] = { 0 };
        bool isOnce = false;
        napi_ref handlerRef = nullptr;
        EventListener *back = nullptr;
        EventListener *next = nullptr;
    };
    struct EventTargetCB {
        napi_env env;
        napi_ref thisVarRef;
        EventListener *first;
        EventListener *last;
        sptr<EventTarget> &eventTarget;
        const char *type;
        Event *event;
    };
    EventTarget::EventTarget(napi_env env, napi_value thisVar)
    {
        IMSA_HILOGI("EventTarget::EventTarget");
        env_ = env;
        first_ = last_ = nullptr;
        thisVarRef_ = nullptr;
        napi_create_reference(env, thisVar, 1, &thisVarRef_);
    }

    EventTarget::~EventTarget()
    {
        EventListener *temp = nullptr;
        for (EventListener *i = first_; i != nullptr; i = temp) {
            temp = i->next;
            if (i == first_) {
                first_ = first_->next;
            } else if (i == last_) {
                last_ = last_->back;
            } else {
                i->next->back = i->back;
                i->back->next = i->next;
            }
            napi_delete_reference(env_, i->handlerRef);
            delete i;
            i = nullptr;
        }
        napi_delete_reference(env_, thisVarRef_);
    }

    void EventTarget::On(const char *type, napi_value handler)
    {
        IMSA_HILOGI("EventTarget::On");
        auto tmp = new EventListener();

        if (strncpy_s(tmp->type, LISTENER_TYPTE_MAX_LENGTH, type, strlen(type)) == -1) {
            delete tmp;
            tmp = nullptr;
            return;
        }

        if (first_ == nullptr) {
            first_ = last_ = tmp;
        } else {
            last_->next = tmp;
            last_->next->back = last_;
            last_ = last_->next;
        }
        last_->isOnce = false;
        napi_create_reference(env_, handler, 1, &last_->handlerRef);
    }

    void EventTarget::Once(const char *type, napi_value handler)
    {
        IMSA_HILOGI("EventTarget::Once");
        auto tmp = new EventListener();

        if (strncpy_s(tmp->type, LISTENER_TYPTE_MAX_LENGTH, type, strlen(type)) == -1) {
            delete tmp;
            tmp = nullptr;
            return;
        }

        if (first_ == nullptr) {
            first_ = last_ = tmp;
        } else {
            last_->next = tmp;
            last_->next->back = last_;
            last_ = last_->next;
        }
        last_->isOnce = true;
        napi_create_reference(env_, handler, 1, &last_->handlerRef);
    }

    void EventTarget::Off(const char *type, napi_value handler)
    {
        IMSA_HILOGI("EventTarget::Off");
        napi_handle_scope scope = nullptr;
        napi_open_handle_scope(env_, &scope);
        if (scope == nullptr) {
            HILOG_ERROR("scope is nullptr");
            return;
        }

        EventListener *temp = nullptr;
        for (EventListener *eventListener = first_; eventListener != nullptr; eventListener = temp) {
            temp = eventListener->next;
            bool isEquals = false;
            napi_value handlerTemp = nullptr;
            napi_get_reference_value(env_, eventListener->handlerRef, &handlerTemp);
            napi_strict_equals(env_, handlerTemp, handler, &isEquals);
            if (strcmp(eventListener->type, type) == 0 && isEquals) {
                if (eventListener == first_) {
                    first_ = first_->next;
                } else if (eventListener == last_) {
                    last_ = last_->back;
                } else {
                    eventListener->next->back = eventListener->back;
                    eventListener->back->next = eventListener->next;
                }
                napi_delete_reference(env_, eventListener->handlerRef);
                delete eventListener;
                eventListener = nullptr;
                break;
            }
        }
        napi_close_handle_scope(env_, scope);
    }

    void EventTarget::Off(const char *type)
    {
        IMSA_HILOGI("EventTarget::Off");
        EventListener *temp = nullptr;
        for (EventListener *eventListener = first_; eventListener != nullptr; eventListener = temp) {
            temp = eventListener->next;
            if (strcmp(eventListener->type, type) == 0) {
                if (eventListener == first_) {
                    first_ = first_->next;
                } else if (eventListener == last_) {
                    last_ = last_->back;
                } else {
                    eventListener->next->back = eventListener->back;
                    eventListener->back->next = eventListener->next;
                }
                napi_delete_reference(env_, eventListener->handlerRef);
                delete eventListener;
                eventListener = nullptr;
            }
        }
    }

    void EventTarget::Emit(sptr<EventTarget> &eventTarget, const char *type, Event *event)
    {
        IMSA_HILOGI("EventTarget::Emit");
        uv_loop_s *loop = nullptr;
        napi_get_uv_event_loop(env_, &loop);
        if (loop == nullptr) {
            IMSA_HILOGI("EventTarget::Emit loop == nullptr");
            return;
        }

        uv_work_t *work = new (std::nothrow) uv_work_t;
        if (work == nullptr) {
            IMSA_HILOGI("EventTarget::Emit No memory work == nullptr");
            delete work;
            return;
        }

        EventTargetCB *eventTargetCB = new (std::nothrow) EventTargetCB {.env = env_, .thisVarRef = thisVarRef_,
            .first = first_, .last = last_, .type = type, .event = event, .eventTarget = eventTarget};

        work->data = (void *)eventTargetCB;

        int ret = uv_queue_work(loop, work, [](uv_work_t *work) {}, [](uv_work_t *work, int status) {
            // Js Thread
            if (work == nullptr) {
                IMSA_HILOGI("EventTarget::Emit work == nullptr");
                return;
            }
            EventTargetCB *eventTargetCB = (EventTargetCB *)work->data;
            do {
                napi_handle_scope scope = nullptr;
                napi_open_handle_scope(eventTargetCB->env, &scope);

                napi_value thisVar = nullptr;
                napi_get_reference_value(eventTargetCB->env, eventTargetCB->thisVarRef, &thisVar);
                for (EventListener *eventListener = eventTargetCB->first; eventListener != nullptr;
                    eventListener = eventListener->next) {
                    if (strcmp(eventListener->type, eventTargetCB->type) == 0) {
                        napi_value jsEvent = eventTargetCB->event ? eventTargetCB->event->ToJsObject() : nullptr;
                        napi_value handler = nullptr;
                        napi_value result = nullptr;
                        napi_get_reference_value(eventTargetCB->env, eventListener->handlerRef, &handler);
                        napi_call_function(eventTargetCB->env, thisVar, handler,
                                jsEvent ? 1 : 0, jsEvent ? &jsEvent : nullptr, &result);
                        if (eventListener->isOnce) {
                            eventTargetCB->eventTarget->Off(eventTargetCB->type, handler);
                        }
                    }
                }
                napi_close_handle_scope(eventTargetCB->env, scope);
            } while (0);
            if (eventTargetCB) {
                delete eventTargetCB;
            }
        });
        if (ret != 0) {
            IMSA_HILOGI("EventTarget::Emit failed to execute libuv work queue");
        }
        delete work;
    }
}
}