.PHONY: copy-libs

copy-libs:
	rm -rf lib/motors/stepper_motor
	mkdir -p lib/motors/stepper_motor
	cp -r ../../libs/motors/stepper_motor/* lib/motors/stepper_motor/

	rm -rf lib/system/timer
	mkdir -p lib/system/timer
	cp -r ../../libs/system/timer/* lib/system/timer/

	mkdir -p lib/input
	cp ../../libs/input/command_handler.h lib/input/command_handler.h

	rm -rf lib/input/joystick
	mkdir -p lib/input/joystick
	cp -r ../../libs/input/joystick/* lib/input/joystick/

	rm -rf lib/motion/belt
	mkdir -p lib/motion/belt
	cp -r ../../libs/motion/belt/* lib/motion/belt/

	rm -rf lib/logic/limit_switch
	mkdir -p lib/logic/limit_switch
	cp -r ../../libs/logic/limit_switch/* lib/logic/limit_switch/

	rm -rf lib/communication/radio_transmitter
	mkdir -p lib/communication/radio_transmitter
	cp -r ../../libs/communication/radio_transmitter/* lib/communication/radio_transmitter/
