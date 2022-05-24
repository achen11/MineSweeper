#include "board.hxx"
#include <algorithm>
#include <ge211.hxx>


Board::Board(Dimensions dims, int n_mines)
        : num_mines_(n_mines),
        dims_(dims)
{
    if (dims_.width < 2 || dims_.height < 2)
    {
        throw ge211::Client_logic_error("Board::Board: dims too small");
    }
    if(num_mines_>(dims_.width*dims_.height))
    {
        throw ge211::Client_logic_error("Board::Board: too many mines");
    }
    if(num_mines_<0)
    {
        throw ge211::Client_logic_error
            ("Board::Board: no negative number of mines");
    }
    pos_mines_ = std::vector<int>(num_mines_,num_mines_);
    initiate_pos_data_();
    add_bombs_();
    add_numbers_();
}

Board::Dimensions
Board::dimensions() const
{
    return dims_;
}

bool
Board::good_position(Position pos) const
{
    return 0 <= pos.x && pos.x < dims_.width &&
           0 <= pos.y && pos.y < dims_.height;
}

Board::Rectangle
Board::all_positions() const
{
    return Rectangle::from_top_left(ge211::the_origin, dims_);
}



///OUR FUNCTIONS

int
Board::pos_to_int(Position pos) const
{
    return (dims_.width*pos.y)+pos.x;
}

Board::Position
Board::int_to_pos(int n) const
{
    int x = n%dims_.width;
    int y = trunc(n/dims_.width);
    return {x,y};
}
std::vector<Board::Position>
 Board::adj_directions(Position pos)
 {
    return std::vector<Position>{{pos.x,pos.y-1},{pos.x+1,pos.y-1},
                                 {pos.x+1,pos.y},{pos.x+1,pos.y+1},
                                 {pos.x,pos.y+1},{pos.x-1,pos.y+1},
                                 {pos.x-1,pos.y},{pos.x-1,pos.y-1}};
 }



 ///
///GETTERS
///
Board::pos_states
Board::get_pos_data(Position pos) const
{
    int n = pos_to_int(pos);
    return pos_data_[n];
}

bool
Board::get_mine(Position pos)const
{
    int n = pos_to_int(pos);
    return pos_data_[n].mine;
}

bool
Board::get_flagged(Position pos)const
{
    int n = pos_to_int(pos);
    return pos_data_[n].flagged;
}

bool
Board::get_unveiled(Position pos)const
{
    int n = pos_to_int(pos);
    return pos_data_[n].unveiled;
}

int
Board::get_surrounding_mines(Position pos)const
{
    int n = pos_to_int(pos);
    return pos_data_[n].num_of_mines;
}


std::vector<Board::Position>
Board::get_pos_mines() const
{
    std::vector<Board::Position> v (num_mines_, {0,0});
    for(int i = 0; i<num_mines_; i++)
    {
        v[i] = int_to_pos(pos_mines_[i]);
    }
    return v;
}


///
/// MODIFIERS
///
bool
Board::flag_unflag(Position pos)
{
    int n = pos_to_int(pos);
    //IF NOT FLAGGED YET
    if (!pos_data_[n].flagged)
    {
        pos_data_[n].flagged = true;
        return true;
    }
    //IF ALREADY FLAGGED
    else
    {
        pos_data_[n].flagged = false;
        return false;
    }
}

int
Board::unveil_number(Position pos)
{
    int n = pos_to_int(pos);
    int count =0;
    pos_data_[n].unveiled = true;
    count++;
    for(Position adj_pos:adj_directions(pos))
    {
        int adj_n = pos_to_int(adj_pos);
        if(good_position(adj_pos)&&(!pos_data_[adj_n].mine)
                                &&!pos_data_[adj_n].unveiled)
        {
            if(pos_data_[adj_n].num_of_mines == 0)
            {
                pos_data_[adj_n].unveiled = true;
                count++;
            }
        }
    }
    return count;
}

void
Board::unveil_mines()
{
    for(auto n: pos_mines_)
    {
        //unveils all the mines
        pos_data_[n].unveiled = true;
        if(pos_data_[n].flagged)
        {
            //removes the flags from the unveiled mines
            pos_data_[n].flagged = false;
        }
    }
}


///
///HELPERS FOR BOARD CONSTRUCTOR
///
void
Board::initiate_pos_data_()
{
    //initial state for all the structs (states)->
    // not flagged, not unveiled, not a mine, no surrounding mines
    struct pos_states init_state = {false, false, false, 0};
    int n_pos = dims_.width*dims_.height;     //number of tiles
    pos_data_ = std::vector<struct pos_states>(n_pos, init_state);
}

void
Board::add_bombs_()
{
    int width = dims_.width;
    int i = 0;
    while(i<num_mines_)
    {
        ge211::Random_source<int> r(0,width);
        int rand_x = r.next();
        int rand_y = r.next();
        int n = pos_to_int({rand_x,rand_y});
        //access list and see if it is valid and mine is false
        if (good_position({rand_x,rand_y})&&(!pos_data_[n].mine))
        {
            pos_data_[n].mine = true;
            pos_mines_[i] = n;
            i++;
        }
    }
}

void
Board::add_numbers_()
{
    Position pos = {0,0};
    for(int n = 0; n<dims_.width*dims_.height; n++)
    {
        pos = int_to_pos(n);
        for(Position adj_pos:adj_directions(pos))
        {
            if(good_position(adj_pos))
            {
                if(pos_data_[pos_to_int(adj_pos)].mine && !pos_data_[n].mine)
                {
                    pos_data_[n].num_of_mines++;
                }
            }
        }
    }
}

bool
Board::set_mines(std::vector<int> v)
{
    int i =0;
    if(static_cast<int>(v.size())==num_mines_)
    {
        //clear states
        initiate_pos_data_();
        //set mines
        for(int n: v)
        {
            pos_data_[n].mine= true;
            pos_mines_[i] = n;
            i++;
        }
        //set numbers
        add_numbers_();
        return true;
    }
    return false;
}



///
///FUNCTIONS FOR TESTING
///
bool
Board::compare_pos(Board::Position pos1, Board::Position pos2)
{
    return pos1.x == pos2.x && pos2.y == pos1.y;
}

bool
Board::compare_boards(Board b1)
{
    for(int i = 0; i<num_mines_; i++)
    {
        if(pos_mines_[i] != b1.pos_mines_[i])
        {
            return false;
        }
    }
    return true;
}

