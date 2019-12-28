#include "nemu.h"
#include"isa/mmu.h"
static paddr_t page_translate(vaddr_t addr);
uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  if((addr>>12)!=(addr+len-1)>>12){
    assert(0);
  }
  else{
    paddr_t paddr=page_translate(addr);
    return paddr_read(paddr, len);
  }
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  if((addr>>12)!=(addr+len-1)>>12){
    assert(0);
  }
  else{
    paddr_t paddr=page_translate(addr);
    paddr_write(paddr, data, len);
  }
}
static paddr_t page_translate(vaddr_t addr){
  CR0 cr0;
  cr0.val=cpu.CR0;
  if(cr0.paging){
    uint32_t addr_dir=addr>>22;
    uint32_t addr_page=(addr<<10)>>22;
    uint32_t addr_offset=(addr<<20)>>20;
    paddr_t PDE_addr=((cpu.CR3>>12)<<12)|(addr_dir<<2);
    PDE pde;
    PDE pte;
    pde.val=paddr_read(PDE_addr,4);
    assert(pde.present);
    paddr_t PTE_addr=(pde.page_frame<<12)|(addr_page<<2);
    pte.val=paddr_read(PTE_addr,4);
    assert(pte.present);
    paddr_t paddr=(pte.page_frame<<12)|(addr_offset);
    return paddr;
  }
  return addr;
}