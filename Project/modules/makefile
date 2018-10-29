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
		vision/Camera/src/CameraCV.cpp \
		vision/Classifier/src/Classifier.cpp \
		motion/Gripper/src/IKGripper.cpp \
		motion/Gripper/src/FKGripper.cpp \
		vision/InfraredSensor/src/InfraredSensor.cpp \
		vision/UltrasonicSensor/src/UltrasonicSensor.cpp
FLAGS_CV := -lm -lwiringPi -L/opt/vc/lib -I/usr/local/include/ -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util -lopencv_core -lopencv_highgui -I.
FLAGS := -lm -lwiringPi -L/opt/vc/lib -I/usr/local/include/ -lraspicam -lmmal -lmmal_core -lmmal_util -I.
FLAGS_D := -lm -lwiringPi -ggdb -O0 -L/opt/vc/lib -I/usr/local/include/ -lraspicam -lmmal -lmmal_core -lmmal_util -I.
make: $(SRCS)
	g++ -std=c++11 -o main $(SRCS) $(FLAGS)

debug: $(SRCS)
	g++ -std=c++11 -o main $(SRCS) $(FLAGS_D)

cv: $(SRCS)
	g++ -std=c++11 -o main $(SRCS) $(FLAGS_CV)
