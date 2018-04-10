class SpreadsheetCell {
public:
	void setValue(double value);
	double getValue() const;
private:
	double value;
};

void SpreadsheetCell::setValue(double value) {
	this->value = value;
}
double SpreadsheetCell::getValue() const {
	return this->value;
}
