# NEX: (N)op (EX)ecutable

- 4 bytes for signature("NEX!", with the "N" at the LSB)
- 4 bytes for the name(no null byte, pad with spaces)
- 4 bytes for mapping address(code runs 12 bytes after this address)
- the rest just for the code!
