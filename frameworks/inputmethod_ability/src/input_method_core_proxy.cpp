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

#include "input_method_core_proxy.h"
#include "message_parcel.h"
#include "message_option.h"
#include "input_attribute.h"
#include <string_ex.h>

namespace OHOS {
namespace MiscServices {
    InputMethodCoreProxy::InputMethodCoreProxy(const OHOS::sptr<OHOS::IRemoteObject> &impl)
        : IRemoteProxy<IInputMethodCore>(impl)
    {
    }

    InputMethodCoreProxy::~InputMethodCoreProxy() = default;

    int32_t InputMethodCoreProxy::initializeInput(sptr<IRemoteObject> &startInputToken, int32_t displayId,
                                                  sptr<IInputControlChannel> &inputControlChannel)
    {
        IMSA_HILOGI("InputMethodCoreProxy::initializeInput");
        auto remote = Remote();
        if (!remote) {
            IMSA_HILOGI("remote is nullptr");
            return ErrorCode::ERROR_NULL_POINTER;
        }
        if (!startInputToken) {
            IMSA_HILOGI("InputMethodCoreProxy::initializeInput startInputToken is nullptr");
        }

        if (!inputControlChannel) {
            IMSA_HILOGI("InputMethodCoreProxy::initializeInput inputControlChannel is nullptr");
            return ErrorCode::ERROR_NULL_POINTER;
        }
        IMSA_HILOGI("InputMethodCoreProxy::initializeInput displayId = %{public}d", displayId);
        MessageParcel data, reply;
        data.WriteInterfaceToken(GetDescriptor());
        data.WriteInt32(displayId);
        sptr<IRemoteObject> channelObject = inputControlChannel->AsObject();
        if (!channelObject) {
            IMSA_HILOGI("InputMethodCoreProxy::initializeInput channelObject is nullptr");
            return ErrorCode::ERROR_NULL_POINTER;
        }
        bool wor = data.WriteRemoteObject(channelObject);
        if (wor) {
            IMSA_HILOGI("InputMethodCoreProxy::initializeInput Success to write inputControlChannel");
        } else {
            IMSA_HILOGI("InputMethodCoreProxy::initializeInput Failed to write inputControlChannel");
        }
        MessageOption option {
            MessageOption::TF_SYNC
        };
        int32_t status = remote->SendRequest(INITIALIZE_INPUT, data, reply, option);
        if (status != ErrorCode::NO_ERROR) {
            IMSA_HILOGI("InputMethodCoreProxy::initializeInput status = %{public}d", status);
            return status;
        }
        int32_t code = reply.ReadException();
        return code;
    }

    int32_t InputMethodCoreProxy::InitInputControlChannel(sptr<IInputControlChannel> &inputControlChannel)
    {
        IMSA_HILOGI("InputMethodCoreProxy::InitInputControlChannel");
        auto remote = Remote();
        if (!remote) {
            IMSA_HILOGI("remote is nullptr");
            return ErrorCode::ERROR_NULL_POINTER;
        }
        if (!inputControlChannel) {
            IMSA_HILOGI("InputMethodCoreProxy::InitInputControlChannel inputControlChannel is nullptr");
            return ErrorCode::ERROR_EX_NULL_POINTER;
        }
        MessageParcel data, reply;
        data.WriteInterfaceToken(GetDescriptor());
        sptr<IRemoteObject> channelObject = inputControlChannel->AsObject();
        if (!channelObject) {
            IMSA_HILOGI("InputMethodCoreProxy::InitInputControlChannel channelObject is nullptr");
            return ErrorCode::ERROR_EX_NULL_POINTER;
        }
        data.WriteRemoteObject(channelObject);
        MessageOption option {
            MessageOption::TF_SYNC
        };
        int32_t status = remote->SendRequest(INIT_INPUT_CONTROL_CHANNEL, data, reply, option);
        if (status != ErrorCode::NO_ERROR) {
            IMSA_HILOGI("InputMethodCoreProxy::InitInputControlChannel status = %{public}d", status);
            return status;
        }
        int32_t code = reply.ReadException();
        return code;
    }

    void InputMethodCoreProxy::SetClientState(bool state)
    {
        IMSA_HILOGI("InputMethodCoreProxy::SetClientState");
        auto remote = Remote();
        if (!remote) {
            IMSA_HILOGI("remote is nullptr");
            return;
        }
        MessageParcel data;
        if (!(data.WriteInterfaceToken(GetDescriptor())
            && data.WriteBool(state))) {
            IMSA_HILOGI("InputMethodCoreProxy::SetClientState write error");
            return;
        }
        MessageParcel reply;
        MessageOption option {
            MessageOption::TF_SYNC
        };

        int32_t status = remote->SendRequest(SET_CLIENT_STATE, data, reply, option);
        if (status != ErrorCode::NO_ERROR) {
            IMSA_HILOGI("InputMethodCoreProxy::SetClientState status = %{public}d", status);
        }
    }
    bool InputMethodCoreProxy::startInput(const sptr<IInputDataChannel> &inputDataChannel,
                                          const InputAttribute& editorAttribute, bool supportPhysicalKbd)
    {
        IMSA_HILOGI("InputMethodCoreProxy::startInput");
        auto remote = Remote();
        if (!remote) {
            IMSA_HILOGI("remote is nullptr");
            return false;
        }
        if (!inputDataChannel) {
            IMSA_HILOGI("InputMethodCoreProxy::startInput inputDataChannel is nullptr");
            return false;
        }

        MessageParcel data;
        if (!(data.WriteInterfaceToken(GetDescriptor())
            && data.WriteRemoteObject(inputDataChannel->AsObject())
            && data.WriteParcelable(&editorAttribute)
            && data.WriteBool(supportPhysicalKbd))) {
            IMSA_HILOGI("InputMethodCoreProxy::startInput write error");
            return false;
        }
        MessageParcel reply;
        MessageOption option {
            MessageOption::TF_SYNC
        };

        int32_t status = remote->SendRequest(START_INPUT, data, reply, option);
        if (status != ErrorCode::NO_ERROR) {
            IMSA_HILOGI("InputMethodCoreProxy::startInput status = %{public}d", status);
            return false;
        }

        int32_t code = reply.ReadException();
        return code == ErrorCode::NO_ERROR;
    }

    int32_t InputMethodCoreProxy::stopInput()
    {
        IMSA_HILOGI("InputMethodCoreProxy::stopInput");
        auto remote = Remote();
        if (!remote) {
            IMSA_HILOGI("remote is nullptr");
            return ErrorCode::ERROR_NULL_POINTER;
        }
        MessageParcel data, reply;
        data.WriteInterfaceToken(GetDescriptor());
        MessageOption option {
            MessageOption::TF_SYNC
        };
        int32_t status = remote->SendRequest(STOP_INPUT, data, reply, option);
        if (status != ErrorCode::NO_ERROR) {
            IMSA_HILOGI("InputMethodCoreProxy::stopInput status = %{public}d", status);
            return status;
        }
        int code = reply.ReadException();
        if (code != ErrorCode::NO_ERROR) {
            IMSA_HILOGI("InputMethodCoreProxy::stopInput code = %{public}d", code);
            return code;
        }
        return reply.ReadInt32();
    }

    bool InputMethodCoreProxy::showKeyboard(const sptr<IInputDataChannel> &inputDataChannel)
    {
        IMSA_HILOGI("InputMethodCoreProxy::showKeyboard");
        auto remote = Remote();
        if (!remote) {
            IMSA_HILOGI("InputMethodCoreProxy::showKeyboard remote is nullptr");
            return false;
        }

        MessageParcel data;
        if (!(data.WriteInterfaceToken(GetDescriptor())
            && data.WriteRemoteObject(inputDataChannel->AsObject()))) {
            return false;
        }
        MessageParcel reply;
        MessageOption option {
            MessageOption::TF_SYNC
        };

        int32_t res = remote->SendRequest(SHOW_KEYBOARD, data, reply, option);
        if (res != ErrorCode::NO_ERROR) {
            return false;
        }
        return true;
    }

    void InputMethodCoreProxy::StopInputService(std::string imeId)
    {
        IMSA_HILOGI("InputMethodCoreProxy::StopInputService");
        auto remote = Remote();
        if (!remote) {
            IMSA_HILOGI("InputMethodCoreProxy::StopInputService remote is nullptr");
            return;
        }
        MessageParcel data;
        if (!(data.WriteInterfaceToken(GetDescriptor())
            && data.WriteString16(Str8ToStr16(imeId)))) {
            return;
        }
        MessageParcel reply;
        MessageOption option {
            MessageOption::TF_SYNC
        };

        int32_t res = remote->SendRequest(STOP_INPUT_SERVICE, data, reply, option);
        if (res != ErrorCode::NO_ERROR) {
            return;
        }
    }

    bool InputMethodCoreProxy::hideKeyboard(int32_t flags)
    {
        IMSA_HILOGI("InputMethodCoreProxy::hideKeyboard");
        auto remote = Remote();
        if (!remote) {
            return false;
        }

        MessageParcel data;
        if (!(data.WriteInterfaceToken(GetDescriptor()) && data.WriteInt32(flags))) {
            return false;
        }
        MessageParcel reply;
        MessageOption option {
            MessageOption::TF_SYNC
        };

        int32_t res = remote->SendRequest(HIDE_KEYBOARD, data, reply, option);
        if (res != ErrorCode::NO_ERROR) {
            return false;
        }
        return true;
    }

    int32_t InputMethodCoreProxy::setKeyboardType(const KeyboardType& type)
    {
        IMSA_HILOGI("InputMethodCoreProxy::setKeyboardType");
        auto remote = Remote();
        if (!remote) {
            IMSA_HILOGI("remote is nullptr");
            return ErrorCode::ERROR_NULL_POINTER;
        }
        MessageParcel data, reply;
        data.WriteInterfaceToken(GetDescriptor());
        data.WriteParcelable(&type);
        MessageOption option {
            MessageOption::TF_SYNC
        };
        int32_t status = remote->SendRequest(SET_KEYBOARD_TYPE, data, reply, option);
        if (status != ErrorCode::NO_ERROR) {
            return status;
        }
        int32_t code = reply.ReadException();
        return code;
    }

    int32_t InputMethodCoreProxy::getKeyboardWindowHeight(int32_t &retHeight)
    {
        IMSA_HILOGI("InputMethodCoreProxy::getKeyboardWindowHeight");
        auto remote = Remote();
        if (!remote) {
            IMSA_HILOGI("remote is nullptr");
            return ErrorCode::ERROR_NULL_POINTER;
        }
        MessageParcel data, reply;
        data.WriteInterfaceToken(GetDescriptor());
        MessageOption option {
            MessageOption::TF_SYNC
        };
        int32_t status = remote->SendRequest(GET_KEYBOARD_WINDOW_HEIGHT, data, reply, option);
        if (status != ErrorCode::NO_ERROR) {
            return status;
        }
        int32_t code = reply.ReadException();
        if (code) {
            return code;
        }
        retHeight = reply.ReadInt32();
        return ErrorCode::NO_ERROR;
    }
} // namespace MiscServices
} // namespace OHOS
