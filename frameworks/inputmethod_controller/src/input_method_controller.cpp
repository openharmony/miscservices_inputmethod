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
        if (msgHandler != nullptr) {
            delete msgHandler;
            msgHandler = nullptr;
            stop_ = false;
        }
    }

    sptr<InputMethodController> InputMethodController::GetInstance()
    {
        IMSA_HILOGI("InputMethodController::GetInstance");
        if (instance_ == nullptr) {
            std::lock_guard<std::mutex> autoLock(instanceLock_);
            if (instance_ == nullptr) {
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
        return true;
    }

    sptr<InputMethodSystemAbilityProxy> InputMethodController::GetImsaProxy()
    {
        IMSA_HILOGI("InputMethodController::GetImsaProxy");
        sptr<ISystemAbilityManager> systemAbilityManager =
            SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
        if (systemAbilityManager == nullptr) {
            IMSA_HILOGI("InputMethodController::GetImsaProxy systemAbilityManager is nullptr");
            return nullptr;
        }

        auto systemAbility = systemAbilityManager->GetSystemAbility(INPUT_METHOD_SYSTEM_ABILITY_ID, "");
        if (systemAbility == nullptr) {
            IMSA_HILOGI("InputMethodController::GetImsaProxy systemAbility is nullptr");
            return nullptr;
        }

        if (deathRecipient_ == nullptr) {
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
                    if (textListener != nullptr) {
                        textListener->InsertText(text);
                    }
                    break;
                }

                case MSG_ID_DELETE_FORWARD: {
                    IMSA_HILOGI("InputMethodController::MSG_ID_DELETE_FORWARD");
                    MessageParcel *data = msg->msgContent_;
                    int32_t length = data->ReadInt32();
                    if (textListener != nullptr) {
                        textListener->DeleteForward(length);
                    }
                    break;
                }
                case MSG_ID_DELETE_BACKWARD: {
                    IMSA_HILOGI("InputMethodController::MSG_ID_DELETE_BACKWARD");
                    MessageParcel *data = msg->msgContent_;
                    int32_t length = data->ReadInt32();
                    if (textListener != nullptr) {
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
                    int32_t ret = data->ReadInt32();
                    if (ret != ErrorCode::NO_ERROR) {
                        if (textListener != nullptr) {
                            textListener->SetKeyboardStatus(false);
                        }
                        mAgent = nullptr;
                        break;
                    }
                    sptr<IRemoteObject> object = data->ReadRemoteObject();
                    mAgent = new InputMethodAgentProxy(object);
                    if (textListener != nullptr) {
                        textListener->SetKeyboardStatus(true);
                    }
                    break;
                }
                case MSG_ID_EXIT_SERVICE: {
                    MessageParcel *data = msg->msgContent_;
                    int32_t ret = data->ReadInt32();
                    textListener = nullptr;
                    IMSA_HILOGI("MSG_ID_EXIT_SERVICE : %{public}d", ret);
                    break;
                }
                case MSG_ID_SEND_KEYBOARD_STATUS: {
                    MessageParcel *data = msg->msgContent_;
                    int32_t ret = data->ReadInt32();
                    IMSA_HILOGI("MSG_ID_SEND_KEYBOARD_STATUS : %{public}d", ret);
                    KeyboardInfo *info = new KeyboardInfo();
                    info->SetKeyboardStatus(ret);
                    if (textListener != nullptr) {
                        textListener->SendKeyboardInfo(*info);
                    }
                    break;
                }
                case MSG_ID_SEND_FUNCTION_KEY: {
                    MessageParcel *data = msg->msgContent_;
                    int32_t ret = data->ReadInt32();
                    IMSA_HILOGI("MSG_ID_SEND_FUNCTION_KEY : %{public}d", ret);
                    KeyboardInfo *info = new KeyboardInfo();
                    info->SetFunctionKey(ret);
                    if (textListener != nullptr) {
                        textListener->SendKeyboardInfo(*info);
                    }
                    break;
                }
                case MSG_ID_MOVE_CURSOR: {
                    MessageParcel *data = msg->msgContent_;
                    int32_t ret = data->ReadInt32();
                    IMSA_HILOGI("MSG_ID_MOVE_CURSOR : %{public}d", ret);
                    if (textListener != nullptr) {
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
        PrepareInput(0, mClient, mInputDataChannel, mAttribute);
        textListener = listener;
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

    void InputMethodController::Close()
    {
        ReleaseInput(mClient);
        textListener = nullptr;
    }

    void InputMethodController::PrepareInput(int32_t displayId, sptr<InputClientStub> &client,
                                             sptr<InputDataChannelStub> &channel, InputAttribute &attribute)
    {
        IMSA_HILOGI("InputMethodController::PrepareInput");
        if (mImms == nullptr) {
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

    void InputMethodController::StartInput(sptr<InputClientStub> &client)
    {
        IMSA_HILOGI("InputMethodController::StartInput");
        if (mImms == nullptr) {
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
        if (mImms == nullptr) {
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
        if (mImms == nullptr) {
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
        if (mAgent == nullptr) {
            IMSA_HILOGI("InputMethodController::OnCursorUpdate mAgent is nullptr");
            return;
        }

        mAgent->OnCursorUpdate(cursorInfo.left, cursorInfo.top, cursorInfo.height);
    }

    void InputMethodController::OnSelectionChange(std::u16string text, int start, int end)
    {
        IMSA_HILOGI("InputMethodController::OnSelectionChange");
        mTextString = text;
        mSelectOldBegin = mSelectNewBegin;
        mSelectOldEnd = mSelectNewEnd;
        mSelectNewBegin = start;
        mSelectNewEnd = end;
        if (mAgent == nullptr) {
            IMSA_HILOGI("InputMethodController::OnSelectionChange mAgent is nullptr");
            return;
        }
        mAgent->OnSelectionChange(mTextString, mSelectOldBegin, mSelectOldEnd, mSelectNewBegin, mSelectNewEnd);
    }

    void InputMethodController::OnConfigurationChange(Configuration info)
    {
        IMSA_HILOGI("InputMethodController::OnConfigurationChange");
    }

    std::u16string InputMethodController::GetTextBeforeCursor()
    {
        IMSA_HILOGI("InputMethodController::GetTextBeforeCursor");
        if (!mTextString.empty()) {
            return mTextString.substr(0, mSelectNewBegin);
        }
        return u"";
    }

    std::u16string InputMethodController::GetTextAfterCursor()
    {
        IMSA_HILOGI("InputMethodController::GetTextBeforeCursor");
        if (!mTextString.empty()) {
            if (mTextString.size() > mSelectNewEnd) {
                return mTextString.substr(mSelectNewEnd);
            }
        }
        return u"";
    }

    bool InputMethodController::dispatchKeyEvent(std::shared_ptr<MMI::KeyEvent> keyEvent)
    {
        IMSA_HILOGI("InputMethodController::dispatchKeyEvent");
        if (mAgent == nullptr) {
            IMSA_HILOGI("InputMethodController::dispatchKeyEvent mAgent is nullptr");
            return false;
        }
        IMSA_HILOGI("InputMethodController::dispatchKeyEvent (%{public}d, %{public}d)", keyEvent->GetKeyCode(), keyEvent->GetKeyAction());
        mAgent->DispatchKey(keyEvent->GetKeyCode(), keyEvent->GetKeyAction());
        return true;
    }
}
}
