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

#ifndef FRAMEWORKS_INPUTMETHOD_ABILITY_INCLUDE_INPUT_METHOD_CORE_STUB_H
#define FRAMEWORKS_INPUTMETHOD_ABILITY_INCLUDE_INPUT_METHOD_CORE_STUB_H

#include <mutex>
#include <condition_variable>
#include <cstdint>
#include "iremote_broker.h"
#include "iremote_stub.h"
#include "i_input_method_agent.h"
#include "input_channel.h"
#include "message_parcel.h"
#include "input_attribute.h"
#include "i_input_data_channel.h"
#include "i_input_method_core.h"
#include "i_input_control_channel.h"
#include "keyboard_type.h"
#include "message_handler.h"

namespace OHOS {
namespace MiscServices {
    class InputMethodCoreStub : public IRemoteStub<IInputMethodCore> {
    public:
        DISALLOW_COPY_AND_MOVE(InputMethodCoreStub);
        explicit InputMethodCoreStub(int userId);
        virtual ~InputMethodCoreStub();
        int OnRemoteRequest(uint32_t code,
                                    MessageParcel &data,
                                    MessageParcel &reply,
                                    MessageOption &option) override;

        int32_t initializeInput(sptr<IRemoteObject>& startInputToken, int32_t displayId,
                                        sptr<IInputControlChannel>& inputControlChannel) override;
        bool startInput(const sptr<IInputDataChannel>& inputDataChannel,
                                const InputAttribute& editorAttribute,
                                bool supportPhysicalKbd) override;
        int32_t stopInput() override;
        bool showKeyboard(const sptr<IInputDataChannel>& inputDataChannel) override;
        bool hideKeyboard(int32_t flags)override;
        int32_t setKeyboardType(const KeyboardType& type) override;
        int32_t getKeyboardWindowHeight(int32_t &retHeight) override;
        int32_t InitInputControlChannel(sptr<IInputControlChannel> &inputControlChannel) override;
        void SetClientState(bool state) override;
        void StopInputService(std::string imeId) override;
        void SetMessageHandler(MessageHandler *msgHandler);

    private:
        int userId_;
        MessageHandler *msgHandler_;
    };
} // namespace MiscServices
} // namespace OHOS
#endif // FRAMEWORKS_INPUTMETHOD_ABILITY_INCLUDE_INPUT_METHOD_CORE_STUB_H
