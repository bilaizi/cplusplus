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
	Spreadsheet(const Spreadsheet& src);
	Spreadsheet& operator=(const Spreadsheet& rhs);
	Spreadsheet(Spreadsheet&& src) noexcept; // Move constructor
	Spreadsheet& operator=(Spreadsheet&& rhs) noexcept;  // Move assignment
	~Spreadsheet();
	void setCellAt(size_t x, size_t y, const SpreadsheetCell& cell);
	SpreadsheetCell& getCellAt(size_t x, size_t y);
	friend void swap(Spreadsheet& first, Spreadsheet& second) noexcept;
private:
	Spreadsheet() = default;
	void verifyCoordinate(size_t x, size_t y) const;
	size_t mWidth = 0;
	size_t mHeight = 0;
	SpreadsheetCell** mCells = nullptr;
};
