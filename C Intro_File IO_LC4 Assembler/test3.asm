.CODE
.ADDR x0000
ADD R1, R0, R1
MUL R2, R1, R1
SUB R3, R2, R1
DIV R1, R3, R2
NOT R1, R3   ;TEST COMMENT
AND R1, R2, R3
.CODE
.ADDR x00AA
OR R1, R3, R2
XOR R1, R3, R2
ADD R1, R0 0xA
.DATA
.ADDR x4000