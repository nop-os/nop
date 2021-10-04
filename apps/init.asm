[org 0xE0000000]
[bits 32]

nex_header:
  db "NEX!" ; header signature
  db "INIT" ; program name
nex_start:
  ret
