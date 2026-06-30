# Yoruassert

Yoruassert is a lightweight assertion helper for **STM32-style embedded C projects**.

It is designed to stay small and direct:

- single-header
- usable with default configuration
- supports custom failure hook
- supports optional file / line capture
- supports optional message string
- can optionally reuse Yorulog as the output path

---

> **Yoru Series**
>
> A family of lightweight utility libraries for STM32 HAL. Each library can be used independently or combined as needed.
>
> | Library | Role |
> | --- | --- |
> | [Yorulog](https://github.com/ExMikuPro/Yorulog) | Lightweight UART logger |
> | [Yorush](https://github.com/ExMikuPro/Yorush) | Lightweight UART shell / command parser |
> | [Yorunvm](https://github.com/ExMikuPro/Yorunvm) | STM32 on-chip NVM / Flash / EEPROM access helper |
> | [Yorukv](https://github.com/ExMikuPro/Yorukv) | Lightweight KV configuration library |
> | [Yorubench](https://github.com/ExMikuPro/Yorubench) | Lightweight performance measurement library |
> | [Yoruassert](https://github.com/ExMikuPro/Yoruassert.git) | Lightweight assertion helper |

---

## Current Scope

- single-header library
- default config can be used directly
- optional failure hook
- optional Yorulog output
- supports `assert / panic / unreachable`
- supports extracting short filename from `__FILE__`

---

## Design Goals

- does not force one output path
- does not require the C standard library
- does not require Yorulog
- keeps the failure path configurable

---

## Minimal Example

```c
#define YORUASSERT_DEFINE_GLOBALS
#include "yoruassert.h"

int main(void)
{
    YORUASSERT_Init();

    YORUASSERT(1 == 1);
    YORUASSERT_MSG(2 + 2 == 4, "math ok");

    return 0;
}
```

Note:

- define `YORUASSERT_DEFINE_GLOBALS` in exactly one `.c` file
- call `YORUASSERT_Init()` once during startup

---

## Using a Custom Hook

```c
#define YORUASSERT_DEFAULT_TRAP 0
#define YORUASSERT_DEFINE_GLOBALS
#include "yoruassert.h"

static void app_assert_hook(const YORUASSERT_InfoTypeDef *info)
{
    (void)info;
    /* user handling */
}

int main(void)
{
    YORUASSERT_Init();
    YORUASSERT_SetHook(app_assert_hook);

    YORUASSERT_MSG(0, "demo fail");
    return 0;
}
```

When a failure happens, the hook receives:

- failure type
- expression text
- file path
- line number
- optional message

---

## Using Yorulog as the Output Path

```c
#define YORUASSERT_USE_YORULOG 1
#define YORUASSERT_YORULOG_HEADER "yorulog.h"
#define YORUASSERT_DEFINE_GLOBALS
#include "yoruassert.h"
```

With this mode enabled, the default failure path can print text through Yorulog.

If you only want your own hook handling, keep:

```c
#define YORUASSERT_USE_YORULOG 0
```

---

## Configuration Macros

- `YORUASSERT_ENABLE`
  Turn the whole library on or off.

- `YORUASSERT_ENABLE_MSG`
  Enable or remove message string support.

- `YORUASSERT_USE_YORULOG`
  Enable optional Yorulog-based default output.

- `YORUASSERT_YORULOG_HEADER`
  Override the Yorulog header path when integration is enabled.

- `YORUASSERT_USE_FILE_LINE`
  Enable or remove file / line capture.

- `YORUASSERT_DEFAULT_TRAP`
  Control whether the default failure path enters trap after reporting.

- `YORUASSERT_TRAP()`
  Override the default trap behavior.

---

## Current API

- `YORUASSERT_Init()`
- `YORUASSERT_SetHook()`
- `YORUASSERT_Fail()`
- `YORUASSERT_FileName()`
- `YORUASSERT()`
- `YORUASSERT_MSG()`
- `YORUASSERT_PANIC()`
- `YORUASSERT_UNREACHABLE()`

---

## Failure Types

- `YORUASSERT_TYPE_ASSERT`
- `YORUASSERT_TYPE_PANIC`
- `YORUASSERT_TYPE_UNREACHABLE`

---

## Typical Use Cases

- basic runtime checks in firmware code
- panic reporting in impossible branches
- unreachable markers in state machines
- hook-based fault reporting without binding to one logger

