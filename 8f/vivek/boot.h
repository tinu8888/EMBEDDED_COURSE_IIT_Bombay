void chk_isr(void);
extern void _startup (void);
#pragma code _HIGH_INTERRUPT_VECTOR = 0x1008
void high_ISR (void)
{
 	_asm
	GOTO chk_isr
	_endasm      
}
#pragma code _RESET_INTERRUPT_VECTOR = 0x1000

void _reset (void)
{ _asm goto _startup _endasm
    
}

#pragma code

#pragma code _LOW_INTERRUPT_VECTOR = 0x1018
void low_ISR (void)
{
}
#pragma code


