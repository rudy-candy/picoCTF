
/* WARNING: Removing unreachable block (ram,0x0010256c) */
/* WARNING: Removing unreachable block (ram,0x001019b2) */
/* WARNING: Removing unreachable block (ram,0x00101e29) */
/* WARNING: Removing unreachable block (ram,0x00101976) */
/* WARNING: Removing unreachable block (ram,0x00101b67) */
/* WARNING: Removing unreachable block (ram,0x00102057) */
/* WARNING: Removing unreachable block (ram,0x00101e65) */
/* WARNING: Removing unreachable block (ram,0x001025a8) */
/* WARNING: Removing unreachable block (ram,0x00101b2b) */
/* WARNING: Removing unreachable block (ram,0x0010201b) */

int w2c_check_flag(void)

{
  uint uVar1;
  int iVar2;
  char cVar3;
  byte bVar4;
  byte bVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  undefined4 uVar9;
  
  wasm_rt_call_stack_depth = wasm_rt_call_stack_depth + 1;
  if (500 < wasm_rt_call_stack_depth) {
    wasm_rt_trap(7);
  }
  iVar2 = w2c_g0;
  uVar1 = w2c_g0 - 0x10;
  w2c_g0 = uVar1;
  i32_store(w2c_memory,(ulong)uVar1 + 0xc,0);
  while( true ) {
    uVar6 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
    cVar3 = i32_load8_u(w2c_memory,(ulong)uVar6 + 0x430);
    if (cVar3 == '\0') break;
    uVar6 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
    cVar3 = i32_load8_u(w2c_memory,(ulong)uVar6 + 0x430);
    i32_store8(w2c_memory,(ulong)uVar6 + 0x430,(int)cVar3 ^ 0x14);
    iVar7 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
    if (0 < iVar7) {
      iVar7 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
      bVar4 = i32_load8_u(w2c_memory,(ulong)(iVar7 - 1) + 0x430);
      uVar6 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
      bVar5 = i32_load8_u(w2c_memory,(ulong)uVar6 + 0x430);
      i32_store8(w2c_memory,(ulong)uVar6 + 0x430,(int)(char)(bVar5 ^ bVar4));
    }
    iVar7 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
    if (2 < iVar7) {
      iVar7 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
      bVar4 = i32_load8_u(w2c_memory,(ulong)(iVar7 - 3) + 0x430);
      uVar6 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
      bVar5 = i32_load8_u(w2c_memory,(ulong)uVar6 + 0x430);
      i32_store8(w2c_memory,(ulong)uVar6 + 0x430,(int)(char)(bVar5 ^ bVar4));
    }
    iVar7 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
    uVar6 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
    cVar3 = i32_load8_u(w2c_memory,(ulong)uVar6 + 0x430);
    i32_store8(w2c_memory,(ulong)uVar6 + 0x430,(int)cVar3 ^ iVar7 % 10);
    iVar7 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
    if (iVar7 % 2 == 0) {
      uVar6 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
      cVar3 = i32_load8_u(w2c_memory,(ulong)uVar6 + 0x430);
      i32_store8(w2c_memory,(ulong)uVar6 + 0x430,(int)cVar3 ^ 9);
    }
    else {
      uVar6 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
      cVar3 = i32_load8_u(w2c_memory,(ulong)uVar6 + 0x430);
      i32_store8(w2c_memory,(ulong)uVar6 + 0x430,(int)cVar3 ^ 8);
    }
    iVar7 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
    if (iVar7 % 3 == 0) {
      uVar6 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
      cVar3 = i32_load8_u(w2c_memory,(ulong)uVar6 + 0x430);
      i32_store8(w2c_memory,(ulong)uVar6 + 0x430,(int)cVar3 ^ 7);
    }
    else {
      iVar7 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
      if (iVar7 % 3 == 1) {
        uVar6 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
        cVar3 = i32_load8_u(w2c_memory,(ulong)uVar6 + 0x430);
        i32_store8(w2c_memory,(ulong)uVar6 + 0x430,(int)cVar3 ^ 6);
      }
      else {
        uVar6 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
        cVar3 = i32_load8_u(w2c_memory,(ulong)uVar6 + 0x430);
        i32_store8(w2c_memory,(ulong)uVar6 + 0x430,(int)cVar3 ^ 5);
      }
    }
    iVar7 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
    i32_store(w2c_memory,(ulong)uVar1 + 0xc,iVar7 + 1);
  }
  i32_store(w2c_memory,(ulong)uVar1 + 4,0);
  while( true ) {
    iVar7 = i32_load(w2c_memory,(ulong)uVar1 + 4);
    iVar8 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
    if (iVar8 <= iVar7) break;
    iVar7 = i32_load(w2c_memory,(ulong)uVar1 + 4);
    if (iVar7 % 2 == 0) {
      iVar7 = i32_load(w2c_memory,(ulong)uVar1 + 4);
      iVar8 = i32_load(w2c_memory,(ulong)uVar1 + 0xc);
      if (iVar7 + 1 < iVar8) {
        uVar6 = i32_load(w2c_memory,(ulong)uVar1 + 4);
        uVar9 = i32_load8_u(w2c_memory,(ulong)uVar6 + 0x430);
        i32_store8(w2c_memory,(ulong)uVar1 + 0xb,uVar9);
        iVar7 = i32_load(w2c_memory,(ulong)uVar1 + 4);
        uVar9 = i32_load8_u(w2c_memory,(ulong)(iVar7 + 1) + 0x430);
        uVar6 = i32_load(w2c_memory,(ulong)uVar1 + 4);
        i32_store8(w2c_memory,(ulong)uVar6 + 0x430,uVar9);
        uVar9 = i32_load8_u(w2c_memory,(ulong)uVar1 + 0xb);
        iVar7 = i32_load(w2c_memory,(ulong)uVar1 + 4);
        i32_store8(w2c_memory,(ulong)(iVar7 + 1) + 0x430,uVar9);
      }
    }
    iVar7 = i32_load(w2c_memory,(ulong)uVar1 + 4);
    i32_store(w2c_memory,(ulong)uVar1 + 4,iVar7 + 1);
  }
  iVar7 = w2c_strcmp(0x400,0x430);
  w2c_g0 = iVar2;
  wasm_rt_call_stack_depth = wasm_rt_call_stack_depth - 1;
  return (iVar7 != 0 ^ 0xffU) & 1;
}