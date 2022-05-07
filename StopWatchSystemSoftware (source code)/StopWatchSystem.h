
/*******************************************************************************
 *                Interrupt Functions Prototype                      *
 *******************************************************************************/
void Timer1_init_compare_mode(void);
void INT0_Init(void);
void INT1_Init(void);
void INT2_Init(void);

/*******************************************************************************
 *                General Functions Prototype                      *
 *******************************************************************************/
void _7segment_config(void);
void countUpController(void);
void SegmentBySegment_WRITE(unsigned char SMH , unsigned char spot);
unsigned char button_is_pressed(unsigned char PINx , unsigned char spot);
void MIN_INC(unsigned char PINx,unsigned char spot);
void HOUR_INC(unsigned char PINx,unsigned char spot);
void SEC58_MIN59(void);
