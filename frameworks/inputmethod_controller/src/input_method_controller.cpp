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

#include "input_method_controller.h"
#include "iservice_registry.h"
#include "system_ability_definition.h"
#include "global.h"
#include "inputmethod_sysevent.h"
#include "inputmethod_trace.h"

namespace OHOS {
namespace MiscServices {
using namespace MessageID;
    sptr<InputMethodController> InputMethodController::instance_;
    std::mutex InputMethodController::instanceLock_;

    InputMethodController::InputMethodController() : stop_(false)
    {
        IMSA_HILOGI("InputMethodController structure");
        Initialize();
    }

    InputMethodController::~InputMethodController()
    {
        if (msgHandler) {
            delete msgHandler;
            msgHandler = nullptr;
            stop_ = false;
        }
    }

    sptr<InputMethodController> InputMethodController::GetInstance()
    {
        if (!instance_) {
            std::lock_guard<std::mutex> autoLock(instanceLock_);
            if (!instance_) {
                IMSA_HILOGI("InputMethodController::GetInstance instance_ is nullptr");
                instance_ = new InputMethodController();
            }
        }
        return instance_;
    }

    bool InputMethodController::Initialize()
    {
        mImms = GetImsaProxy();

        msgHandler = new MessageHandler();

        mClient = new InputClientStub();
        mClient->SetHandler(msgHandler);

        mInputDataChannel = new InputDataChannelStub();
        mInputDataChannel->SetHandler(msgHandler);

        workThreadHandler = std::thread([this] {WorkThread();});
        mAttribute.SetInputPattern(InputAttribute::PATTERN_TEXT);

        textListener = nullptr;
        IMSA_HILOGI("InputMethodController::Initialize textListener is nullptr");
        PrepareInput(0, mClient, mInputDataChannel, mAttribute);
        return true;
    }

    sptr<InputMethodSystemAbilityProxy> InputMethodController::GetImsaProxy()
    {
        IMSA_HILOGI("InputMethodController::GetImsaProxy");
        sptr<ISystemAbilityManager> systemAbilityManager =
            SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
        if (!systemAbilityManager) {
            IMSA_HILOGI("InputMethodController::GetImsaProxy systemAbilityManager is nullptr");
            return nullptr;
        }

        int32_t uid = IPCSkeleton::GetCallingUid();
        std::string strBundleName = "com.inputmethod.default";
        auto systemAbility = systemAbilityManager->GetSystemAbility(INPUT_METHOD_SYSTEM_ABILITY_ID, "");
        if (!systemAbility) {
            IMSA_HILOGI("InputMethodController::GetImsaProxy systemAbility is nullptr");
            FaultReporter(uid, strBundleName, ErrorCode::ERROR_NULL_POINTER);
            return nullptr;
        }

        if (!deathRecipient_) {
            deathRecipient_ = new ImsaDeathRecipient();
        }
        systemAbility->AddDeathRecipient(deathRecipient_);

        sptr<InputMethodSystemAbilityProxy> iface = new InputMethodSystemAbilityProxy(systemAbility);
        return iface;
    }

    void InputMethodController::WorkThread()
    {
        while (!stop_) {
            Message *msg = msgHandler->GetMessage();
            switch (msg->msgId_) {
                case MSG_ID_INSERT_CHAR: {
                    MessageParcel *data = msg->msgContent_;
                    std::u16string text = data->ReadString16();
                    IMSA_HILOGI("InputMethodController::WorkThread InsertText");
                    if (textListener) {
                        textListener->InsertText(text);
                    }
                    break;
                }

                case MSG_ID_DELETE_FORWARD: {
                    MessageParcel *data = msg->msgContent_;
                    int32_t length = data->ReadInt32();
                    IMSA_HILOGI("InputMethodController::WorkThread DeleteForward");
                    if (textListener) {
                        textListener->DeleteForward(length);
                    }
                    break;
                }
                case MSG_ID_DELETE_BACKWARD: {
                    MessageParcel *data = msg->msgContent_;
                    int32_t length = data->ReadInt32();
                    IMSA_HILOGI("InputMethodController::WorkThread DeleteBackward");
                    if (textListener) {
                        textListener->DeleteBackward(length);
                    }
                    break;
                }
                case MSG_ID_SET_DISPLAY_MODE: {
                    MessageParcel *data = msg->msgContent_;
                    int32_t ret = data->ReadInt32();
                    IMSA_HILOGI("MSG_ID_SET_DISPLAY_MODE : %{public}d", ret);
                    break;
                }
                case MSG_ID_ON_INPUT_READY: {
                    MessageParcel *data = msg->msgContent_;
                    sptr<IRemoteObject> object = data->ReadRemoteObject();
                    if (object) {
                        mAgent = new InputMethodAgentProxy(object);
                    }
                    break;
                }
                case MSG_ID_EXIT_SERVICE: {
                    MessageParcel *data = msg->msgContent_;
                    int32_t ret = data->ReadInt32();
                    textListener = nullptr;
                    IMSA_HILOGI("InputMethodController::WorkThread MSG_ID_EXIT_SERVICE : %{public}d", ret);
                    break;
                }
                case MSG_ID_SEND_KEYBOARD_STATUS: {
                    MessageParcel *data = msg->msgContent_;
                    int32_t ret = data->ReadInt32();
                    KeyboardInfo *info = new KeyboardInfo();
                    info->SetKeyboardStatus(ret);
                    IMSA_HILOGI("InputMethodController::WorkThread SendKeyboardInfo");
                    if (textListener) {
                        textListener->SendKeyboardInfo(*info);
                    }
                    delete info;
                    break;
                }
                case MSG_ID_SEND_FUNCTION_KEY: {
                    MessageParcel *data = msg->msgContent_;
                    int32_t ret = data->ReadInt32();
                    KeyboardInfo *info = new KeyboardInfo();
                    info->SetFunctionKey(ret);
                    IMSA_HILOGI("InputMethodController::WorkThread SendKeyboardInfo");
                    if (textListener) {
                        textListener->SendKeyboardInfo(*info);
                    }
                    delete info;
                    break;
                }
                case MSG_ID_MOVE_CURSOR: {
                    MessageParcel *data = msg->msgContent_;
                    int32_t ret = data->ReadInt32();
                    IMSA_HILOGI("InputMethodController::WorkThread MoveCursor");
                    if (textListener) {
                        Direction direction = static_cast<Direction>(ret);
                        textListener->MoveCursor(direction);
                    }
                    break;
                }
                default: {
                    break;
                }
            }
            delete msg;
            msg = nullptr;
        }
    }

    void InputMethodController::Attach(sptr<OnTextChangedListener> &listener)
    {
        textListener = listener;
        IMSA_HILOGI("InputMethodController::Attach");
        InputmethodTrace tracer("InputMethodController Attach trace.");
        StartInput(mClient);
        PrepareInput(0, mClient, mInputDataChannel, mAttribute);
    }

    void InputMethodController::ShowTextInput()
    {
        IMSA_HILOGI("InputMethodController::ShowTextInput");
        StartInput(mClient);
    }

    void InputMethodController::HideTextInput()
    {
        IMSA_HILOGI("InputMethodController::HideTextInput");
        StopInput(mClient);
    }

    void InputMethodController::HideCurrentInput()
    {
        IMSA_HILOGI("InputMethodController::HideCurrentInput");
        if (!mImms) {
            return;
        }
        MessageParcel data;
        if (!(data.WriteInterfaceToken(mImms->GetDescriptor()))) {
            return;
        }
        mImms->HideCurrentInput(data);
    }

    void InputMethodController::Close()
    {
        ReleaseInput(mClient);
        InputmethodTrace tracer("InputMethodController Close trace.");
        textListener = nullptr;
        IMSA_HILOGI("InputMethodController::Close");
    }

    void InputMethodController::PrepareInput(int32_t displayId, sptr<InputClientStub> &client,
                                             sptr<InputDataChannelStub> &channel, InputAttribute &attribute)
    {
        IMSA_HILOGI("InputMethodController::PrepareInput");
        if (!mImms) {
            return;
        }
        MessageParcel data;
        if (!(data.WriteInterfaceToken(mImms->GetDescriptor())
            && data.WriteInt32(displayId)
            && data.WriteRemoteObject(client->AsObject())
            && data.WriteRemoteObject(channel->AsObject())
            && data.WriteParcelable(&attribute))) {
            return;
        }
        mImms->prepareInput(data);
    }

    void InputMethodController::DisplayOptionalInputMethod()
    {
        IMSA_HILOGI("InputMethodController::DisplayOptionalInputMethod");
        if (!mImms) {
            return;
        }
        MessageParcel data;
        if (!(data.WriteInterfaceToken(mImms->GetDescriptor()))) {
            return;
        }
        mImms->displayOptionalInputMethod(data);
    }

    std::vector<InputMethodProperty*> InputMethodController::ListInputMethod()
    {
        IMSA_HILOGI("InputMethodController::listInputMethod");
        std::vector<InputMethodProperty*> properties;
        if (!mImms) {
            return properties;
        }
        mImms->listInputMethod(&properties);
        return properties;
    }

    void InputMethodController::StartInput(sptr<InputClientStub> &client)
    {
        IMSA_HILOGI("InputMethodController::StartInput");
        if (!mImms) {
            return;
        }
        MessageParcel data;
        if (!(data.WriteInterfaceToken(mImms->GetDescriptor())
            && data.WriteRemoteObject(client->AsObject()))) {
            return;
        }
        mImms->startInput(data);
    }

    void InputMethodController::ReleaseInput(sptr<InputClientStub> &client)
    {
        IMSA_HILOGI("InputMethodController::ReleaseInput");
        if (!mImms) {
            return;
        }
        MessageParcel data;
        if (!(data.WriteInterfaceToken(mImms->GetDescriptor())
            && data.WriteRemoteObject(client->AsObject().GetRefPtr()))) {
            return;
        }
        mImms->releaseInput(data);
    }

    void InputMethodController::StopInput(sptr<InputClientStub> &client)
    {
        IMSA_HILOGI("InputMethodController::StopInput");
        if (!mImms) {
            return;
        }
        MessageParcel data;
        if (!(data.WriteInterfaceToken(mImms->GetDescriptor())
            && data.WriteRemoteObject(client->AsObject().GetRefPtr()))) {
            return;
        }
        mImms->stopInput(data);
    }

    void InputMethodController::OnRemoteSaDied(const wptr<IRemoteObject> &remote)
    {
        mImms = GetImsaProxy();
    }

    ImsaDeathRecipient::ImsaDeathRecipient()
    {
    }

    void ImsaDeathRecipient::OnRemoteDied(const wptr<IRemoteObject> &object)
    {
        InputMethodController::GetInstance()->OnRemoteSaDied(object);
    }

    void InputMethodController::OnCursorUpdate(CursorInfo cursorInfo)
    {
        if (!mAgent) {
            IMSA_HILOGI("InputMethodController::OnCursorUpdate mAgent is nullptr");
            return;
        }

        if (cursorInfo_.left == cursorInfo.left && cursorInfo_.top == cursorInfo.top
            && cursorInfo_.height == cursorInfo.height) {
            return;
        }
        cursorInfo_ = cursorInfo;
        mAgent->OnCursorUpdate(cursorInfo.left, cursorInfo.top, cursorInfo.height);
    }

    void InputMethodController::OnSelectionChange(std::u16string text, int start, int end)
    {
        if (mTextString == text && mSelectNewBegin == start && mSelectNewEnd == end) {
            return;
        }
        IMSA_HILOGI("InputMethodController::OnSelectionChange");
        mTextString = text;
        mSelectOldBegin = mSelectNewBegin;
        mSelectOldEnd = mSelectNewEnd;
        mSelectNewBegin = start;
        mSelectNewEnd = end;
        if (!mAgent) {
            IMSA_HILOGI("InputMethodController::OnSelectionChange mAgent is nullptr");
            return;
        }
        mAgent->OnSelectionChange(mTextString, mSelectOldBegin, mSelectOldEnd, mSelectNewBegin, mSelectNewEnd);
    }

    void InputMethodController::OnConfigurationChange(Configuration info)
    {
        IMSA_HILOGI("InputMethodController::OnConfigurationChange");
        enterKeyType_ = static_cast<uint32_t>(info.GetEnterKeyType());
        inputPattern_ = static_cast<uint32_t>(info.GetTextInputType());
    }

    std::u16string InputMethodController::GetTextBeforeCursor(int32_t number)
    {
        IMSA_HILOGI("InputMethodController::GetTextBeforeCursor");
        if (!mTextString.empty()) {
            int32_t startPos = (mSelectNewBegin >= number ? (mSelectNewBegin - number + 1) : 0);
            return mTextString.substr(startPos, mSelectNewBegin);
        }
        return u"";
    }

    std::u16string InputMethodController::GetTextAfterCursor(int32_t number)
    {
        IMSA_HILOGI("InputMethodController::GetTextBeforeCursor");
        if (!mTextString.empty() && mTextString.size() <= INT_MAX) {
            int32_t endPos = (mSelectNewEnd + number < static_cast<int32_t>(mTextString.size()))
                                 ? (mSelectNewEnd + number)
                                 : mTextString.size();
            return mTextString.substr(mSelectNewEnd, endPos);
        }
        return u"";
    }

    bool InputMethodController::dispatchKeyEvent(std::shared_ptr<MMI::KeyEvent> keyEvent)
    {
        IMSA_HILOGI("InputMethodController::dispatchKeyEvent");
        if (!mAgent) {
            IMSA_HILOGI("InputMethodController::dispatchKeyEvent mAgent is nullptr");
            return false;
        }
        MessageParcel data;
        if (!(data.WriteInterfaceToken(mAgent->GetDescriptor())
            && data.WriteInt32(keyEvent->GetKeyCode())
            && data.WriteInt32(keyEvent->GetKeyAction()))) {
            return false;
        }

        return mAgent->DispatchKeyEvent(data);
    }

    int32_t InputMethodController::GetEnterKeyType()
    {
        IMSA_HILOGI("InputMethodController::GetEnterKeyType");
        return enterKeyType_;
    }

    int32_t InputMethodController::GetInputPattern()
    {
        IMSA_HILOGI("InputMethodController::GetInputPattern");
        return inputPattern_;
    }

    void InputMethodController::SetCallingWindow(uint32_t windowId)
    {
        IMSA_HILOGI("InputMethodController::SetCallingWindow windowId = %{public}d", windowId);
        if (!mAgent) {
            IMSA_HILOGI("InputMethodController::SetCallingWindow mAgent is nullptr");
            return;
        }
        mAgent->SetCallingWindow(windowId);
        return;
    }

    int32_t InputMethodController::SwitchInputMethod(InputMethodProperty *target)
    {
        IMSA_HILOGI("InputMethodController::SwitchInputMethod");
        if (!mImms) {
            return false;
        }
        return mImms->SwitchInputMethod(target);
    }
} // namespace MiscServices
} // namespace OHOS
