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

#ifndef FM_IMC_PROJECT_INPUT_METHOD_UTILS_H
#define FM_IMC_PROJECT_INPUT_METHOD_UTILS_H

namespace OHOS {
    namespace MiscServices {
        class Configuration {
        public:
            enum {
                TEXT_FIELD_ACTION_UNSPECIFIED = 0,
                TEXT_FIELD_ACTION_NONE,
                TEXT_FIELD_ACTION_GO,
                TEXT_FIELD_ACTION_SEARCH,
                TEXT_FIELD_ACTION_SEND,
                TEXT_FIELD_ACTION_NEXT,
                TEXT_FIELD_ACTION_DONE,
                TEXT_FIELD_ACTION_PREVIOUS
            };
            int mTextFieldAction;
        };
        class CursorInfo {
        };
        class KeyEvent {
        };
        class KeyboardStatus {
        public:
            enum {
                KEYBOARD_STATUS_HIDE = 0,
                KEYBOARD_STATUS_SHOW
            };
            int mKeyboardStatus;
        };
    }
}
#endif // FM_IMC_PROJECT_INPUT_METHOD_UTILS_H