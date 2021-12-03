//
// Created by OpenHarmonyUser on 2021/11/30.
//

#ifndef FM_IMC_PROJECT_INPUT_METHOD_UTILS_H
#define FM_IMC_PROJECT_INPUT_METHOD_UTILS_H

#endif //FM_IMC_PROJECT_INPUT_METHOD_UTILS_H
namespace OHOS {
    namespace MiscServices {
        class Configuration{
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
        class CursorInfo{};
        class KeyEvent{};
        class KeyboardStatus{
        public:
            enum{
                KEYBOARD_STATUS_HIDE = 0,
                KEYBOARD_STATUS_SHOW
            };
            int mKeyboardStatus;
        };
    }
}