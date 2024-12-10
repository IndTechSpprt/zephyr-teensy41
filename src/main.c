#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/drivers/uart.h>

// Create GPIO - LED Association
static const struct gpio_dt_spec led_board_0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0),gpios);
// Create USB Device
static const struct device *const dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

int main (void) {
	//Local variables
    int ret;
	bool led_state = true;
	uint32_t dtr = 0;

	//Check if gpios are ready
    if (!gpio_is_ready_dt(&led_board_0)) {
		return 0;
	}

	//If configuration failed, return
    ret = gpio_pin_configure_dt(&led_board_0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	//If usb enable failed, return
	if (usb_enable(NULL)) {
		return 0;
	}

	//Wait for console to start up
	/* Poll if the DTR flag was set */
	while (!dtr) {
		uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
		/* Give CPU resources to low priority threads. */
		k_sleep(K_MSEC(100));
	}

    while (1)
    {
		//Toggle LED
        ret = gpio_pin_toggle_dt(&led_board_0);
		//Return if there are any errors when toggling LED
		if (ret < 0) {
			return 0;
		}
		//Update state
		led_state = !led_state;
		//Print using printk
		printk("LED state: %s\n", led_state ? "ON" : "OFF");
		//Sleep for a second
		k_msleep(1000);
    }
    return 0;
}