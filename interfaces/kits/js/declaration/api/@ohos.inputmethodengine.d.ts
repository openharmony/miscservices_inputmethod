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

import { AsyncCallback } from './basic';

/**
 * inputmethod
 *
 * @since 7
 * @devices phone, tablet, tv, wearable
 */
declare namespace inputMethodEngine {
  function on(type: 'keyboardShow', callback: () => void): void;
  function off(type: 'keyboardShow', callback: () => void): void;

  function on(type: 'keyboardHide', callback: () => void): void;
  function off(type: 'keyboardHide', callback: () => void): void;

  function InsertText(text: string, callback: AsyncCallback<boolean>): void;
  function InsertText(text: string): Promise<boolean>;
  
  function DeleteForward(length: number, callback: () => void): void;
  function DeleteForward(length: number): Promise<void>;

  function DeleteBackward(length: number, callback: () => void): void;
  function DeleteBackward(length: number): Promise<void>;

  function HideKeyboardSelf(callback: () => void): void;
  function HideKeyboardSelf(): Promise<void>;

  function GetTextBeforeCursor(callback: () => string): void;
  function GetTextBeforeCursor(): Promise<string>;

  function GetTextAfterCursor(callback: () => string): void;
  function GetTextAfterCursor(): Promise<string>;

  function SendFunctionKey(callback: () => void): void;
  function SendFunctionKey(): Promise<void>;

  const FUNCTION_KEY_CONFIRM: number;
}

export default inputMethodEngine;
