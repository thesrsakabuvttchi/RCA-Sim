all: BASIC_SCHED BASIC_BLOCKED_SCHED MNIST_DNN MNIST_MLP

setup: setup_DNN setup_MLP

setup_DNN:
	python3 create_mnist_DNN.py

setup_MLP:
	python3 create_mnist_MLP.py

BASIC_SCHED: testLimitedScheduler.cpp
	g++ testLimitedScheduler.cpp -o testLimitedScheduler

BASIC_BLOCKED_SCHED: testBrainDeadScheduler.cpp
	g++ testBrainDeadScheduler.cpp -o testBlockedScheduler

MNIST_DNN: MNIST_Test_DNN.cpp
	g++ MNIST_Test_DNN.cpp -o MNIST_Test_DNN

MNIST_MLP: MNIST_Test_MLP.cpp
	g++ MNIST_Test_MLP.cpp -o MNIST_Test_MLP

clean:
	rm testBlockedScheduler MNIST_Test_DNN MNIST_Test_MLP