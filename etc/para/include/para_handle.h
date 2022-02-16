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

#ifndef OHOS_MISCSERVICES_INPUTMETHOD_PARA_HANDLE_H
#define OHOS_MISCSERVICES_INPUTMETHOD_PARA_HANDLE_H
#include <string>
namespace OHOS {
    namespace MiscServices {
        class ParaHandle {
        public:
            ParaHandle() = default;
            virtual ~ParaHandle() = default;
            static bool SetDefaultIme(int32_t userId, const std::string &imeName);
            static std::string GetDefaultIme(int32_t userId);
        private:
            static const char *DEFAULT_IME_KEY;
            static const char *DEFAULT_IME_NAME;
            static constexpr int CONFIG_LEN = 128;
            static const int32_t main_userId = 100;
        };
    }
}
#endif // OHOS_MISCSERVICES_INPUTMETHOD_PARA_HANDLE_H
