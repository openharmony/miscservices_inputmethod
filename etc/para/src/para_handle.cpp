/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
#include "para_handle.h"
#include "parameter.h"

namespace OHOS {
    namespace MiscServices {
        const char *ParaHandle::DEFAULT_IME_KEY = "persist.sys.default_ime";
        const char *ParaHandle::DEFAULT_IME_NAME = "com.example.kikakeyboard/ServiceExtAbility";
        bool ParaHandle::SetDefaultIme(int32_t userId, const std::string &imeName)
        {
            if (userId != main_userId) {
                return SetParameter(DEFAULT_IME_KEY + userId, imeName.data()) == 0;
            } else {
                return SetParameter(DEFAULT_IME_KEY, imeName.data()) == 0;
            }
        }

        std::string ParaHandle::GetDefaultIme(int32_t userId)
        {
            char value[CONFIG_LEN];
            int code = 0;
            if (userId != main_userId) {
                code = GetParameter(DEFAULT_IME_KEY + userId, "", value, CONFIG_LEN);
            } else {
                code = GetParameter(DEFAULT_IME_KEY, "", value, CONFIG_LEN);
            }
            if (code > 0) {
                return value;
            }

            SetDefaultIme(userId, DEFAULT_IME_NAME);
            return DEFAULT_IME_NAME;
        }
    }
}
