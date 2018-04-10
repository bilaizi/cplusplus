class SpreadsheetCell {
public:
	SpreadsheetCell() = default;
	SpreadsheetCell(double value);
	SpreadsheetCell(const SpreadsheetCell& other);
	SpreadsheetCell& operator=(const SpreadsheetCell& other);
	void setValue(double value);
	double getValue() const;
private:
	double value;
};
SpreadsheetCell::SpreadsheetCell(double value):value{ value } {}
// SpreadsheetCell::SpreadsheetCell(const SpreadsheetCell& other) = default;
SpreadsheetCell::SpreadsheetCell(const SpreadsheetCell& other) : value{ other.value } { }
SpreadsheetCell& SpreadsheetCell::operator=(constSpreadsheetCell& other) {
	if (this == &other) {
		return *this;
	}
	this->value = other.mValue;
	return *this;
}
void SpreadsheetCell::setValue(d ouble value) {
	this->value = value;
}
double SpreadsheetCell::getValue() const {
	return this->value;
}
