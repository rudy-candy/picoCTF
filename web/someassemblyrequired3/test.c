#ifndef WASM_H_GENERATED_
#define WASM_H_GENERATED_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "wasm-rt.h"

#ifndef WASM_RT_MODULE_PREFIX
#define WASM_RT_MODULE_PREFIX
#endif

#define WASM_RT_PASTE_(x, y) x ## y
#define WASM_RT_PASTE(x, y) WASM_RT_PASTE_(x, y)
#define WASM_RT_ADD_PREFIX(x) WASM_RT_PASTE(WASM_RT_MODULE_PREFIX, x)

/* TODO(binji): only use stdint.h types in header */
typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;
typedef float f32;
typedef double f64;

extern void WASM_RT_ADD_PREFIX(init)(void);

/* export: 'memory' */
extern wasm_rt_memory_t (*WASM_RT_ADD_PREFIX(Z_memory));
/* export: '__wasm_call_ctors' */
extern void (*WASM_RT_ADD_PREFIX(Z___wasm_call_ctorsZ_vv))(void);
/* export: 'strcmp' */
extern u32 (*WASM_RT_ADD_PREFIX(Z_strcmpZ_iii))(u32, u32);
/* export: 'check_flag' */
extern u32 (*WASM_RT_ADD_PREFIX(Z_check_flagZ_iv))(void);
/* export: 'input' */
extern u32 (*WASM_RT_ADD_PREFIX(Z_inputZ_i));
/* export: 'copy_char' */
extern void (*WASM_RT_ADD_PREFIX(Z_copy_charZ_vii))(u32, u32);
/* export: 'key' */
extern u32 (*WASM_RT_ADD_PREFIX(Z_keyZ_i));
/* export: '__dso_handle' */
extern u32 (*WASM_RT_ADD_PREFIX(Z___dso_handleZ_i));
/* export: '__data_end' */
extern u32 (*WASM_RT_ADD_PREFIX(Z___data_endZ_i));
/* export: '__global_base' */
extern u32 (*WASM_RT_ADD_PREFIX(Z___global_baseZ_i));
/* export: '__heap_base' */
extern u32 (*WASM_RT_ADD_PREFIX(Z___heap_baseZ_i));
/* export: '__memory_base' */
extern u32 (*WASM_RT_ADD_PREFIX(Z___memory_baseZ_i));
/* export: '__table_base' */
extern u32 (*WASM_RT_ADD_PREFIX(Z___table_baseZ_i));
#ifdef __cplusplus
}
#endif

#endif  /* WASM_H_GENERATED_ */
#include <math.h>
#include <string.h>

#include "wasm.h"
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#define LIKELY(x) __builtin_expect(!!(x), 1)

#define TRAP(x) (wasm_rt_trap(WASM_RT_TRAP_##x), 0)

#define FUNC_PROLOGUE                                            \
  if (++wasm_rt_call_stack_depth > WASM_RT_MAX_CALL_STACK_DEPTH) \
    TRAP(EXHAUSTION)

#define FUNC_EPILOGUE --wasm_rt_call_stack_depth

#define UNREACHABLE TRAP(UNREACHABLE)

#define CALL_INDIRECT(table, t, ft, x, ...)          \
  (LIKELY((x) < table.size && table.data[x].func &&  \
          table.data[x].func_type == func_types[ft]) \
       ? ((t)table.data[x].func)(__VA_ARGS__)        \
       : TRAP(CALL_INDIRECT))

#define MEMCHECK(mem, a, t)  \
  if (UNLIKELY((a) + sizeof(t) > mem->size)) TRAP(OOB)

#define DEFINE_LOAD(name, t1, t2, t3)              \
  static inline t3 name(wasm_rt_memory_t* mem, u64 addr) {   \
    MEMCHECK(mem, addr, t1);                       \
    t1 result;                                     \
    memcpy(&result, &mem->data[addr], sizeof(t1)); \
    return (t3)(t2)result;                         \
  }

#define DEFINE_STORE(name, t1, t2)                           \
  static inline void name(wasm_rt_memory_t* mem, u64 addr, t2 value) { \
    MEMCHECK(mem, addr, t1);                                 \
    t1 wrapped = (t1)value;                                  \
    memcpy(&mem->data[addr], &wrapped, sizeof(t1));          \
  }

DEFINE_LOAD(i32_load, u32, u32, u32);
DEFINE_LOAD(i64_load, u64, u64, u64);
DEFINE_LOAD(f32_load, f32, f32, f32);
DEFINE_LOAD(f64_load, f64, f64, f64);
DEFINE_LOAD(i32_load8_s, s8, s32, u32);
DEFINE_LOAD(i64_load8_s, s8, s64, u64);
DEFINE_LOAD(i32_load8_u, u8, u32, u32);
DEFINE_LOAD(i64_load8_u, u8, u64, u64);
DEFINE_LOAD(i32_load16_s, s16, s32, u32);
DEFINE_LOAD(i64_load16_s, s16, s64, u64);
DEFINE_LOAD(i32_load16_u, u16, u32, u32);
DEFINE_LOAD(i64_load16_u, u16, u64, u64);
DEFINE_LOAD(i64_load32_s, s32, s64, u64);
DEFINE_LOAD(i64_load32_u, u32, u64, u64);
DEFINE_STORE(i32_store, u32, u32);
DEFINE_STORE(i64_store, u64, u64);
DEFINE_STORE(f32_store, f32, f32);
DEFINE_STORE(f64_store, f64, f64);
DEFINE_STORE(i32_store8, u8, u32);
DEFINE_STORE(i32_store16, u16, u32);
DEFINE_STORE(i64_store8, u8, u64);
DEFINE_STORE(i64_store16, u16, u64);
DEFINE_STORE(i64_store32, u32, u64);

#define I32_CLZ(x) ((x) ? __builtin_clz(x) : 32)
#define I64_CLZ(x) ((x) ? __builtin_clzll(x) : 64)
#define I32_CTZ(x) ((x) ? __builtin_ctz(x) : 32)
#define I64_CTZ(x) ((x) ? __builtin_ctzll(x) : 64)
#define I32_POPCNT(x) (__builtin_popcount(x))
#define I64_POPCNT(x) (__builtin_popcountll(x))

#define DIV_S(ut, min, x, y)                                 \
   ((UNLIKELY((y) == 0)) ?                TRAP(DIV_BY_ZERO)  \
  : (UNLIKELY((x) == min && (y) == -1)) ? TRAP(INT_OVERFLOW) \
  : (ut)((x) / (y)))

#define REM_S(ut, min, x, y)                                \
   ((UNLIKELY((y) == 0)) ?                TRAP(DIV_BY_ZERO) \
  : (UNLIKELY((x) == min && (y) == -1)) ? 0                 \
  : (ut)((x) % (y)))

#define I32_DIV_S(x, y) DIV_S(u32, INT32_MIN, (s32)x, (s32)y)
#define I64_DIV_S(x, y) DIV_S(u64, INT64_MIN, (s64)x, (s64)y)
#define I32_REM_S(x, y) REM_S(u32, INT32_MIN, (s32)x, (s32)y)
#define I64_REM_S(x, y) REM_S(u64, INT64_MIN, (s64)x, (s64)y)

#define DIVREM_U(op, x, y) \
  ((UNLIKELY((y) == 0)) ? TRAP(DIV_BY_ZERO) : ((x) op (y)))

#define DIV_U(x, y) DIVREM_U(/, x, y)
#define REM_U(x, y) DIVREM_U(%, x, y)

#define ROTL(x, y, mask) \
  (((x) << ((y) & (mask))) | ((x) >> (((mask) - (y) + 1) & (mask))))
#define ROTR(x, y, mask) \
  (((x) >> ((y) & (mask))) | ((x) << (((mask) - (y) + 1) & (mask))))

#define I32_ROTL(x, y) ROTL(x, y, 31)
#define I64_ROTL(x, y) ROTL(x, y, 63)
#define I32_ROTR(x, y) ROTR(x, y, 31)
#define I64_ROTR(x, y) ROTR(x, y, 63)

#define FMIN(x, y)                                          \
   ((UNLIKELY((x) != (x))) ? NAN                            \
  : (UNLIKELY((y) != (y))) ? NAN                            \
  : (UNLIKELY((x) == 0 && (y) == 0)) ? (signbit(x) ? x : y) \
  : (x < y) ? x : y)

#define FMAX(x, y)                                          \
   ((UNLIKELY((x) != (x))) ? NAN                            \
  : (UNLIKELY((y) != (y))) ? NAN                            \
  : (UNLIKELY((x) == 0 && (y) == 0)) ? (signbit(x) ? y : x) \
  : (x > y) ? x : y)

#define TRUNC_S(ut, st, ft, min, max, maxop, x)                             \
   ((UNLIKELY((x) != (x))) ? TRAP(INVALID_CONVERSION)                       \
  : (UNLIKELY((x) < (ft)(min) || (x) maxop (ft)(max))) ? TRAP(INT_OVERFLOW) \
  : (ut)(st)(x))

#define I32_TRUNC_S_F32(x) TRUNC_S(u32, s32, f32, INT32_MIN, INT32_MAX, >=, x)
#define I64_TRUNC_S_F32(x) TRUNC_S(u64, s64, f32, INT64_MIN, INT64_MAX, >=, x)
#define I32_TRUNC_S_F64(x) TRUNC_S(u32, s32, f64, INT32_MIN, INT32_MAX, >,  x)
#define I64_TRUNC_S_F64(x) TRUNC_S(u64, s64, f64, INT64_MIN, INT64_MAX, >=, x)

#define TRUNC_U(ut, ft, max, maxop, x)                                    \
   ((UNLIKELY((x) != (x))) ? TRAP(INVALID_CONVERSION)                     \
  : (UNLIKELY((x) <= (ft)-1 || (x) maxop (ft)(max))) ? TRAP(INT_OVERFLOW) \
  : (ut)(x))

#define I32_TRUNC_U_F32(x) TRUNC_U(u32, f32, UINT32_MAX, >=, x)
#define I64_TRUNC_U_F32(x) TRUNC_U(u64, f32, UINT64_MAX, >=, x)
#define I32_TRUNC_U_F64(x) TRUNC_U(u32, f64, UINT32_MAX, >,  x)
#define I64_TRUNC_U_F64(x) TRUNC_U(u64, f64, UINT64_MAX, >=, x)

#define DEFINE_REINTERPRET(name, t1, t2)  \
  static inline t2 name(t1 x) {           \
    t2 result;                            \
    memcpy(&result, &x, sizeof(result));  \
    return result;                        \
  }

DEFINE_REINTERPRET(f32_reinterpret_i32, u32, f32)
DEFINE_REINTERPRET(i32_reinterpret_f32, f32, u32)
DEFINE_REINTERPRET(f64_reinterpret_i64, u64, f64)
DEFINE_REINTERPRET(i64_reinterpret_f64, f64, u64)


static u32 func_types[4];

static void init_func_types(void) {
  func_types[0] = wasm_rt_register_func_type(0, 0);
  func_types[1] = wasm_rt_register_func_type(2, 1, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32);
  func_types[2] = wasm_rt_register_func_type(0, 1, WASM_RT_I32);
  func_types[3] = wasm_rt_register_func_type(2, 0, WASM_RT_I32, WASM_RT_I32);
}

static void __wasm_call_ctors(void);
static u32 strcmp_0(u32, u32);
static u32 check_flag(void);
static void copy_char(u32, u32);

static u32 g0;
static u32 input;
static u32 key;
static u32 __dso_handle;
static u32 __data_end;
static u32 __global_base;
static u32 __heap_base;
static u32 __memory_base;
static u32 __table_base;

static void init_globals(void) {
  g0 = 66864u;
  input = 1072u;
  key = 1067u;
  __dso_handle = 1024u;
  __data_end = 1328u;
  __global_base = 1024u;
  __heap_base = 66864u;
  __memory_base = 0u;
  __table_base = 1u;
}

static wasm_rt_memory_t memory;

static wasm_rt_table_t T0;

static void __wasm_call_ctors(void) {
  FUNC_PROLOGUE;
  FUNC_EPILOGUE;
}

static u32 strcmp_0(u32 p0, u32 p1) {
  u32 l2 = 0, l3 = 0, l4 = 0, l5 = 0, l6 = 0, l7 = 0, l8 = 0, l9 = 0, 
      l10 = 0, l11 = 0, l12 = 0, l13 = 0, l14 = 0, l15 = 0, l16 = 0, l17 = 0, 
      l18 = 0, l19 = 0, l20 = 0, l21 = 0, l22 = 0, l23 = 0, l24 = 0, l25 = 0, 
      l26 = 0, l27 = 0, l28 = 0, l29 = 0, l30 = 0, l31 = 0, l32 = 0, l33 = 0, 
      l34 = 0, l35 = 0, l36 = 0, l37 = 0, l38 = 0, l39 = 0, l40 = 0, l41 = 0, 
      l42 = 0, l43 = 0;
  FUNC_PROLOGUE;
  u32 i0, i1;
  i0 = g0;
  l2 = i0;
  i0 = 32u;
  l3 = i0;
  i0 = l2;
  i1 = l3;
  i0 -= i1;
  l4 = i0;
  i0 = l4;
  i1 = p0;
  i32_store((&memory), (u64)(i0 + 24), i1);
  i0 = l4;
  i1 = p1;
  i32_store((&memory), (u64)(i0 + 20), i1);
  i0 = l4;
  i0 = i32_load((&memory), (u64)(i0 + 24));
  l5 = i0;
  i0 = l4;
  i1 = l5;
  i32_store((&memory), (u64)(i0 + 16), i1);
  i0 = l4;
  i0 = i32_load((&memory), (u64)(i0 + 20));
  l6 = i0;
  i0 = l4;
  i1 = l6;
  i32_store((&memory), (u64)(i0 + 12), i1);
  L1: 
    i0 = l4;
    i0 = i32_load((&memory), (u64)(i0 + 16));
    l7 = i0;
    i0 = 1u;
    l8 = i0;
    i0 = l7;
    i1 = l8;
    i0 += i1;
    l9 = i0;
    i0 = l4;
    i1 = l9;
    i32_store((&memory), (u64)(i0 + 16), i1);
    i0 = l7;
    i0 = i32_load8_u((&memory), (u64)(i0));
    l10 = i0;
    i0 = l4;
    i1 = l10;
    i32_store8((&memory), (u64)(i0 + 11), i1);
    i0 = l4;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l11 = i0;
    i0 = 1u;
    l12 = i0;
    i0 = l11;
    i1 = l12;
    i0 += i1;
    l13 = i0;
    i0 = l4;
    i1 = l13;
    i32_store((&memory), (u64)(i0 + 12), i1);
    i0 = l11;
    i0 = i32_load8_u((&memory), (u64)(i0));
    l14 = i0;
    i0 = l4;
    i1 = l14;
    i32_store8((&memory), (u64)(i0 + 10), i1);
    i0 = l4;
    i0 = i32_load8_u((&memory), (u64)(i0 + 11));
    l15 = i0;
    i0 = 255u;
    l16 = i0;
    i0 = l15;
    i1 = l16;
    i0 &= i1;
    l17 = i0;
    i0 = l17;
    if (i0) {goto B2;}
    i0 = l4;
    i0 = i32_load8_u((&memory), (u64)(i0 + 11));
    l18 = i0;
    i0 = 255u;
    l19 = i0;
    i0 = l18;
    i1 = l19;
    i0 &= i1;
    l20 = i0;
    i0 = l4;
    i0 = i32_load8_u((&memory), (u64)(i0 + 10));
    l21 = i0;
    i0 = 255u;
    l22 = i0;
    i0 = l21;
    i1 = l22;
    i0 &= i1;
    l23 = i0;
    i0 = l20;
    i1 = l23;
    i0 -= i1;
    l24 = i0;
    i0 = l4;
    i1 = l24;
    i32_store((&memory), (u64)(i0 + 28), i1);
    goto B0;
    B2:;
    i0 = l4;
    i0 = i32_load8_u((&memory), (u64)(i0 + 11));
    l25 = i0;
    i0 = 255u;
    l26 = i0;
    i0 = l25;
    i1 = l26;
    i0 &= i1;
    l27 = i0;
    i0 = l4;
    i0 = i32_load8_u((&memory), (u64)(i0 + 10));
    l28 = i0;
    i0 = 255u;
    l29 = i0;
    i0 = l28;
    i1 = l29;
    i0 &= i1;
    l30 = i0;
    i0 = l27;
    l31 = i0;
    i0 = l30;
    l32 = i0;
    i0 = l31;
    i1 = l32;
    i0 = i0 == i1;
    l33 = i0;
    i0 = 1u;
    l34 = i0;
    i0 = l33;
    i1 = l34;
    i0 &= i1;
    l35 = i0;
    i0 = l35;
    if (i0) {goto L1;}
  i0 = l4;
  i0 = i32_load8_u((&memory), (u64)(i0 + 11));
  l36 = i0;
  i0 = 255u;
  l37 = i0;
  i0 = l36;
  i1 = l37;
  i0 &= i1;
  l38 = i0;
  i0 = l4;
  i0 = i32_load8_u((&memory), (u64)(i0 + 10));
  l39 = i0;
  i0 = 255u;
  l40 = i0;
  i0 = l39;
  i1 = l40;
  i0 &= i1;
  l41 = i0;
  i0 = l38;
  i1 = l41;
  i0 -= i1;
  l42 = i0;
  i0 = l4;
  i1 = l42;
  i32_store((&memory), (u64)(i0 + 28), i1);
  B0:;
  i0 = l4;
  i0 = i32_load((&memory), (u64)(i0 + 28));
  l43 = i0;
  i0 = l43;
  goto Bfunc;
  Bfunc:;
  FUNC_EPILOGUE;
  return i0;
}

static u32 check_flag(void) {
  u32 l0 = 0, l1 = 0, l2 = 0, l3 = 0, l4 = 0, l5 = 0, l6 = 0, l7 = 0, 
      l8 = 0, l9 = 0, l10 = 0;
  FUNC_PROLOGUE;
  u32 i0, i1;
  i0 = 0u;
  l0 = i0;
  i0 = 1072u;
  l1 = i0;
  i0 = 1024u;
  l2 = i0;
  i0 = l2;
  i1 = l1;
  i0 = strcmp_0(i0, i1);
  l3 = i0;
  i0 = l3;
  l4 = i0;
  i0 = l0;
  l5 = i0;
  i0 = l4;
  i1 = l5;
  i0 = i0 != i1;
  l6 = i0;
  i0 = 4294967295u;
  l7 = i0;
  i0 = l6;
  i1 = l7;
  i0 ^= i1;
  l8 = i0;
  i0 = 1u;
  l9 = i0;
  i0 = l8;
  i1 = l9;
  i0 &= i1;
  l10 = i0;
  i0 = l10;
  goto Bfunc;
  Bfunc:;
  FUNC_EPILOGUE;
  return i0;
}

static void copy_char(u32 p0, u32 p1) {
  u32 l2 = 0, l3 = 0, l4 = 0, l5 = 0, l6 = 0, l7 = 0, l8 = 0, l9 = 0, 
      l10 = 0, l11 = 0, l12 = 0, l13 = 0, l14 = 0, l15 = 0, l16 = 0, l17 = 0, 
      l18 = 0;
  FUNC_PROLOGUE;
  u32 i0, i1;
  i0 = g0;
  l2 = i0;
  i0 = 16u;
  l3 = i0;
  i0 = l2;
  i1 = l3;
  i0 -= i1;
  l4 = i0;
  i0 = l4;
  i1 = p0;
  i32_store((&memory), (u64)(i0 + 12), i1);
  i0 = l4;
  i1 = p1;
  i32_store((&memory), (u64)(i0 + 8), i1);
  i0 = l4;
  i0 = i32_load((&memory), (u64)(i0 + 12));
  l5 = i0;
  i0 = l5;
  i0 = !(i0);
  if (i0) {goto B0;}
  i0 = 4u;
  l6 = i0;
  i0 = l4;
  i0 = i32_load((&memory), (u64)(i0 + 8));
  l7 = i0;
  i0 = 5u;
  l8 = i0;
  i0 = l7;
  i1 = l8;
  i0 = I32_REM_S(i0, i1);
  l9 = i0;
  i0 = l6;
  i1 = l9;
  i0 -= i1;
  l10 = i0;
  i0 = l10;
  i0 = i32_load8_u((&memory), (u64)(i0 + 1067));
  l11 = i0;
  i0 = 24u;
  l12 = i0;
  i0 = l11;
  i1 = l12;
  i0 <<= (i1 & 31);
  l13 = i0;
  i0 = l13;
  i1 = l12;
  i0 = (u32)((s32)i0 >> (i1 & 31));
  l14 = i0;
  i0 = l4;
  i0 = i32_load((&memory), (u64)(i0 + 12));
  l15 = i0;
  i0 = l15;
  i1 = l14;
  i0 ^= i1;
  l16 = i0;
  i0 = l4;
  i1 = l16;
  i32_store((&memory), (u64)(i0 + 12), i1);
  B0:;
  i0 = l4;
  i0 = i32_load((&memory), (u64)(i0 + 12));
  l17 = i0;
  i0 = l4;
  i0 = i32_load((&memory), (u64)(i0 + 8));
  l18 = i0;
  i0 = l18;
  i1 = l17;
  i32_store8((&memory), (u64)(i0 + 1072), i1);
  goto Bfunc;
  Bfunc:;
  FUNC_EPILOGUE;
}

static const u8 data_segment_data_0[] = {
  0x9d, 0x6e, 0x93, 0xc8, 0xb2, 0xb9, 0x41, 0x8b, 0x90, 0xc2, 0xdd, 0x63, 
  0x93, 0x93, 0x92, 0x8f, 0x64, 0x92, 0x9f, 0x94, 0xd5, 0x62, 0x91, 0xc5, 
  0xc0, 0x8e, 0x66, 0xc4, 0x97, 0xc0, 0x8f, 0x31, 0xc1, 0x90, 0xc4, 0x8b, 
  0x61, 0xc2, 0x94, 0xc9, 0x90, 0x00, 0x00, 
};

static const u8 data_segment_data_1[] = {
  0xf1, 0xa7, 0xf0, 0x07, 0xed, 
};

static void init_memory(void) {
  wasm_rt_allocate_memory((&memory), 2, 65536);
  memcpy(&(memory.data[1024u]), data_segment_data_0, 43);
  memcpy(&(memory.data[1067u]), data_segment_data_1, 5);
}

static void init_table(void) {
  uint32_t offset;
  wasm_rt_allocate_table((&T0), 1, 1);
}

/* export: 'memory' */
wasm_rt_memory_t (*WASM_RT_ADD_PREFIX(Z_memory));
/* export: '__wasm_call_ctors' */
void (*WASM_RT_ADD_PREFIX(Z___wasm_call_ctorsZ_vv))(void);
/* export: 'strcmp' */
u32 (*WASM_RT_ADD_PREFIX(Z_strcmpZ_iii))(u32, u32);
/* export: 'check_flag' */
u32 (*WASM_RT_ADD_PREFIX(Z_check_flagZ_iv))(void);
/* export: 'input' */
u32 (*WASM_RT_ADD_PREFIX(Z_inputZ_i));
/* export: 'copy_char' */
void (*WASM_RT_ADD_PREFIX(Z_copy_charZ_vii))(u32, u32);
/* export: 'key' */
u32 (*WASM_RT_ADD_PREFIX(Z_keyZ_i));
/* export: '__dso_handle' */
u32 (*WASM_RT_ADD_PREFIX(Z___dso_handleZ_i));
/* export: '__data_end' */
u32 (*WASM_RT_ADD_PREFIX(Z___data_endZ_i));
/* export: '__global_base' */
u32 (*WASM_RT_ADD_PREFIX(Z___global_baseZ_i));
/* export: '__heap_base' */
u32 (*WASM_RT_ADD_PREFIX(Z___heap_baseZ_i));
/* export: '__memory_base' */
u32 (*WASM_RT_ADD_PREFIX(Z___memory_baseZ_i));
/* export: '__table_base' */
u32 (*WASM_RT_ADD_PREFIX(Z___table_baseZ_i));

static void init_exports(void) {
  /* export: 'memory' */
  WASM_RT_ADD_PREFIX(Z_memory) = (&memory);
  /* export: '__wasm_call_ctors' */
  WASM_RT_ADD_PREFIX(Z___wasm_call_ctorsZ_vv) = (&__wasm_call_ctors);
  /* export: 'strcmp' */
  WASM_RT_ADD_PREFIX(Z_strcmpZ_iii) = (&strcmp_0);
  /* export: 'check_flag' */
  WASM_RT_ADD_PREFIX(Z_check_flagZ_iv) = (&check_flag);
  /* export: 'input' */
  WASM_RT_ADD_PREFIX(Z_inputZ_i) = (&input);
  /* export: 'copy_char' */
  WASM_RT_ADD_PREFIX(Z_copy_charZ_vii) = (&copy_char);
  /* export: 'key' */
  WASM_RT_ADD_PREFIX(Z_keyZ_i) = (&key);
  /* export: '__dso_handle' */
  WASM_RT_ADD_PREFIX(Z___dso_handleZ_i) = (&__dso_handle);
  /* export: '__data_end' */
  WASM_RT_ADD_PREFIX(Z___data_endZ_i) = (&__data_end);
  /* export: '__global_base' */
  WASM_RT_ADD_PREFIX(Z___global_baseZ_i) = (&__global_base);
  /* export: '__heap_base' */
  WASM_RT_ADD_PREFIX(Z___heap_baseZ_i) = (&__heap_base);
  /* export: '__memory_base' */
  WASM_RT_ADD_PREFIX(Z___memory_baseZ_i) = (&__memory_base);
  /* export: '__table_base' */
  WASM_RT_ADD_PREFIX(Z___table_baseZ_i) = (&__table_base);
}

void WASM_RT_ADD_PREFIX(init)(void) {
  init_func_types();
  init_globals();
  init_memory();
  init_table();
  init_exports();
}
