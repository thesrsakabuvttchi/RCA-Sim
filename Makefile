TESTPREFIX=./tests
all: BASIC_SCHED BASIC_BLOCKED_SCHED MNIST_DNN MNIST_MLP
tests: BASIC_SCHED BASIC_BLOCKED_SCHED MNIST_DNN MNIST_MLP

setup: setup_DNN setup_MLP

setup_DNN:
	python3 create_mnist_DNN.py

setup_MLP:
	python3 create_mnist_MLP.py

BASIC_SCHED: $(TESTPREFIX)/testLimitedScheduler.cpp
	g++ $(TESTPREFIX)/testLimitedScheduler.cpp -o testLimitedScheduler

BASIC_BLOCKED_SCHED: $(TESTPREFIX)/testBrainDeadScheduler.cpp
	g++ $(TESTPREFIX)/testBrainDeadScheduler.cpp -o testBlockedScheduler

MNIST_DNN: $(TESTPREFIX)/MNIST_Test_DNN.cpp
	g++ $(TESTPREFIX)/MNIST_Test_DNN.cpp -o MNIST_Test_DNN

MNIST_MLP: $(TESTPREFIX)/MNIST_Test_MLP.cpp
	g++ $(TESTPREFIX)/MNIST_Test_MLP.cpp -o MNIST_Test_MLP

clean:
	rm testLimitedScheduler testBlockedScheduler MNIST_Test_DNN MNIST_Test_MLP