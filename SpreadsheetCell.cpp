class SpreadsheetCell {
public:
	SpreadsheetCell() = default;
	SpreadsheetCell(double value);
	SpreadsheetCell(const SpreadsheetCell& other);
	void setValue(double value);
	double getValue() const;
private:
	double value;
};
SpreadsheetCell::SpreadsheetCell(double value):value{ value } {}
void SpreadsheetCell::setValue(double value) {
	this->value = value;
}
double SpreadsheetCell::getValue() const {
	return this->value;
}
