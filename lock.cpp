class StackElement 
{ StackElement * Next; 
  int Wait;
}; 
class LIFOLock
{ StackElement * Head
Acquire(L) :
  StackElement A ;      // on-stack auto-variable
  auto h = L->Head
  if h == 0 :
    h = CAS (&L->Head, 0, 1)
    if (h == 0 ) 
      return;  //uncontended fast-path acquisition
  for
    //assert h != 0
    A.Next = h & ~1
    A.Wait = 1 
    auto v = CAS (&L->Head, h, &A) 
    if v == h : break 
    if v == 0 : 
      v = CAS (&L->Head, 0, 1) 
      if v == 0 : return
    h = v

  while A.Wait != 0 : Pause() 
  assert L->Head != 0 
Release(L) :
  auto h = L->Head
  assert h != 0
  if h == 1
    h = CAS (&L->Head, 1, 0)
    if h == 1 : return      // simple uncontended release

  assert h != 0 && h != 1

  for 
    auto next = h->Next 
    auto v = CAS (&L->Head, h, next) 
    if v == h :
      assert h->Wait == 1
      h->Wait = 0 
      break 
    h = v
};