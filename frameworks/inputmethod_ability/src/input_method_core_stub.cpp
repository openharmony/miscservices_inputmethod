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
#include "input_method_core_stub.h"

#include <chrono>
#include <cstdint>
#include "message_handler.h"
#include "i_input_data_channel.h"
#include "input_channel.h"
#include "platform.h"
#include "message_parcel.h"
#include "input_control_channel_proxy.h"
#include "input_method_ability.h"
#include <string_ex.h>

namespace OHOS {
namespace MiscServices {
    using namespace MessageID;
    /**
     * param userId the id of the user to whom the object is linking
     * @param userId
     */
    InputMethodCoreStub::InputMethodCoreStub(int userId)
    {
        userId_ = userId;
        msgHandler_ = nullptr;
    }

    InputMethodCoreStub::~InputMethodCoreStub()
    {
    }

    int32_t InputMethodCoreStub::OnRemoteRequest(uint32_t code, MessageParcel& data, MessageParcel& reply,
                                                 MessageOption& option)
    {
        IMSA_HILOGI("InputMethodCoreStub::OnRemoteRequest");
        auto descriptorToken = data.ReadInterfaceToken();
        if (descriptorToken != GetDescriptor()) {
            IMSA_HILOGI("InputMethodCoreStub::OnRemoteRequest descriptorToken is invalid");
            return ErrorCode::ERROR_STATUS_UNKNOWN_TRANSACTION;
        }
        switch (code) {
            case INITIALIZE_INPUT: {
                sptr<IRemoteObject> startInputToken = nullptr;
                int32_t displayId = data.ReadInt32();
                IMSA_HILOGI("InputMethodCoreStub::OnRemoteRequest displayId = %{public}d", displayId);
                sptr<IRemoteObject> channelObject = data.ReadRemoteObject();
                if (!channelObject) {
                    IMSA_HILOGI("InputMethodCoreStub::OnRemoteRequest channelObject is nullptr");
                }
                sptr<IInputControlChannel> inputControlChannel = new InputControlChannelProxy(channelObject);
                if (!inputControlChannel) {
                    IMSA_HILOGI("InputMethodCoreStub::OnRemoteRequest inputControlChannel is nullptr");
                }

                initializeInput(startInputToken, displayId, inputControlChannel);
                reply.WriteNoException();
                break;
            }
            case INIT_INPUT_CONTROL_CHANNEL: {
                sptr<IRemoteObject> channelObject = data.ReadRemoteObject();
                if (!channelObject) {
                    IMSA_HILOGI("InputMethodCoreStub::OnRemoteRequest channelObject is nullptr");
                }
                sptr<IInputControlChannel> inputControlChannel = new InputControlChannelProxy(channelObject);
                if (!inputControlChannel) {
                    IMSA_HILOGI("InputMethodCoreStub::OnRemoteRequest inputControlChannel is nullptr");
                }

                InitInputControlChannel(inputControlChannel);
                reply.WriteNoException();
                break;
            }
            case START_INPUT: {
                sptr<IInputDataChannel> inputDataChannel = iface_cast<IInputDataChannel>(data.ReadRemoteObject());
                InputAttribute *editorAttribute = data.ReadParcelable<InputAttribute>();
                bool supportPhysicalKbd = data.ReadBool();

                if (!inputDataChannel) {
                    IMSA_HILOGI("InputMethodCoreStub::OnRemoteRequest START_INPUT inputDataChannel is nulltpr");
                }
                startInput(inputDataChannel, *editorAttribute, supportPhysicalKbd);
                delete editorAttribute;
                reply.WriteNoException();
                break;
            }
            case SET_CLIENT_STATE: {
                bool state = data.ReadBool();
                SetClientState(state);
                break;
            }
            case STOP_INPUT: {
                stopInput();
                reply.WriteNoException();
                break;
            }
            case SHOW_KEYBOARD: {
                sptr<IInputDataChannel> inputDataChannel = iface_cast<IInputDataChannel>(data.ReadRemoteObject());
                showKeyboard(inputDataChannel);
                reply.WriteNoException();
                break;
            }
            case HIDE_KEYBOARD: {
                int32_t flags = data.ReadInt32();
                hideKeyboard(flags);
                reply.WriteNoException();
                break;
            }
            case SET_KEYBOARD_TYPE: {
                KeyboardType *type = data.ReadParcelable<KeyboardType>();
                setKeyboardType(*type);
                delete type;
                reply.WriteNoException();
                break;
            }
            case GET_KEYBOARD_WINDOW_HEIGHT: {
                int32_t retHeight = 0;
                getKeyboardWindowHeight(retHeight);
                reply.WriteNoException();
                break;
            }
            case STOP_INPUT_SERVICE: {
                std::string imeId = Str16ToStr8(data.ReadString16());
                StopInputService(imeId);
                reply.WriteNoException();
                break;
            }
            default: {
                return IRemoteStub::OnRemoteRequest(code, data, reply, option);
            }
        }
        return NO_ERROR;
    }

    int32_t InputMethodCoreStub::initializeInput(sptr<IRemoteObject>& startInputToken, int32_t displayId,
        sptr<IInputControlChannel>& inputControlChannel)
    {
        IMSA_HILOGI("InputMethodCoreStub::initializeInput");
        if (!msgHandler_) {
            return ErrorCode::ERROR_NULL_POINTER;
        }

        if (!startInputToken) {
            IMSA_HILOGI("InputMethodCoreStub::initializeInput startInputToken is nullptr");
        }

        MessageParcel *data = new MessageParcel();
        data->WriteInt32(displayId);
        if (inputControlChannel) {
            IMSA_HILOGI("InputMethodCoreStub::initializeInput. inputControlChannel is not nullptr");
            data->WriteRemoteObject(inputControlChannel->AsObject());
        }
        Message *msg = new Message(MessageID::MSG_ID_INITIALIZE_INPUT, data);
        msgHandler_->SendMessage(msg);
        return ErrorCode::NO_ERROR;
    }

    int32_t InputMethodCoreStub::InitInputControlChannel(sptr<IInputControlChannel>& inputControlChannel)
    {
        IMSA_HILOGI("InputMethodCoreStub::initializeInput");
        if (!msgHandler_) {
            return ErrorCode::ERROR_NULL_POINTER;
        }

        MessageParcel *data = new MessageParcel();
        if (inputControlChannel) {
            IMSA_HILOGI("InputMethodCoreStub::initializeInput. inputControlChannel is not nullptr");
            data->WriteRemoteObject(inputControlChannel->AsObject());
        }
        Message *msg = new Message(MessageID::MSG_ID_INIT_INPUT_CONTROL_CHANNEL, data);
        msgHandler_->SendMessage(msg);
        return ErrorCode::NO_ERROR;
    }

    bool InputMethodCoreStub::startInput(const sptr<IInputDataChannel>& inputDataChannel,
        const InputAttribute& editorAttribute, bool supportPhysicalKbd)
    {
        IMSA_HILOGI("InputMethodCoreStub::startInput");
        if (!msgHandler_) {
            return ErrorCode::ERROR_NULL_POINTER;
        }
        MessageParcel *data = new MessageParcel();
        if (inputDataChannel) {
            IMSA_HILOGI("InputMethodCoreStub::startInput inputDataChannel is not nullptr");
            data->WriteRemoteObject(inputDataChannel->AsObject());
        }
        data->WriteParcelable(&editorAttribute);
        data->WriteBool(supportPhysicalKbd);
        Message *msg = new Message(MessageID::MSG_ID_START_INPUT, data);
        msgHandler_->SendMessage(msg);
        return true;
    }

    int32_t InputMethodCoreStub::stopInput()
    {
        IMSA_HILOGI("InputMethodCoreStub::stopInput");
        if (!msgHandler_) {
            return ErrorCode::ERROR_NULL_POINTER;
        }
        MessageParcel *data = new MessageParcel();
        Message *msg = new Message(MessageID::MSG_ID_STOP_INPUT, data);
        msgHandler_->SendMessage(msg);
        return ErrorCode::NO_ERROR;
    }

    void InputMethodCoreStub::SetClientState(bool state)
    {
        IMSA_HILOGI("InputMethodCoreStub::SetClientState");
        if (!msgHandler_) {
            return;
        }
        MessageParcel *data = new MessageParcel();

        data->WriteBool(state);
        Message *msg = new Message(MessageID::MSG_ID_SET_CLIENT_STATE, data);
        msgHandler_->SendMessage(msg);
    }

    bool InputMethodCoreStub::showKeyboard(const sptr<IInputDataChannel>& inputDataChannel)
    {
        IMSA_HILOGI("InputMethodCoreStub::showKeyboard");
        if (!msgHandler_) {
            return false;
        }
        MessageParcel *data = new MessageParcel();
        if (inputDataChannel) {
            IMSA_HILOGI("InputMethodCoreStub::showKeyboard inputDataChannel is not nullptr");
            data->WriteRemoteObject(inputDataChannel->AsObject());
        }

        Message *msg = new Message(MessageID::MSG_ID_SHOW_KEYBOARD, data);
        msgHandler_->SendMessage(msg);
        return true;
    }

    bool InputMethodCoreStub::hideKeyboard(int32_t flags)
    {
        IMSA_HILOGI("InputMethodCoreStub::hideKeyboard");
        if (!msgHandler_) {
            return ErrorCode::ERROR_NULL_POINTER;
        }
        MessageParcel *data = new MessageParcel();
        data->WriteInt32(userId_);
        data->WriteInt32(flags);

        Message *msg = new Message(MessageID::MSG_ID_HIDE_KEYBOARD, data);
        msgHandler_->SendMessage(msg);
        return true;
    }

    int32_t InputMethodCoreStub::setKeyboardType(const KeyboardType& type)
    {
        IMSA_HILOGI("InputMethodCoreStub::setKeyboardType");
        if (!msgHandler_) {
            return ErrorCode::ERROR_NULL_POINTER;
        }
        MessageParcel *data = new MessageParcel();
        data->WriteParcelable(&type);

        Message *msg = new Message(MessageID::MSG_ID_SET_KEYBOARD_TYPE, data);
        msgHandler_->SendMessage(msg);
        return ErrorCode::NO_ERROR;
    }

    void InputMethodCoreStub::StopInputService(std::string imeId)
    {
        IMSA_HILOGI("InputMethodCoreStub::StopInputService");
        if (!msgHandler_) {
            return;
        }
        MessageParcel *data = new MessageParcel();
        data->WriteString16(Str8ToStr16(imeId));

        Message *msg = new Message(MessageID::MSG_ID_STOP_INPUT_SERVICE, data);
        msgHandler_->SendMessage(msg);
    }

    int32_t InputMethodCoreStub::getKeyboardWindowHeight(int32_t &retHeight)
    {
        IMSA_HILOGI("InputMethodCoreStub::getKeyboardWindowHeight");
        if (!msgHandler_) {
            return ErrorCode::ERROR_NULL_POINTER;
        }
        MessageParcel *data = new MessageParcel();

        Message *msg = new Message(MessageID::MSG_ID_GET_KEYBOARD_WINDOW_HEIGHT, data);
        msgHandler_->SendMessage(msg);
        return ErrorCode::NO_ERROR;
    }

    void InputMethodCoreStub::SetMessageHandler(MessageHandler *msgHandler)
    {
        msgHandler_ = msgHandler;
    }
} // namespace MiscServices
} // namespace OHOS
