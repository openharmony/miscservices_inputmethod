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
        const char *ParaHandle::DEFAULT_IME = "persist.sys.default_ime";
        bool ParaHandle::SetDefaultIme(const std::string &imeName)
        {
            return SetParameter(DEFAULT_IME, imeName.data()) == 0;
        }

        std::string ParaHandle::GetDefaultIme()
        {
            char value[CONFIG_LEN];
            int code = GetParameter(DEFAULT_IME, "", value, CONFIG_LEN);
            if (code > 0) {
                return value;
            }
            return "";
        }
    }
}
