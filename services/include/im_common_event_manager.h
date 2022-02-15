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

#ifndef FM_IMMS_PROJECT_IMCOMMONEVENTMANAGER_H
#define FM_IMMS_PROJECT_IMCOMMONEVENTMANAGER_H

#include <mutex>
#include "common_event_subscriber.h"
#include "common_event_subscribe_info.h"
#include "common_event_data.h"
#include "matching_skills.h"

namespace OHOS {
namespace MiscServices {
class ImCommonEventManager : public RefBase {
public:
    ImCommonEventManager();
    ~ImCommonEventManager();
    static sptr<ImCommonEventManager> GetInstance();
    bool SubscribeEvent(const std::string &event);
    bool UnsubscribeEvent();

    class EventSubscriber : public EventFwk::CommonEventSubscriber {
    public:
        EventSubscriber(const EventFwk::CommonEventSubscribeInfo &subscribeInfo) :
            EventFwk::CommonEventSubscriber(subscribeInfo) {}
        void OnReceiveEvent(const EventFwk::CommonEventData &data);
        void startUser(int32_t newUserId);
    };

private:
    static std::mutex instanceLock_;
    static sptr<ImCommonEventManager> instance_;
};
} // namespace MiscServices
} // namespace OHOS
#endif // FM_IMMS_PROJECT_IPLATFORMCALLBACK_H
