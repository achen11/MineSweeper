#include "view.hxx"

// Convenient type aliases:
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

//Constants (size and color）
static int const padding = 2;
static int const grid_size = 50;
static int const tile_size= (grid_size - 2*padding);
static int const mine_size= 23;
static int const mouse_size= 10;
static ge211::Color const light_gray_color {170, 170, 170};
static ge211::Color const gray_color {80, 80, 80};


View::View(Model const& model)
        : model_(model),
          bottom_tile_sprite({tile_size,tile_size}, gray_color),
          top_tile_sprite({tile_size,tile_size}, light_gray_color),
          mine_sprite(mine_size, Color::medium_red()),
          number_sprite1("1", sans),
          number_sprite2("2", sans),
          number_sprite3("3", sans),
          number_sprite4("4", sans),
          number_sprite5("5", sans),
          number_sprite6("6", sans),
          number_sprite7("7", sans),
          number_sprite8("8", sans),
          win_sprite("You won!", big_sans),
          lose_sprite("You lost!", big_sans),
          counter_sprite(counter_str,
                         big_sans),
          flag_sprite(mine_size, Color::medium_yellow()),
          mouse_sprite(mouse_size, Color::medium_blue()),
          valid_mouse_sprite(mouse_size, Color::medium_cyan()),
          restart_sprite("restart_button.png")

{ }


void View::draw(Sprite_set& set, Position mouse_position)
{
    int board_width = model_.get_dims_().width;
    int board_height = model_.get_dims_().height;
    counter_str = std::to_string(model_.get_screen_counter());

    //constructing builder for counter_sprite
    ge211::Text_sprite::Builder counter_builder(big_sans);
    counter_builder.message(counter_str);
    counter_sprite.reconfigure(counter_builder);
    set.add_sprite(counter_sprite, {model_.get_dims_().width,model_.get_dims_
            ().height}, 7);

    for (auto pos: model_.board())
    {
        Position tile_pos = {(board_to_screen(pos).x+ padding),board_to_screen
                             (pos).y+padding};
        Position number_pos = {(board_to_screen(pos).x+ 4*padding),board_to_screen
                                                                       (pos).y+2*padding};
        //drawing all the bottom tiles
        set.add_sprite(bottom_tile_sprite, tile_pos, 0);

        //drawing mines
        if(model_.check_mine_(pos))
        {
            set.add_sprite(mine_sprite, tile_pos, 1);
        }

        //drawing number tiles depending on how many surrounding mines are
        //around the tile
        if(model_.surrounding_mines_(pos) == 1)
        {
            set.add_sprite(number_sprite1, number_pos, 1);
        }
        if(model_.surrounding_mines_(pos) == 2)
        {
            set.add_sprite(number_sprite2, number_pos, 1);
        }
        if(model_.surrounding_mines_(pos) == 3)
        {
            set.add_sprite(number_sprite3, number_pos, 1);
        }
        if(model_.surrounding_mines_(pos) == 4)
        {
            set.add_sprite(number_sprite4, number_pos, 1);
        }
        if(model_.surrounding_mines_(pos) == 5)
        {
            set.add_sprite(number_sprite5, number_pos, 1);
        }
        if(model_.surrounding_mines_(pos) == 6)
        {
            set.add_sprite(number_sprite6, number_pos, 1);
        }
        if(model_.surrounding_mines_(pos) == 7)
        {
            set.add_sprite(number_sprite7, number_pos, 1);
        }
        if(model_.surrounding_mines_(pos) == 8)
        {
            set.add_sprite(number_sprite8, number_pos, 1);
        }

        //if tile has not been unveiled, draw top tiles
        if(!model_.check_unveiled_(pos))
        {
            set.add_sprite(top_tile_sprite, tile_pos, 2);
        }

        //drawing flags
        if(model_.check_flagged_(pos))
        {
            set.add_sprite(flag_sprite, tile_pos, 3);
        }

        //If tile is unveiled or there is a flag over tile, then draw regular mouse_sprite (blue).
        //Otherwise, if tile is veiled (it's a valid tile to uncover), valid_mouse_sprite is drawn (cyan)
        if(model_.check_unveiled_(screen_to_board(mouse_position))
            || model_.check_flagged_(screen_to_board(mouse_position)) )
        {
            set.add_sprite(mouse_sprite, {mouse_position.x-mouse_size,
                                          mouse_position.y-mouse_size} , 6);
        }
        else
        {
            set.add_sprite(valid_mouse_sprite, {mouse_position.x-mouse_size,
                                                mouse_position.y-mouse_size} , 6);
        }

        //when game is over, draws restart button and determines whether to draw win or lose sprite
        if(model_.is_game_over())
        {
            Position above = board_to_screen({board_width/8,board_height/4});
            Position left_corner = board_to_screen({board_width*11/23, board_height*6/11});
            set.add_sprite(restart_sprite, left_corner ,6, ge211::Transform::scale(0.20));

            if(model_.win_or_loss())
            {
                set.add_sprite(win_sprite, above, 6);
            }
            else
            {
                set.add_sprite(lose_sprite, above, 6);
            }
        }
    }
}

View::Dimensions
View::initial_window_dimensions() const
{
    return grid_size * model_.board().dimensions();
}

std::string
View::initial_window_title() const
{
    return "Minesweeper";
}


View::Position View::board_to_screen(Model::Position logical) const {
    logical.x *= grid_size;
    logical.y *= grid_size;
    return logical;
}

Model::Position View::screen_to_board(View::Position physical) const {
    physical.x /= grid_size;
    physical.y /= grid_size;
    return physical;
}


