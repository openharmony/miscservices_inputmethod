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


#ifndef FRAMEWORKS_INPUTMETHOD_CONTROLLER_INCLUDE_INPUT_METHOD_CONTROLLER_H
#define FRAMEWORKS_INPUTMETHOD_CONTROLLER_INCLUDE_INPUT_METHOD_CONTROLLER_H

#include <mutex>
#include <thread>
#include "input_data_channel_stub.h"
#include "input_client_stub.h"
#include "input_method_system_ability_proxy.h"
#include "input_method_agent_proxy.h"
#include "i_input_method_agent.h"
#include "message_handler.h"
#include "iremote_object.h"
#include "input_method_utils.h"
#include "key_event.h"

namespace OHOS {
namespace MiscServices {
    class InputDataChannelStub;
    class InputMethodSystemAbilityProxy;
    class OnTextChangedListener : public virtual RefBase {
    public:
        virtual void InsertText(const std::u16string& text) = 0;
        virtual void DeleteForward(int32_t length) = 0;
        virtual void DeleteBackward(int32_t length) = 0;
        virtual void SendKeyEventFromInputMethod(const KeyEvent& event) = 0;
        virtual void SendKeyboardInfo(const KeyboardInfo& info) = 0;
        virtual void SetKeyboardStatus(bool status) = 0;
        virtual void MoveCursor(const Direction direction) = 0;
    };

    class ImsaDeathRecipient : public IRemoteObject::DeathRecipient {
    public:
        explicit ImsaDeathRecipient();
        ~ImsaDeathRecipient() = default;

        void OnRemoteDied(const wptr<IRemoteObject> &object) override;
    };

    class InputMethodController : public RefBase {
    public:
        static sptr<InputMethodController> GetInstance();
        void Attach(sptr<OnTextChangedListener> &listener);
        std::u16string GetTextBeforeCursor(int32_t number);
        std::u16string GetTextAfterCursor(int32_t number);
        void ShowTextInput();
        void HideTextInput();
        void Close();
        void OnRemoteSaDied(const wptr<IRemoteObject> &object);
        void OnCursorUpdate(CursorInfo cursorInfo);
        void OnSelectionChange(std::u16string text, int start, int end);
        void OnConfigurationChange(Configuration info);
        bool dispatchKeyEvent(std::shared_ptr<MMI::KeyEvent> keyEvent);
        void DisplayOptionalInputMethod();
        std::vector<InputMethodProperty*> ListInputMethod();
        int32_t GetEnterKeyType();
        int32_t GetInputPattern();
        void HideCurrentInput();
        void SetCallingWindow(uint32_t windowId);

    private:
        InputMethodController();
        ~InputMethodController();

        bool Initialize();
        sptr<InputMethodSystemAbilityProxy> GetImsaProxy();
        void PrepareInput(int32_t displayId, sptr<InputClientStub> &client, sptr<InputDataChannelStub> &channel,
                          InputAttribute &attribute);
        void StartInput(sptr<InputClientStub> &client);
        void StopInput(sptr<InputClientStub> &client);
        void ReleaseInput(sptr<InputClientStub> &client);
        void WorkThread();

        sptr<InputDataChannelStub> mInputDataChannel;
        sptr<InputClientStub> mClient;
        sptr<InputMethodSystemAbilityProxy> mImms;
        sptr<ImsaDeathRecipient> deathRecipient_;
        sptr<InputMethodAgentProxy> mAgent;
        sptr<OnTextChangedListener> textListener;
        InputAttribute mAttribute;
        std::u16string mTextString;
        int mSelectOldBegin = 0;
        int mSelectOldEnd = 0;
        int mSelectNewBegin = 0;
        int mSelectNewEnd = 0;
        CursorInfo cursorInfo_;

        static std::mutex instanceLock_;
        static sptr<InputMethodController> instance_;
        std::thread workThreadHandler;
        MessageHandler *msgHandler;
        bool stop_;
        int32_t enterKeyType_ = 0;
        int32_t inputPattern_ = 0;
    };
} // namespace MiscServices
} // namespace OHOS
#endif // FRAMEWORKS_INPUTMETHOD_CONTROLLER_INCLUDE_INPUT_METHOD_CONTROLLER_H
