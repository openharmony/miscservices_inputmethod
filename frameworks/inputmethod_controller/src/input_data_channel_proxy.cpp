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

#include "input_data_channel_proxy.h"
#include "message_parcel.h"
#include "utils.h"

namespace OHOS {
namespace MiscServices {
    InputDataChannelProxy::InputDataChannelProxy(const sptr<IRemoteObject> &object)
        : IRemoteProxy<IInputDataChannel>(object)
    {
    }

    bool InputDataChannelProxy::InsertText(const std::u16string& text)
    {
        IMSA_HILOGI("InputDataChannelProxy::InsertText");
        MessageParcel data, reply;
        MessageOption option;
        data.WriteInterfaceToken(GetDescriptor());
        data.WriteString16(text);

        auto ret = Remote()->SendRequest(INSERT_TEXT, data, reply, option);
        if (ret != NO_ERROR) {
            return false;
        }
        auto result = reply.ReadBool();
        return result;
    }

    bool InputDataChannelProxy::DeleteForward(int32_t length)
    {
        IMSA_HILOGI("InputDataChannelProxy::DeleteForward");
        MessageParcel data, reply;
        MessageOption option;
        data.WriteInterfaceToken(GetDescriptor());
        data.WriteInt32(length);

        auto ret = Remote()->SendRequest(DELETE_FORWARD, data, reply, option);
        if (ret != NO_ERROR) {
            return false;
        }
        auto result = reply.ReadBool();
        return result;
    }

    bool InputDataChannelProxy::DeleteBackward(int32_t length)
    {
        IMSA_HILOGI("InputDataChannelProxy::DeleteBackward");
        MessageParcel data, reply;
        MessageOption option;
        data.WriteInterfaceToken(GetDescriptor());
        data.WriteInt32(length);

        auto ret = Remote()->SendRequest(DELETE_BACKWARD, data, reply, option);
        if (ret != NO_ERROR) {
            return false;
        }
        auto result = reply.ReadBool();
        return result;
    }

    void InputDataChannelProxy::Close()
    {
        IMSA_HILOGI("InputDataChannelProxy::Close");
        MessageParcel data, reply;
        MessageOption option;
        data.WriteInterfaceToken(GetDescriptor());

        auto ret = Remote()->SendRequest(CLOSE, data, reply, option);
        if (ret != NO_ERROR) {
        }
    }

    std::u16string InputDataChannelProxy::GetTextBeforeCursor()
    {
        IMSA_HILOGI("InputDataChannelProxy::GetTextBeforeCursor");
        MessageParcel data, reply;
        MessageOption option;
        data.WriteInterfaceToken(GetDescriptor());

        Remote()->SendRequest(GET_TEXT_BEFORE_CURSOR, data, reply, option);
        auto result = reply.ReadString16();
        return result;
    }

    std::u16string InputDataChannelProxy::GetTextAfterCursor()
    {
        IMSA_HILOGI("InputDataChannelProxy::GetTextAfterCursor");
        MessageParcel data, reply;
        MessageOption option;
        data.WriteInterfaceToken(GetDescriptor());

        Remote()->SendRequest(GET_TEXT_AFTER_CURSOR, data, reply, option);
        auto result = reply.ReadString16();
        return result;
    }

    void InputDataChannelProxy::SendKeyboardStatus(int32_t status)
    {
        IMSA_HILOGI("InputDataChannelProxy::SendKeyboardStatus");
        MessageParcel data, reply;
        MessageOption option;
        data.WriteInterfaceToken(GetDescriptor());
        data.WriteInt32(status);

        Remote()->SendRequest(SEND_KEYBOARD_STATUS, data, reply, option);
    }

    void InputDataChannelProxy::SendFunctionKey(int32_t funcKey)
    {
        IMSA_HILOGI("InputDataChannelProxy::SendFunctionKey");
        MessageParcel data, reply;
        MessageOption option;
        data.WriteInterfaceToken(GetDescriptor());
        data.WriteInt32(funcKey);

        Remote()->SendRequest(SEND_FUNCTION_KEY, data, reply, option);
    }

    void InputDataChannelProxy::MoveCursor(int32_t keyCode)
    {
        IMSA_HILOGI("InputDataChannelProxy::MoveCursor");

        MessageParcel data, reply;
        MessageOption option;
        data.WriteInterfaceToken(GetDescriptor());
        data.WriteInt32(keyCode);

        Remote()->SendRequest(MOVE_CURSOR, data, reply, option);
    }
}
}
}