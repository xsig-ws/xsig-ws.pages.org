.SUFFIXES: $(SUFFIXES) .java .class

TARGET = GridChallengeApi.class
JAVAC = javac

all: $(TARGET)
	jar cvf grch.jar *.class
clean:
	rm -f $(TARGET)

GridChallengeApi.class: GridChallengeApi.java
GridChallengeProblem.class: GridChallengeProblem.java
GRCH_Test.class: GRCH_Test.java

.java.class:
	$(JAVAC) $<
