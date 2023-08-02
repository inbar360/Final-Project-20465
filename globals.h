

#define MAX_LINE 80

typedef enum registers{
	r0 = 0,
	r1,
	r2,
	r3,
	r4,
	r5,
	r6,
	r7,
	NON_REG = -1
} reg;

typedef enum instructions{
  mov = 0,
  cmp,
  add,
  sub,
  lea,
  not,
  clr,
  inc,
  dec,
  jmp,
  bne,
  red,
  prn,
  jsr,
  rts,
  stop,
  NON_INSTRUCTION = -1
} instruction;


char[] opcode_in_binary(char[] op){
  if(strcmp(op, "mov") == 0)
    return "0000";
  if(strcmp(op, "cmp") == 0)
    return "0001";
  if(strcmp(op, "add") == 0)
    return "0010";
  if(strcmp(op, "sub") == 0)
    return "0011";
  if(strcmp(op, "not") == 0)
    return "0100";
  if(strcmp(op, "clr") == 0)
    return "0101";
  if(strcmp(op, "lea") == 0)
    return "0110";
  if(strcmp(op, "inc") == 0)
    return "0111";
  if(strcmp(op, "dec") == 0)
    return "1000";
  if(strcmp(op, "jmp") == 0)
    return "1001";
  if(strcmp(op, "bne") == 0)
    return "1010";
  if(strcmp(op, "red") == 0)
    return "1011";
  if(strcmp(op, "prn") == 0)
    return "1100";
  if(strcmp(op, "jsr") == 0)
    return "1101";
  if(strcmp(op, "rts") == 0)
    return "1110";
  if(strcmp(op, "stop") == 0)
    return "1111";
}

