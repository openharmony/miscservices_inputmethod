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

#include "im_common_event_manager.h"
#include "common_event_manager.h"
#include "common_event_support.h"
#include "global.h"
#include "ipc_skeleton.h"
#include "message_handler.h"
#include "input_method_system_ability_stub.h"
#include "os_account_manager.h"

namespace OHOS {
namespace MiscServices {
    using namespace MessageID;
    sptr<ImCommonEventManager> ImCommonEventManager::instance_;
    std::mutex ImCommonEventManager::instanceLock_;

    /*! Constructor
    */
    ImCommonEventManager::ImCommonEventManager()
    {}

    /*! Destructor
    */
    ImCommonEventManager::~ImCommonEventManager()
    {}

    sptr<ImCommonEventManager> ImCommonEventManager::GetInstance()
    {
        if (instance_ == nullptr) {
            std::lock_guard<std::mutex> autoLock(instanceLock_);
            if (instance_ == nullptr) {
                IMSA_HILOGI("ImCommonEventManager::GetInstance instance_ is nullptr");
                instance_ = new ImCommonEventManager();
            }
        }
        return instance_;
    }

    bool ImCommonEventManager::SubscribeEvent(const std::string &event)
    {
        EventFwk::MatchingSkills matchingSkills;
        matchingSkills.AddEvent(event);

        EventFwk::CommonEventSubscribeInfo subscriberInfo(matchingSkills);

        std::shared_ptr<EventSubscriber> subscriber = std::make_shared<EventSubscriber>(subscriberInfo);
        if (subscriber == nullptr) {
            IMSA_HILOGI("ImCommonEventManager::SubscribeEvent subscriber is nullptr");
            return false;
        }

        if (!EventFwk::CommonEventManager::SubscribeCommonEvent(subscriber)) {
            IMSA_HILOGI("ImCommonEventManager::SubscribeEvent fail");
            return false;
        }

        return true;
    }

    bool ImCommonEventManager::UnsubscribeEvent()
    {
        return true;
    }

    void ImCommonEventManager::EventSubscriber::OnReceiveEvent(const EventFwk::CommonEventData &data)
    {
        auto want = data.GetWant();
        std::string action = want.GetAction();
        IMSA_HILOGI("ImCommonEventManager::EventSubscriber data.GetCode = %{public}u", data.GetCode());
        if (action == EventFwk::CommonEventSupport::COMMON_EVENT_USER_SWITCHED) {
            // do something
            IMSA_HILOGI("ImCommonEventManager::EventSubscriber user switched!!!");
            startUser(data.GetCode());
        }
    }

    void ImCommonEventManager::EventSubscriber::startUser(int newUserId)
    {
        IMSA_HILOGI("ImCommonEventManager::startUser 1");

        MessageParcel *parcel = new MessageParcel();
        parcel->WriteInt32(newUserId);

        IMSA_HILOGI("ImCommonEventManager::startUser 2");
        Message *msg = new Message(MessageID::MSG_ID_USER_START, parcel);
        MessageHandler::Instance()->SendMessage(msg);
        IMSA_HILOGI("ImCommonEventManager::startUser 3");
    }
} // namespace MiscServices
} // namespace OHOS
