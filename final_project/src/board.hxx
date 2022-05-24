#ifndef FINAL_PROJECT_BOARD_HXX
  #define FINAL_PROJECT_BOARD_HXX

#endif //FINAL_PROJECT_BOARD_HXX

#pragma once

#include <ge211.hxx>

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <map>


/// Represents the state of the board.
class Board
{
public:
    //STRUCT USED TO HOLD INFO ABOUT STATES
    struct pos_states
    {
        bool flagged;
        bool unveiled;
        bool mine;
        int num_of_mines;
    };

    /// Board dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<int>;
    /// Board positions will use `int` coordinates.
    using Position = ge211::Posn<int>;
    /// Board rectangles will use `int` coordinates.
    using Rectangle = ge211::Rect<int>;
    ///Board rand_source will use `int` coordinates.
    using Random_source = ge211::Random_source<int>;


private:
    // PRIVATE DATA MEMBERS
    int num_mines_;
    ///Dimensions of Board(size)
    Dimensions dims_;
    ///each index represents a position, and its value is the respective pos_states
    std::vector<struct pos_states> pos_data_;
    ///vector of the positions of mines in int form (mostly for testing)
    std::vector<int> pos_mines_;

public:
    // PUBLIC CONSTRUCTOR & FUNCTION MEMBERS (taken from reversi)

    /// Constructs a board with the given dimensions.
    /// ## Errors: throws `ge211::Client_logic_error` if either dimension is less than 2 or greater than 8.
    explicit Board(Dimensions dims, int n_mines);
    /// Returns the same `Dimensions` value passed to the constructor.
    Dimensions dimensions() const;
    /// Returns whether the given position is in bounds.
    bool good_position(Position) const;
    /// Returns a rectangle containing all the positions of the board.
    Rectangle all_positions() const;

    // PUBLIC CONSTRUCTOR & FUNCTION MEMBERS (taken from reversi)

    /// changes position to int
    int pos_to_int(Position pos) const;
    Position int_to_pos(int n) const;
    //HELPER that returns a vector of directions based on position
    std::vector<Position> adj_directions(Position pos);

    //Getters
    ///gets the pos_states fields
    pos_states get_pos_data(Position pos)const;
    ///gets the whether the position is a mine
    bool get_mine(Position pos)const;
    ///gets the whether the position has a flag
    bool get_flagged(Position pos)const;
    ///gets the whether the position is covered
    bool get_unveiled(Position pos)const;
    ///gets the number of surrounding mine tiles
    int get_surrounding_mines(Position pos)const;
    ///gets NUM_MINES_
    int get_num_mines() const
    {
        return num_mines_;
    }
    ////gets pos_mines_ in terms of positions, not ints
    std::vector<Position> get_pos_mines()const;


    //Modifiers
    ///if unflagged(True), flags tile and vice-versa(False)
    bool flag_unflag(Position pos);
    ///unveils tile and surrounding tiles if they are 0, then returns the
        ///number of revealed tiles
    int unveil_number(Position pos);
    ///unveils all mines
    void unveil_mines();
    ///sets mines at all positions within the vector. v.size() == NUM_MINES_
    bool set_mines(std::vector<int> v);

    //FUNCTIONS FOR TESTING
    bool compare_pos(Position pos1, Position pos2);
    bool compare_boards(Board b1);

private:
    //
    // PRIVATE HELPER FUNCTION MEMBERS
    //

    ///THE FOLLOWING HELPERS HAVE TO BE CALLED IN THE FOLLOWING ORDER TO WORK

    ///HELPER to init pos_data vector
    void initiate_pos_data_() ;
    ///HELPER to add bombs to map
    void add_bombs_();
    ///HELPER to add number of surrounding mines to map
    void add_numbers_();
};
