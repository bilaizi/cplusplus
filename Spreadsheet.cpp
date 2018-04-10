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
// SpreadsheetCell& SpreadsheetCell::operator=(constSpreadsheetCell& other) = default;
SpreadsheetCell& SpreadsheetCell::operator=(constSpreadsheetCell& other) {
	if (this == &other) {
		return *this;
	}
	value = other.value;
	return *this;
}
void SpreadsheetCell::setValue(d ouble value) {
	this->value = value;
}
double SpreadsheetCell::getValue() const {
	return this->value;
}

class Spreadsheet {
public:
	Spreadsheet(size_t width, size_t height);
	void setCellAt(size_t x, size_t y, const SpreadsheetCell& cell);
	SpreadsheetCell& getCellAt(size_t x, size_t y);
private:
	bool inRange(size_t value, size_t upper) const;
	size_t mWidth = 0;
	size_t mHeight = 0;
	SpreadsheetCell** mCells = nullptr;
};
