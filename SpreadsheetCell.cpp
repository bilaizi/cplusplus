class SpreadsheetCell {
public:
	SpreadsheetCell(double value);
	void setValue(double value);
	double getValue() const;
private:
	double value;
};
SpreadsheetCell::SpreadsheetCell(double value):value{value} {}
void SpreadsheetCell::setValue(double value) {
	this->value = value;
}
double SpreadsheetCell::getValue() const {
	return this->value;
}
