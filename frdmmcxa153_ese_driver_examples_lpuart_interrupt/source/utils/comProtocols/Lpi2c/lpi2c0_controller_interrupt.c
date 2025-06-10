#include "lpi2c0_controller_interrupt.h"
#include "../source/utils/comProtocols/fifo16.h"

bool lpi2c0_txfifo_full(void);
bool lpi2c0_rxfifo_empty(void);

static fifo_t tx;
static fifo_t rx;
static uint16_t tx_buffer[32];
static uint16_t rx_buffer[32];

void lpi2c0_controller_init(void)
{
    f16_init(&tx, tx_buffer, sizeof(tx_buffer));
    f16_init(&rx, rx_buffer, sizeof(rx_buffer));

    MRCC0->MRCC_LPI2C0_CLKSEL = MRCC_MRCC_LPI2C0_CLKSEL_MUX(2);
    MRCC0->MRCC_LPI2C0_CLKDIV = 0;
    MRCC0->MRCC_GLB_CC0_SET = MRCC_MRCC_GLB_CC0_LPI2C0(1);
    MRCC0->MRCC_GLB_CC1_SET = MRCC_MRCC_GLB_CC1_PORT3(1);
    MRCC0->MRCC_GLB_CC1_SET = MRCC_MRCC_GLB_CC1_GPIO3(1);
    MRCC0->MRCC_GLB_RST0_SET = MRCC_MRCC_GLB_RST0_LPI2C0(1);
    MRCC0->MRCC_GLB_RST1_SET = MRCC_MRCC_GLB_RST1_PORT3(1);
    MRCC0->MRCC_GLB_RST1_SET = MRCC_MRCC_GLB_CC1_GPIO3(1);
    PORT3->PCR[28] = PORT_PCR_LK(1) | PORT_PCR_MUX(2) | PORT_PCR_PE(1) | PORT_PCR_PS(1) | PORT_PCR_ODE(1) | PORT_PCR_IBE(1);
    PORT3->PCR[27] = PORT_PCR_LK(1) | PORT_PCR_MUX(2) | PORT_PCR_PE(1) | PORT_PCR_PS(1) | PORT_PCR_ODE(1) | PORT_PCR_IBE(1);
    LPI2C0->MCFGR1 = LPI2C_MCFGR1_PRESCALE(0);
    LPI2C0->MCFGR2 = LPI2C_MCFGR2_FILTSDA(1) | LPI2C_MCFGR2_FILTSCL(1);
    LPI2C0->MCCR0 = LPI2C_MCCR0_DATAVD(0x0F) | LPI2C_MCCR0_SETHOLD(0x1D) | LPI2C_MCCR0_CLKHI(0x35) | LPI2C_MCCR0_CLKLO(0x3E);
    LPI2C0->MCFGR1 |= LPI2C_MCFGR1_AUTOSTOP(1);
    LPI2C0->MIER |= LPI2C_MIER_RDIE(1);
    NVIC_SetPriority(LPI2C0_IRQn, 3);
    NVIC_ClearPendingIRQ(LPI2C0_IRQn);
    NVIC_EnableIRQ(LPI2C0_IRQn);
    __enable_irq();
    lpi2c0_wait_busy();
    LPI2C0->MCR |= LPI2C_MCR_MEN(1);
}

void lpi2c0_putword(const uint16_t data)
{
    while(!f16_push(&tx, data))
    {}
    LPI2C0->MIER |= LPI2C_MIER_TDIE(1);
}

uint16_t lpi2c0_getword(void)
{
    uint16_t c=0;
    while(!f16_pop(&rx, &c))
    {}
    return c;
}

uint32_t lpi2c0_rxcnt(void)
{
    return f16_cnt(&rx);
}

uint32_t lpi2c0_txcnt(void)
{
    return f16_cnt(&tx);
}

inline void lpi2c0_wait_busy(void)
{
    while((LPI2C0->MSR & (LPI2C_MSR_BBF_MASK | LPI2C_MSR_MBF_MASK)) != 0)
    {}
}

inline bool lpi2c0_txfifo_full(void)
{
    uint32_t n = (LPI2C0->MFSR & LPI2C_MFSR_TXCOUNT_MASK) >> LPI2C_MFSR_TXCOUNT_SHIFT;
    return n == 4;
}

inline bool lpi2c0_rxfifo_empty(void)
{
    uint32_t n = (LPI2C0->MFSR & LPI2C_MFSR_RXCOUNT_MASK) >> LPI2C_MFSR_RXCOUNT_SHIFT;
    return n == 0;
}

void LPI2C0_IRQHandler(void)
{
    uint16_t c;
    NVIC_ClearPendingIRQ(LPI2C0_IRQn);
    if((LPI2C0->MSR & LPI2C_MSR_TDF_MASK) != 0)
    {
        while(!lpi2c0_txfifo_full())
        {
            if(f16_pop(&tx, &c))
            {
                LPI2C0->MTDR = c;
            }
            else
            {
                break;
            }
        }
        if(f16_cnt(&tx) == 0)
        {
            LPI2C0->MIER &= ~LPI2C_MIER_TDIE(1);
        }
    }
    if((LPI2C0->MSR & LPI2C_MSR_RDF_MASK) !=0)
    {
        c = (uint16_t)(LPI2C0->MRDR);
        if(!f16_push(&rx, c))
        {
            while (1)
            {}
        }
    }
}
