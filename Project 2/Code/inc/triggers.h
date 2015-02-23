#define TRIGGER_1 (8)
#define TRIGGER_2 (9)

#define TRIGGER_1_ASSERTED !(PTB->PDIR & MASK(TRIGGER_1))
#define TRIGGER_2_ASSERTED !(PTB->PDIR & MASK(TRIGGER_2))
#define ST ((TRIGGER_1_ASSERTED && triggerState == 1) || (TRIGGER_2_ASSERTED && triggerState == 2) || (!TRIGGER_1_ASSERTED && !TRIGGER_2_ASSERTED && triggerState == 0))

void init_triggers(void);
