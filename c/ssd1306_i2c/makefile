all: demo  oled_ipAddress

demo:
	gcc demo.c ssd1306_i2c.c -lwiringPi -o demo

oled_ipAddress:
	gcc oled_ipAddress.c ssd1306_i2c.c -lwiringPi -o oled_ipAddress

clean:
	rm -rf *o demo oled_ipAddress
