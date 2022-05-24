#pragma once

#include <ge211.hxx>
#include "board.hxx"

#include <ge211.hxx>

#include <iostream>
#include <vector>
class Model
{
public:
    using Dimensions = Board::Dimensions;
    using Position = Board::Position;
    using Rectangle = Board::Rectangle;
    using Random_source = Board::Random_source;

    //CONSTRUCTORS
    Model(int width, int height, int n_mines);
    Model(int size, int n_mines);

    //METHODS
    //Getters-> return constant values
    ////returns the number of flags
    int get_flag_counter() const
    {
        return flag_counter_;
    }
    ///returns whether the game is over
    bool is_game_over() const
    {
        return game_over_;
    }
    ///returns whether the player has won
    bool win_or_loss() const
    {
        return victory_;
    }
    ////gets board_ in terms of positions
    Rectangle board() const;
    ////gets board_ in as a vector<pos_states>
    Board get_board() const
    {
        return board_;
    }
    ////gets unveiled_tiles_
    int get_unveiled_tiles_()
    {
        return unveiled_tiles_;
    }
    ////gets dims_ of board
    Board::Dimensions get_dims_() const
    {
        return board_.dimensions();
    }
    ///gets NUM_MINES_
    int get_num_mines()const
    {
        return board_.get_num_mines();
    }
    ////returns the number of guessed(flagged) mines left
    int get_screen_counter() const
    {
        return get_num_mines()-get_flag_counter();
    }
    ////checks whether tile is a mine
    bool check_mine_(Position pos) const;
    ////checks whether the tile is flagged(true) or not(false)
    bool check_flagged_(Position pos) const;
    ////checks whether the tile is unveiled(true) or not(false)
    bool check_unveiled_(Position pos) const;
    ////returns the # of surrounding mines, will return 0 if a mine
    int surrounding_mines_(Position pos) const;

    //MODIFIERS
    ///left click: unveils the tile and checks if its a mine (PLAY MOVE)
    void unveil_tile(Position pos);
    ///right click: flags/unflags changes flag_counter_
    void flag_tile(Position pos);
    ///creates a new board()
    void restart();
    ///takes a int position and converts it to a Position
    Board::Position int_to_pos(int n)
    {
        return board_.int_to_pos(n);
    }

    //TESTING HELPER
    ///takes a Position position and converts it to a int
    int pos_to_int(Position pos)
    {
        return board_.pos_to_int(pos);
    }
    ///returns the actual board; can be changed
    Board change_board()
    {
        return board_;
    }
    ////sets mines to the positions
    bool set_mines(std::vector<Position> v_pos);
    ////returns # of number tiles
    int get_number_nonmines()
    {
        int tot_tiles = board_.dimensions().width*board_.dimensions().height;
        return tot_tiles-board_.get_num_mines();
    }


private:
    //PRIVATE MEMBER VARIABLES
    Board board_;
    bool victory_;
    bool game_over_;
    int flag_counter_;
    int unveiled_tiles_;
    int num_tiles_;
    Random_source rand_;
};