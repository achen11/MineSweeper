#include "model.hxx"

using namespace ge211;

//CONSTRUCTORS

Model::Model(int width, int height, int n_mines)
        : board_({width, height}, n_mines),
          victory_(false),
          game_over_(false),
          flag_counter_(0),
          unveiled_tiles_(0),
          num_tiles_(height*width),
          rand_((num_tiles_))
{}

//METHOD BODIES

//
//GETTERS
//
Model::Rectangle
Model::board() const
{
    return board_.all_positions();
}

bool
Model::check_mine_(Position pos) const
{
    return board_.get_pos_data(pos).mine;
}

bool
Model::check_flagged_(Position pos) const
{
    return board_.get_flagged(pos);
}

bool
Model::check_unveiled_(Position pos) const
{
    return board_.get_unveiled(pos);
}

int
Model::surrounding_mines_(Position pos) const
{
    if(board_.get_mine(pos))
    {
        return 0;
    }
    else
    {
       return board_.get_surrounding_mines(pos);
    }
}


//
//MODIFIERS
//
void
Model::flag_tile(Position pos)
{
    //if not flagged-> flag, and then increase counter
    if(board_.flag_unflag(pos))
    {
        flag_counter_++;
    }
    //if flagged-> remove flag, decrease counter
    else
    {
        flag_counter_--;
    }
}

void
Model::unveil_tile(Position pos)
{
    //only can unveil tiles that aren't flagged
    if(!check_flagged_(pos))
    {
        //if unveiled pos is a mine: unveil ALL mines, end game as a loss
        if(board_.get_pos_data(pos).mine)
        {
            board_.unveil_mines();
            unveiled_tiles_ += board_.get_num_mines();
            game_over_ = true;
        }
        else
        {
            unveiled_tiles_+= board_.unveil_number(pos);
            //if all number tiles have been unveiled: end game as win
            // (# unveiled tiles == # total tile - num_mines)
            if(unveiled_tiles_ == get_number_nonmines())
            {
                game_over_= true;
                victory_ = true;
            }
        }
    }
}

void
Model::restart()
{
    int n_mines = board_.get_num_mines();
    //all values of v_n are invalid so they should all be replaced
    std::vector<int> v_n(n_mines,n_mines);
    int i = 0;
    while(i<n_mines)
    {
        int r = rand_.next();
        if(board_.good_position(int_to_pos(r)) && !check_mine_(int_to_pos(r)))
        {
            v_n[i] = r;
            i++;
        }
    }
    //set model parameters to their init states
    board_.set_mines(v_n);
    game_over_ =false;
    victory_ = false;
    flag_counter_ = 0;
    unveiled_tiles_ = 0;
}




///
///TEST HELPER
///
bool
Model::set_mines(std::vector<Position> v_pos)
{
    int NUM_MINES_ = board_.get_num_mines();
    std::vector<int> v (NUM_MINES_,NUM_MINES_);
    if((v.size())==(v_pos.size()))
    {
        for (int i = 0; i < NUM_MINES_; i++)
        {
            if (board_.good_position(v_pos[i]))
            {
                v[i] = board_.pos_to_int(v_pos[i]);
            }
        }
        return board_.set_mines(v);
    }
    return false;
}