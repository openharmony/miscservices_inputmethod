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

#ifndef FM_IMC_PROJECT_INPUTMETHODAGENTSTUB_H
#define FM_IMC_PROJECT_INPUTMETHODAGENTSTUB_H

#include "iremote_stub.h"
#include "message_parcel.h"
#include "message_option.h"
#include "i_input_method_agent.h"
#include "message_handler.h"

namespace OHOS {
namespace MiscServices {
    class InputMethodAgentStub : public IRemoteStub<IInputMethodAgent> {
    public:
        explicit InputMethodAgentStub();
        virtual ~InputMethodAgentStub();
        virtual int32_t OnRemoteRequest(uint32_t code,
                                        MessageParcel &data,
                                        MessageParcel &reply,
                                        MessageOption &option) override;
        virtual bool DispatchKeyEvent(MessageParcel &data) override;
        virtual void OnCursorUpdate(int32_t positionX, int32_t positionY, int height) override;
        virtual void OnSelectionChange(std::u16string text, int32_t oldBegin, int32_t oldEnd,
                                       int32_t newBegin, int32_t newEnd) override;
        void SetCallingWindow(uint32_t windowId) override;
        void SetMessageHandler(MessageHandler *msgHandler);
    private:
        MessageHandler *msgHandler_;
    };
}
}
#endif // FM_IMC_PROJECT_INPUTMETHODAGENTSTUB_H