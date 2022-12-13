#include <math.h>
#include <string.h>

#include "ZoRd23o0wd.h"
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
static u32 __dso_handle;
static u32 __data_end;
static u32 __global_base;
static u32 __heap_base;
static u32 __memory_base;
static u32 __table_base;

static void init_globals(void) {
  g0 = 66864u;
  input = 1072u;
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
      l8 = 0, l9 = 0, l10 = 0, l11 = 0, l12 = 0, l13 = 0, l14 = 0, l15 = 0, 
      l16 = 0, l17 = 0, l18 = 0, l19 = 0, l20 = 0, l21 = 0, l22 = 0, l23 = 0, 
      l24 = 0, l25 = 0, l26 = 0, l27 = 0, l28 = 0, l29 = 0, l30 = 0, l31 = 0, 
      l32 = 0, l33 = 0, l34 = 0, l35 = 0, l36 = 0, l37 = 0, l38 = 0, l39 = 0, 
      l40 = 0, l41 = 0, l42 = 0, l43 = 0, l44 = 0, l45 = 0, l46 = 0, l47 = 0, 
      l48 = 0, l49 = 0, l50 = 0, l51 = 0, l52 = 0, l53 = 0, l54 = 0, l55 = 0, 
      l56 = 0, l57 = 0, l58 = 0, l59 = 0, l60 = 0, l61 = 0, l62 = 0, l63 = 0, 
      l64 = 0, l65 = 0, l66 = 0, l67 = 0, l68 = 0, l69 = 0, l70 = 0, l71 = 0, 
      l72 = 0, l73 = 0, l74 = 0, l75 = 0, l76 = 0, l77 = 0, l78 = 0, l79 = 0, 
      l80 = 0, l81 = 0, l82 = 0, l83 = 0, l84 = 0, l85 = 0, l86 = 0, l87 = 0, 
      l88 = 0, l89 = 0, l90 = 0, l91 = 0, l92 = 0, l93 = 0, l94 = 0, l95 = 0, 
      l96 = 0, l97 = 0, l98 = 0, l99 = 0, l100 = 0, l101 = 0, l102 = 0, l103 = 0, 
      l104 = 0, l105 = 0, l106 = 0, l107 = 0, l108 = 0, l109 = 0, l110 = 0, l111 = 0, 
      l112 = 0, l113 = 0, l114 = 0, l115 = 0, l116 = 0, l117 = 0, l118 = 0, l119 = 0, 
      l120 = 0, l121 = 0, l122 = 0, l123 = 0, l124 = 0, l125 = 0, l126 = 0, l127 = 0, 
      l128 = 0, l129 = 0, l130 = 0, l131 = 0, l132 = 0, l133 = 0, l134 = 0, l135 = 0, 
      l136 = 0, l137 = 0, l138 = 0, l139 = 0, l140 = 0, l141 = 0, l142 = 0, l143 = 0, 
      l144 = 0, l145 = 0, l146 = 0, l147 = 0, l148 = 0, l149 = 0, l150 = 0, l151 = 0, 
      l152 = 0, l153 = 0, l154 = 0, l155 = 0, l156 = 0, l157 = 0, l158 = 0, l159 = 0, 
      l160 = 0, l161 = 0, l162 = 0, l163 = 0, l164 = 0;
  FUNC_PROLOGUE;
  u32 i0, i1;
  i0 = g0;
  l0 = i0;
  i0 = 16u;
  l1 = i0;
  i0 = l0;
  i1 = l1;
  i0 -= i1;
  l2 = i0;
  i0 = l2;
  g0 = i0;
  i0 = 0u;
  l3 = i0;
  i0 = l2;
  i1 = l3;
  i32_store((&memory), (u64)(i0 + 12), i1);
  L1: 
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l4 = i0;
    i0 = l4;
    i0 = i32_load8_u((&memory), (u64)(i0 + 1072));
    l5 = i0;
    i0 = 24u;
    l6 = i0;
    i0 = l5;
    i1 = l6;
    i0 <<= (i1 & 31);
    l7 = i0;
    i0 = l7;
    i1 = l6;
    i0 = (u32)((s32)i0 >> (i1 & 31));
    l8 = i0;
    i0 = l8;
    i0 = !(i0);
    if (i0) {goto B0;}
    i0 = 0u;
    l9 = i0;
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l10 = i0;
    i0 = l10;
    i0 = i32_load8_u((&memory), (u64)(i0 + 1072));
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
    i0 = 20u;
    l15 = i0;
    i0 = l14;
    i1 = l15;
    i0 ^= i1;
    l16 = i0;
    i0 = l10;
    i1 = l16;
    i32_store8((&memory), (u64)(i0 + 1072), i1);
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l17 = i0;
    i0 = l17;
    l18 = i0;
    i0 = l9;
    l19 = i0;
    i0 = l18;
    i1 = l19;
    i0 = (u32)((s32)i0 > (s32)i1);
    l20 = i0;
    i0 = 1u;
    l21 = i0;
    i0 = l20;
    i1 = l21;
    i0 &= i1;
    l22 = i0;
    i0 = l22;
    i0 = !(i0);
    if (i0) {goto B2;}
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l23 = i0;
    i0 = 1u;
    l24 = i0;
    i0 = l23;
    i1 = l24;
    i0 -= i1;
    l25 = i0;
    i0 = l25;
    i0 = i32_load8_u((&memory), (u64)(i0 + 1072));
    l26 = i0;
    i0 = 24u;
    l27 = i0;
    i0 = l26;
    i1 = l27;
    i0 <<= (i1 & 31);
    l28 = i0;
    i0 = l28;
    i1 = l27;
    i0 = (u32)((s32)i0 >> (i1 & 31));
    l29 = i0;
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l30 = i0;
    i0 = l30;
    i0 = i32_load8_u((&memory), (u64)(i0 + 1072));
    l31 = i0;
    i0 = 24u;
    l32 = i0;
    i0 = l31;
    i1 = l32;
    i0 <<= (i1 & 31);
    l33 = i0;
    i0 = l33;
    i1 = l32;
    i0 = (u32)((s32)i0 >> (i1 & 31));
    l34 = i0;
    i0 = l34;
    i1 = l29;
    i0 ^= i1;
    l35 = i0;
    i0 = l30;
    i1 = l35;
    i32_store8((&memory), (u64)(i0 + 1072), i1);
    B2:;
    i0 = 2u;
    l36 = i0;
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l37 = i0;
    i0 = l37;
    l38 = i0;
    i0 = l36;
    l39 = i0;
    i0 = l38;
    i1 = l39;
    i0 = (u32)((s32)i0 > (s32)i1);
    l40 = i0;
    i0 = 1u;
    l41 = i0;
    i0 = l40;
    i1 = l41;
    i0 &= i1;
    l42 = i0;
    i0 = l42;
    i0 = !(i0);
    if (i0) {goto B3;}
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l43 = i0;
    i0 = 3u;
    l44 = i0;
    i0 = l43;
    i1 = l44;
    i0 -= i1;
    l45 = i0;
    i0 = l45;
    i0 = i32_load8_u((&memory), (u64)(i0 + 1072));
    l46 = i0;
    i0 = 24u;
    l47 = i0;
    i0 = l46;
    i1 = l47;
    i0 <<= (i1 & 31);
    l48 = i0;
    i0 = l48;
    i1 = l47;
    i0 = (u32)((s32)i0 >> (i1 & 31));
    l49 = i0;
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l50 = i0;
    i0 = l50;
    i0 = i32_load8_u((&memory), (u64)(i0 + 1072));
    l51 = i0;
    i0 = 24u;
    l52 = i0;
    i0 = l51;
    i1 = l52;
    i0 <<= (i1 & 31);
    l53 = i0;
    i0 = l53;
    i1 = l52;
    i0 = (u32)((s32)i0 >> (i1 & 31));
    l54 = i0;
    i0 = l54;
    i1 = l49;
    i0 ^= i1;
    l55 = i0;
    i0 = l50;
    i1 = l55;
    i32_store8((&memory), (u64)(i0 + 1072), i1);
    B3:;
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l56 = i0;
    i0 = 10u;
    l57 = i0;
    i0 = l56;
    i1 = l57;
    i0 = I32_REM_S(i0, i1);
    l58 = i0;
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l59 = i0;
    i0 = l59;
    i0 = i32_load8_u((&memory), (u64)(i0 + 1072));
    l60 = i0;
    i0 = 24u;
    l61 = i0;
    i0 = l60;
    i1 = l61;
    i0 <<= (i1 & 31);
    l62 = i0;
    i0 = l62;
    i1 = l61;
    i0 = (u32)((s32)i0 >> (i1 & 31));
    l63 = i0;
    i0 = l63;
    i1 = l58;
    i0 ^= i1;
    l64 = i0;
    i0 = l59;
    i1 = l64;
    i32_store8((&memory), (u64)(i0 + 1072), i1);
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l65 = i0;
    i0 = 2u;
    l66 = i0;
    i0 = l65;
    i1 = l66;
    i0 = I32_REM_S(i0, i1);
    l67 = i0;
    i0 = l67;
    if (i0) {goto B5;}
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l68 = i0;
    i0 = l68;
    i0 = i32_load8_u((&memory), (u64)(i0 + 1072));
    l69 = i0;
    i0 = 24u;
    l70 = i0;
    i0 = l69;
    i1 = l70;
    i0 <<= (i1 & 31);
    l71 = i0;
    i0 = l71;
    i1 = l70;
    i0 = (u32)((s32)i0 >> (i1 & 31));
    l72 = i0;
    i0 = 9u;
    l73 = i0;
    i0 = l72;
    i1 = l73;
    i0 ^= i1;
    l74 = i0;
    i0 = l68;
    i1 = l74;
    i32_store8((&memory), (u64)(i0 + 1072), i1);
    goto B4;
    B5:;
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l75 = i0;
    i0 = l75;
    i0 = i32_load8_u((&memory), (u64)(i0 + 1072));
    l76 = i0;
    i0 = 24u;
    l77 = i0;
    i0 = l76;
    i1 = l77;
    i0 <<= (i1 & 31);
    l78 = i0;
    i0 = l78;
    i1 = l77;
    i0 = (u32)((s32)i0 >> (i1 & 31));
    l79 = i0;
    i0 = 8u;
    l80 = i0;
    i0 = l79;
    i1 = l80;
    i0 ^= i1;
    l81 = i0;
    i0 = l75;
    i1 = l81;
    i32_store8((&memory), (u64)(i0 + 1072), i1);
    B4:;
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l82 = i0;
    i0 = 3u;
    l83 = i0;
    i0 = l82;
    i1 = l83;
    i0 = I32_REM_S(i0, i1);
    l84 = i0;
    i0 = l84;
    if (i0) {goto B7;}
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l85 = i0;
    i0 = l85;
    i0 = i32_load8_u((&memory), (u64)(i0 + 1072));
    l86 = i0;
    i0 = 24u;
    l87 = i0;
    i0 = l86;
    i1 = l87;
    i0 <<= (i1 & 31);
    l88 = i0;
    i0 = l88;
    i1 = l87;
    i0 = (u32)((s32)i0 >> (i1 & 31));
    l89 = i0;
    i0 = 7u;
    l90 = i0;
    i0 = l89;
    i1 = l90;
    i0 ^= i1;
    l91 = i0;
    i0 = l85;
    i1 = l91;
    i32_store8((&memory), (u64)(i0 + 1072), i1);
    goto B6;
    B7:;
    i0 = 1u;
    l92 = i0;
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l93 = i0;
    i0 = 3u;
    l94 = i0;
    i0 = l93;
    i1 = l94;
    i0 = I32_REM_S(i0, i1);
    l95 = i0;
    i0 = l95;
    l96 = i0;
    i0 = l92;
    l97 = i0;
    i0 = l96;
    i1 = l97;
    i0 = i0 == i1;
    l98 = i0;
    i0 = 1u;
    l99 = i0;
    i0 = l98;
    i1 = l99;
    i0 &= i1;
    l100 = i0;
    i0 = l100;
    i0 = !(i0);
    if (i0) {goto B9;}
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l101 = i0;
    i0 = l101;
    i0 = i32_load8_u((&memory), (u64)(i0 + 1072));
    l102 = i0;
    i0 = 24u;
    l103 = i0;
    i0 = l102;
    i1 = l103;
    i0 <<= (i1 & 31);
    l104 = i0;
    i0 = l104;
    i1 = l103;
    i0 = (u32)((s32)i0 >> (i1 & 31));
    l105 = i0;
    i0 = 6u;
    l106 = i0;
    i0 = l105;
    i1 = l106;
    i0 ^= i1;
    l107 = i0;
    i0 = l101;
    i1 = l107;
    i32_store8((&memory), (u64)(i0 + 1072), i1);
    goto B8;
    B9:;
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l108 = i0;
    i0 = l108;
    i0 = i32_load8_u((&memory), (u64)(i0 + 1072));
    l109 = i0;
    i0 = 24u;
    l110 = i0;
    i0 = l109;
    i1 = l110;
    i0 <<= (i1 & 31);
    l111 = i0;
    i0 = l111;
    i1 = l110;
    i0 = (u32)((s32)i0 >> (i1 & 31));
    l112 = i0;
    i0 = 5u;
    l113 = i0;
    i0 = l112;
    i1 = l113;
    i0 ^= i1;
    l114 = i0;
    i0 = l108;
    i1 = l114;
    i32_store8((&memory), (u64)(i0 + 1072), i1);
    B8:;
    B6:;
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l115 = i0;
    i0 = 1u;
    l116 = i0;
    i0 = l115;
    i1 = l116;
    i0 += i1;
    l117 = i0;
    i0 = l2;
    i1 = l117;
    i32_store((&memory), (u64)(i0 + 12), i1);
    goto L1;
  B0:;
  i0 = 0u;
  l118 = i0;
  i0 = l2;
  i1 = l118;
  i32_store((&memory), (u64)(i0 + 4), i1);
  L11: 
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 4));
    l119 = i0;
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l120 = i0;
    i0 = l119;
    l121 = i0;
    i0 = l120;
    l122 = i0;
    i0 = l121;
    i1 = l122;
    i0 = (u32)((s32)i0 < (s32)i1);
    l123 = i0;
    i0 = 1u;
    l124 = i0;
    i0 = l123;
    i1 = l124;
    i0 &= i1;
    l125 = i0;
    i0 = l125;
    i0 = !(i0);
    if (i0) {goto B10;}
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 4));
    l126 = i0;
    i0 = 2u;
    l127 = i0;
    i0 = l126;
    i1 = l127;
    i0 = I32_REM_S(i0, i1);
    l128 = i0;
    i0 = l128;
    if (i0) {goto B12;}
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 4));
    l129 = i0;
    i0 = 1u;
    l130 = i0;
    i0 = l129;
    i1 = l130;
    i0 += i1;
    l131 = i0;
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 12));
    l132 = i0;
    i0 = l131;
    l133 = i0;
    i0 = l132;
    l134 = i0;
    i0 = l133;
    i1 = l134;
    i0 = (u32)((s32)i0 < (s32)i1);
    l135 = i0;
    i0 = 1u;
    l136 = i0;
    i0 = l135;
    i1 = l136;
    i0 &= i1;
    l137 = i0;
    i0 = l137;
    i0 = !(i0);
    if (i0) {goto B12;}
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 4));
    l138 = i0;
    i0 = l138;
    i0 = i32_load8_u((&memory), (u64)(i0 + 1072));
    l139 = i0;
    i0 = l2;
    i1 = l139;
    i32_store8((&memory), (u64)(i0 + 11), i1);
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 4));
    l140 = i0;
    i0 = 1u;
    l141 = i0;
    i0 = l140;
    i1 = l141;
    i0 += i1;
    l142 = i0;
    i0 = l142;
    i0 = i32_load8_u((&memory), (u64)(i0 + 1072));
    l143 = i0;
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 4));
    l144 = i0;
    i0 = l144;
    i1 = l143;
    i32_store8((&memory), (u64)(i0 + 1072), i1);
    i0 = l2;
    i0 = i32_load8_u((&memory), (u64)(i0 + 11));
    l145 = i0;
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 4));
    l146 = i0;
    i0 = 1u;
    l147 = i0;
    i0 = l146;
    i1 = l147;
    i0 += i1;
    l148 = i0;
    i0 = l148;
    i1 = l145;
    i32_store8((&memory), (u64)(i0 + 1072), i1);
    B12:;
    i0 = l2;
    i0 = i32_load((&memory), (u64)(i0 + 4));
    l149 = i0;
    i0 = 1u;
    l150 = i0;
    i0 = l149;
    i1 = l150;
    i0 += i1;
    l151 = i0;
    i0 = l2;
    i1 = l151;
    i32_store((&memory), (u64)(i0 + 4), i1);
    goto L11;
  B10:;
  i0 = 0u;
  l152 = i0;
  i0 = 1072u;
  l153 = i0;
  i0 = 1024u;
  l154 = i0;
  i0 = l154;
  i1 = l153;
  i0 = strcmp_0(i0, i1);
  l155 = i0;
  i0 = l155;
  l156 = i0;
  i0 = l152;
  l157 = i0;
  i0 = l156;
  i1 = l157;
  i0 = i0 != i1;
  l158 = i0;
  i0 = 4294967295u;
  l159 = i0;
  i0 = l158;
  i1 = l159;
  i0 ^= i1;
  l160 = i0;
  i0 = 1u;
  l161 = i0;
  i0 = l160;
  i1 = l161;
  i0 &= i1;
  l162 = i0;
  i0 = 16u;
  l163 = i0;
  i0 = l2;
  i1 = l163;
  i0 += i1;
  l164 = i0;
  i0 = l164;
  g0 = i0;
  i0 = l162;
  goto Bfunc;
  Bfunc:;
  FUNC_EPILOGUE;
  return i0;
}

static void copy_char(u32 p0, u32 p1) {
  u32 l2 = 0, l3 = 0, l4 = 0, l5 = 0, l6 = 0;
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
  i0 = l4;
  i0 = i32_load((&memory), (u64)(i0 + 8));
  l6 = i0;
  i0 = l6;
  i1 = l5;
  i32_store8((&memory), (u64)(i0 + 1072), i1);
  goto Bfunc;
  Bfunc:;
  FUNC_EPILOGUE;
}

static const u8 data_segment_data_0[] = {
  0x18, 0x6a, 0x7c, 0x61, 0x11, 0x38, 0x69, 0x37, 0x18, 0x09, 0x79, 0x0e, 
  0x68, 0x1b, 0x03, 0x3f, 0x07, 0x13, 0x42, 0x26, 0x60, 0x6d, 0x1b, 0x5d, 
  0x73, 0x04, 0x6c, 0x47, 0x52, 0x35, 0x5d, 0x17, 0x1f, 0x73, 0x33, 0x38, 
  0x40, 0x51, 0x77, 0x57, 0x51, 0x00, 0x00, 
};

static void init_memory(void) {
  wasm_rt_allocate_memory((&memory), 2, 65536);
  memcpy(&(memory.data[1024u]), data_segment_data_0, 43);
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
