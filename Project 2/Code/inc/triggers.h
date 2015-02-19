#define TRIGGER_1 (8)
#define TRIGGER_2 (9)

#define TRIGGER_1_ASSERTED !(PTB->PDIR & MASK(TRIGGER_1))
#define TRIGGER_2_ASSERTED !(PTB->PDIR & MASK(TRIGGER_2))

void init_triggers(void);
