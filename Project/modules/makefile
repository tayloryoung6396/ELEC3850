SRCS := main.cpp \
		behaviour/src/SimplePathPlanner.cpp \
		input/AutonomousController/src/AutonomousControl.cpp \
        input/PS3Controller/src/v2/Joystick.cpp \
		input/PS3Controller/src/v2/PS3Walk.cpp \
		localisation/src/Localisation.cpp \
		platform/src/DynamixelProtocol.cpp \
		motion/MotorController/src/MotorController.cpp \
		platform/src/Kinematics.cpp \
		platform/src/uart.cpp \
		platform/src/SensorFilter.cpp \
		vision/Camera/src/Camera.cpp \
		vision/Classifier/src/Classifier.cpp \
		motion/Gripper/src/IKGripper.cpp \
		motion/Gripper/src/FKGripper.cpp \
		vision/InfraredSensor/src/InfraredSensor.cpp \
		vision/UltrasonicSensor/src/UltrasonicSensor.cpp
FLAGS := -lm -lwiringPi -ggdb -O0 -I.

all: $(SRCS)
	g++ -std=c++11 -o main $(SRCS) $(FLAGS)

makepi: $(SRCS)
	g++ -std=c++11 -o main $(SRCS) $(FLAGS)
