SRCS := main.cpp \
		behaviour/src/SimplePathPlanner.cpp \
		input/src/AutonomousControl.cpp \
		input/src/PS3Control.cpp \
		localisation/src/Localisation.cpp \
		platform/src/DynamixelProtocol.cpp \
		motion/src/MotorController.cpp \
		platform/src/Kinematics.cpp \
		platform/src/uart.cpp \
		platform/src/SensorFilter.cpp \
		vision/src/Camera.cpp \
		vision/src/Classifier.cpp \
		motion/src/IKGripper.cpp \
		vision/src/IRCamera.cpp
FLAGS := -lm -lwiringPi -I.

all: $(SRCS)
	g++ -o main $(SRCS) $(FLAGS)

makepi: $(SRCS)
	g++ -o main $(SRCS) $(FLAGS)
