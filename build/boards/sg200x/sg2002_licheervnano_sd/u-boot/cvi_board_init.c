__attribute__((optimize("O0")))
// 1.26ms
void suck_loop(uint64_t loop) {
        loop = loop * 50 * 100;
        uint64_t a;
        while (loop > 0) {
                a = loop / (uint64_t)99;
                a = loop / (uint64_t)77;
                a = loop / (uint64_t)55;
                a = loop / (uint64_t)33;
                a = loop / (uint64_t)11;
                a = loop / (uint64_t)999;
                a = loop / (uint64_t)777;
                a = loop / (uint64_t)555;
                a = loop / (uint64_t)333;
                a = loop / (uint64_t)111;
                a = loop / (uint64_t)9999;
                a = loop / (uint64_t)7777;
                a = loop / (uint64_t)5555;
                a = loop / (uint64_t)3333;
                a = loop / (uint64_t)1111;
                a = loop / (uint64_t)99999;
                a = loop / (uint64_t)77777;
                a = loop / (uint64_t)55555;
                a = loop / (uint64_t)33333;
                a = loop / (uint64_t)11111;
                loop--;
        }
}

static inline void user_led_on(void) {
	uint32_t val;

        val = mmio_read_32(0x03020000);
        val |= (1 << 14);
        mmio_write_32(0x03020000, val);
}

static inline void user_led_off(void) {
	uint32_t val;

        val = mmio_read_32(0x03020000);
        val &= ~(1 << 14);
        mmio_write_32(0x03020000, val);
}

static inline void user_led_toggle(void) {
	uint32_t val;

        val = mmio_read_32(0x03020000);
        val ^= (1 << 14);
        mmio_write_32(0x03020000, val);
}

int cvi_board_init(void)
{
        uint32_t val;

	// user led
	mmio_write_32(0x03001038, 0x3); // GPIOA 14 GPIO_MODE
	val = mmio_read_32(0x03020004); // GPIOA DIR
        val |= (1 << 14); // output
        mmio_write_32(0x03020004, val);
	user_led_toggle();

        // PWM
        //mmio_write_32(0x03001068, 0x2); // GPIOA 18 PWM 6

        // lcd reset
        mmio_write_32(0x030010A4, 0x0); // PWRGPIO 0 GPIO_MODE

	user_led_toggle();
        // lcd backlight
        //mmio_write_32(0x030010EC, 0x0); // GPIOB 0 PWM0_BUCK
	// for licheervnano alpha
	val = mmio_read_32(0x03021000); // signal level
        val |= (1 << 0); // set level to high
        mmio_write_32(0x03021000, val);
        val = mmio_read_32(0x03021004); // GPIOB DIR
        val |= (1 << 0); // output
        mmio_write_32(0x03021004, val);
        mmio_write_32(0x030010EC, 0x3); // GPIOB 0 GPIO_MODE

	// for licheervnano beta
	//mmio_write_32(0x030010ac, 0x4); // PWRGPIO 2 PWM 10
	mmio_write_32(0x030010ac, 0x0); // PWRGPIO 2 GPIO_MODE

        // camera function
        //mmio_write_32(0x0300116C, 0x5); // RX4N CAM_MCLK0 for alpha
        mmio_write_32(0x0300118C, 0x5); // RX0N CAM_MCLK1 for beta

	// spi1 on mipi csi 
	/*
	mmio_write_32(0x0300116C, 0x7); // spi1 clk   GPIOC2 MIPI_RX4N
	mmio_write_32(0x03001170, 0x7); // spi1 cs    GPIOC3 MIPI_RX4P
	mmio_write_32(0x03001174, 0x7); // spi1 miso  GPIOC4 MIPI_RX3N
	mmio_write_32(0x03001178, 0x7); // spi1 mosi  GPIOC5 MIPI_RX3P
	*/
	

        // camera/tp i2c
        mmio_write_32(0x03001090, 0x5); // PWR_GPIO6 IIC4_SCL
        mmio_write_32(0x03001098, 0x5); // PWR_GPIO8 IIC4_SDA

        // tp function
        mmio_write_32(0x03001084, 0x3); // PWR_SEQ1 PWR_GPIO[3]
        mmio_write_32(0x03001088, 0x3); // PWR_SEQ2 PWR_GPIO[4]
        mmio_write_32(0x05027078, 0x11);// Unlock PWR_GPIO[3]
        mmio_write_32(0x0502707c, 0x11);// Unlock PWR_GPIO[4]

	// bitbang i2c
        mmio_write_32(0x0300103C, 0x03); // GPIOA 15 GPIO_MODE
	mmio_write_32(0x03001058, 0x03); // GPIOA 27 GPIO_MODE

	// bitbang spi
	mmio_write_32(0x03001060, 0x03); // GPIOA 24 GPIO_MODE
	mmio_write_32(0x0300105C, 0x03); // GPIOA 23 GPIO_MODE
	mmio_write_32(0x03001054, 0x03); // GPIOA 25 GPIO_MODE
	mmio_write_32(0x03001050, 0x03); // GPIOA 22 GPIO_MODE

        // wait hardware bootup
        suck_loop(50);
	user_led_off();
        return 0;
}
