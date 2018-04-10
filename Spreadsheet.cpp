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
	mutable size_t counter = 0;
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
	counter++;
	return this->value;
}

#include <utility>

class Spreadsheet {
public:
	Spreadsheet(size_t width, size_t height);
	Spreadsheet(const Spreadsheet& other);
	Spreadsheet& operator=(const Spreadsheet& other);
	Spreadsheet(Spreadsheet&& src) noexcept; // Move constructor
	Spreadsheet& operator=(Spreadsheet&& other) noexcept;  // Move assignment
	~Spreadsheet();
	void setCellAt(size_t x, size_t y, const SpreadsheetCell& cell);
	SpreadsheetCell& getCellAt(size_t x, size_t y);
	const SpreadsheetCell& getCellAt(size_t x, size_t y) const;
	friend void swap(Spreadsheet& first, Spreadsheet& second) noexcept;
private:
	Spreadsheet() = default;
	void verifyCoordinate(size_t x, size_t y) const;
	size_t mWidth = 0;
	size_t mHeight = 0;
	SpreadsheetCell** mCells = nullptr;
};
Spreadsheet::Spreadsheet(size_t width, size_t height): mWidth{ width }, mHeight{ height } {
	mCells = new SpreadsheetCell*[mWidth];
	for (size_t i = 0; i < mWidth; i++) {
		mCells[i] = new SpreadsheetCell[mHeight];
	}
}

void swap(Spreadsheet& first, Spreadsheet& second) noexcept {
	using std::swap;
	swap(first.mWidth, second.mWidth);
	swap(first.mHeight, second.mHeight);
	swap(first.mCells, second.mCells);
}
Spreadsheet::Spreadsheet(const Spreadsheet& other): Spreadsheet(other.mWidth, other.mHeight){
	for (size_t i = 0; i < mWidth; i++) {
		for (size_t j = 0; j < mHeight; j++) {
			mCells[i][j] =other.mCells[i][j];
		}
	}
}
Spreadsheet& Spreadsheet::operator=(const Spreadsheet& other) {
	if (this == &other) {
		return *this;
	}
	Spreadsheet temp{ other }; 
	swap(*this, temp);
	return *this;
}
Spreadsheet::Spreadsheet(Spreadsheet&& other) noexcept : Spreadsheet() { swap(*this, other); }
Spreadsheet& Spreadsheet::operator=(Spreadsheet&& other) noexcept { 
	Spreadsheet temp{ std::move(other) }; 
	swap(*this, temp);
	return *this;
}
Spreadsheet::~Spreadsheet(){
	for (size_t i = 0; i < mWidth; i++) {
		delete[] mCells[i];
	}
	delete[] mCells;
	mCells = nullptr;
}
void Spreadsheet::verifyCoordinate(size_t x, size_t y) const {
	if (x >= mWidth || y >= mHeight) {
		throw std::out_of_range("");
	}
}

void Spreadsheet::setCellAt(size_t x, size_t y, const SpreadsheetCell& cell) {
	verifyCoordinate(x, y);
	mCells[x][y] = cell;
}

const SpreadsheetCell& Spreadsheet::getCellAt(size_t x, size_t y) const  {
	verifyCoordinate(x, y);
	return mCells[x][y];
}

SpreadsheetCell& Spreadsheet::getCellAt(size_t x, size_t y){
	// pre c++17:return const_cast<SpreadsheetCell&>(static_cast<const Spreadsheet&>(*this).getCellAt(x, y));
	return const_cast<SpreadsheetCell&>(std::as_const(*this).getCellAt(x, y));
}
