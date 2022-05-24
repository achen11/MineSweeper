#include "controller.hxx"

Controller::Controller(int size, int n_mines)
        : Controller(size,size, n_mines)
{ }

Controller::Controller(int width, int height, int n_mines)
        : model_(width, height, n_mines),
          view_(model_)
{ }

void
Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites, mouse_position_);
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}

void
Controller::on_mouse_down(ge211::Mouse_button button, ge211::Posn<int> position)
{
    Model::Position board_pos = view_.screen_to_board(position);
    static int const left_corner_x = 150;
    static int const left_corner_y = 200;
    static double restart_sprite_size = 396*.2;


    //left click with mouse to unveil tile
    if(button == ge211::Mouse_button::left && !model_.is_game_over() &&
    !model_.check_unveiled_(board_pos) && !model_.check_flagged_(board_pos))
    {
        model_.unveil_tile(board_pos);

    }

    //right click with mouse to place or remove a flag
    if(!model_.is_game_over() && button == ge211::Mouse_button::right)
    {
        model_.flag_tile(board_pos);
    }

    //when game is over, right-click on the button to restart the game
    if(model_.is_game_over() && button == ge211::Mouse_button::right &&
            (position.x >= left_corner_x && position.x <= left_corner_x + restart_sprite_size) &&
            (position.y >= left_corner_y && position.y <= left_corner_y + restart_sprite_size))
    {
        model_.restart();
    }
}

//follows position of the mouse
void
Controller::on_mouse_move(View::Position screen_position)
{
    mouse_position_ = screen_position;
}

