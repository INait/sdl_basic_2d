#include <vector>
#include <algorithm>
#include <functional>

typedef std::function< void (int, int, int) > PrintFunc;

enum Type
{
	EMPTY = -1,
	ZERO = 0,
	CROSS = 1
};

struct Cell
{

	Type type;

	Cell( const Type& type ) : type( type ) { }
};

class Field
{
public:
	Field(int width, int height) : width_( width ), height_( height ), cells( width * height, Cell( Type::EMPTY ) )
	{
	}

	~Field() { }

	int SetCell(int x, int y, const Type& type)
	{
		if( x >= width_ || y >= height_ )
			return -1; // OUT_OF_BOUNDS

		if( cells.at( x + y * height_ ).type != Type::EMPTY )
			return -2; // CELL_IS_FILLED

		cells.at( x + y * height_ ).type = type;

		return 0;
	}

	bool IsEndGame()
	{
		return ( std::find_if( cells.begin(), cells.end(),
					[] (Cell cell) -> bool
					{
						return cell.type == Type::EMPTY;
					} )
				== cells.end() );
	}

	int CheckWinner()
	{
		int directions[8][3] = {{0,1,2}, {0,3,6}, {0,4,8}, {1,4,7}, {2,5,8}, {2,4,6}, {3,4,5}, {6,7,8}};
		for( int i = 0; i < 8; ++i )
		{
			int* direction = directions[i];
			if ( cells.at( direction[0] ).type == cells.at( direction[1] ).type &&
				 cells.at( direction[0] ).type == cells.at( direction[2] ).type &&
				 cells.at( direction[0] ).type != Type::EMPTY )
			{
				return (int)cells.at( direction[0] ).type;
			}
		}

		return -1;
	}

	void PrintField( PrintFunc func )
	{
		for( unsigned int i = 0; i < cells.size(); ++i )
			func( i % width_, i / width_, (int)cells[i].type );
	}

private:
	int width_;
	int height_;

	std::vector< Cell > cells;
};
