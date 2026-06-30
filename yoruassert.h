#ifndef YORUASSERT_H
#define YORUASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

/* =========================================================
 *  Minimal Integer Types (no stdint dependency)
 * ========================================================= */

typedef
#if defined(__UINT8_TYPE__)
__UINT8_TYPE__
#else
unsigned char
#endif
yoruassert_u8_t;

typedef
#if defined(__UINT32_TYPE__)
__UINT32_TYPE__
#else
unsigned int
#endif
yoruassert_u32_t;

/* =========================================================
 *  User Configuration
 * ========================================================= */

#ifndef YORUASSERT_ENABLE
#define YORUASSERT_ENABLE 1
#endif

#ifndef YORUASSERT_ENABLE_MSG
#define YORUASSERT_ENABLE_MSG 1
#endif

#ifndef YORUASSERT_USE_YORULOG
#define YORUASSERT_USE_YORULOG 0
#endif

#if YORUASSERT_USE_YORULOG
  #ifndef YORUASSERT_YORULOG_HEADER
    #define YORUASSERT_YORULOG_HEADER "yorulog.h"
  #endif
  #include YORUASSERT_YORULOG_HEADER
#endif

#ifndef YORUASSERT_USE_FILE_LINE
#define YORUASSERT_USE_FILE_LINE 1
#endif

#ifndef YORUASSERT_DEFAULT_TRAP
#define YORUASSERT_DEFAULT_TRAP 1
#endif

#ifndef YORUASSERT_TRAP
#define YORUASSERT_TRAP() do { for (;;) {} } while (0)
#endif

/* =========================================================
 *  Type Definitions
 * ========================================================= */

typedef enum
{
    YORUASSERT_TYPE_ASSERT = 0,
    YORUASSERT_TYPE_PANIC = 1,
    YORUASSERT_TYPE_UNREACHABLE = 2
} YORUASSERT_TypeDef;

typedef struct
{
    YORUASSERT_TypeDef Type;
    const char *Expr;
    const char *File;
    const char *Msg;
    yoruassert_u32_t Line;
} YORUASSERT_InfoTypeDef;

typedef void (*YORUASSERT_HookTypeDef)(const YORUASSERT_InfoTypeDef *Info);

static inline const char *YORUASSERT_FileName(const char *Path);

/* =========================================================
 *  Global Hook Storage
 * =========================================================
 *  Define YORUASSERT_DEFINE_GLOBALS in exactly one .c file
 *  before including yoruassert.h.
 */

#if defined(YORUASSERT_DEFINE_GLOBALS)
YORUASSERT_HookTypeDef hYoruassertHook = (YORUASSERT_HookTypeDef)0;
#else
extern YORUASSERT_HookTypeDef hYoruassertHook;
#endif

/* =========================================================
 *  Internal Helpers
 * ========================================================= */

static inline void yoruassert__default_hook_(const YORUASSERT_InfoTypeDef *Info)
{
#if YORUASSERT_USE_YORULOG
    const char *file_name;

    if (Info != (const YORUASSERT_InfoTypeDef *)0)
    {
        file_name = YORUASSERT_FileName(Info->File);

        YORULOG_PrintRaw("[A] ");
        if (file_name != (const char *)0)
        {
            YORULOG_PrintRaw(file_name);
        }
        else
        {
            YORULOG_PrintRaw("(null)");
        }

        if (Info->Line != 0u)
        {
            YORULOG_PrintRaw(":");
            YORULOG_Print(Info->Line);
        }

        YORULOG_PrintRaw(" ");
        switch (Info->Type)
        {
            case YORUASSERT_TYPE_ASSERT:
                YORULOG_PrintRaw("assert");
                break;
            case YORUASSERT_TYPE_PANIC:
                YORULOG_PrintRaw("panic");
                break;
            case YORUASSERT_TYPE_UNREACHABLE:
                YORULOG_PrintRaw("unreachable");
                break;
            default:
                YORULOG_PrintRaw("unknown");
                break;
        }

        if (Info->Expr != (const char *)0)
        {
            YORULOG_PrintRaw(": ");
            YORULOG_PrintRaw(Info->Expr);
        }
        YORULOG_PrintRawln("");

#if YORUASSERT_ENABLE_MSG
        if (Info->Msg != (const char *)0)
        {
            YORULOG_PrintRaw("[A] msg: ");
            YORULOG_PrintRawln(Info->Msg);
        }
#endif
    }
#else
    (void)Info;
#endif
#if YORUASSERT_DEFAULT_TRAP
    YORUASSERT_TRAP();
#endif
}

static inline yoruassert_u32_t yoruassert__line_(yoruassert_u32_t line)
{
#if YORUASSERT_USE_FILE_LINE
    return line;
#else
    (void)line;
    return 0u;
#endif
}

static inline const char *yoruassert__msg_(const char *msg)
{
#if YORUASSERT_ENABLE_MSG
    return msg;
#else
    (void)msg;
    return (const char *)0;
#endif
}

#if YORUASSERT_USE_FILE_LINE
  #define YORUASSERT__FILE__ __FILE__
#else
  #define YORUASSERT__FILE__ ((const char *)0)
#endif

static inline const char *YORUASSERT_FileName(const char *Path)
{
    const char *name;

    if (Path == (const char *)0)
    {
        return (const char *)0;
    }

    name = Path;
    while (*Path != '\0')
    {
        if ((*Path == '/') || (*Path == '\\'))
        {
            name = Path + 1;
        }
        ++Path;
    }

    return name;
}

/* =========================================================
 *  Public API
 * ========================================================= */

static inline void YORUASSERT_Init(void)
{
    hYoruassertHook = (YORUASSERT_HookTypeDef)0;
}

static inline void YORUASSERT_SetHook(YORUASSERT_HookTypeDef Hook)
{
    hYoruassertHook = Hook;
}

static inline void YORUASSERT_Fail(YORUASSERT_TypeDef Type,
                                   const char *Expr,
                                   const char *File,
                                   yoruassert_u32_t Line,
                                   const char *Msg)
{
#if YORUASSERT_ENABLE
    YORUASSERT_InfoTypeDef info;
    YORUASSERT_HookTypeDef hook;

    info.Type = Type;
    info.Expr = Expr;
    info.File = File;
    info.Msg = Msg;
    info.Line = Line;

    hook = hYoruassertHook;
    if (hook != (YORUASSERT_HookTypeDef)0)
    {
        hook(&info);
        return;
    }

    yoruassert__default_hook_(&info);
#else
    (void)Type;
    (void)Expr;
    (void)File;
    (void)Line;
    (void)Msg;
#endif
}

/* =========================================================
 *  User Macros
 * ========================================================= */

#if YORUASSERT_ENABLE
  #define YORUASSERT(expr) \
    do { \
        if (!(expr)) { \
            YORUASSERT_Fail(YORUASSERT_TYPE_ASSERT, #expr, YORUASSERT__FILE__, yoruassert__line_((yoruassert_u32_t)__LINE__), (const char *)0); \
        } \
    } while (0)

  #define YORUASSERT_MSG(expr, msg) \
    do { \
        if (!(expr)) { \
            YORUASSERT_Fail(YORUASSERT_TYPE_ASSERT, #expr, YORUASSERT__FILE__, yoruassert__line_((yoruassert_u32_t)__LINE__), yoruassert__msg_(msg)); \
        } \
    } while (0)

  #define YORUASSERT_PANIC(msg) \
    do { \
        YORUASSERT_Fail(YORUASSERT_TYPE_PANIC, (const char *)0, YORUASSERT__FILE__, yoruassert__line_((yoruassert_u32_t)__LINE__), yoruassert__msg_(msg)); \
    } while (0)

  #define YORUASSERT_UNREACHABLE() \
    do { \
        YORUASSERT_Fail(YORUASSERT_TYPE_UNREACHABLE, (const char *)0, YORUASSERT__FILE__, yoruassert__line_((yoruassert_u32_t)__LINE__), (const char *)0); \
    } while (0)
#else
  #define YORUASSERT(expr)          do { (void)0; } while (0)
  #define YORUASSERT_MSG(expr, msg) do { (void)0; } while (0)
  #define YORUASSERT_PANIC(msg)     do { (void)0; } while (0)
  #define YORUASSERT_UNREACHABLE()  do { (void)0; } while (0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* YORUASSERT_H */
