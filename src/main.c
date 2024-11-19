#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec led_board_0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0),gpios);


int main (void) {
    int ret;
	bool led_state = true;

    if (!gpio_is_ready_dt(&led_board_0)) {
		return 0;
	}

    ret = gpio_pin_configure_dt(&led_board_0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

    while (1)
    {
        ret = gpio_pin_toggle_dt(&led_board_0);
		if (ret < 0) {
			return 0;
		}
		led_state = !led_state;
		printf("LED state: %s\n", led_state ? "ON" : "OFF");
		k_msleep(1000);
    }
    return 0;
}