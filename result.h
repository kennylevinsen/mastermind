#ifndef RESULT_H
#define RESULT_H

/*
 * The result object - acts as a tuple
 */
class Result {
public:
	Result(int correct, int almost) : _correct(correct), _almost(almost) {}

	int getCorrect() const { return _correct; }
	int getAlmost() const { return _almost; }

private:
	int _correct;
	int _almost;

};

#endif /* RESULT_H */
