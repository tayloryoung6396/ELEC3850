SRCS := main.cpp \
		behaviour/src/SimplePathPlanner.cpp \
		input/AutonomousControl/src/AutonomousControl.cpp \
		input/PS3Control/src/PS3Control.cpp \
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
		vision/InfraredCamera/src/InfraredCamera.cpp \
		vision/UltrasonicSensor/src/UltrasonicSensor.cpp
FLAGS := -lm -lwiringPi -I.

all: $(SRCS)
	g++ -o main $(SRCS) $(FLAGS)

makepi: $(SRCS)
	g++ -o main $(SRCS) $(FLAGS)
