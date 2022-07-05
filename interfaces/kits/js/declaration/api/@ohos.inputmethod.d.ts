/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

import {AsyncCallback} from './basic';

/**
 * inputmethod
 *
 * @since 8
 * @devices phone, tablet, tv, wearable
 */
declare namespace inputMethod {
    const MAX_TYPE_NUM: number

    function getInputMethodSetting(): InputMethodSetting;

    function getInputMethodController(): InputMethodController;

    /**
     * Switch input method
     * @since 9
     * @param target Indicates the input method which will replace the current one
     * @return -
     * @syscap SystemCapability.MiscServices.InputMethodFramework
     * @StageModelOnly
     */
     function switchInputMethod(target: InputMethodProperty): Promise<boolean>;

    interface InputMethodSetting {
        listInputMethod(callback: AsyncCallback<Array<InputMethodProperty>>): void;
        listInputMethod(): Promise<Array<InputMethodProperty>>;

        displayOptionalInputMethod(callback: AsyncCallback<void>): void;
        displayOptionalInputMethod(): Promise<void>;
    }

    interface InputMethodController {
        stopInput(callback: AsyncCallback<boolean>): void;
        stopInput(): Promise<boolean>;
    }

    interface InputMethodProperty {
        readonly packageName: string;
        readonly methodId: string;
    }
}

export default inputMethod;
