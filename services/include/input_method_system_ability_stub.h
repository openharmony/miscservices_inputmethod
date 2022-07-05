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

#ifndef SERVICES_INCLUDE_INPUT_METHOD_SYSTEM_ABILITY_STUB_H
#define SERVICES_INCLUDE_INPUT_METHOD_SYSTEM_ABILITY_STUB_H
#include <errors.h>
#include "refbase.h"
#include "i_input_method_system_ability.h"
#include "iremote_stub.h"
#include "global.h"
#include "message_parcel.h"

namespace OHOS {
namespace MiscServices {
    class InputMethodSystemAbilityStub : public IRemoteStub<IInputMethodSystemAbility> {
    public:
        int32_t OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
                                MessageOption &option) override;

        void prepareInput(MessageParcel& data) override;
        void releaseInput(MessageParcel& data) override;
        void startInput(MessageParcel& data) override;
        void stopInput(MessageParcel& data) override;
        void SetCoreAndAgent(MessageParcel& data) override;
        void HideCurrentInput(MessageParcel& data) override;
        void displayOptionalInputMethod(MessageParcel& data) override;
        virtual int32_t listInputMethodByUserId(int32_t userId, std::vector<InputMethodProperty*> *properties) = 0;
        int32_t SwitchInputMethod(MessageParcel &data, MessageParcel &reply);

    protected:
        int32_t getUserId(int32_t uid);
        int USER_ID_CHANGE_VALUE = 200000; // user range
    };
} // namespace MiscServices
} // namespace OHOS

#endif // SERVICES_INCLUDE_INPUT_METHOD_SYSTEM_ABILITY_STUB_H
