#pragma once

template<class T>
class Grid
{
public:
	Grid(int size = 0);
	T* at(int x, int y);
	void shift(int x_dir, int y_dir, int range);
	int size();
	void resize(int size);

	void printDimensions();

private:
	std::vector< std::vector< T >> _contents;
	int _size;
	int _column_offset = 0;
	int _row_offset = 0;
};

template<class T>
inline Grid<T>::Grid(int size) : _size(size)
{
	_contents.resize(_size);

	for (int x = 0; x < _size; ++x)
	{
		_contents[x].resize(_size);
	}
}

template<class T>
inline T* Grid<T>::at(int x, int y)
{
	if (x >= _size || y >= _size)
	{
		std::cout << "Index out of grid range...\n";
		return nullptr;
	}

	T* ret = nullptr;

	int x_index = x + _column_offset;
	int y_index = y + _row_offset;

	if (x_index >= _size) x_index -= _size;
	if (y_index >= _size) y_index -= _size;

	if (x_index < 0 || y_index < 0)
	std::cout << x_index << ", " << y_index << "\n";

	ret = &_contents[x_index][y_index];

	return ret;
}

template<class T>
inline void Grid<T>::shift(int x_dir, int y_dir, int range)
{
	if (std::abs(x_dir * range) > _size) range = _size;
	if (std::abs(y_dir * range) > _size) range = _size;

	_column_offset += x_dir * range;
	_row_offset += y_dir * range;
}

template<class T>
inline int Grid<T>::size()
{
	return _size*_size;
}

template<class T>
inline void Grid<T>::resize(int size)
{
	_size = size;
	_contents.resize(_size);

	for (int x = 0; x < _size; ++x)
	{
		_contents[x].resize(_size);
	}
}

template<class T>
inline void Grid<T>::printDimensions()
{
	std::cout << _contents.size() << "\n";
}
