#include <stdarg.h>
#include <stdlib.h>

#include <stdio.h>

#include <windows.h> // Windows only
#include <process.h> // Windows only
#include <conio.h> // Windows only

#include "../../includes/input/readChar.h"

#include "../../includes/types/input.h"

#include "../../includes/utils/sleep.h"

typedef struct _ReadCertainCharTaskResult {
  SustainedReadCertainCharResult* sustainedReadCertainCharResult;
  char* s;
  int length;
} _ReadCertainCharTaskResult;

_ReadCertainCharTaskResult _readCertainCharTaskResult;

LRESULT CALLBACK keyboardHandler (int nCode, WPARAM wParam, LPARAM lParam) {
  char c;
  if (nCode == HC_ACTION) {
    KBDLLHOOKSTRUCT* pKeyboardStruct = (KBDLLHOOKSTRUCT*)lParam;
    int keycode = pKeyboardStruct->vkCode;
    c = (keycode >= 'A' && keycode <= 'Z') ? keycode - 'A' + 'a' : keycode;
    if (wParam == WM_KEYDOWN) /* keydown */ {
      for (int i = 0; i < _readCertainCharTaskResult.length; i++) {
        if (c == *(_readCertainCharTaskResult.s + i)) {
          _readCertainCharTaskResult.sustainedReadCertainCharResult->character = c;
          if (
            !_readCertainCharTaskResult.sustainedReadCertainCharResult->isInputStart &&
            !_readCertainCharTaskResult.sustainedReadCertainCharResult->isInputEnd
          ) /* never input before, then input starts */ {
            _readCertainCharTaskResult.sustainedReadCertainCharResult->isInputStart = true;
          }
        }
      }
    } else if (
      wParam == WM_KEYUP && /* keyup */
      _readCertainCharTaskResult.sustainedReadCertainCharResult->isInputStart /* input is begun */
    ) /* then input ends */ {
      _readCertainCharTaskResult.sustainedReadCertainCharResult->isInputStart = false;
      _readCertainCharTaskResult.sustainedReadCertainCharResult->isInputEnd = true;
      setbuf(stdin, NULL);
    }
  }

  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

unsigned __stdcall _readCertainCharTask (void* ignore) {
  char c;

  while (!_readCertainCharTaskResult.sustainedReadCertainCharResult->isInputEnd) {
    HHOOK hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHandler, NULL, 0);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(hKeyboardHook);
  }

  return 0;
}

HANDLE _thread = NULL;
void endSustainedReadCertainChar ();

void startSustainedReadCertainChar (SustainedReadCertainCharResult* sustainedReadCertainCharResult, int arg, ...) {
  if (_thread != NULL) { endSustainedReadCertainChar(); }

  char* s = (char *)malloc(sizeof(char) * arg);
  int length = 0;
  va_list list;
	va_start(list, arg);
	while(arg > 0) {
		*(s + length) = va_arg(list, int);
    length++;
		arg--;
	}
	va_end(list);

  _readCertainCharTaskResult.length = length;
  _readCertainCharTaskResult.s = s;
  _readCertainCharTaskResult.sustainedReadCertainCharResult = sustainedReadCertainCharResult;

  unsigned int threadID;
  HANDLE thread = (HANDLE)_beginthreadex(NULL, 0, &_readCertainCharTask, NULL, 0, &threadID);
  _thread = thread;
}

void endSustainedReadCertainChar () {
  CloseHandle(_thread);
  free(_readCertainCharTaskResult.s);
  _thread = NULL;
}
