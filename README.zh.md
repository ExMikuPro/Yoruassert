# Yoruassert

Yoruassert 是一个面向 **STM32 风格嵌入式 C 项目** 的轻量级断言辅助库。

它的目标是尽量小、尽量直接：

- 单头文件
- 默认配置可直接使用
- 支持自定义失败 hook
- 支持可选文件 / 行号捕获
- 支持可选消息字符串
- 可选复用 Yorulog 作为输出路径

---

> **Yoru 系列**
>
> 一组面向 STM32 HAL 的轻量级工具库。各库可独立使用，也可以组合使用。
>
> | 库 | 定位 |
> | --- | --- |
> | [Yorulog](https://github.com/ExMikuPro/Yorulog) | 轻量级 UART 日志库 |
> | [Yorush](https://github.com/ExMikuPro/Yorush) | 轻量级 UART Shell / 命令解析器 |
> | [Yorunvm](https://github.com/ExMikuPro/Yorunvm) | STM32 片上 NVM / Flash / EEPROM 访问辅助库 |
> | [Yorukv](https://github.com/ExMikuPro/Yorukv) | 轻量级 KV 配置库 |
> | [Yorubench](https://github.com/ExMikuPro/Yorubench) | 轻量级性能测量库 |
> | [Yoruassert](https://github.com/ExMikuPro/Yoruassert.git) | 轻量级断言辅助库 |

---

## 当前能力

- 单头文件库
- 默认配置可直接包含
- 支持可选失败 hook
- 支持可选 Yorulog 输出
- 支持 `assert / panic / unreachable`
- 支持从 `__FILE__` 中提取短文件名

---

## 设计目标

- 不强制绑定某一种输出方式
- 不依赖 C 标准库
- 不强依赖 Yorulog
- 让失败路径保持可配置

---

## 最小示例

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

注意：

- `YORUASSERT_DEFINE_GLOBALS` 只能在一个 `.c` 文件里定义一次
- `YORUASSERT_Init()` 建议在启动阶段调用一次

---

## 使用自定义 Hook

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

失败发生时，hook 可以拿到：

- 失败类型
- 表达式文本
- 文件路径
- 行号
- 可选消息

---

## 使用 Yorulog 作为输出路径

```c
#define YORUASSERT_USE_YORULOG 1
#define YORUASSERT_YORULOG_HEADER "yorulog.h"
#define YORUASSERT_DEFINE_GLOBALS
#include "yoruassert.h"
```

启用后，默认失败路径可以通过 Yorulog 输出文本。

如果你只想使用自己的 hook 处理，保持：

```c
#define YORUASSERT_USE_YORULOG 0
```

---

## 配置宏

- `YORUASSERT_ENABLE`
  控制整个库是否启用。

- `YORUASSERT_ENABLE_MSG`
  控制是否保留消息字符串支持。

- `YORUASSERT_USE_YORULOG`
  控制是否启用基于 Yorulog 的默认输出。

- `YORUASSERT_YORULOG_HEADER`
  在启用联动时，覆盖 Yorulog 头文件路径。

- `YORUASSERT_USE_FILE_LINE`
  控制是否保留文件 / 行号捕获。

- `YORUASSERT_DEFAULT_TRAP`
  控制默认失败路径在上报后是否进入 trap。

- `YORUASSERT_TRAP()`
  覆盖默认 trap 行为。

---

## 当前 API

- `YORUASSERT_Init()`
- `YORUASSERT_SetHook()`
- `YORUASSERT_Fail()`
- `YORUASSERT_FileName()`
- `YORUASSERT()`
- `YORUASSERT_MSG()`
- `YORUASSERT_PANIC()`
- `YORUASSERT_UNREACHABLE()`

---

## 失败类型

- `YORUASSERT_TYPE_ASSERT`
- `YORUASSERT_TYPE_PANIC`
- `YORUASSERT_TYPE_UNREACHABLE`

---

## 典型使用场景

- 固件代码中的基础运行时检查
- 不可能分支里的 panic 上报
- 状态机中的 unreachable 标记
- 不绑定单一日志库的 hook 式故障上报
