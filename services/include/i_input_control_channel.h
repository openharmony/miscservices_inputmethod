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

#ifndef FM_IMMS_PROJECT_IINPUTCONTROLCHANNEL_H
#define FM_IMMS_PROJECT_IINPUTCONTROLCHANNEL_H

#include "iremote_broker.h"
#include "input_channel.h"
#include "i_input_method_agent.h"
#include "global.h"

namespace OHOS {
    namespace MiscServices {
        class IInputControlChannel : public IRemoteBroker {
        public:
            enum {
                HIDE_KEYBOARD_SELF = FIRST_CALL_TRANSACTION,
                ADVANCE_TO_NEXT,
                SET_DISPLAY_MODE,
                ON_KEYBOARD_SHOWED,
            };

            DECLARE_INTERFACE_DESCRIPTOR(u"ohos.miscservices.inputmethod.InputControlChannel");

            virtual void hideKeyboardSelf(int flags) = 0;
            virtual bool advanceToNext(bool isCurrentIme) = 0;
            virtual void setDisplayMode(int mode) = 0;
            virtual void onKeyboardShowed() = 0;
        };
    } // namespace MiscServices
} // namespace OHOS
#endif // SERVICES_INCLUDE_I_INPUT_CONTROL_CHANNEL_H
