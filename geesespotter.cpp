#include "geesespotter_lib.h"

char * createBoard( std::size_t xdim, std::size_t ydim )
{
	char * new_board { new char[xdim*ydim]{0} };
	return new_board;
}

void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim)
{
	const char goose_number = 9;
	// searched size_t w/ negative values
	for ( std::ptrdiff_t x = 0; x < xdim; x++ )
	{
		for ( std::ptrdiff_t y = 0; y < ydim; y++ )
		{
			if ( board[y * xdim + x] != goose_number )
			{
				for ( std::ptrdiff_t neighbor_x = x - 1; neighbor_x <= x + 1; neighbor_x++ )
				{
					for ( std::ptrdiff_t neighbor_y = y - 1; neighbor_y <= y + 1; neighbor_y++ )
					{
						if ( neighbor_x >= 0 && neighbor_x < xdim && neighbor_y >= 0 && neighbor_y < ydim )
						{

							if ( board[neighbor_y * xdim + neighbor_x] == goose_number )
							{
								board[y * xdim + x] += 1;
							}
						}
					}
				}

			}
		}
	}
}

void hideBoard(char * board, std::size_t xdim, std::size_t ydim)
{
	for ( std::size_t print_index{0}; print_index <= (xdim * ydim - 1); print_index++ )
	{
		board[print_index] = board[print_index] | hiddenBit();
	}
}

void cleanBoard(char * board)
{
	delete [] board;
	board = nullptr;
}

void printBoard(char * board, std::size_t xdim, std::size_t ydim)
 {
	for ( std::size_t y = 0; y < ydim; y++ )
	{
		for ( std::size_t x = 0; x < xdim; x++ )
		{
			char field_value = board[y * xdim + x];
			bool marked = (field_value & markedBit()) == markedBit();
			bool hidden = (field_value & hiddenBit()) == hiddenBit();
			char value = field_value & valueMask();

			if ( marked )
			{
				std::cout << "M";
			} else if ( hidden )
			{
				std::cout << "*";
			} else
			{
				printf( "%d", value );
			}
		}
		std::cout << "\n";
	}
}

int reveal(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc)
 {
	char field_value = board[yloc * xdim + xloc];
	char value = field_value & valueMask();
	bool marked = (field_value & markedBit()) == markedBit();
	bool hidden = (field_value & hiddenBit()) == hiddenBit();

	if ( marked )
	{
		return 1;
	}
	if ( !hidden )
	{
		return 2;
	}
	if ( value == 9 )
	{
		return 9;
	}
	board[yloc * xdim + xloc] = value;
	if ( value == 0 )
	{
		for ( int reveal_x = (int) xloc - 1; reveal_x <= (int) xloc + 1; reveal_x++ )
		{
			for ( int reveal_y = (int) yloc - 1; reveal_y <= (int) yloc + 1; reveal_y++ )
			{
				if ( reveal_x < xdim && reveal_y < ydim && reveal_x >= 0 && reveal_y >= 0 )
				{
					char neighbor_value = board[reveal_y * xdim + reveal_x];
					bool neighbor_marked = ( neighbor_value & markedBit() ) != 0;
					char neighbor_og_value = neighbor_value & valueMask();

					if ( !neighbor_marked )
					{
						board[reveal_y * xdim + reveal_x] = neighbor_og_value;
					}
				}
			}
		}
	}
	return 0;
}

int mark(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc)
{
	std::size_t print_index{(yloc * xdim) + xloc};
	if ( (board[print_index] & markedBit()) != markedBit() )
	{
		board[print_index] += markedBit();
	}
	else if ( (board[print_index] & markedBit()) == markedBit() )
	{
		board[print_index] -= markedBit();
	}
	else
	{
		if ( (board[print_index] & hiddenBit()) != hiddenBit() )
		{
			return 2;
		}
	}
	return 0;
}

bool isGameWon(char * board, std::size_t xdim, std::size_t ydim)
 {
	for ( std::size_t i = 0; i < ydim; i++ )
	{
		for ( std::size_t j = 0; j < xdim; j++ )
		{
			std::size_t print_index = i * xdim + j;

			char field_value = board[print_index];
			char value = field_value & valueMask();
			bool marked = ( field_value & markedBit() ) != 0;
			bool hidden = ( field_value & hiddenBit() ) != 0;

			if ( (marked || hidden) && value != 9 )
			{
				return false;
			}
		}
	}
	return true;
}
