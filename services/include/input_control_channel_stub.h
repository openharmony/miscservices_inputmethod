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

#ifndef SERVICES_INCLUDE_INPUT_CONTROL_CHANNEL_STUB_H
#define SERVICES_INCLUDE_INPUT_CONTROL_CHANNEL_STUB_H

#include <mutex>
#include <condition_variable>
#include "iremote_broker.h"
#include "iremote_stub.h"
#include "i_input_method_agent.h"
#include "input_channel.h"
#include "message_parcel.h"
#include "i_input_control_channel.h"

namespace OHOS {
    namespace MiscServices {
    class InputControlChannelStub : public IRemoteStub<IInputControlChannel> {
    public:
        explicit InputControlChannelStub(int userId);
        virtual ~InputControlChannelStub();
        int OnRemoteRequest(uint32_t code,
                                      MessageParcel &data,
                                      MessageParcel &reply,
                                      MessageOption &option) override;
        void hideKeyboardSelf(int flags) override;
        bool advanceToNext(bool isCurrentIme) override;
        void setDisplayMode(int mode) override;
        void onKeyboardShowed() override;

        void ResetFlag();
        bool WaitKeyboardReady();

    private:
        int userId_;

        std::mutex mtx;
        std::condition_variable cv;
        bool agentReadyFlag = false;
        bool keyboardReadyFlag = false;
        const int32_t sleepTime = 300;
    };
} // namespace MiscServices
} // namespace OHOS
#endif // SERVICES_INCLUDE_INPUT_CONTROL_CHANNEL_STUB_H
